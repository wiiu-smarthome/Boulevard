// This file is primarily a port of what we need from libogc:
// https://github.com/devkitPro/libogc/blob/master/libogc/network_wii.c
#include <string.h>

#include "errno.h"
#include "heap.h"
#include "ipc.h"
#include "mem.h"
#include "network.h"
#include "syscalls.h"
#include "types.h"

static char __manage_fs[] ATTRIBUTE_ALIGN(32) = "/dev/net/ncd/manage";
static char __iptop_fs[] ATTRIBUTE_ALIGN(32) = "/dev/net/ip/top";
static char __kd_fs[] ATTRIBUTE_ALIGN(32) = "/dev/net/kd/request";

static s32 ncd_fd = -1;
static s32 iptop_fd = -1;
static s32 kd_fd = -1;

s32 setsockopt(u32 s, u32 level, u32 optname, const void *optval, socklen_t optlen)
{
    s32 ret;
    STACK_ALIGN(struct setsockopt_params, params, 1, 32);

    // TODO: In release builds, remove the sanity checks since we will be doing this properly
    if (iptop_fd < 0)
        return -ENXIO; // no such device
    if (optlen < 0 || optlen > 20)
        return -EINVAL;

    memset(params, 0, sizeof(struct setsockopt_params));
    params->socket = s;
    params->level = level;
    params->optname = optname;
    params->optlen = optlen;
    if (optval && optlen)
        memcpy(params->optval, optval, optlen);

    return os_ioctl(iptop_fd, IOCTL_SO_SETSOCKOPT, params, sizeof(struct setsockopt_params), NULL,
                    0);
}

s32 socket(u32 domain, u32 type, u32 protocol)
{
    s32 ret;
    STACK_ALIGN(u32, params, 3, 32);

    // TODO: In release builds, remove the sanity checks since we will be doing this properly
    if (iptop_fd < 0)
        return -ENXIO;

    params[0] = domain;
    params[1] = type;
    params[2] = protocol;

    ret = os_ioctl(iptop_fd, IOCTL_SO_SOCKET, params, 12, NULL, 0);
    if (ret >= 0) {
        // libogc: "set tcp window size to 32kb"
        // TODO: do we need to
        int window_size = 32768;
        setsockopt(ret, SOL_SOCKET, SO_RCVBUF, (char *)&window_size, sizeof(window_size));
    }

    return ret;
}

s32 bind(u32 s, struct sockaddr *addr, socklen_t addrlen)
{
    s32 ret;
    STACK_ALIGN(struct bind_params, params, 1, 32);

    // TODO: In release builds, remove the sanity checks since we will be doing this properly
    if (iptop_fd < 0)
        return -ENXIO; // no such device

    // We only support AF_INET, so no need to check for it:
    // if(addr->sa_family != AF_INET) return -EAFNOSUPPORT;

    addr->sa_len = sizeof(struct wii_sockaddr_in);

    memset(params, 0, sizeof(struct bind_params));
    params->socket = s;
    params->has_name = 1;
    memcpy(params->name, addr, sizeof(struct wii_sockaddr_in));

    return os_ioctl(iptop_fd, IOCTL_SO_BIND, params, sizeof(struct bind_params), NULL, 0);
}

s32 net_init(void)
{
    ioctlv *vec;
    s32 *nwc24startup_buffer = os_heap_alloc((u32)heapspace, 0x32);

    /* Open network manager device */
    ncd_fd = os_open(__manage_fs, 0);
    if (ncd_fd < 0) {
        return -2;
    }

    /* Initialize vec for link status request */
    vec = Mem_Alloc(sizeof(vec));
    if (!vec) {
        return IPC_ENOMEM;
    }

    memset(vec, 0, sizeof(vec));

    vec->data = os_heap_alloc_aligned((u32)heapspace, 32, 0x20);
    if (!vec->data) {
        os_heap_free((u32)heapspace, vec);
        return -1;
    }

    /* Get link status */
    s32 ret = os_ioctlv(ncd_fd, IOCTL_NCD_GETLINKSTATUS, 0, 0, vec);
    if (ret < 0) {
        os_close(ncd_fd);
        os_heap_free((u32)heapspace, vec);
        return -3;
    }

    /* Close network manager device and free vec, we don't need it anymore */
    os_close(ncd_fd);
    os_heap_free((u32)heapspace, vec);

    /* Open top ip device */
    iptop_fd = os_open(__iptop_fs, 0);
    if (iptop_fd < 0) {
        return -4;
    }

    /* Open KD (NWC24) device, not sure why, but we do */
    kd_fd = os_open(__kd_fs, 0);
    if (kd_fd < 0) {
        return -5;
    }

    /* Startup NWC24... still not sure why */
    ret = os_ioctl(kd_fd, IOCTL_NWC24_STARTUP, NULL, 0, nwc24startup_buffer, 0x20);
    if (nwc24startup_buffer == -15)
        goto done;
    else if (ret < 0) {
        return -6;
    }

    /* Socket startup */
    ret = os_ioctl(iptop_fd, IOCTL_SO_STARTUP, 0, 0, 0, 0);
    if (ret < 0) {
        return -7;
    }

    /* Check ip/Get host id */
    ret = os_ioctl(iptop_fd, IOCTL_SO_GETHOSTID, 0, 0, 0, 0);
    if (ret < 0) {
        return -8;
    }

done:
    return 0;
}
