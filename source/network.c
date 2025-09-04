#include "mem.h"
#include "syscalls.h"
#include "types.h"

#define IPC_ENOMEM -22;

static char __manage_fs[] ATTRIBUTE_ALIGN(32) = "/dev/net/ncd/manage";
static char __iptop_fs[] ATTRIBUTE_ALIGN(32) = "/dev/net/ip/top";

static s32 msg_queue;

static s32 net_init_chain(s32 result, void *usrdata) {
    // ipcmessage *message = NULL;

    return -1;
}

s32 net_init(void) {
    void* buffer = Mem_Alloc(0x20);
    if(!buffer) {
        return IPC_ENOMEM;
    }

    msg_queue = os_message_queue_create(buffer, 0x8);
    if(msg_queue < 0) {
        return msg_queue;
    }

    // for debug
    s32 ret = os_device_register("/dev/blvd", msg_queue);
    if(ret < 0) {
        return ret;
    }

    return net_init_chain(0, NULL);
}
