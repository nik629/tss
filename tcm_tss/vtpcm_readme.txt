可信软件栈代码，与bmc_server同级
基于tpm2-tss添加文件完成通过核间通信调用TCM功能(仿照tcti-device改写，主要功能函数为transmit和receive)
include/tss2/tss2-secmodule.h
src/tss2-tcti/tcti-secmodule.c
src/tss2-tcti/tcti-secmodule.h
修改文件，动态加载tcti层
tctildr-nodl.c
tctildr-dl.c
所有编译文件，添加类似tcti-device的编译文件
tss2-tcti-secmodule.map
tss2-tcti-secmodule.pc.in
tss2-tcti-secmodule.def

tss找不到tcti层解决方法：
1.tss手动指定tcti层：export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/lib64/lib，/usr/lib64/lib为tcti-secmodule存放路径
2.安装tpm2-abrmd可自动调用可用的tcti层
	                      