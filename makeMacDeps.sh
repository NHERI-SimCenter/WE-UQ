#!/bin/bash
set -euo pipefail

QT=/Users/fmckenna/Qt/6.10.1/macos
PREFIX=$(pwd)/DEPS
ARCH=""

while [[ $# -gt 0 ]]; do
    case "$1" in
        --arch|-a)
            ARCH="$2"
            if [[ "$ARCH" != "x86_64" && "$ARCH" != "arm64" ]]; then
                echo "Error: Invalid architecture '$ARCH'."
                echo "Supported architectures: x86_64, arm64"
                exit 1
            fi
            PREFIX="${PREFIX}_$ARCH"
            shift 2
            ;;
	
        *)
            echo "Unknown argument: $1"
            echo "Usage: $0 --arch <arm64|x86_64>"
            exit 1
            ;;
    esac
done

CMAKE_ARCH_FLAG=""
if [ -n "${ARCH}" ]; then
    CMAKE_ARCH_FLAG="-DCMAKE_OSX_ARCHITECTURES=${ARCH}"
fi

mkdir -p "$PREFIX"

if [ ! -d "VTK-9.6.2" ]; then
    echo "--- Cloning VTK 9.6.2 repository ---"
    git clone --branch v9.6.2 --depth 1 https://github.com/Kitware/VTK.git VTK-9.6.2
fi

cd VTK-9.6.2
mkdir -p build
cd build

cmake .. \
    -DCMAKE_INSTALL_PREFIX="$PREFIX" \
    -DCMAKE_PREFIX_PATH="$QT" \
    -DCMAKE_BUILD_TYPE=Release ${CMAKE_ARCH_FLAG} \
    -DBUILD_SHARED_LIBS=ON \
    -DVTK_GROUP_ENABLE_StandAlone=WANT \
    -DVTK_GROUP_ENABLE_Rendering=WANT \
    -DVTK_GROUP_ENABLE_Qt=WANT \
    -DVTK_QT_VERSION=6 \
    -DVTK_GROUP_ENABLE_MPI=DONT_WANT \
    -DVTK_USE_COCOA=ON

cmake --build . --config Release
cmake --install .

cd ../..

echo "Build complete!"
