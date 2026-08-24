# Velora
A programming language.

> **Platform support**
> - Linux x86_64 — supported
> - Linux arm64 — supported  
> - macOS — untested
> - Windows — not supported

## Requirements

- llvm-18
- clang

## Install

Download the latest binary from [releases](https://github.com/shv-ng/velora/releases).

### Linux
```bash
curl https://raw.githubusercontent.com/shv-ng/velora/refs/heads/main/install.sh | bash
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
