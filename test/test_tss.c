#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* TSS2 头文件 */
#include <tss2/tss2_esys.h>
#include <tss2/tss2_tctildr.h>
#include <tss2/tss2_rc.h>

/* 模拟你代码中的错误处理宏 */
#define goto_if_error(r, msg, label) \
    if (r != TSS2_RC_SUCCESS) { \
        fprintf(stderr, "%s [RC: 0x%X]\n", msg, r); \
        goto label; \
    }

/**
 * Startup 测试函数
 */
static int
test_esys_startup(ESYS_CONTEXT *esys_context)
{
    TSS2_RC r;
    r = Esys_Startup(esys_context, TPM2_SU_CLEAR);
    
    /* 如果 TPM 已经启动，忽略错误 */
    if (r == TPM2_RC_INITIALIZE) {
        printf("[Startup] TPM 已经初始化过，跳过。\n");
        return EXIT_SUCCESS;
    }
    
    goto_if_error(r, "ERROR: ESYS_STARTUP", error);
    printf("[Startup] 成功！\n");
    return EXIT_SUCCESS;

error:
    return EXIT_FAILURE;
}

/**
 * GetRandom 测试函数
 */
static int
test_esys_getrandom(ESYS_CONTEXT *esys_context, UINT16 size, BYTE* out_buffer)
{
    TSS2_RC r;
    TPM2B_DIGEST *randomBytes = NULL;

    r = Esys_GetRandom(
        esys_context,
        ESYS_TR_NONE, ESYS_TR_NONE, ESYS_TR_NONE,
        size,
        &randomBytes);
    
    goto_if_error(r, "ERROR: ESYS_GETRANDOM", error);

    printf("[GetRandom] 成功获取 %d 字节数据。\n", randomBytes->size);
    if (out_buffer && randomBytes->size <= size) {
        memcpy(out_buffer, randomBytes->buffer, randomBytes->size);
    }

    free(randomBytes);
    return EXIT_SUCCESS;

error:
    if (randomBytes) free(randomBytes);
    return EXIT_FAILURE;
}

int main(int argc, char *argv[])
{
    TSS2_RC r;
    TSS2_TCTI_CONTEXT *tcti_ctx = NULL;
    ESYS_CONTEXT *esys_ctx = NULL;
    BYTE random_data[16];

    printf("=== 开始 TCM ESAPI 自动化测试 ===\n");

    /* 1. 初始化 TCTI (加载你的 secmodule) */
    // r = Tss2_TctiLdr_Initialize("secmodule", &tcti_ctx);
    // goto_if_error(r, "无法加载 TCTI secmodule", exit_main);

    /* 2. 初始化 ESYS 上下文 */
    r = Esys_Initialize(&esys_ctx, tcti_ctx, NULL);
    goto_if_error(r, "ESYS 初始化失败", cleanup_tcti);

    /* 3. 调用 Startup */
    if (test_esys_startup(esys_ctx) != EXIT_SUCCESS) {
        goto cleanup_esys;
    }

    /* 4. 调用 GetRandom */
    if (test_esys_getrandom(esys_ctx, 16, random_data) == EXIT_SUCCESS) {
        printf("随机数内容: ");
        for(int i=0; i<16; i++) printf("%02X ", random_data[i]);
        printf("\n");
    }

    printf("=== 所有测试完成 ===\n");

cleanup_esys:
    if (esys_ctx) Esys_Finalize(&esys_ctx);
cleanup_tcti:
    if (tcti_ctx) Tss2_TctiLdr_Finalize(&tcti_ctx);
exit_main:
    return (r == TSS2_RC_SUCCESS) ? EXIT_SUCCESS : EXIT_FAILURE;
}