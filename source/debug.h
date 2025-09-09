#include "types.h"

#define BLVD_STATE_START          0
#define BLVD_STATE_INIT_HEAP      1
#define BLVD_STATE_INIT_NET       2
#define BLVD_STATE_INIT_AVENUE    3
#define BLVD_STATE_BINDING_SOCKET 4
#define BLVD_STATE_LISTENING      5

#define BLVD_STATE_HEAP_INIT_FAIL                     -1
#define BLVD_STATE_OPEN_NCD_FAIL                      -2
#define BLVD_STATE_ALLOC_VEC_LINK_STATUS_REQUEST_FAIL -3
#define BLVD_STATE_LINK_STATUS_IOCTL_FAIL             -4
#define BLVD_STATE_OPEN_IPTOP_FAIL                    -5
#define BLVD_STATE_OPEN_KD_FAIL                       -6
#define BLVD_STATE_NWC24_IOCTL_FAIL                   -7
#define BLVD_STATE_SO_STARTUP_IOCTL_FAIL              -8
#define BLVD_STATE_SO_GETHOSTID_IOCTL_FAIL            -9

#define BLVD_STATE_SOCKET_CREATE_FAIL -10
#define BLVD_STATE_SETSOCKOPT_FAIL    -11
#define BLVD_STATE_LISTEN_FAIL        -13

static s32 blvd_state;

s32 debug_init(void);
