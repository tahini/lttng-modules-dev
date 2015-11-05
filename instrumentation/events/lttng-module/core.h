#undef TRACE_SYSTEM
#define TRACE_SYSTEM core

#if !defined(LTTNG_TRACE_CORE_H) || defined(TRACE_HEADER_MULTI_READ)
#define LTTNG_TRACE_CORE_H

#include <linux/irqflags.h>
#include <linux/preempt.h>
#include "../../../probes/lttng-tracepoint-event.h"

/*
 * Tracepoint for critical timings max/threshold hit.
 */
LTTNG_TRACEPOINT_EVENT(core_critical_timing_hit,

	TP_PROTO(unsigned long ip, unsigned long parent_ip,
			unsigned long start_ip, unsigned long flags,
			int preempt_cnt, cycles_t delta_ns),

	TP_ARGS(ip, parent_ip, start_ip, flags, preempt_cnt, delta_ns),

	TP_FIELDS(
		ctf_integer_hex(unsigned long, ip, ip)
		ctf_integer_hex(unsigned long, parent_ip, parent_ip)
		ctf_integer_hex(unsigned long, start_ip, start_ip)
		ctf_integer(cycles_t, delta_ns, delta_ns)
		ctf_integer(uint8_t, irqs_disabled,
			!!(raw_irqs_disabled_flags(flags)))
		ctf_integer(uint8_t, preempt_disabled,
			!!(preempt_cnt & PREEMPT_MASK))
		ctf_integer(uint8_t, in_softirq,
			!!(preempt_cnt & SOFTIRQ_MASK))
		ctf_integer(uint8_t, in_irq,
			!!(preempt_cnt & HARDIRQ_MASK))
		ctf_integer(uint8_t, in_nmi,
			!!(preempt_cnt & NMI_MASK))
	)
)

/*
 * Tracepoint for critical timings start/stop.
 */
LTTNG_TRACEPOINT_EVENT_CLASS(core_critical_timing,

	TP_PROTO(unsigned long ip, unsigned long parent_ip,
			unsigned long flags, int preempt_cnt),

	TP_ARGS(ip, parent_ip, flags, preempt_cnt),

	TP_FIELDS(
		ctf_integer_hex(unsigned long, ip, ip)
		ctf_integer_hex(unsigned long, parent_ip, parent_ip)
		ctf_integer(uint8_t, irqs_disabled,
			!!(raw_irqs_disabled_flags(flags)))
		ctf_integer(uint8_t, preempt_disabled,
			!!(preempt_cnt & PREEMPT_MASK))
		ctf_integer(uint8_t, in_softirq,
			!!(preempt_cnt & SOFTIRQ_MASK))
		ctf_integer(uint8_t, in_irq,
			!!(preempt_cnt & HARDIRQ_MASK))
		ctf_integer(uint8_t, in_nmi,
			!!(preempt_cnt & NMI_MASK))
	)
)

LTTNG_TRACEPOINT_EVENT_INSTANCE(core_critical_timing,

	core_critical_timing_start,

	TP_PROTO(unsigned long ip, unsigned long parent_ip,
			unsigned long flags, int preempt_cnt),

	TP_ARGS(ip, parent_ip, flags, preempt_cnt)
)


LTTNG_TRACEPOINT_EVENT_INSTANCE(core_critical_timing,

	core_critical_timing_stop,

	TP_PROTO(unsigned long ip, unsigned long parent_ip,
			unsigned long flags, int preempt_cnt),

	TP_ARGS(ip, parent_ip, flags, preempt_cnt)
)

#endif /* LTTNG_TRACE_CORE_H */

/* This part must be outside protection */
#include "../../../probes/define_trace.h"
