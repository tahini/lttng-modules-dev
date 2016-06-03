#undef TRACE_SYSTEM
#define TRACE_SYSTEM rseq

#if !defined(LTTNG_TRACE_RSEQ_H) || defined(TRACE_HEADER_MULTI_READ)
#define LTTNG_TRACE_RSEQ_H

#include <probes/lttng-tracepoint-event.h>

LTTNG_TRACEPOINT_EVENT(rseq_inc,

	TP_PROTO(uint32_t event_counter, int ret),

	TP_ARGS(event_counter, ret),

	TP_FIELDS(
		ctf_integer(uint32_t, event_counter, event_counter)
		ctf_integer(int, ret, ret)
	)
)

LTTNG_TRACEPOINT_EVENT(rseq_ip_fixup,

	TP_PROTO(void __user *regs_ip, void __user *post_commit_ip,
		void __user *abort_ip, uint32_t regs_evcount,
		uint32_t kevcount, int ret),

	TP_ARGS(regs_ip, post_commit_ip, abort_ip, regs_evcount,
		kevcount, ret),

	TP_FIELDS(
		ctf_integer_hex(void __user *, regs_ip, regs_ip)
		ctf_integer_hex(void __user *, post_commit_ip, post_commit_ip)
		ctf_integer_hex(void __user *, abort_ip, abort_ip)
		ctf_integer(uint32_t, regs_evcount, regs_evcount)
		ctf_integer(uint32_t, kevcount, kevcount)
		ctf_integer(int, ret, ret)
	)
)

#endif /*  LTTNG_TRACE_IRQ_H */

/* This part must be outside protection */
#include <probes/define_trace.h>
