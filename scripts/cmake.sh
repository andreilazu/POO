#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="${ROOT}/build"
GENERATOR=""
CONFIG=""
TOOLCHAIN="${VCPKG_ROOT:-}/scripts/buildsystems/vcpkg.cmake"

usage() {
    echo "Usage: $0 configure|build [-g Ninja|\"Visual Studio 17 2022\"] [--config Debug|Release]"
    exit 1
}

cmd="${1:-}"
shift || true

while [[ $# -gt 0 ]]; do
    case "$1" in
        -g) GENERATOR="$2"; shift 2 ;;
        --config) CONFIG="$2"; shift 2 ;;
        *) usage ;;
    esac
done

if [[ "$GENERATOR" == *"Visual Studio"* ]]; then
    BUILD_DIR="${ROOT}/build-vs"
fi

case "$cmd" in
    configure)
        args=(-S "$ROOT" -B "$BUILD_DIR")
        if [[ -n "$GENERATOR" && "$GENERATOR" != *"Visual Studio"* ]]; then
            args+=(-DCMAKE_BUILD_TYPE=Debug)
        fi
        [[ -n "$GENERATOR" ]] && args+=(-G "$GENERATOR")
        if [[ "$GENERATOR" == *"Visual Studio"* ]]; then
            args+=(-A x64)
        fi
        # VS generator: do not pass vcpkg toolchain (VS injects it from vcpkg.json).
        if [[ -f "$TOOLCHAIN" && "$GENERATOR" != *"Visual Studio"* ]]; then
            args+=(-DCMAKE_TOOLCHAIN_FILE="$TOOLCHAIN")
        fi
        cmake "${args[@]}"
        ;;
    build)
        build_args=(--build "$BUILD_DIR")
        [[ -n "$CONFIG" ]] && build_args+=(--config "$CONFIG")
        cmake "${build_args[@]}"
        ;;
    *)
        usage
        ;;
esac
