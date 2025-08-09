#!/usr/bin/env bash
# generate.sh -- Standalone CMake configuration generator for SparkEngine

set -e

GEN="Unix Makefiles"
CONFIG=Debug

# Parse CLI args
while [[ $# -gt 0 ]]; do
    case "$1" in
        release|Release) CONFIG=Release ;;
        debug|Debug)     CONFIG=Debug ;;
        -g) GEN="$2"; shift ;;
        *) echo "Usage: $0 [debug|release] [-g <Generator>]" ; exit 1 ;;
    esac
    shift
done

mkdir -p build
cd build

echo "[*] Configuring CMake with generator '$GEN' ($CONFIG)..."
cmake .. -G "$GEN" -DCMAKE_BUILD_TYPE=$CONFIG

cd ..
echo "[*] CMake configuration/project files generated."