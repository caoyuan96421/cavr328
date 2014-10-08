
#include <avr/io.h>

FUSEMEM __fuse_t fusedata = {
	0xFF,
	0xD8,		// BOOTRST = 0 (enabled)
	0xFF
};
