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

// From
// https://github.com/devkitPro/libogc/blob/475da950ccf7a294e15abe985e44cedc2d6d9caa/libogc/network_wii.c#L121:
// wii doesn't have, or use, sin_zero in sockaddr_in and all communications assumes its not there
// bsd sockets does have it, so we ignore it. the struct is directly compatible with bsd sockets
// normally
struct wii_sockaddr_in {
    u8 sin_len;
    u8 sin_family;
    u16 sin_port;
    u32 sin_addr;
};

struct setsockopt_params {
    u32 socket;
    u32 level;
    u32 optname;
    u32 optlen;
    u8 optval[20];
};

// FIXME: shouldn't `name` be renamed to `addr`?
struct bind_params {
    u32 socket;
    u32 has_name;
    u8 name[28];
};

struct sockaddr {
    u8 sa_len;
    u8 sa_family;
    s8 sa_data[14];
};

/*
 * Option flags per-socket.
 */
#define SO_DEBUG       0x0001 /* turn on debugging info recording */
#define SO_ACCEPTCONN  0x0002 /* socket has had listen() */
#define SO_REUSEADDR   0x0004 /* allow local address reuse */
#define SO_KEEPALIVE   0x0008 /* keep connections alive */
#define SO_DONTROUTE   0x0010 /* just use interface addresses */
#define SO_BROADCAST   0x0020 /* permit sending of broadcast msgs */
#define SO_USELOOPBACK 0x0040 /* bypass hardware when possible */
#define SO_LINGER      0x0080 /* linger on close if data present */
#define SO_OOBINLINE   0x0100 /* leave received OOB data in line */
#define SO_REUSEPORT   0x0200 /* allow local address & port reuse */

#define SO_DONTLINGER (int)(~SO_LINGER)

/*
 * Additional options, not kept in so_options.
 */
#define SO_SNDBUF   0x1001 /* send buffer size */
#define SO_RCVBUF   0x1002 /* receive buffer size */
#define SO_SNDLOWAT 0x1003 /* send low-water mark */
#define SO_RCVLOWAT 0x1004 /* receive low-water mark */
#define SO_SNDTIMEO 0x1005 /* send timeout */
#define SO_RCVTIMEO 0x1006 /* receive timeout */
#define SO_ERROR    0x1007 /* get error status and clear */
#define SO_TYPE     0x1008 /* get socket type */

/*
 * Level number for (get/set)sockopt() to apply to socket itself.
 */
#define SOL_SOCKET 0xfff /* options for socket level */

#define AF_UNSPEC 0
#define AF_INET   2
#define PF_INET   AF_INET
#define PF_UNSPEC AF_UNSPEC

#define IPPROTO_IP  0
#define IPPROTO_TCP 6
#define IPPROTO_UDP 17

#define INADDR_ANY       0
#define INADDR_BROADCAST 0xffffffff

/* Flags we can use with send and recv. */
#define MSG_DONTWAIT 0x40 /* Nonblocking i/o for this operation only */

#define socklen_t int

s32 socket(u32 domain, u32 type, u32 protocol);
s32 bind(u32 s, struct sockaddr *addr, socklen_t addrlen);
s32 setsockopt(u32 s, u32 level, u32 optname, const void *optval, socklen_t optlen);

s32 net_init(void);
