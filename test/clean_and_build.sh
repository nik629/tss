#!/bin/bash

set -e  # 出错就停止

echo "===> Running make clean..."
make clean

echo "===> Running cmake..."
cmake .

echo "===> Running make..."
make

echo "===> Build finished successfully!"