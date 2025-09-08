#include <assert.h>
#include <stdarg.h>

#include "avenue.h"
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

    ret = heap_init();
    if (ret < 0) {
        return ret;
    }

    ret = net_init();
    if (ret < 0) {
        return ret;
    }

    ret = avenue_init();
    if (ret < 0) {
        return ret;
    }

    return ret;
}
