#!/bin/bash
set -e

VERSION=$1
if [ -z "$VERSION" ]; then
  echo "usage: ./release.sh v1.0.0"
  exit 1
fi

# build for each platform
just build
# or whatever your build command is

# create release and upload
gh release create $VERSION \
  dist/velora-linux-amd64 \
  dist/velora-linux-arm64 \
  dist/velora-darwin-arm64 \
  --title "$VERSION" \
  --generate-notes
