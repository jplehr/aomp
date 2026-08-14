#!/usr/bin/env python3

"""
run.py - Developer helper for the manylinux buildbot docker images.

Quickly set up an environment from one of the provided image directories
(manylinux-build-only, manylinux-hip-tpl, etc) for local debugging / reproduction.

Examples:
  # Download the base Dockerfile + helper scripts
  python run.py manylinux-build-only --pull

  # Build the base image, the target image, and run a container
  python run.py manylinux-build-only --build

  # Remove the selected container
  python run.py manylinux-build-only --clean --name test-manylinux-build-only-12345

  # Remove the selected container and images
  python run.py manylinux-build-only --clean-all --name test-manylinux-build-only-12345
"""

import argparse
import random
import shutil
import subprocess
import sys
import urllib.request
from pathlib import Path

SCRIPT_DIR = Path(__file__).resolve().parent

BASE_IMAGE = "localhost/manylinux:base"
BASE_DOCKERFILE = "build_manylinux_x86_64.Dockerfile"
THEROCK_LINK = "https://raw.githubusercontent.com/ROCm/TheRock/main/dockerfiles"
# Update this when onboarding a new buildbot image.
TARGETS = ["manylinux-build-only", "manylinux-hip-tpl", "manylinux-devel"]

# Host LLVM source tree is mounted here in the container.
LLVM_MOUNT_TARGET = "/home/botworker/bbot/llvm-project"

# Necessary files.
BASE_FILES = [
    BASE_DOCKERFILE,
    "install_ccache.sh",
    "install_sccache.sh",
    "install_cmake.sh",
    "install_ninja.sh",
    "install_awscli.sh",
    "install_googletest.sh",
    "install_rust.sh",
    "install_patchelf.sh",
    "install_shared_pythons.sh",
]

GPU_RUN_FLAGS = [
    "--device=/dev/kfd",
    "--device=/dev/dri",
    "--group-add", "video",
    "--group-add", "render",
]


def log(msg):
    print(f"[run.py] {msg}", flush=True)


def run_cmd(cmd, check=True, capture=False):
    """Echo and run a command, streaming output. Returns CompletedProcess."""
    log("$ " + " ".join(cmd))
    return subprocess.run(cmd, check=check, text=True, capture_output=capture)


def require_docker():
    if shutil.which("docker") is None:
        sys.exit("error: 'docker' not found on PATH.")


def image_exists(tag):
    res = run_cmd(["docker", "images", "-q", tag], check=False, capture=True)
    return bool(res.stdout.strip())


def container_exists(name):
    res = run_cmd(
        ["docker", "ps", "-aq", "-f", f"name=^{name}$"],
        check=False,
        capture=True,
    )
    return bool(res.stdout.strip())


def get_target_dir(target):
    return SCRIPT_DIR / target


def base_context_dir(dest):
    return Path(dest).resolve() / "manylinux-base"


def generated_container_name(target):
    for _ in range(10):
        name = f"test-{target}-{random.randint(0, 99999):05d}"
        if not container_exists(name):
            return name
    sys.exit("error: failed to generate a unique container name")


def build_container_name(args, target):
    return args.name or generated_container_name(target)


def clean_container_name(args):
    if not args.name:
        sys.exit("error: --clean requires --name to specify the container to remove.")
    return args.name


def download(url, output_file, retries=3):
    last_err = None
    for attempt in range(1, retries + 1):
        try:
            with urllib.request.urlopen(url, timeout=60) as resp:
                output_file.write_bytes(resp.read())
            return
        except Exception as err:
            last_err = err
            log(f"attempt {attempt}/{retries} failed: {err}")
    sys.exit(f"error: failed to download {url}: {last_err}")


# Upper bound on Python's byte-compile parallelism during `make install`.
# CPython's Makefile hardcodes `compileall -j0`, which resolves to
# os.cpu_count(). On very-high-core hosts (e.g. 512) that fan-out deadlocks the
# multiprocessing pool: all workers sleep at 0% CPU forever and the base image
# build hangs. Docker CPU limits do NOT help (compileall reads the host count,
# ignoring cgroup/affinity). The only reliable fix is to cap the job count, so
# after pulling we patch install_shared_pythons.sh to rewrite the generated
# Makefile's `-j0` before `make install`. Override with DODEVEL_COMPILEALL_JOBS.
COMPILEALL_JOBS_CAP = 8
PYTHON_INSTALLER = "install_shared_pythons.sh"
# Anchor line in the upstream script; the cap is injected right after it (the
# build Makefile exists at this point, before `make install` runs).
PATCH_ANCHOR = '  make -j"$(nproc)" > build.log 2>&1'
PATCH_MARKER = "# dodevel: cap compileall parallelism"


def patch_python_installer(ctx):
    """Cap CPython's hardcoded `compileall -j0` in the pulled installer script.

    Idempotent and re-applied on every pull. Fails loudly if the anchor line is
    gone, which signals the upstream script changed and this patch needs review.
    """
    script = ctx / PYTHON_INSTALLER
    text = script.read_text()

    if PATCH_MARKER in text:
        return

    if PATCH_ANCHOR not in text:
        sys.exit(
            f"error: cannot patch {PYTHON_INSTALLER}: anchor line not found.\n"
            f"       The upstream script changed; review the compileall -j0 "
            f"deadlock workaround in run.py."
        )

    injection = (
        f"{PATCH_ANCHOR}\n"
        f"  {PATCH_MARKER} (-j0 deadlocks on high-core hosts); see run.py\n"
        f'  sed -i "s/-j0/-j${{DODEVEL_COMPILEALL_JOBS:-{COMPILEALL_JOBS_CAP}}}/g" Makefile'
    )
    script.write_text(text.replace(PATCH_ANCHOR, injection, 1))
    script.chmod(0o755)
    log(f"patched {PYTHON_INSTALLER}: capped compileall -j0 to "
        f"-j${{DODEVEL_COMPILEALL_JOBS:-{COMPILEALL_JOBS_CAP}}}")


def run_pull(args):
    ctx = base_context_dir(args.dest)
    ctx.mkdir(parents=True, exist_ok=True)

    log(f"Downloading base build context into {ctx}")
    for name in BASE_FILES:
        output_file = ctx / name
        url = f"{THEROCK_LINK}/{name}"

        log(f"fetch: {url}")
        download(url, output_file)

        if name.endswith(".sh"):
            output_file.chmod(0o755)

    patch_python_installer(ctx)
    log("Pull complete.")


def build_base_image(args):
    ctx = base_context_dir(args.dest)

    if args.rebuild_base or not image_exists(BASE_IMAGE):
        dockerfile = ctx / BASE_DOCKERFILE
        if not dockerfile.is_file():
            sys.exit(
                f"error: base image files were not found "
                f"in {ctx}.\n       Run a pull first, e.g. "
                f"python run.py {args.target} --pull --dest {args.dest}"
            )
        if args.rebuild_base:
            log(f"Rebuilding base image {BASE_IMAGE}")
        else:
            log(f"Building base image {BASE_IMAGE}")

        run_cmd([
            "docker", "build",
            "--progress=plain",
            "-t", BASE_IMAGE,
            "-f", str(dockerfile),
            str(ctx),
        ])
    else:
        log(f"Base image {BASE_IMAGE} already present.")


def run_build(args):
    require_docker()
    target_dir = get_target_dir(args.target)

    build_base_image(args)

    # Some targets (e.g. manylinux-devel used by dodevel) build their own final
    # image and only need the shared base; stop after the base is ready.
    if args.base_only:
        log("Base image ready (--base-only).")
        return

    image_tag = args.target
    log(f"Building target image {image_tag}")
    run_cmd([
        "docker", "build",
        "--progress=plain",
        "-t", image_tag,
        "-f", str(target_dir / "Dockerfile"),
        str(target_dir),
    ])
    log("Build complete.")

    name = build_container_name(args, args.target)
    if container_exists(name):
        log(f"Removing pre-existing container {name}")
        run_cmd(["docker", "rm", "-f", name], check=False)
    run_args = ["docker", "run", "-dit", "--network=host", "--name", name]

    if not args.no_gpu:
        run_args += GPU_RUN_FLAGS

    if args.llvm_src:
        src = Path(args.llvm_src).expanduser().resolve()
        if not src.is_dir():
            sys.exit(f"error: --llvm-src path not found or not a directory: {src}")
        run_args += ["-v", f"{src}:{LLVM_MOUNT_TARGET}"]

    run_args += [image_tag, "sleep", "infinity"]

    log(f"Starting container {name}")
    run_cmd(run_args)
    log("Container started.")
    log(f"Open a shell with: docker exec -it {name} bash")


def run_clean(args):
    require_docker()
    name = clean_container_name(args)

    log(f"Removing container {name}")
    run_cmd(["docker", "rm", "-f", name], check=False)

    if args.clean_all:
        log(f"Removing image {args.target}")
        run_cmd(["docker", "rmi", args.target], check=False)

        log(f"Removing base image {BASE_IMAGE}")
        run_cmd(["docker", "rmi", BASE_IMAGE], check=False)

    log("Clean complete.")


def build_parser():
    parser = argparse.ArgumentParser(
        description="Helper to set up manylinux buildbot docker images.",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog=__doc__,
    )

    parser.add_argument("target", choices=TARGETS, help="image type, e.g. manylinux-build-only")
    parser.add_argument("--pull", action="store_true",
                        help="download base Dockerfile + helper scripts")
    parser.add_argument("--build", action="store_true",
                        help="build images and create/run the container")
    parser.add_argument("--rebuild-base", action="store_true",
                        help="rebuild the base image even if it already exists")
    parser.add_argument("--base-only", action="store_true",
                        help="build only the shared base image, not the target "
                             "image or a container (targets like manylinux-devel "
                             "build their own final image)")
    parser.add_argument("--clean", action="store_true",
                        help="remove the selected container")
    parser.add_argument("--clean-all", action="store_true",
                        help="remove the selected container plus target and base images")
    parser.add_argument("--dest", default=".",
                        help="base build-context location (default: current dir)")
    parser.add_argument("--name", help="container name (generated during build, required for clean)")
    parser.add_argument("--no-gpu", action="store_true",
                        help="skip GPU device/group run flags (build)")
    parser.add_argument("--llvm-src", metavar="PATH",
                        help="mount a local LLVM source tree at "
                             "/home/botworker/bbot/llvm-project in the container")


    return parser


def main(argv=None):
    args = build_parser().parse_args(argv)

    if args.rebuild_base or args.base_only:
        args.build = True

    if args.clean_all:
        args.clean = True

    # By default, perform a pull + build.
    if not (args.pull or args.build or args.clean):
        args.pull = True
        args.build = True

    try:
        if args.clean:
            run_clean(args)
        if args.pull:
            run_pull(args)
        if args.build:
            run_build(args)
    except subprocess.CalledProcessError as err:
        sys.exit(f"error: step failed ({err}); aborting.")


if __name__ == "__main__":
    main()
