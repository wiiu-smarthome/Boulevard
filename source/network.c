// This file is primarily a port of what we need from libogc:
// https://github.com/devkitPro/libogc/blob/master/libogc/network_wii.c
#include <string.h>

#include "network.h"
#include "heap.h"
#include "ipc.h"
#include "log.h"
#include "mem.h"
#include "str_utils.h"
#include "syscalls.h"
#include "types.h"

#define IPC_ENOENT -6
#define IPC_ENOMEM -22

static char __manage_fs[] ATTRIBUTE_ALIGN(32) = "/dev/net/ncd/manage";
static char __iptop_fs[] ATTRIBUTE_ALIGN(32) = "/dev/net/ip/top";

static s32 msg_queue;

static s32 net_init_chain(s32 result, void *usrdata)
{
    struct init_data *data = (struct init_data *)usrdata;
    static char state_buf[1];

    LOG_Write("Entered state:", "net_init_chain", __LINE__);
    LOG_Write(itoa(data->state, state_buf, 10), "", __LINE__);

    switch (data->state) {
    // Open network manager FD
    case 0:
        data->state = 1;

        LOG_Write("Calling open", "net_init_chain", __LINE__);
        data->result = os_open_async(__manage_fs, 0, net_init_chain, data);
        if (data->result < 0) {
            LOG_Write("result failure", "net_init_chain", __LINE__);
            LOG_Write(itoa(data->result, state_buf, 10), "", __LINE__);
            return data->result;
        }

        goto done;
    default:
        LOG_Write("Unknown state", "net_init_chain", __LINE__);
        data->result = -1;

        break;
    }

done:
    os_heap_free((u32)heapspace, data->buf);
    os_heap_free((u32)heapspace, data);

    return 0;
}

s32 net_init(void)
{
    struct init_data *data;

    void *queue_buffer = os_heap_alloc((u32)heapspace, 0x20);
    if (!queue_buffer) {
        return IPC_ENOMEM;
    }

    // TODO: as we are doing initialization, there is not really a need to spin network
    // initialization onto another thread. is this necessary?
    msg_queue = os_message_queue_create(queue_buffer, 0x8);
    if (msg_queue < 0) {
        return msg_queue;
    }

    // for debug???
    s32 ret = os_device_register("/dev/blvd/net_init", msg_queue);
    if (ret < 0) {
        return ret;
    }

    data = Mem_Alloc(sizeof(data));
    if (!data) {
        return IPC_ENOMEM;
    }

    memset(data, 0, sizeof(data));

    data->buf = os_heap_alloc_aligned((u32)heapspace, 32, 0x20);
    if (!data->buf) {
        os_heap_free((u32)heapspace, data);
        return -1;
    }

    net_init_chain(IPC_ENOENT, data);

    return 0;
}
