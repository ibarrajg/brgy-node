#include <stdio.h>
#include <string.h>
#include "get_message.h"

#define MAX_MSG_LEN 512
int get_user_message(char *out_msg)
{
    if (fgets(out_msg, MAX_MSG_LEN, stdin) != NULL)
    {
        // remove newline
        out_msg[strcspn(out_msg, "\n")] = '\0';

        // ignore empty input (just ENTER)
        if (strlen(out_msg) == 0) {
            return 0;
        }

        return 1;
    }

    return 0;
}