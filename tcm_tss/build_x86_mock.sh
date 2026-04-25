#!/bin/bash
# 专门用于在 x86 宿主机上编译和测试 TSS + Mock Secmodule

set -e

echo "=== 开始配置 x86 Native 编译环境 ==="

# 1. 强制使用系统的原生 x86 编译器 (去掉 aarch64 交叉编译)
export CC=gcc
export CXX=g++

# 2. 注入 Mock 宏开关！这是最关键的一步
# -DSIMULATE_X86 会激活我们之前在 dal_tcm_adapter.c 里写的 x86 假数据逻辑
export CFLAGS="-g -O0 -DSIMULATE_X86"
export CXXFLAGS="-g -O0 -DSIMULATE_X86"

# 3. 如果是从 git 仓库克隆的源码，确保生成 configure 文件
if [ ! -f "configure" ]; then
    echo "运行 bootstrap 生成 configure..."
    ./bootstrap
fi

# 4. 创建一个本地目录用于存放编译结果，避免污染你的系统 /usr/lib
INSTALL_DIR="/home/nik/workspace/x86_mock_out"
mkdir -p "$INSTALL_DIR"

echo "=== 执行 Configure ==="
# 注意：不需要指定 --host，让它默认按 x86 编译
# 确保启用了 tcti-secmodule
./configure \
    --prefix="$INSTALL_DIR" \
    --enable-tcti-secmodule \
    --disable-doxygen-doc

echo "=== 执行 Make ==="
make clean
make -j$(nproc)

echo "=== 执行 Install ==="
make install

echo "=================================================="
echo "编译成功！所有的库和工具已安装至: $INSTALL_DIR"
echo "=================================================="