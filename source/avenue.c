#include "avenue.h"
#include "debug.h"
#include "network.h"

#define SOCK_STREAM 1

// https://github.com/devkitPro/libogc/blob/475da950ccf7a294e15abe985e44cedc2d6d9caa/libogc/network_common.c#L172
u16 htons(u16 n)
{
    return ((n & 0xff) << 8) | ((n & 0xff00) >> 8);
}

u32 htonl(u32 n)
{
    return ((n & 0xff) << 24) | ((n & 0xff00) << 8) | ((n & 0xff0000) >> 8) |
           ((n & 0xff000000) >> 24);
}

s32 avenue_init(void)
{
    u32 sockfd;

    /* Create socket, this appears to be the normal procedure */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        blvd_state = BLVD_STATE_SOCKET_CREATE_FAIL;
        return BLVD_STATE_SOCKET_CREATE_FAIL;
    }

    /* Set SO_REUSEADDR */
    int opt = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        blvd_state = BLVD_STATE_SETSOCKOPT_FAIL;
        return BLVD_STATE_SETSOCKOPT_FAIL;
    }

    /* Following https://github.com/kissbeni/tinyhttp/blob/main/http.cpp */

    struct wii_sockaddr_in remote = {};

    remote.sin_family = AF_INET;
    remote.sin_port = htons(8080);

    // FIXME: other projects seem to use remote.sin_addr.s_addr... something is missing
    remote.sin_addr.s_addr = htonl(INADDR_ANY);

    s32 ret;
    blvd_state = BLVD_STATE_BINDING_SOCKET;
    while (1) {
        ret = bind(sockfd, (struct sockaddr *)&remote, sizeof(remote));
        if (ret >= 0) {
            break;
        }
    }

    blvd_state = BLVD_STATE_LISTENING;
    ret = listen(sockfd, 5);
    if (ret < 0) {
        blvd_state = BLVD_STATE_LISTEN_FAIL;
        return BLVD_STATE_LISTEN_FAIL;
    }

    return 0;
}
