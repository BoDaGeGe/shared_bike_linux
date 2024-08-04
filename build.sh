#! /bin/bash

# 脚本的目录路径
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROTOBUF_DIR="${SCRIPT_DIR}/protobuf"
BUILD_DIR="${SCRIPT_DIR}/build"

# 确保脚本在项目根目录下运行
cd "$SCRIPT_DIR" || exit 1

# 检查 Protobuf 是否已克隆
if [ ! -d "$PROTOBUF_DIR" ]; then
    echo "Protobuf directory not found. Cloning repository..."
    git clone https://github.com/protocolbuffers/protobuf.git "$PROTOBUF_DIR"

    # 进入 Protobuf 目录，更新子模块
    cd "$PROTOBUF_DIR" || exit 1
    git submodule update --init --recursive
    ./autogen.sh
    ./configure
    make
    sudo make install

    # 返回到项目根目录
    cd "$SCRIPT_DIR" || exit 1
else
    echo "Protobuf directory already exists. Skipping clone."
fi

# 检查 Build 目录是否已创建
if [ ! -d "$BUILD_DIR" ]; then
    echo "Build directory not found. Make it."
    mkdir -p "$BUILD_DIR"
else
    echo "Build directory already exists."
    rm -rf $BUILD_DIR/*
fi

# 进入构建目录
cd "$BUILD_DIR" || exit 1

# 配置 CMake 项目
cmake ..
echo "Cmake Success !!!"

# 编译项目
make -j4
echo "make Success !!!"

