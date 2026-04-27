#!/bin/bash
# 专为带有自定义 TCTI secmodule 的 TCM_TSS 编写的 ARM 交叉编译脚本

set -e

echo "=== [1] 初始化构建环境 ==="
G_CUR_DIR=$(pwd)
# 定义编译输出目录 (输出在当前目录的 arm_out 文件夹下，方便你打包)
OUTPUT_DIR="${G_CUR_DIR}/arm_out"
mkdir -p ${OUTPUT_DIR}

# 华为 RTOS SDK 环境配置 (保持原样)
export G_VERSION="208.10.0"
export G_CPU="arm64le"
export G_KERNEL="5.10_ek_preempt_pro"
export CROSS_COMPILE_INSTALL_PATH=/opt/hcc_arm64le
export G_SYSROOT="/opt/RTOS/${G_VERSION}/${G_CPU}_${G_KERNEL}/sdk"

# 引入华为交叉编译工具链环境变量
source /opt/RTOS/${G_VERSION}/dlsetenv.sh -p ${G_CPU}_${G_KERNEL} --sdk-path=${CROSS_COMPILE_INSTALL_PATH}

export CC=aarch64-target-linux-gnu-gcc
export CXX=aarch64-target-linux-gnu-g++
export STRIP=aarch64-target-linux-gnu-strip
export G_HOST=aarch64-linux-gnu

echo "=== [2] 重新生成 Autotools 构建脚本 ==="
# 从 x86 切换回 ARM，必须强制刷新 configure 脚本
autoreconf -fiv

echo "=== [3] 执行 Configure ==="
# 注意: 
# 1. 移除了旧脚本中大量的 jenkins 绝对路径 -L
# 2. 新增了 --enable-tcti-secmodule
# 3. 保留了关闭其他默认 tcti 的选项

./configure \
    --prefix=${OUTPUT_DIR} \
    --host=${G_HOST} \
    --with-sysroot=${G_SYSROOT} \
    CPPFLAGS="-I${G_SYSROOT}/usr/include -fstack-protector-strong -D_FORTIFY_SOURCE=2 -O2 -Wl,-z,relro,-z,now" \
    CFLAGS="-fstack-protector-strong -D_FORTIFY_SOURCE=2 -O2 -Wl,-z,relro,-z,now" \
    CRYPTO_LIBS="${G_SYSROOT}/usr/lib64" \
    CRYPTO_CFLAGS="${G_SYSROOT}/usr/include/openssl" \
    JSONC_LIBS="${G_SYSROOT}/usr/lib64" \
    JSONC_CFLAGS="${G_SYSROOT}/usr/include" \
    LDFLAGS="-L${G_SYSROOT}/usr/lib64 -L${G_SYSROOT}/lib64 -lcrypto -lssl -ljson-c" \
    --disable-doxygen-doc --disable-doxygen-man --disable-doxygen-rtf --disable-doxygen-html \
    --disable-tcti-device --disable-tcti-mssim --disable-tcti-swtpm --disable-tcti-pcap --disable-tcti-libtpms --disable-tcti-cmd \
    --enable-tcti-secmodule

echo "=== [4] 替换静态库路径 (针对特定 OpenSSL 链接修复) ==="
# 如果你的开发板环境强依赖静态编译的 openssl，请取消注释下面两行并修改为实际存在的路径
# sed -i 's/-lcrypto/\/你的\/实际路径\/libcrypto.a/g' Makefile
# sed -i 's/-lssl/\/你的\/实际路径\/libssl.a/g' Makefile

echo "=== [5] 开始编译与安装 ==="
make clean
make -j$(nproc)
make install

echo "=== [6] 编译完成！输出目录: ${OUTPUT_DIR} ==="