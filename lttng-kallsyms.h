/* SPDX-License-Identifier: (GPL-2.0 or LGPL-2.1)
 *
 * lttng-kallsyms.h
 *
 * Copyright (C) 2019 Geneviève Bastien <gbastien@versatic.net>
 */

#ifndef _LTTNG_KALLSYMS_H
#define _LTTNG_KALLSYMS_H

#include <lttng-events.h>

int lttng_enumerate_kernel_symbols(struct lttng_session *session);

#endif /* _LTTNG_KALLSYMS_H */
