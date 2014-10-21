
#ifndef __AVRPRINTF_H__
#define __AVRPRINTF_H__

#include <stdio.h>
#include "Channel.h"

void avrvprintf(OutputChannel *outp, const char *fmt, va_list ap);

static inline void avrprintf(OutputChannel *outp, const char *fmt, ...){
	va_list ap;
	va_start(ap, fmt);
	avrvprintf(outp, fmt, ap);
	va_end(ap);
}

#endif /*__AVRPRINTF_H__*/