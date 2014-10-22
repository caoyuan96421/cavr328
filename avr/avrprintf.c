#include <stdarg.h>
#include "mcuconf.h"
#include "Channel.h"
#include <stdint.h>
#include <stdbool.h>

#define MAX_FILLER 11
#define FLOAT_PRECISION 5

#if MCUCONF_USE_PRINTF

static char *long_to_string_with_divisor(char *p, long num, unsigned radix, long divisor){
	int i;
	char *q;
	long l, ll;

	l = num;
	if (divisor == 0) {
		ll = num;
	} else {
		ll = divisor;
	}

	q = p + MAX_FILLER;
	do {
		i = (int)(l % radix);
		i += '0';
		if (i > '9')
			i += 'A' - '0' - 10;
		*--q = i;
		l /= radix;
	} while ((ll /= radix) != 0);

	i = (int)(p + MAX_FILLER - q);
	do
		*p++ = *q++;
	while (--i);

	return p;
}

static char *ltoa(char *p, long num, unsigned radix) {
	return long_to_string_with_divisor(p, num, radix, 0);
}

#if MCUCONF_PRINTF_USE_FLOAT
static long long pow10[] = {10, 100, 1000, 10000, 100000, 1000000, 10000000,
                       100000000, 1000000000, 10000000000, 100000000000,
                       1000000000000, 10000000000000};

static char *ftoa(char *p, double num, unsigned long precision) {
	long l;

	if (precision == 0)
		precision = FLOAT_PRECISION;
	precision = pow10[precision - 1];
	l = (long)num;
	p = long_to_string_with_divisor(p, l, 10, 0);
	*p++ = '.';
	l = (long)((num - l) * precision);
	return long_to_string_with_divisor(p, l, 10, precision / 10);
}
#endif

/**
 * @brief   System formatted output function.
 * @details This function implements a minimal @p vprintf()-like functionality
 *          with output on a @p BaseSequentialStream.
 *          The general parameters format is: %[-][width|*][.precision|*][l|L]p.
 *          The following parameter types (p) are supported:
 *          - <b>x</b> hexadecimal integer.
 *          - <b>X</b> hexadecimal long.
 *          - <b>o</b> octal integer.
 *          - <b>O</b> octal long.
 *          - <b>d</b> decimal signed integer.
 *          - <b>D</b> decimal signed long.
 *          - <b>u</b> decimal unsigned integer.
 *          - <b>U</b> decimal unsigned long.
 *          - <b>c</b> character.
 *          - <b>s</b> string.
 *          .
 *
 * @param[in] chp       pointer to a @p BaseSequentialStream implementing object
 * @param[in] fmt       formatting string
 * @param[in] ap        list of parameters
 *
 * @api
 */
void avrvprintf(OutputChannel *outp, const char *fmt, va_list ap) {
	char *p, *s, c, filler;
	int i, precision, width;
	bool is_long, left_align;
	long l;
#if MCUCONF_PRINTF_USE_FLOAT
	float f;
	char tmpbuf[2*MAX_FILLER + 1];
#else
	char tmpbuf[MAX_FILLER + 1];
#endif

	while (1) {
		c = *fmt++;
		if (c == 0)
			return;
		if (c != '%') {
			outp->put((uint8_t)c);
			continue;
		}
		p = tmpbuf;
		s = tmpbuf;
		left_align = 0;
		if (*fmt == '-') {
			fmt++;
			left_align = 1;
		}
		filler = ' ';
		if (*fmt == '0') {
			fmt++;
			filler = '0';
		}
		width = 0;
		while (1) {
			c = *fmt++;
			if (c >= '0' && c <= '9')
				c -= '0';
			else if (c == '*')
				c = va_arg(ap, int);
			else
				break;
			width = width * 10 + c;
		}
		precision = 0;
		if (c == '.') {
			while (1) {
				c = *fmt++;
				if (c >= '0' && c <= '9')
					c -= '0';
				else if (c == '*')
					c = va_arg(ap, int);
				else
					break;
				precision *= 10;
				precision += c;
			}
		}
		/* Long modifier.*/
		if (c == 'l' || c == 'L') {
			is_long = 1;
			if (*fmt)
				c = *fmt++;
		}
		else
			is_long = (c >= 'A') && (c <= 'Z');

		/* Command decoding.*/
		switch (c) {
			case 'c':
				filler = ' ';
				*p++ = va_arg(ap, int);
				break;
			case 's':
				filler = ' ';
				if ((s = va_arg(ap, char *)) == 0)
					s = "(null)";
				if (precision == 0)
					precision = 32767;
				for (p = s; *p && (--precision >= 0); p++);
				break;
			case 'D':case 'd':case 'I':case 'i':
				if (is_long)
					l = va_arg(ap, long);
				else
					l = va_arg(ap, int);
				if (l < 0) {
					*p++ = '-';
					l = -l;
				}
				p = ltoa(p, l, 10);
				break;
			#if MCUCONF_PRINTF_USE_FLOAT
			case 'f':
				f = (float) va_arg(ap, double);
				if (f < 0) {
					*p++ = '-';
					f = -f;
				}
				p = ftoa(p, f, precision);
				break;
			#endif
			case 'X':case 'x':
				c = 16;
				goto unsigned_common;
			case 'U':case 'u':
				c = 10;
				goto unsigned_common;
			case 'O':case 'o':
				c = 8;
unsigned_common:
				if (is_long)
					l = va_arg(ap, unsigned long);
				else
					l = va_arg(ap, unsigned int);
					p = ltoa(p, l, c);
				break;
			default:
				*p++ = c;
			break;
		}
		i = (int)(p - s);
		if ((width -= i) < 0)
			width = 0;
		if (left_align == 0)
			width = -width;
		if (width < 0) {
			if (*s == '-' && filler == '0') {
				outp->put((uint8_t)*s++);
			i--;
			}
			do {
				outp->put((uint8_t)filler);
			} while (++width != 0);
		}
		while (--i >= 0)
			outp->put((uint8_t)*s++);
		while (width) {
			outp->put((uint8_t)filler);
			width--;
		}
	}
}

#endif