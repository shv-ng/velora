#!/bin/bash
set -e

VERSION=${1:-$(git describe --tags --always)}
COMMIT=$(git rev-parse --short HEAD)
DATE=$(date +%Y-%m-%d)

# download llvm 22 with static libs
LLVM_VER="22.1.8"
if [ ! -d "llvm" ]; then
  echo "Downloading LLVM $LLVM_VER..."
  curl -L "https://github.com/llvm/llvm-project/releases/download/llvmorg-${LLVM_VER}/LLVM-${LLVM_VER}-Linux-X64.tar.xz" -o llvm.tar.xz
  mkdir -p llvm
  tar -xf llvm.tar.xz --strip-components=1 -C llvm
  rm llvm.tar.xz
fi

just build
