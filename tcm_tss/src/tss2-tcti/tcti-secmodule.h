/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2018 Intel Corporation
 * All rights reserved.
 */
#ifndef TCTI_SECMODULE_H
#define TCTI_SECMODULE_H

#include "tcti-common.h"
#include "dal_tcm_adapter.h"


#define TCTI_SECMODULE_MAGIC 0x19205e72e319e5bbULL
#define MAX_TPCM_TCM_CMD_LEN 2048

typedef struct {
    TSS2_TCTI_COMMON_CONTEXT common;
} TSS2_TCTI_SECMODULE_CONTEXT;

#endif /* TCTI_SECMODULE_H */
