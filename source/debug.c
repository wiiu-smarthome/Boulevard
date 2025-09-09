#include "debug.h"
#include "heap.h"
#include "ipc.h"
#include "network.h"
#include "syscalls.h"

#define BLVD_GET_STATUS 0x00000001

#define IPC_EINVAL -101

static char device_name[] ATTRIBUTE_ALIGN(32) = "/dev/blvd";
s32 queuehandle = -1;
s32 timerid = -1;

// FIXME: should be in libc, how do we include it in build
int strcmp(const char *s1, const char *s2)
{
    while (*s1 != '\0' && *s1 == *s2) {
        s1++;
        s2++;
    }

    return (*(unsigned char *)s1) - (*(unsigned char *)s2);
}

s32 __DebugThreadLoop()
{
    void *buffer = NULL;

    /* Create buffer for info message queue */
    buffer = os_heap_alloc((s32)heapspace, 0x80);
    if (!buffer) {
        return IPC_ENOMEM;
    }

    /* Create queue */
    s32 ret = os_message_queue_create(buffer, 32);
    if (ret < 0) {
        os_heap_free((s32)heapspace, buffer);
        return ret;
    }

    /* Register device */
    os_device_register(device_name, ret);

    /* Copy queue handle */
    queuehandle = ret;

    timerid = os_create_timer(1000 * 5, 1000 * 1000 * 10, queuehandle, 0);

    while (1) {
        ipcmessage *message = NULL;

        /* Wait for message */
        ret = os_message_queue_receive(queuehandle, (void *)&message, 0);
        if (ret)
            continue;

        /* Command handling */
        switch (message->command) {
        case IOS_OPEN:
            // taken from mload, I think the standard is just to ensure we are opening /dev/blvd
            if (!strcmp(message->open.device, device_name)) {
                ret = message->open.resultfd;
            } else {
                ret = IPC_ENOENT;
            }

            break;
        case IOS_CLOSE:
            // Nothing todo?
            break;
        case IOS_IOCTLV:
            // general data
            ioctlv *vector = message->ioctlv.vector;
            u32 inlen = message->ioctlv.num_in;
            u32 iolen = message->ioctlv.num_io;
            u32 cmd = message->ioctlv.command;

            /* Invalidate cache, whatever this does */
            InvalidateVector(vector, inlen, iolen);

            switch (cmd) {
            case BLVD_GET_STATUS: {
                ret = blvd_state;
                break;
            }
            default:
                ret = IPC_EINVAL;
                break;
            }

            break;
        default:
            // Invalid command
            ret = IPC_EINVAL;
            break;
        }

        /* "Acknowledge" message */
        os_message_queue_ack(message, ret);
    }

    return 0;
}

s32 debug_init()
{
    static u8 thread_stack[1024] ATTRIBUTE_ALIGN(32);
    u32 thread_id, priority;
    priority = os_thread_get_priority(os_get_thread_id());

    thread_id =
        os_thread_create((void *)__DebugThreadLoop, NULL, &thread_stack[1024], 1024, priority, 0);

    if (thread_id >= 0) {
        os_thread_continue(thread_id);
    }
    return 0;
}
