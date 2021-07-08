CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
#指定项目名称
PROJECT(SERVER)
add_definitions(-std=c++11)
file(COPY config.txt DESTINATION . FOLLOW_SYMLINK_CHAIN)
file(COPY server.sh DESTINATION . FOLLOW_SYMLINK_CHAIN)
file(COPY build.sh DESTINATION . FOLLOW_SYMLINK_CHAIN)

#指定源文件目录
FILE(GLOB SRC_LIST_CPP ${PROJECT_SOURCE_DIR}/src/*.cpp)
SET(SOURCE ${SRC_LIST_CPP})

#指定头文件目录
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)

#指定链接库文件目录
LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/lib)

#生成可执行文件
ADD_EXECUTABLE(server ${SOURCE})

#链接库
TARGET_LINK_LIBRARIES(server x264 pthread dl v4l2wrapper config++ mysqlcppconn cpp_redis tacopie)