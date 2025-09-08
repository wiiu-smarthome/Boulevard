#include "avenue.h"
#include "network.h"

s32 avenue_init(void) {
    u32 sockfd;

    /* Create socket, this appears to be the normal procedure */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0) {
        return -1;
    }

    /* Set SO_REUSEADDR */
    int opt = 1;
    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        return -1;
    }

    /* Following https://github.com/kissbeni/tinyhttp/blob/main/http.cpp */

    struct wii_sockaddr_in remote = {};

    remote.sin_family = AF_INET;
    remote.sin_port = htons(8080);

    // FIXME: other projects seem to use remote.sin_addr.s_addr... something is missing
    remote.sin_addr.s_addr = htonl(INADDR_ANY);

    s32 ret;
    while(1) {
        ret = bind(sockfd, (struct sockaddr*)&remote, sizeof(remote));
        if(ret >= 0) {
            break;
        }
    }

    ret = listen(sockfd, 5);
    if(ret < 0) {
        return -3;
    }
    return 0;
}
