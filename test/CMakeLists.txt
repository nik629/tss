cmake_minimum_required(VERSION 3.10)
project(TCM_TSS_Test C)

# 1. 设置 TSS 安装的根目录 (根据你的路径修改)
set(TSS_ROOT "/home/nik/workspace/x86_mock_out")

# 2. 添加头文件搜索路径
include_directories(${TSS_ROOT}/include)

# 3. 查找必要的动态库
# 使用 find_library 并指定 PATHS，确保 CMake 优先去你的 mock 目录查找
find_library(TSS2_ESYS_LIB NAMES tss2-esys PATHS ${TSS_ROOT}/lib NO_DEFAULT_PATH)
find_library(TSS2_TCTILDR_LIB NAMES tss2-tctildr PATHS ${TSS_ROOT}/lib NO_DEFAULT_PATH)

# 如果上面的库没找到，可以增加一个报错提示
if(NOT TSS2_ESYS_LIB OR NOT TSS2_TCTILDR_LIB)
    message(FATAL_ERROR "未能在 ${TSS_ROOT}/lib 找到所需的 TSS2 库文件")
endif()

# 4. 生成可执行程序
add_executable(test_tss test_tss.c)

# 5. 链接库
target_link_libraries(test_tss ${TSS2_ESYS_LIB} ${TSS2_TCTILDR_LIB})

# 6. (可选) 设置 RPATH，这样运行程序时就不需要手动 export LD_LIBRARY_PATH 了
set_target_properties(test_tss PROPERTIES INSTALL_RPATH "${TSS_ROOT}/lib")
set_target_properties(test_tss PROPERTIES BUILD_WITH_INSTALL_RPATH TRUE)