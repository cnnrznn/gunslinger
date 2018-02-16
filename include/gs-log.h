#ifndef _GS_LOG_H
#define _GS_LOG_H

#define gs_log(fmt, ...)                                \
        _gs_log("GUNSLINGER: " fmt "\n", ##__VA_ARGS__)

extern void _gs_log(const char *, ...);

#endif /* _GS_LOG_H */
