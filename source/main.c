#include <assert.h>
#include <stdarg.h>

#include "avenue.h"
#include "debug.h"
#include "heap.h"
#include "ios.h"
#include "network.h"
#include "syscalls.h"

char *moduleName = "BOULEVARD";

int main(void)
{
    int ret = 0;

    /* Print info */
    svc_write("$IOSVersion: Boulevard running\n");

    blvd_state = BLVD_STATE_START;

    ret = debug_init();
    if (ret < 0) {
        return ret;
    }

    /* Initialize heap */
    blvd_state = BLVD_STATE_INIT_HEAP;
    ret = heap_init();
    if (ret < 0) {
        blvd_state = BLVD_STATE_HEAP_INIT_FAIL;
        return ret;
    }

    blvd_state = BLVD_STATE_INIT_NET;
    ret = net_init();
    if (ret < 0) {
        return ret;
    }

    blvd_state = BLVD_STATE_INIT_AVENUE;
    ret = avenue_init();
    if (ret < 0) {
        return ret;
    }

    return ret;
}
