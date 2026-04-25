#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#ifdef __VXWORKS__
#include <sys/poll.h>
#else
#include <poll.h>
#endif
#include <unistd.h>

#include "tss2_tcti.h"
#include "tss2_tcti_secmodule.h"
#include "tss2_mu.h"
#include "tcti-common.h"
#include "tcti-secmodule.h"
#include "util-io/io.h"
#include "util/aux_util.h"
#define LOGMODULE tcti
#include "util/log.h"
extern TPCM_TCM_cmd p_tpcm_tcm;
/*存放发送给M3的已封装完成的命令的字节流及返回的字节流*/



TSS2_TCTI_SECMODULE_CONTEXT*
tcti_secmodule_context_cast (TSS2_TCTI_CONTEXT *tcti_ctx)
{
    if (tcti_ctx == NULL)
        return NULL;

    return (TSS2_TCTI_SECMODULE_CONTEXT*)tcti_ctx;
}
/*
 * This function down-casts the secmodule TCTI context to the common context
 * defined in the tcti-common module.
 */
TSS2_TCTI_COMMON_CONTEXT*
tcti_secmodule_down_cast (TSS2_TCTI_SECMODULE_CONTEXT *tcti_dev)
{
    if (tcti_dev == NULL) {
        return NULL;
    }
    return &tcti_dev->common;
}

TSS2_RC
tcti_secmodule_transmit (
    TSS2_TCTI_CONTEXT *tctiContext,
    size_t command_size,
    const uint8_t *command_buffer)
{
    LOG_TRACE("TCTI-Secmodule transmit begin");
    TSS2_TCTI_SECMODULE_CONTEXT *tcti_dev = tcti_secmodule_context_cast (tctiContext);
    TSS2_TCTI_COMMON_CONTEXT *tcti_common = tcti_secmodule_down_cast (tcti_dev);
    TSS2_RC rc = TSS2_RC_SUCCESS;
    /*检查并进行准备*/
    /*检查传输大小*/
    if (command_size > MAX_TPCM_TCM_CMD_LEN) {
        LOG_ERROR ("command size is larger than the max tcti-secmodule-len");
    }
    /*开辟空间*/
    p_tpcm_tcm.data = calloc(1, MAX_TPCM_TCM_CMD_LEN*sizeof(char));
    memcpy(p_tpcm_tcm.data, command_buffer, command_size);

    rc = tcti_common_transmit_checks (tcti_common,
                                      command_buffer,
                                      TCTI_SECMODULE_MAGIC);
    if (rc != TSS2_RC_SUCCESS) {
        return rc;
    }
    LOGBLOB_DEBUG (command_buffer,
                   command_size,
                   "sending %zu byte command buffer:",
                   command_size);
    p_tpcm_tcm.req_len = command_size;
    int len = dal_get_bmc_tpcm_tcm();
    LOGBLOB_DEBUG (p_tpcm_tcm.data,
                   len,
                   "receive %zu byte command buffer:",
                   len);
    p_tpcm_tcm.resp_len = len;
    if (p_tpcm_tcm.resp_len <= 0) {
        LOG_ERROR ("wrong number of bytes written.");
        return TSS2_TCTI_RC_IO_ERROR;
    }
    tcti_common->state = TCTI_STATE_RECEIVE;
    LOG_TRACE("TCTI-Secmodule transmit end");
    return TSS2_RC_SUCCESS;
}
TSS2_RC
tcti_secmodule_receive (
    TSS2_TCTI_CONTEXT *tctiContext,
    size_t *response_size,
    uint8_t *response_buffer,
    int32_t timeout)
{
    LOG_TRACE("TCTI-Secmodule receive begin.");
    TSS2_TCTI_SECMODULE_CONTEXT *tcti_dev = tcti_secmodule_context_cast (tctiContext);
    TSS2_TCTI_COMMON_CONTEXT *tcti_common = tcti_secmodule_down_cast (tcti_dev);
    TSS2_RC rc = TSS2_RC_SUCCESS;
    uint8_t header[TPM_HEADER_SIZE];
    size_t offset = 2;
    UINT32 partial_size;
    ssize_t size = 0;
    rc = tcti_common_receive_checks (tcti_common,
                                     response_size,
                                     TCTI_SECMODULE_MAGIC);
    if (rc != TSS2_RC_SUCCESS) {
        return rc;
    }
    if (timeout != TSS2_TCTI_TIMEOUT_BLOCK) {
        LOG_TRACE("secmodule Asynchronous I/O not actually implemented.");
    }
    if (!response_buffer) {
        if (!tcti_common->partial_read_supported) {
            LOG_DEBUG("Partial read not supported ");
            *response_size = 4096;
            return TSS2_RC_SUCCESS;
        } else {
            /* Read the header only and get the response size out of it */
            memcpy(header, p_tpcm_tcm.data, TPM_HEADER_SIZE);
            size = TPM_HEADER_SIZE;
            rc = Tss2_MU_UINT32_Unmarshal(header, TPM_HEADER_SIZE,
                                          &offset, &partial_size);
            if (rc != TSS2_RC_SUCCESS) {
                LOG_ERROR ("Failed to unmarshal response size.");
                return rc;
            }

            if (partial_size < TPM_HEADER_SIZE) {
                LOG_ERROR ("Received %zu bytes, not enough to hold a TPM2"
               " response header.", size);
                return TSS2_TCTI_RC_GENERAL_FAILURE;
            }
            LOG_DEBUG("Partial read - received response size %d.", partial_size);
            tcti_common->partial = true;
            *response_size = partial_size;
            memcpy(&tcti_common->header, header, TPM_HEADER_SIZE);
            return rc;
        }
    }
    if (tcti_common->partial == true && *response_size == TPM_HEADER_SIZE) {
        memcpy(response_buffer, &tcti_common->header, TPM_HEADER_SIZE);
        tcti_common->partial = false;
        goto out;
    }
    if (tcti_common->partial == true) {
        memcpy(response_buffer, &tcti_common->header, TPM_HEADER_SIZE);
        memcpy(response_buffer + TPM_HEADER_SIZE, p_tpcm_tcm.data+TPM_HEADER_SIZE, *response_size-TPM_HEADER_SIZE);
        size = *response_size - TPM_HEADER_SIZE;
    }

    rc = header_unmarshal (response_buffer, &tcti_common->header);
    if (rc != TSS2_RC_SUCCESS)
        goto out;
    size += tcti_common->partial ? TPM_HEADER_SIZE : 0;
    tcti_common->partial = false;
    *response_size = size;

out:
    tcti_common->state = TCTI_STATE_TRANSMIT;
    LOGBLOB_DEBUG (response_buffer,
                   *response_size,
                   "receive %zu byte command buffer:",
                   *response_size);
    LOG_TRACE("TCTI-Secmodule receive end.");
    return rc;
}
void
tcti_secmodule_finalize (
    TSS2_TCTI_CONTEXT *tctiContext)
{
    TSS2_TCTI_SECMODULE_CONTEXT *tcti_dev = tcti_secmodule_context_cast (tctiContext);
    TSS2_TCTI_COMMON_CONTEXT *tcti_common = tcti_secmodule_down_cast (tcti_dev);
    tcti_common->state = TCTI_STATE_FINAL;
}

TSS2_RC
tcti_secmodule_cancel (
    TSS2_TCTI_CONTEXT *tctiContext)
{
    /* Linux driver doesn't expose a mechanism to cancel commands. */
    UNUSED(tctiContext);
    return TSS2_TCTI_RC_NOT_IMPLEMENTED;
}

TSS2_RC
tcti_secmodule_get_poll_handles (
    TSS2_TCTI_CONTEXT *tctiContext,
    TSS2_TCTI_POLL_HANDLE *handles,
    size_t *num_handles)
{
    UNUSED(tctiContext);
    UNUSED(handles);
    UNUSED(num_handles);
    return TSS2_TCTI_RC_NOT_IMPLEMENTED;
}
TSS2_RC
tcti_secmodule_set_locality (
    TSS2_TCTI_CONTEXT *tctiContext,
    uint8_t locality)
{
    /*
     * Linux driver doesn't expose a mechanism for user space applications
     * to set locality.
     */
    UNUSED(tctiContext);
    UNUSED(locality);
    return TSS2_TCTI_RC_NOT_IMPLEMENTED;
}
TSS2_RC
Tss2_Tcti_Secmodule_Init (
    TSS2_TCTI_CONTEXT *tctiContext,
    size_t *size,
    const char *conf)
{
    (void)(conf);
    TSS2_TCTI_SECMODULE_CONTEXT *tcti_dev;
    TSS2_TCTI_COMMON_CONTEXT *tcti_common;

    if (tctiContext == NULL && size == NULL) {
        return TSS2_TCTI_RC_BAD_VALUE;
    } else if (tctiContext == NULL) {
        *size = sizeof (TSS2_TCTI_SECMODULE_CONTEXT);
        return TSS2_RC_SUCCESS;
    }

    /* Init TCTI context */
    TSS2_TCTI_MAGIC (tctiContext) = TCTI_SECMODULE_MAGIC;
    TSS2_TCTI_VERSION (tctiContext) = TCTI_VERSION;
    TSS2_TCTI_TRANSMIT (tctiContext) = tcti_secmodule_transmit;
    TSS2_TCTI_RECEIVE (tctiContext) = tcti_secmodule_receive;
    TSS2_TCTI_FINALIZE (tctiContext) = tcti_secmodule_finalize;
    TSS2_TCTI_CANCEL (tctiContext) = tcti_secmodule_cancel;
    TSS2_TCTI_GET_POLL_HANDLES (tctiContext) = tcti_secmodule_get_poll_handles;
    TSS2_TCTI_SET_LOCALITY (tctiContext) = tcti_secmodule_set_locality;
    TSS2_TCTI_MAKE_STICKY (tctiContext) = tcti_make_sticky_not_implemented;
    tcti_dev = tcti_secmodule_context_cast (tctiContext);
    tcti_common = tcti_secmodule_down_cast (tcti_dev);
    tcti_common->state = TCTI_STATE_TRANSMIT;
    memset (&tcti_common->header, 0, sizeof (tcti_common->header));
    tcti_common->locality = 0;
    tcti_common->partial_read_supported = 1;
    return TSS2_RC_SUCCESS;
}

const TSS2_TCTI_INFO tss2_tcti_info = {
    .version = TCTI_VERSION,
    .name = "tcti-secmodule",
    .description = "TCTI module for communication with Linux kernel interface.",
    .config_help = "Path to TPM character secmodule. Default value is: "
        "TCTI_SECMODULE_DEFAULT",
    .init = Tss2_Tcti_Secmodule_Init,
};

const TSS2_TCTI_INFO*
Tss2_Tcti_Info (void)
{
    return &tss2_tcti_info;
}