#!/usr/bin/env bash
# shellcheck shell=bash
#
# Print the ROCm nightly anchor date as YYYYMMDD: the most recent Sunday
# (inclusive of today) in US-central time.
#
# Rationale: the openmp-ci wrappers run `docker build` on every invocation and
# rely on the layer cache. Passing this anchor as a build-arg keeps the value
# constant Monday..Saturday (so the ROCm install layer stays cached) and only
# changes on Sunday, which busts the cache and bumps the pinned nightly once a
# week. The choice of Sunday must be evaluated in a single fixed timezone so
# hosts in different timezones agree on the week boundary; we use US-central
# (America/Chicago) regardless of the host clock.

set -euo pipefail

CentralTz="America/Chicago"

# Fail loudly if the tz database lacks the central zone, rather than silently
# falling back to UTC and computing "Sunday" against the wrong clock.
if ! TZ="${CentralTz}" date >/dev/null 2>&1 || \
   [ ! -e "/usr/share/zoneinfo/${CentralTz}" ]; then
  echo "error: timezone '${CentralTz}' is not available; install tzdata so the weekly anchor is computed in US-central time" >&2
  exit 1
fi

# Day-of-week in central time: 0=Sunday..6=Saturday. Subtract that many days
# from today's central date to land on (or stay at) the most recent Sunday.
Dow="$(TZ="${CentralTz}" date +%w)"
TZ="${CentralTz}" date -d "today - ${Dow} days" +%Y%m%d
