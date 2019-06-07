/* SPDX-License-Identifier: (GPL-2.0 or LGPL-2.1)
 *
 * lttng-kallsyms.c
 *
 * Copyright (C) 2019 Geneviève Bastien <gbastien@versatic.net>
 */

#include <linux/module.h>
#include <linux/kallsyms.h>
#include <lttng-events.h>
#include <lttng-kallsyms.h>

/* Define the tracepoints, but do not build the probes */
#define CREATE_TRACE_POINTS
#define TRACE_INCLUDE_PATH instrumentation/events/lttng-module
#define TRACE_INCLUDE_FILE lttng-kallsyms
#define LTTNG_INSTRUMENTATION
#include <instrumentation/events/lttng-module/lttng-kallsyms.h>

DEFINE_TRACE(lttng_kallsyms_kernel_symbol);

/*
 * Store the symbols
 *
 * data: The lttng lttng_session
 * symbol_name: The function name this symbol resolves to
 * module: The module containing this symbol. Can be NULL
 * symbol_addr: The symbol address
 */
static
int _lttng_one_symbol_received(void * data, const char * symbol_name,
              struct module * module, unsigned long symbol_addr) {
    struct lttng_session *session = data;

    if (module) {
        trace_lttng_kallsyms_kernel_symbol(session, symbol_addr,
            symbol_name, module->name);
    } else {
      trace_lttng_kallsyms_kernel_symbol(session, symbol_addr,
          symbol_name, "");
    }

    return 0;
}

int lttng_enumerate_kernel_symbols(struct lttng_session *session) {
  int ret = 0;

  ret = kallsyms_on_each_symbol(_lttng_one_symbol_received, (void *) session);

  return ret;
}
EXPORT_SYMBOL_GPL(lttng_enumerate_kernel_symbols);

#ifdef CONFIG_MODULES

static
int lttng_kallsyms_module_coming(struct tp_module *tp_mod)
{
	printk("Just saw a module loading in lttng %s", tp_mod->mod->name);
	return 0;
}

static
int lttng_kallsyms_module_going(struct tp_module *tp_mod)
{
  printk("Just saw a module going in lttng %s", tp_mod->mod->name);
	return 0;
}

static
int lttng_kallsyms_notify(struct notifier_block *self,
		unsigned long val, void *data)
{
	struct tp_module *tp_mod = data;
	int ret = 0;

  printk("got a notification '%d'", val);
	switch (val) {
	case MODULE_STATE_COMING:
		ret = lttng_kallsyms_module_coming(tp_mod);
		break;
	case MODULE_STATE_GOING:
		ret = lttng_kallsyms_module_going(tp_mod);
		break;
	default:
		break;
	}
	return ret;
}

static
struct notifier_block lttng_kallsyms_notifier = {
	.notifier_call = lttng_kallsyms_notify,
	.priority = 0,
};

static
int lttng_kallsyms_module_init(void)
{
  printk("hello kallsyms");
	return register_tracepoint_module_notifier(&lttng_kallsyms_notifier);
}

static
void lttng_kallsyms_module_exit(void)
{
  printk("bye kallsyms");
	WARN_ON(unregister_tracepoint_module_notifier(&lttng_kallsyms_notifier));
}

#else /* #ifdef CONFIG_MODULES */

static
int lttng_kallsyms_module_init(void)
{
  printk("hello kallsyms but no");
	return 0;
}

static
void lttng_kallsyms_module_exit(void)
{
  printk("hello kallsyms but no");
}

#endif /* #else #ifdef CONFIG_MODULES */

module_init(lttng_kallsyms_module_init);

module_exit(lttng_kallsyms_module_exit);

MODULE_LICENSE("GPL and additional rights");
MODULE_AUTHOR("Geneviève Bastien <gbastien@versatic.net");
MODULE_DESCRIPTION("LTTng kallsyms");
MODULE_VERSION(__stringify(LTTNG_MODULES_MAJOR_VERSION) "."
	__stringify(LTTNG_MODULES_MINOR_VERSION) "."
	__stringify(LTTNG_MODULES_PATCHLEVEL_VERSION)
	LTTNG_MODULES_EXTRAVERSION);
