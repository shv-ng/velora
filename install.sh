#!/bin/bash

BIN_PATH="$HOME/.local/bin"
mkdir -p "$BIN_PATH"

ARCH=$(uname -m)
case $ARCH in
  x86_64)  BINARY="velora-linux-amd64" ;;
  aarch64) BINARY="velora-linux-arm64" ;;
  *)       echo "unsupported arch: $ARCH"; exit 1 ;;
esac

curl -L "https://github.com/shv-ng/velora/releases/latest/download/$BINARY" -o "$BIN_PATH/velora"
chmod +x "$BIN_PATH/velora"

# detect shell and add to PATH
SHELL_NAME=$(basename "$SHELL")
case $SHELL_NAME in
  zsh)  RC="$HOME/.zshrc" ;;
  bash) RC="$HOME/.bashrc" ;;
  fish) RC="$HOME/.config/fish/config.fish" ;;
  *)    RC="$HOME/.profile" ;;
esac

if [[ ":$PATH:" != *":$BIN_PATH:"* ]] && ! grep -qF "$BIN_PATH" "$RC" 2>/dev/null; then
  echo "$LINE" >> "$RC"
  echo "added $BIN_PATH to PATH in $RC"
fi

echo "installed velora to $BIN_PATH/velora"
echo "restart your shell or run: source $RC"
