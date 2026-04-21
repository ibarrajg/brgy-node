#include <string.h>
#include "loop_guard.h"

int loop_guard(const char *raw_frame)
{
    static char last_raw[1024] = "";

    // Only allow retransmission if this is not the same
    // as the last raw frame already handled
    if (strcmp(raw_frame, last_raw) != 0) {
        strcpy(last_raw, raw_frame);
        return 1;
    }

    return 0;
}
