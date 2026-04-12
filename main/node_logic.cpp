#include <stdio.h>
#include "lora_uart.h"
#include "node_logic.h"

#define NODE_ID 2  // This node's ID
void handle_msg(const char *payload, const char *raw_frame)
{
    // Display the message payload
    printf("%s\n", payload);

    // Retransmit the exact original frame
    lora_uart_send(raw_frame);

    //printf("Retransmitted: %s\n", raw_frame);
}

void handle_ack(int dst_id, const char *payload, const char *raw_frame)
{
    // If the ACK is for this node, display it
    if (dst_id == NODE_ID) {
        printf("%s\n", payload);
    }
    // Otherwise, forward it
    else {
        lora_uart_send(raw_frame);
        //printf("%s\n", raw_frame);
    }
}