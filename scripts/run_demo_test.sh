#!/usr/bin/bash
set -euo pipefail

BUILD_DIR="${1:-build}"
EXE_NAME="${2:-pikmin_swarm}"

find_exe() {
    find "${BUILD_DIR}" \( -name "${EXE_NAME}.exe" -o -name "${EXE_NAME}" \) \
        ! -path '*/CMakeFiles/*' -type f 2>/dev/null | head -1
}

EXE="$(find_exe)"
if [[ -z "${EXE}" ]]; then
    echo "Executable not found under ${BUILD_DIR}"
    exit 1
fi

if command -v cygpath >/dev/null 2>&1; then
    EXE="$(cygpath -u "${EXE}")"
fi

EXE_DIR="$(cd "$(dirname "${EXE}")" && pwd)"
EXE_FILE="$(basename "${EXE}")"

if [[ -n "${VCPKG_ROOT:-}" ]] && [[ -d "${VCPKG_ROOT}/installed" ]]; then
    for TRIPLET in x64-windows x64-mingw-dynamic arm64-osx x64-osx; do
        VCPKG_BIN="${VCPKG_ROOT}/installed/${TRIPLET}/bin"
        if [[ -d "${VCPKG_BIN}" ]]; then
            cp "${VCPKG_BIN}"/*.dll "${EXE_DIR}/" 2>/dev/null || true
        fi
    done
fi

cd "${EXE_DIR}"
echo "Running demo: ${EXE_DIR}/${EXE_FILE}"

if [[ "${RUNNER_OS:-}" == "Windows" ]]; then
    cmd.exe /c "${EXE_FILE} --demo"
else
    "./${EXE_FILE}" --demo
fi
