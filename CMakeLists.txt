cmake_minimum_required(VERSION 3.10)

# 设置项目名称
project(shared_bike)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 添加编译选项
add_compile_options(-Wall -pthread)

# 设置源文件
file(GLOB SOURCES
    "src/*.cpp"
    "log4cpp/*.cpp"
    "ini/*.cpp"
    "threadpool/*.cpp"
    "proto/*.cpp"
)

# 设置头文件目录
include_directories(
    include
    third/include
    log4cpp
    ini
    threadpool
	proto
    ${MYSQL_INCLUDE_DIR}
    #${Protobuf_HEADER_DIR}
    # 如果有其他的include路径，添加到这里
)

# 添加静态库目录
link_directories(third/lib/iniparser
                         third/lib/log4cpp
                         ${MYSQL_LIB_DIR}
                         #${Protobuf_LIB_DIR}
)

# 添加编译选项
# add_compile_options(-Wall -Wextra -pedantic -O2)

# 添加可执行文件
add_executable(shared_bike ${SOURCES})

# 查找 Protobuf 包
#find_package(Protobuf REQUIRED)

# 添加链接库
target_link_libraries(shared_bike
			iniparser
			log4cpp
			pthread
			event
			protobuf
			${MYSQL_LIB_DIR}/libmysqlclient.so
			${MYSQL_LIB_DIR}/libmysqlclient.a
			#${Protobuf_LIBRARIES}/libprotobuf.so
			#${Protobuf_LIB_DIR}/libprotobuf.so
			#${Protobuf_LIB_DIR}/libprotobuf.a
)



# 查找 MySQL 库
#find_package(MySQL REQUIRED)
#include_directories(${MySQL_INCLUDE_DIR})
#target_link_libraries(shared_bike ${MySQL_LIBRARIES})


# 添加外部库的路径
#link_directories(libiniparser
#		   liblog4cpp
#)

