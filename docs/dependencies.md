# Runner Dependencies

This project assumes you're running on a self-hosted GitHub Actions runner. You'll need to install build dependencies on the runner machine first.

## Setup

Run this on your Debian/Ubuntu runner:

```bash
./scripts/deps.sh
```

## What it installs

- Build tools: `build-essential`, `cmake`, `git`
- Linting: `cppcheck`, `clang-format`

## Why not install in CI?

Installing dependencies during each workflow run is slow and can fail due to network issues. Pre-installing them on the runner is faster and more reliable.

## Manual install

```bash
sudo apt-get update
sudo apt-get install -y build-essential cmake git cppcheck clang-format
```