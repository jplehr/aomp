# shellcheck shell=bash
# Sourced by every login shell in the dodevel container. Wires host-provided
# credentials into the session and repairs the ~/.claude symlink in the fresh
# container home. All inputs come from runtime bind mounts set up by dodevel;
# no secret is baked into the image.

# Recreate the ~/.claude symlink so Claude finds its state. On the host ~/.claude
# is a symlink whose target dodevel bind-mounts read-write at the identical path
# (DODEVEL_CLAUDE_TARGET); the freshly useradd-created home has no such link.
if [ -n "${DODEVEL_CLAUDE_TARGET:-}" ] && [ ! -e "${HOME}/.claude" ]; then
  ln -s "${DODEVEL_CLAUDE_TARGET}" "${HOME}/.claude"
fi

# Source the host credential/environment script (Anthropic base URL, tokens,
# etc.). Mounted read-only by dodevel; ownership of token-to-env mapping stays
# with the host setup script.
if [ -r "${HOME}/.load-claude.sh" ]; then
  # shellcheck source=/dev/null
  . "${HOME}/.load-claude.sh"
fi
