# `dodevel` development container

`dodevel` (in `aomp/bin`) turns a task name into a complete AOMP development
environment: it selects or creates a matching `llvm-project` worktree, builds a
shared development image once, and starts a persistent per-task container with
the source tree, tickets, GPU devices, and host credentials wired in.

## Base image

The development image is built `FROM localhost/manylinux:base`. That base cannot
be built from a single file: TheRock's `build_manylinux_x86_64.Dockerfile`
`COPY`s several `install_*.sh` helper scripts that live only in the TheRock repo.
The buildbot helper `run.py` downloads the Dockerfile plus those scripts and
builds the base image:

```bash
# builds only localhost/manylinux:base (downloads the TheRock context first)
aomp/upstream-buildbots/run.py manylinux-devel --base-only
```

You normally do not need to run this by hand: on first use `dodevel` builds the
base automatically (via `run.py --pull --base-only`) if it is missing, then
builds the `jp/aomp-devel` image (downloading a multiarch ROCm nightly).

## Usage

```bash
# Default task "amd-staging": uses ${Workspace}/llvm-project as-is
aomp/bin/dodevel

# Named task: uses (or creates) ${Workspace}/llvm-project-<task> on branch <task>
aomp/bin/dodevel feat-to-impl
```

For a named task, the worktree/branch is resolved as:

- reuse `llvm-project-<task>` if it already exists (must be on branch `<task>`);
- otherwise create it from an existing local branch `<task>`, a matching remote
  branch, or a new branch off local `amd-staging`.

Container lifecycle per task (`jp-<task>`):

- first call: creates and enters the container;
- later call, stopped: `docker start -ai`;
- later call, running: `docker exec` into it.

Containers are reusable (no `--rm`). Run `dodevel <task>` again any time to
re-enter.

## Environment overrides

```bash
# Pin an exact ROCm nightly instead of the weekly Sunday anchor
ROCM_NIGHTLY_DATE=20260809 aomp/bin/dodevel

# Force an image rebuild after editing the Dockerfile or prerequisites
DODEVEL_REBUILD=1 aomp/bin/dodevel
```

## What gets mounted

- `~/git` and the workspace root, read-write at identical paths;
- `/work1/$USER/tickets`, when present;
- `~/.load-claude.sh` (read-only), `~/.claude` state, and `~/.a-tokens`
  (read-only) for Claude;
- the SSH agent socket if available, otherwise `~/.ssh` read-only.

No secrets are baked into the image; all credentials are provided at runtime.
