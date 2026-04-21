#include <stdio.h>
#include <string.h>
#include "get_message.h"

#define MAX_MSG_LEN 300

int get_user_message(char *out_msg)
{
    if (fgets(out_msg, MAX_MSG_LEN, stdin) == NULL) {
        return 0;
    }

    size_t len = strlen(out_msg);

    // If newline is present, this is a complete line
    if (len > 0 && out_msg[len - 1] == '\n') {
        out_msg[len - 1] = '\0';

        if (strlen(out_msg) == 0) {
            return 0;
        }

        return 1;
    }

    // No newline means the input line was longer than the buffer.
    // Discard the rest of the line so it does not become a second message.
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
    }

    // Reject overlong message instead of sending a cut-up partial message
    return 0;
}
