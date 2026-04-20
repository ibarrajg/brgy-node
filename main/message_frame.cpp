#include <stdio.h>
#include <string.h>
#include "message_frame.h"
#include "crc.h"   // use shared CRC module

// ===== Hardcoded fields =====
static const char *SRC  = "02"; // This node's ID, hardcoded, change accordingly for each node in testing
static const char *DST  = "00";
static const char *TYPE = "MSG";


// ===== Frame builder =====
void create_message_frame(const char *input, char *output)
{
    int len = strlen(input);

    char temp[512];

    // Build frame WITHOUT CRC
    // Format: SRC|DST|TYPE|LEN|<PAYLOAD>
    sprintf(temp, "%s|%s|%s|%d|<%s>", SRC, DST, TYPE, len, input);

    // Compute CRC using shared module
    unsigned short crc = calculate_crc16(temp);

    // Final frame with CRC and frame markers { }
    sprintf(output, "{%s|%04X}", temp, crc);
}