#include "heap.h"
#include "mem.h"

// Initialize memory and create shared heap
s32 heap_init(void)
{
    static u32 hs[0x1000] ATTRIBUTE_ALIGN(32);

    s32 ret = Mem_Init(heapspace, sizeof(heapspace));
    if (ret < 0) {
        return ret;
    }

    heapspace = hs;

    return 0;
}
