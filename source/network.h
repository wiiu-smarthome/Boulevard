#include "types.h"

// from libogc
typedef s32 (*netcallback)(s32 result, void *usrdata);
typedef s32 syswd_t;

enum {
    IOCTL_SO_ACCEPT = 1,
    IOCTL_SO_BIND,
    IOCTL_SO_CLOSE,
    IOCTL_SO_CONNECT,
    IOCTL_SO_FCNTL,
    IOCTL_SO_GETPEERNAME, // todo
    IOCTL_SO_GETSOCKNAME, // todo
    IOCTL_SO_GETSOCKOPT,  // todo    8
    IOCTL_SO_SETSOCKOPT,
    IOCTL_SO_LISTEN,
    IOCTL_SO_POLL, // todo    b
    IOCTLV_SO_RECVFROM,
    IOCTLV_SO_SENDTO,
    IOCTL_SO_SHUTDOWN, // todo    e
    IOCTL_SO_SOCKET,
    IOCTL_SO_GETHOSTID,
    IOCTL_SO_GETHOSTBYNAME,
    IOCTL_SO_GETHOSTBYADDR, // todo
    IOCTLV_SO_GETNAMEINFO,  // todo   13
    IOCTL_SO_UNK14,         // todo
    IOCTL_SO_INETATON,      // todo
    IOCTL_SO_INETPTON,      // todo
    IOCTL_SO_INETNTOP,      // todo
    IOCTLV_SO_GETADDRINFO,  // todo
    IOCTL_SO_SOCKATMARK,    // todo
    IOCTLV_SO_UNK1A,        // todo
    IOCTLV_SO_UNK1B,        // todo
    IOCTLV_SO_GETINTERFACEOPT,
    IOCTLV_SO_SETINTERFACEOPT,  // todo
    IOCTL_SO_SETINTERFACE,      // todo
    IOCTL_SO_STARTUP,           // 0x1f
    IOCTL_SO_ICMPSOCKET = 0x30, // todo
    IOCTLV_SO_ICMPPING,         // todo
    IOCTL_SO_ICMPCANCEL,        // todo
    IOCTL_SO_ICMPCLOSE          // todo
};

#define IOCTL_NWC24_STARTUP 0x06

#define IOCTL_NCD_SETIFCONFIG3   0x03
#define IOCTL_NCD_SETIFCONFIG4   0x04
#define IOCTL_NCD_GETLINKSTATUS  0x07
#define IOCTLV_NCD_GETMACADDRESS 0x08

#define IPC_ENOENT -6
#define IPC_ENOMEM -22

s32 net_init(void);
