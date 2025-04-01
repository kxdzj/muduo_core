#!/bin/bash

set -e  # 任何命令失败时立即退出

# 获取当前脚本所在目录的绝对路径
SCRIPT_DIR=$(cd "$(dirname "$0")" && pwd)
BUILD_DIR="${SCRIPT_DIR}/build"

# 清理并创建构建目录
echo "Setting up build directory: ${BUILD_DIR}"
if [ -d "${BUILD_DIR}" ]; then
    echo "Removing existing build directory..."
    rm -rf "${BUILD_DIR}"
fi
mkdir -p "${BUILD_DIR}"

# 构建项目
echo "Configuring and building project..."
cd "${BUILD_DIR}"
cmake ..
make
echo "Build completed successfully."
cd ..

# 安装头文件
INCLUDE_DIR="/usr/include/myMuduo"
if [ ! -d "${INCLUDE_DIR}" ]; then
    mkdir -p "${INCLUDE_DIR}"
fi

for header in ./include/*.h; do
    cp "$header" "${INCLUDE_DIR}"
done

# 安装动态库
cp ./lib/libMymuduo.so /usr/lib

# 刷新动态库缓存
ldconfig

echo "Installation completed successfully."
