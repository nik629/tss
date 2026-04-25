#!/bin/bash
# This script provider global set for build open source software for hi1711 arch cpu.
# Copyright © Huawei Technologies Co., Ltd. 2021-2023. All rights reserved.

set -e

# May you do good and not evil.
# May you find forgiveness for yourself and forgive others.
# May you share freely, never taking more than you give.

# author: hucheng 188524
# date: Wed Dec 18 14:14:09 CST 2013
# version: 0.8.0-1
cd /usr1/jenkins/workspace/compile/V2R2_trunk/application/src/apps/tcm_tss
G_CUR_DIR=$(pwd)
export G_PREFIX="/home/3rd"
export G_HTTPD_PREFIX="/opt/pme/web"
export G_BOOST_DIR="/home/3rd/package/boost_1_54_0/stage/lib"
export G_SYSROOT="/opt/RTOS/208.5.0/arm64le_5.10_ek_preempt_pro/sdk"
export G_PKG_CONFIG_PATH="${G_PREFIX}/lib/pkgconfig"
export CC=aarch64-target-linux-gnu-gcc
export CXX=aarch64-target-linux-gnu-g++
export STRIP=aarch64-target-linux-gnu-strip
export G_HOST=aarch64-linux-gnu
export CROSS_COMPILE=aarch64-target-linux-gnu-
export G_BUILDDIR="aarch64-unknown-linux-gnu"
export G_HOST_ALIAS="aarch64-linux-gnu"
export G_NGINX_PREFIX="/opt/pme/web"

CROSS_COMPILE_INSTALL_PATH=/opt/hcc_arm64le
G_VERSION="208.5.0"
G_CPU="arm64le"
G_KERNEL="5.10_ek_preempt_pro"
source /opt/RTOS/${G_VERSION}/dlsetenv.sh -p ${G_CPU}_${G_KERNEL} --sdk-path=${CROSS_COMPILE_INSTALL_PATH}
sh ./configure  --prefix=/usr1/jenkins/workspace/compile/V2R2_trunk/temp/debug/tmp_root/usr/lib64 --host=${G_HOST} --with-sysroot=${G_SYSROOT} CPPFLAGS="-I/opt/RTOS/208.5.0/arm64le_5.10_ek_preempt_pro/sdk/usr/include -I/usr1/jenkins/workspace/compile/V2R2_trunk/application/src/include -fstack-protector-strong -D_FORTIFY_SOURCE=2 -O2 -Wl,-z,relro,-z,now" CFLAGS=" -fstack-protector-strong -D_FORTIFY_SOURCE=2 -O2 -Wl,-z,relro,-z,now"   CRYPTO_LIBS="/opt/RTOS/208.5.0/arm64le_5.10_ek_preempt_pro/sdk/usr/lib64"  CRYPTO_CFLAGS="/opt/RTOS/208.5.0/arm64le_5.10_ek_preempt_pro/sdk/usr/include/openssl" JSONC_LIBS="/opt/RTOS/208.5.0/arm64le_5.10_ek_preempt_pro/sdk/usr/lib64" JSONC_CFLAGS="/opt/RTOS/208.5.0/arm64le_5.10_ek_preempt_pro/sdk/usr/include" LDFLAGS="-L/opt/RTOS/208.5.0/arm64le_5.10_ek_preempt_pro/sdk/usr/lib64 -L/opt/RTOS/208.5.0/arm64le_5.10_ek_preempt_pro/sdk/lib64 -L/usr1/jenkins/workspace/compile/V2R2_trunk/temp/debug/buildimg/rtos_with_driver/rootfs/usr/lib64 -L/usr1/jenkins/workspace/compile/V2R2_trunk/application/sdk/PME/V100R001C10/lib -L/usr1/jenkins/workspace/compile/V2R2_trunk/application/src/lib_open_source -L/usr1/jenkins/workspace/compile/V2R2_trunk/temp/debug/target_app/src/libs/arithmetic -L/usr1/jenkins/workspace/compile/V2R2_trunk/temp/debug/target_app/src/libs/dalib -ldalib -lnetsnmpagent -lnetsnmphelpers -lnetsnmpmibs -lnetsnmp -lnetsnmptrapd -lpcre -lpcrecpp -lpcreposix  -lcommon -lfile_manage -lpersistance -lthrift-0.16.0 -ldfrpc -lmuparser -lhaslib -lxml_parser -ldflbase -ldflib -lmaint_debug_msg -lmaint_debug_com -larithmetic -lipmi_common -ldrivers -lmaint_debug -lpflash_drv -lcrypto -lssl -ljson-c" --disable-doxygen-doc --disable-doxygen-man --disable-doxygen-rtf --disable-doxygen-html --disable-tcti-device --disable-tcti-mssim --disable-tcti-swtpm --disable-tcti-pcap --disable-tcti-libtpms --disable-tcti-cmd 
sed 's/-lcrypto/\/usr1\/jenkins\/workspace\/compile\/V2R2_trunk\/VPP\/build\/open_source\/openssl\/ossl_IBMC_SUSE12_SP5_ARM64LE_IBMC\/build\/libcrypto.a\//g' Makefile
sed 's/-lssl/\/usr1\/jenkins\/workspace\/compile\/V2R2_trunk\/VPP\/build\/open_source\/openssl\/ossl_IBMC_SUSE12_SP5_ARM64LE_IBMC\/build\/libssl.a\//g' Makefile
make -j$(nproc)
make install
