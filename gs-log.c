#include <linux/kernel.h>
#include <stdarg.h>

void
__gs_log(const char *format, ...)
{
        va_list args;
        va_start(args, format);
        vprintk(format, args);
        va_end(args);
}
