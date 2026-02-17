cmake_minimum_required(VERSION 3.16...3.21)
project(obs-text-slideshow VERSION 2.0.0)

# 启用 Qt 自动处理
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# 定义插件模块
add_library(${CMAKE_PROJECT_NAME} MODULE)

# [关键点 1] 生成插件宏文件
# 之前的报错显示找不到 plugin-macros.generated.h
# 我们在构建时根据 .in 模板生成它，并放在 src 目录下
if(EXISTS "${CMAKE_SOURCE_DIR}/src/plugin-macros.h.in")
    message(STATUS "Generating plugin-macros.generated.h from template")
    configure_file(
        "${CMAKE_SOURCE_DIR}/src/plugin-macros.h.in" 
        "${CMAKE_SOURCE_DIR}/src/plugin-macros.generated.h"
    )
endif()

# [关键点 2] 包含路径设置
# 包含 src：为了找到生成的 .generated.h
# 包含 EXTERNAL_INCLUDE_DIR：为了找到 obs-module.h 等 SDK 头文件
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE 
    ${CMAKE_SOURCE_DIR}/src
    ${EXTERNAL_INCLUDE_DIR}
)

# 核心定义
add_definitions(
    -DUNICODE 
    -D_UNICODE 
    -D_CRT_SECURE_NO_WARNINGS 
    -DMS_PTHREADS 
    -DHAVE_OBSCONFIG_H
)

# 查找 Qt6 环境（由 Workflow 中的 env.Qt6_DIR 提供）
find_package(Qt6 COMPONENTS Widgets Core REQUIRED)

# [关键点 3] 链接由 Workflow 动态生成的导入库 (.lib)
find_library(LIBOBS_LIB NAMES obs HINTS ${GENERATED_LIB_DIR} REQUIRED)
find_library(FRONTEND_API_LIB NAMES obs-frontend-api HINTS ${GENERATED_LIB_DIR} REQUIRED)
find_library(W32_PTHREADS_LIB NAMES ${PTHREAD_LIB_NAME} HINTS ${GENERATED_LIB_DIR} REQUIRED)

# 源代码列表
target_sources(${CMAKE_PROJECT_NAME} PRIVATE 
    src/plugin-main.cpp 
    src/obs-text-freetype2-slideshow.cpp 
    src/obs-text-slideshow.cpp
    src/obs-text-slideshow-dock.cpp 
    src/files.cpp 
    src/utils.cpp
)

# 链接库
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE 
    Qt6::Core 
    Qt6::Widgets
    ${LIBOBS_LIB}
    ${FRONTEND_API_LIB}
    ${W32_PTHREADS_LIB}
)

# Windows 特有源代码和系统库链接
if(WIN32)
    target_sources(${CMAKE_PROJECT_NAME} PRIVATE src/obs-text-gdiplus-slideshow.cpp)
    target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Gdiplus Shlwapi)
endif()

# 设置输出属性：去掉 "lib" 前缀，后缀为 .dll
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES 
    PREFIX "" 
    SUFFIX ".dll"
)
