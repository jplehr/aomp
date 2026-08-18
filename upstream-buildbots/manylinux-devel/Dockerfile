# Interactive AOMP development image, based on TheRock manylinux.
#
# Unlike the sibling manylinux-build-only / manylinux-hip-tpl images (which are
# CI/CD build images), this image is meant for a developer to work inside: it
# adds the missing development headers, a multiarch ROCm install, and Claude
# Code, and runs as the invoking host user so bind-mounted trees keep correct
# ownership.
#
# To use this dockerfile, please first build the base image as
# localhost/manylinux:base with the buildbot helper, which downloads TheRock's
# base Dockerfile plus its install_*.sh helper scripts and builds only the base:
#   ../run.py manylinux-devel --base-only
#
# This image is normally built via aomp/bin/dodevel, which builds the base (via
# run.py) if missing and wires the build args below from the host. See that
# script for the canonical invocation.
FROM localhost/manylinux:base

# Install the confirmed-missing development packages (see prerequisites for the
# rationale on why -devel packages are added here despite the base's stance).
COPY prerequisites /tmp/prerequisites
RUN dnf install -y $(grep -vE '^\s*#|^\s*$' /tmp/prerequisites) && \
    dnf clean all && \
    rm -f /tmp/prerequisites

# Install ROCm from a TheRock nightly "multiarch" dist tarball into /opt/rocm,
# so one development image works across every lab GPU architecture. AOMP build
# scripts assume a host ROCm at /opt/rocm, so this is a hard invariant of the
# image.
#
# Version selection (see install-rocm-nightly.sh for full precedence):
#   ROCM_NIGHTLY_MAX_DATE  newest nightly on-or-before this YYYYMMDD anchor.
#                          dodevel passes the most recent US-central Sunday so
#                          the pinned nightly bumps once a week while the build
#                          layer stays cached Mon..Sat.
#   ROCM_NIGHTLY_DATE      exact pin (overrides the anchor).
#   both empty             newest available nightly (manual/debug builds).
ARG ROCM_BASE_VERSION=7.14
ARG ROCM_NIGHTLY_DATE=
ARG ROCM_NIGHTLY_MAX_DATE=

COPY install-rocm-nightly.sh /tmp/install-rocm-nightly.sh
RUN ROCM_BASE_VERSION="${ROCM_BASE_VERSION}" \
    ROCM_NIGHTLY_DATE="${ROCM_NIGHTLY_DATE}" \
    ROCM_NIGHTLY_MAX_DATE="${ROCM_NIGHTLY_MAX_DATE}" \
    bash /tmp/install-rocm-nightly.sh && \
    rm -f /tmp/install-rocm-nightly.sh

# ROCm library and tool paths for offload build/execution.
ENV LD_LIBRARY_PATH=/opt/rocm/lib:${LD_LIBRARY_PATH}
ENV PATH=/opt/rocm/bin:${PATH}

# Runtime credential/environment wiring. Sourced by every login shell (both the
# initial `docker run` shell and later `docker exec ... --login` shells), so
# credentials and the ~/.claude symlink are set up identically in each. All
# inputs are provided at runtime by dodevel; nothing secret is baked in.
#   - DODEVEL_CLAUDE_TARGET: host ~/.claude symlink target, bind-mounted at the
#     identical path; recreate the symlink in the fresh container home.
#   - ~/.load-claude.sh: host-owned credential/env script, mounted read-only.
COPY profile.d-dodevel.sh /etc/profile.d/zz-dodevel.sh

# Convenience wrapper: run the AOMP build from the task's aomp worktree
# ($AOMP_SRC/bin) rather than whichever aomp/bin the user happens to cd into.
COPY dobuild /usr/local/bin/dobuild
RUN chmod +x /usr/local/bin/dobuild

# Install Node.js (absent from the base) and then Claude Code globally.
RUN dnf module reset -y nodejs && \
    dnf module enable -y nodejs:22 && \
    dnf install -y nodejs npm && \
    dnf clean all && \
    npm install -g @anthropic-ai/claude-code && \
    claude --version

# Map the runtime user to the invoking host user so bind-mounted trees keep
# correct ownership, and put the user in the GPU access groups. Follows the
# openmp-ci docker/Dockerfile user-mapping pattern.
ARG BUILD_USER=devuser
ARG BUILD_UID=1000
ARG BUILD_GID=1000
ARG RENDER_GID=109
ARG VIDEO_GID=44

# Create (or reuse) the GPU groups at the host GIDs so /dev/kfd and /dev/dri are
# accessible. Reuse whatever group already owns the GID if one exists.
RUN set -eux; \
    if ! getent group "${RENDER_GID}" >/dev/null; then \
      groupadd --gid "${RENDER_GID}" render_host; \
    fi; \
    if ! getent group "${VIDEO_GID}" >/dev/null; then \
      groupadd --gid "${VIDEO_GID}" video_host; \
    fi

# Create the runtime user. If BUILD_GID already exists, reuse that group as the
# primary group; otherwise create a matching group.
RUN set -eux; \
    if getent group "${BUILD_GID}" >/dev/null; then \
      PRIMARY="$(getent group "${BUILD_GID}" | cut -d: -f1)"; \
    else \
      groupadd --gid "${BUILD_GID}" "${BUILD_USER}"; \
      PRIMARY="${BUILD_USER}"; \
    fi; \
    RENDER_GRP="$(getent group "${RENDER_GID}" | cut -d: -f1)"; \
    VIDEO_GRP="$(getent group "${VIDEO_GID}" | cut -d: -f1)"; \
    useradd --create-home --uid "${BUILD_UID}" --gid "${PRIMARY}" \
      -G "${RENDER_GRP},${VIDEO_GRP}" --shell /bin/bash "${BUILD_USER}"

USER ${BUILD_USER}
WORKDIR /home/${BUILD_USER}
CMD ["/bin/bash", "--login"]
