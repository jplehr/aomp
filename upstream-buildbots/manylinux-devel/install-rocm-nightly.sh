#!/bin/bash
# shellcheck shell=bash
#
# Install a TheRock ROCm "dist" nightly tarball into a target directory.
#
# The full dist tarball contains the complete ROCm SDK (HIP runtime, device
# libs, rocminfo, rocblas, rocthrust, ...), so it fully replaces the previous
# apt/rocm.repo based install used by the openmp-ci images.
#
# Installs the "multiarch" tarball, which contains kernel packs for all
# supported GPU targets, so a single image works on every GPU architecture.
#
# Inputs are read from the environment so the Dockerfile can wire them through
# build ARGs:
#   ROCM_BASE_VERSION    base ROCm version, e.g. "7.14"        (default 7.14)
#   ROCM_NIGHTLY_DATE    pin an exact build date YYYYMMDD;     (default empty)
#                        empty => use ROCM_NIGHTLY_MAX_DATE / latest
#   ROCM_NIGHTLY_MAX_DATE
#                        upper bound YYYYMMDD; pick the newest  (default empty)
#                        available nightly with date <= this.
#                        Ignored when ROCM_NIGHTLY_DATE is set.
#                        Empty => use the most recent nightly.
#
# Selection precedence: ROCM_NIGHTLY_DATE (exact) > ROCM_NIGHTLY_MAX_DATE
# (newest on-or-before) > latest available.

set -euo pipefail

RocmBaseVersion="${ROCM_BASE_VERSION:-7.14}"
RocmNightlyDate="${ROCM_NIGHTLY_DATE:-}"
RocmNightlyMaxDate="${ROCM_NIGHTLY_MAX_DATE:-}"
RocmInstallDir="/opt/rocm"
RocmNightlyBaseUrl="https://rocm.nightlies.amd.com/tarball-multi-arch"

if [ -n "${RocmNightlyDate}" ] && [[ ! "${RocmNightlyDate}" =~ ^[0-9]{8}$ ]]; then
  echo "error: ROCM_NIGHTLY_DATE must be an 8-digit date (YYYYMMDD), got '${RocmNightlyDate}'" >&2
  exit 1
fi
if [ -n "${RocmNightlyMaxDate}" ] && [[ ! "${RocmNightlyMaxDate}" =~ ^[0-9]{8}$ ]]; then
  echo "error: ROCM_NIGHTLY_MAX_DATE must be an 8-digit date (YYYYMMDD), got '${RocmNightlyMaxDate}'" >&2
  exit 1
fi

# Shared curl options: fail on HTTP errors, stay quiet but show errors, follow
# redirects, bound the connection setup, and retry transient failures. The
# per-call --max-time bounds the whole transfer and is set at each call site
# because the index fetch and the multi-GB tarball download need very different
# ceilings.
CurlOpts=(--fail --silent --show-error --location --connect-timeout 30 --retry 5 --retry-delay 5)

# Resolve the tarball filename. Three modes, in precedence order:
#   1. exact date pin (ROCM_NIGHTLY_DATE)
#   2. newest on-or-before an anchor (ROCM_NIGHTLY_MAX_DATE)
#   3. newest available
# Modes 2 and 3 query the nightly index and filter by version.
function resolveTarball() {
  local Prefix="therock-dist-linux-multiarch-${RocmBaseVersion}.0a"

  if [ -n "${RocmNightlyDate}" ]; then
    echo "${Prefix}${RocmNightlyDate}.tar.gz"
    return 0
  fi

  # The index page embeds the available files; extract every name matching our
  # version, sort lexically (dates are zero-padded YYYYMMDD), newest last.
  # Escape regex metacharacters in the prefix (the version contains '.', which
  # would otherwise match any character) so only the intended builds match.
  local PrefixRe="${Prefix//./\\.}"
  local Available
  Available="$(curl "${CurlOpts[@]}" --max-time 60 "${RocmNightlyBaseUrl}/" |
    grep -oP "${PrefixRe}[0-9]{8}\.tar\.gz" |
    sort -u)"

  if [ -z "${Available}" ]; then
    echo "error: no nightly tarball found for version ${RocmBaseVersion} at ${RocmNightlyBaseUrl}/" >&2
    return 1
  fi

  local Selected
  if [ -n "${RocmNightlyMaxDate}" ]; then
    # Newest build whose 8-digit date is <= the anchor. The date is the 15..8
    # chars from the end of the name (".tar.gz" is 7 chars). Lexical compare on
    # zero-padded YYYYMMDD is equivalent to numeric compare.
    local Name Date
    while IFS= read -r Name; do
      [ -z "${Name}" ] && continue
      Date="${Name: -15:8}"
      if [ "${Date}" -le "${RocmNightlyMaxDate}" ]; then
        Selected="${Name}"
      fi
    done <<<"${Available}"
    if [ -z "${Selected:-}" ]; then
      echo "error: no nightly tarball for version ${RocmBaseVersion} dated on or before ${RocmNightlyMaxDate}" >&2
      return 1
    fi
  else
    Selected="$(echo "${Available}" | tail -1)"
  fi

  echo "${Selected}"
}

function doInstall() {
  local Tarball
  Tarball="$(resolveTarball)"
  local Url="${RocmNightlyBaseUrl}/${Tarball}"

  echo "Installing ROCm nightly: ${Tarball}"
  echo "  from: ${Url}"
  echo "  into: ${RocmInstallDir}"

  local TmpDir
  TmpDir="$(mktemp -d)"
  # shellcheck disable=SC2064
  trap "rm -rf '${TmpDir}'" EXIT

  # The tarball can be multiple GB; allow up to 30 minutes for the transfer.
  curl "${CurlOpts[@]}" --max-time 1800 -o "${TmpDir}/${Tarball}" "${Url}"
  mkdir -p "${RocmInstallDir}"
  tar -xf "${TmpDir}/${Tarball}" -C "${RocmInstallDir}"

  # Record what was actually installed. TheRock's own /opt/rocm/.info/version
  # only carries the base version (e.g. 7.14.0), which cannot distinguish
  # nightlies. The trailing 8 digits of the tarball name are the build date.
  local Date="${Tarball: -15:8}"
  local InfoDir="${RocmInstallDir}/.info"
  if [ ! -d "${InfoDir}" ]; then
    echo "error: expected ${InfoDir} to exist after extraction; the tarball may be incomplete or have an unexpected layout" >&2
    return 1
  fi
  {
    echo "tarball=${Tarball}"
    echo "url=${Url}"
    echo "base_version=${RocmBaseVersion}"
    echo "gfx=multiarch"
    echo "date=${Date}"
  } >"${InfoDir}/nightly"

  echo "ROCm nightly install complete."
}

doInstall
