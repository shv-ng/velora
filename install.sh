#!/bin/bash

ARCH=$(uname -m)
case $ARCH in
  x86_64)  BINARY="velora-linux-amd64" ;;
  aarch64) BINARY="velora-linux-arm64" ;;
  *)       echo "unsupported arch: $ARCH"; exit 1 ;;
esac

curl -L "https://github.com/shv-ng/velora/releases/latest/download/$BINARY" -o $HOME/.local/bin/velora
chmod +x $HOME/.local/bin/velora
