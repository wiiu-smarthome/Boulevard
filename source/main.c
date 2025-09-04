#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ios.h"
#include "log.h"
#include "syscalls.h"

#include "heap.h"
#include "network.h"

int main(void)
{
    int ret = 0;

    /* Print info */
    svc_write("$IOSVersion: Boulevard running\n");

    ret = heap_init();
    if(ret < 0) {
        return ret;
    }

    ret = net_init();
    if(ret < 0) {
        return ret;
    }

    return ret;
}
