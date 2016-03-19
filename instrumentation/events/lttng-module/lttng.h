#undef TRACE_SYSTEM
#define TRACE_SYSTEM lttng

#if !defined(LTTNG_TRACE_LTTNG_H) || defined(TRACE_HEADER_MULTI_READ)
#define LTTNG_TRACE_LTTNG_H

#include <probes/lttng-tracepoint-event.h>

#ifndef ONCE_LTTNG_TRACE_LTTNG_H
#define ONCE_LTTNG_TRACE_LTTNG_H

static struct lttng_event_field myfields[] = {
	[0] = {
		.name = "mystringfield",
		.type = {
			.atype = atype_string,
			.u.basic.string.encoding = lttng_encode_UTF8,
		}
	},
	[1] = {
		.name = "mysecondfield",
		.type = __type_integer(int, 0, 0, 0, __BYTE_ORDER, 10, none),
	},
};

static struct lttng_event_field myseqfields[] = {
	[0] = {
		.name = "myseqstringfield",
		.type = {
			.atype = atype_string,
			.u.basic.string.encoding = lttng_encode_UTF8,
		}
	},
	[1] = {
		.name = "myseqsecondfield",
		.type = __type_integer(int, 0, 0, 0, __BYTE_ORDER, 10, none),
	},
};


static struct lttng_type myseqelemtype = {
	.atype = atype_struct,
	.u._struct.nr_fields = ARRAY_SIZE(myseqfields),
	.u._struct.fields = myseqfields,
};

#endif /* ONCE_LTTNG_TRACE_LTTNG_H */

LTTNG_TRACEPOINT_ENUM(lttng_myenum,
	TP_ENUM_VALUES(
		ctf_enum_range("_mystringfield", 0, 99)
		ctf_enum_range("_mysecondfield", 100, INT_MAX)
	)
)

LTTNG_TRACEPOINT_EVENT_CODE(lttng_logger,
	TP_PROTO(const char __user *text, size_t len),
	TP_ARGS(text, len),

	TP_locvar(
		uint32_t seqlen;
	),

	TP_code_pre(
		tp_locvar->seqlen = len;
	),

	TP_FIELDS(
		ctf_user_sequence_text(char, msg, text, size_t, len)
		ctf_enum(lttng_myenum, int, enumfield, len)
		ctf_custom_field(
			ctf_custom_type(
				.atype = atype_variant,
				.u.variant.tag_name = "enumfield",
				.u.variant.choices = myfields,
				.u.variant.nr_choices = ARRAY_SIZE(myfields),
			),
			fieldvariant,
			ctf_custom_code(
				if (len < 100) {
					ctf_string_type("abc123")
				} else {
					ctf_integer_type(int, len)
				}
			)
		)
		ctf_integer(uint32_t, seqlen, tp_locvar->seqlen)
		ctf_custom_field(
			ctf_custom_type(
				.atype = atype_sequence_compound,
				.u.sequence_compound.length_name = "seqlen",
				.u.sequence_compound.elem_type = &myseqelemtype,
			),
			fieldname,
			ctf_custom_code(
				uint32_t i;

				ctf_align(int)	/* Align on largest field in struct. */
				for (i = 0; i < tp_locvar->seqlen; i++) {
					ctf_string_type("abc123")
					ctf_integer_type(int, i)
				}
			)
		)
	),

	TP_code_post()
)

#endif /* LTTNG_TRACE_LTTNG_H */

/* This part must be outside protection */
#include <probes/define_trace.h>
