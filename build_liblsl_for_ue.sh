#!/bin/bash

# This script builds liblsl.so using the same toolchain as Unreal Engine
# to ensure ABI compatibility.

set -e

GREEN='\033[0;32m'
NC='\033[0m' # No Color

# 1. Prepare the environment
echo "--- Preparing environment ---"

# Find Unreal Engine installations
echo "Searching for Unreal Engine installations..."
ue_installs=($(find /home/$whoami /opt -name "Build.version" -print 2>/dev/null | sed 's|/Engine/Build/Build.version||'))

if [ ${#ue_installs[@]} -eq 0 ]; then
    echo "No Unreal Engine installations found."
    exit 1
fi

echo ""

# Present a menu for the user to select an installation
echo "Please select an Unreal Engine installation:"
select ue_root in "${ue_installs[@]}"; do
    if [ -n "$ue_root" ]; then
        break
    else
        echo "Invalid selection."
    fi
done

echo ""

export UE_ROOT=$ue_root
echo -e "UE_ROOT set to: ${GREEN}${UE_ROOT}${NC}"

# Find the Clang version
clang_versions=($(find "${UE_ROOT}/Engine/Extras/ThirdPartyNotUE/SDKs/HostLinux/Linux_x64/" -maxdepth 1 -type d -name "v*"))

if [ ${#clang_versions[@]} -eq 0 ]; then
    echo "No Clang toolchain found in the selected Unreal Engine installation."
    exit 1
fi

echo ""

# Present a menu for the user to select a Clang version
echo "Please select a Clang version:"
select clang_version_dir in "${clang_versions[@]}"; do
    if [ -n "$clang_version_dir" ]; then
        break
    else
        echo "Invalid selection."
    fi
done

echo ""

# Find the native x86_64 clang executable to avoid cross-compilers
echo "Searching for native (x86_64) clang compiler in $clang_version_dir..."
clang_path=$(find "$clang_version_dir" -path "*x86_64-unknown-linux-gnu/bin/clang" -type f -executable | head -n 1)

# Fallback to the old method if the specific path isn't found
if [ -z "$clang_path" ]; then
    echo "Could not find native x86_64 compiler, searching for any clang executable..."
    clang_path=$(find "$clang_version_dir" -name "clang" -type f -executable | head -n 1)
fi

if [ -z "$clang_path" ]; then
    echo "Error: Could not find the 'clang' executable in $clang_version_dir"
    exit 1
fi

# The SDK root is the parent directory of the 'bin' directory containing clang
clang_bin_dir=$(dirname "$clang_path")
toolchain_dir=$(dirname "$clang_bin_dir")

export UE_CLANG_SDK_ROOT=$toolchain_dir
echo -e "UE_CLANG_SDK_ROOT set to: ${GREEN}${UE_CLANG_SDK_ROOT}${NC}"

export PATH="${clang_bin_dir}:${PATH}"
export CC="${clang_bin_dir}/clang"
export CXX="${clang_bin_dir}/clang++"
export LD_LIBRARY_PATH="${toolchain_dir}/lib:${LD_LIBRARY_PATH}"

# CFLAGS only needs the sysroot to find the correct C library.
export CFLAGS="${SYSROOT_FLAG}"
# CXXFLAGS needs the stdlib and include path.
export CXXFLAGS="${SYSROOT_FLAG} -stdlib=libc++ -I${UE_ROOT}/Engine/Source/ThirdParty/Unix/LibCxx/include/c++/v1"

# LDFLAGS will contain all linker-specific settings.
# -Wl,-rpath-link is a more robust way to tell the linker where to find libraries at link time.
# -lm is added to explicitly link the math library, which is required by some toolchains.
LIBCXX_PATH="${UE_ROOT}/Engine/Source/ThirdParty/Unix/LibCxx/lib/Unix/x86_64-unknown-linux-gnu"
export LDFLAGS="${SYSROOT_FLAG} -L${LIBCXX_PATH} -Wl,-rpath-link,${LIBCXX_PATH} -lc++ -lc++abi -lm"

echo "Environment variables set successfully."

# 2. Create a clean build directory
echo "--- Cleaning and creating build directory ---"
rm -rf build_ue
mkdir build_ue
cd build_ue

# 3. Configure the build with CMake
# We pass CFLAGS for the C compiler, CXXFLAGS for the C++ compiler,
# and the dedicated LDFLAGS for the linker. This is the clean, correct approach.
echo "--- Configuring with CMake ---"
cmake .. \
  -G "Unix Makefiles" \
  -DCMAKE_C_COMPILER="$CC" \
  -DCMAKE_CXX_COMPILER="$CXX" \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_C_FLAGS="${CFLAGS}" \
  -DCMAKE_CXX_FLAGS="${CXXFLAGS}" \
  -DCMAKE_EXE_LINKER_FLAGS="${LDFLAGS}" \
  -DCMAKE_SHARED_LINKER_FLAGS="${LDFLAGS}" \
  -DLSL_OPTIMIZATIONS=OFF \
  -DLSL_BUILD_STATIC=OFF \
  -DCMAKE_INSTALL_PREFIX="$(pwd)/../install"






# 4. Build the library
echo "--- Building liblsl.so ---"
make -j$(nproc)

# 5. Copy the final library to the destination directory
echo ""
echo "--- Copying library ---"

read -p "Enter destination path to copy the libraries (e.g. /home/user/MyProject/Plugins/plugin-UE4/Binaries/Linux/), or press Enter to skip: " USER_DEST_PATH

if [ -n "$USER_DEST_PATH" ]; then
    echo "Copying libraries to ${USER_DEST_PATH}..."
    mkdir -p "${USER_DEST_PATH}"
    cp -v liblsl.so* "${USER_DEST_PATH}/"
    echo "--- Copy complete. ---"
else
    GREEN='\033[0;32m'
    NC='\033[0m' # No Color
    echo -e "${GREEN}Skipping copy step.${NC}"
    echo -e "${GREEN}ACTION REQUIRED${NC}: Manually copy the built libraries."
    echo "From: $(pwd)"
    echo "Files: liblsl.so*"
    echo "To: Your plugin's binary folder. The default path would be:"
    echo "    MyProject/Plugins/plugin-UE4/Binaries/Linux/"
fi

echo "--- Build complete. ---"
