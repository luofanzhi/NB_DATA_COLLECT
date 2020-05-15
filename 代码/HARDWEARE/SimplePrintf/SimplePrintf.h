#ifndef _SimplePrintf_h__
#define _SimplePrintf_h__

int raw_snprintf( char *buf, unsigned int count, const char *format, ... );
int raw_sprintf(char *out, const char *format, ...);
int raw_printf(const char *format, ...);

#endif



