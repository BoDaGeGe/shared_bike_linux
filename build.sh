#! /bin/bash

# 脚本的目录路径
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROTOBUF_DIR="${SCRIPT_DIR}/protobuf"
BUILD_DIR="${SCRIPT_DIR}/build"

# 确保脚本在项目根目录下运行
cd "$SCRIPT_DIR" || exit 1



# 检查 MySQL 客户端库是否已经安装
if dpkg -l | grep -i "mysql-client"; then
    echo "MySQL client library is already installed."
else
    echo "MySQL client library is not installed."
    echo "Installing MySQL client library..."

    # 更新包列表并安装 MySQL 客户端开发库
    sudo apt-get update
    sudo apt-get install -y libmysqlclient-dev

    echo "MySQL client library installed."
fi

# 检查 MySQL 服务器是否已经安装
if dpkg -l | grep -i "mysql-server"; then
    echo "MySQL server is already installed."
else
    echo "MySQL server is not installed."
    echo "Installing MySQL server..."

    # 更新包列表并安装 MySQL 服务器
    sudo apt-get update
    sudo apt-get install -y mysql-server

    echo "MySQL server installed."
fi

# 确保 MySQL 服务正在运行
echo "Starting MySQL service..."
sudo systemctl start mysql
sudo systemctl enable mysql

# 输出 MySQL 状态
echo "MySQL service status:"
sudo systemctl status mysql

# 使用 find 命令搜索 libmysqlclient.so 文件的位置
MYSQL_LIB_PATH=$(find /usr -name libmysqlclient.so 2>/dev/null | head -n 1)

# 检查是否找到 mysql.h 文件
if [ -z "$MYSQL_LIB_PATH" ]; then
    echo "libmysqlclient.so not found."
    exit 1
fi

# 提取包含 libmysqlclient.so 的目录路径
MYSQL_LIB_DIR=$(dirname "$MYSQL_LIB_PATH")

# 输出安装路径
echo "MySQL client library can be found at ${MYSQL_LIB_DIR}"

# 使用 find 命令搜索 mysql.h 文件的位置
MYSQL_HEADER_PATH=$(find /usr -name mysql.h 2>/dev/null | head -n 1)

# 检查是否找到 mysql.h 文件
if [ -z "$MYSQL_HEADER_PATH" ]; then
    echo "mysql.h not found."
    exit 1
fi

# 提取包含 mysql.h 的目录路径
MYSQL_INCLUDE_DIR=$(dirname "$MYSQL_HEADER_PATH")

echo "Found mysql.h in: ${MYSQL_INCLUDE_DIR}"



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

# 使用 find 命令搜索 libprotobuf.so 文件的位置
Protobuf_LIB_PATH=$(find /usr -name libprotobuf.* 2>/dev/null | head -n 1)

# 检查是否找到 mysql.h 文件
if [ -z "$Protobuf_LIB_PATH" ]; then
    echo "libprotobuf.so not found."
    exit 1
fi

# 提取包含 libprotobuf.so 的目录路径
Protobuf_LIB_DIR=$(dirname "$Protobuf_LIB_PATH")

# 输出安装路径
echo "Protobuf library can be found at ${Protobuf_LIB_DIR}"

# 使用 find 命令搜索 Protobuf 头文件的位置
Protobuf_HEADER_PATH=$(find /usr -name protobuf* 2>/dev/null | head -n 1)

# 检查是否找到 Protobuf 头文件
if [ -z "$Protobuf_HEADER_PATH" ]; then
    echo "Protobuf_HEADER not found."
    exit 1
fi

# 提取包含 mysql.h 的目录路径
Protobuf_HEADER_DIR=$(dirname "$Protobuf_HEADER_PATH")

echo "Found mysql.h in: ${Protobuf_HEADER_DIR}"




# 检查 Build 目录是否已创建
if [ ! -d "$BUILD_DIR" ]; then
    echo "Build directory not found. Make it."
    mkdir -p "$BUILD_DIR"
else
    echo "Build directory already exists."
    rm -rf $BUILD_DIR/*
	cd ..
fi

# 进入构建目录
cd "$BUILD_DIR" || exit 1

# 使用 cmake 并传递 MYSQL_INCLUDE_DIR 和 MYSQL_LIB_DIR 变量   并且编译
cmake -DMYSQL_INCLUDE_DIR="${MYSQL_INCLUDE_DIR}" -DMYSQL_LIB_DIR="${MYSQL_LIB_DIR}" -DProtobuf_LIB_DIR="${Protobuf_LIB_DIR}" -DProtobuf_HEADER_DIR="${Protobuf_HEADER_DIR}"  ..
echo "Cmake Success !!!"

# 编译项目
make -j4
if [ $? -eq 0 ]; then
    echo "make Success !!!"
else
    echo "make failed. Check the errors above."
    exit 1
fi

