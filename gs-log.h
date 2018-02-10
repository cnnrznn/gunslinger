#define gs_log(fmt, ...)                                \
        __gs_log("GUNSLINGER: " fmt, ##__VA_ARGS__)

extern void __gs_log(const char *, ...);
