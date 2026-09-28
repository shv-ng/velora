# Velora
A programming language.

> **Platform support**
> - Linux x86_64 — supported
> - Linux arm64 — supported  
> - macOS — untested
> - Windows — supported via WSL

## Requirements

- [clang](https://clang.llvm.org/) or [gcc](https://gcc.gnu.org/) required for linking output binaries"

## Install

Download the latest binary from [releases](https://github.com/shv-ng/velora/releases). 

### Linux or Windows (WSL)

```bash
curl -fsSL https://raw.githubusercontent.com/shv-ng/velora/refs/heads/main/install.sh | bash
```

## Usage

```bash
$ velora
usage:
    velora <command> [arguments]

commands:
    run <path>       compile and run program
    build <path>     compile program
    help             print this msg
    version          print version

arguments:
    -h, --help       print this msg
    -v, --version    print version
```

