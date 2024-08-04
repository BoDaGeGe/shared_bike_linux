cmake_minimum_required(VERSION 3.10)

# 设置项目名称
project(shared_bike)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 设置源文件
file(GLOB SOURCES
    "src/*.cpp"
    "log4cpp/*.cpp"
    "ini/*.cpp"
    "threadpool/*.cpp"
)

# 设置头文件目录
include_directories(
    include
    third/include
    log4cpp
    ini
    threadpool
    # 如果有其他的include路径，添加到这里
)

# 添加静态库目录
link_directories(third/lib/iniparser
		 third/lib/log4cpp
)

# 添加编译选项
# add_compile_options(-Wall -Wextra -pedantic -O2)

# 添加可执行文件
add_executable(shared_bike ${SOURCES})

# 添加链接库
target_link_libraries(shared_bike
			libiniparser
			liblog4cpp
)

# 添加外部库的路径
link_directories(libiniparser
		   liblog4cpp
)

