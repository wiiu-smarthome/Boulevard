#include "types.h"

// from libogc
typedef s32 (*netcallback)(s32 result, void *usrdata);
typedef s32 syswd_t;

struct init_data {
    u32 state;
    s32 fd;
    s32 prevres;
    s32 result;
    syswd_t alarm;
    u32 retries;
    netcallback cb;
    void *usrdata;
    u8 *buf;
};

static s32 net_init_chain(s32 result, void *usrdata);

s32 net_init(void);
