
#ifndef __CHANNEL_H__
#define __CHANNEL_H__

#define _interface_output_channel	\
void (*put)(char)

typedef struct {
	_interface_output_channel;
} OutputChannel;

typedef struct _il{
	struct _il *next;
	void (*processInput)(char);
} InputListener;

#define _interface_input_channel \
void (*registerListener)(InputListener *)
typedef struct {
	_interface_input_channel;
} InputChannel;

#endif /*__CHANNEL_H__*/