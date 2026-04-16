#include <stdio.h>
#include <string.h>
#include "lora_uart.h"
#include "node_logic.h"
#include "loop_guard.h"

// definitions
#define NODE_ID 2  // This node's ID

static int ack_received_flag = 0;

void reset_ack_flag(void)
{
    ack_received_flag = 0;
}

int is_ack_received(void)
{
    return ack_received_flag;
}

static int handle_msg(int sender_id, const char *payload, const char *raw_frame)
{
    // Ignore my own message when it comes back through retransmission
    if (sender_id == NODE_ID) {
        return ACTION_NONE;
    }

    // Ignore duplicate raw frames to prevent endless retransmission
    if (!loop_guard(raw_frame)) {
        return ACTION_NONE;
    }

    // Retransmit the exact original frame
    lora_uart_send(raw_frame);

    return ACTION_SHOW_MSG;
}

static int handle_ack(int dst_id, const char *payload, const char *raw_frame)
{
    // ACK is for this node
    if (dst_id == NODE_ID)
    {
        // Only display the first ACK for the current sent message
        if (ack_received_flag == 0)
        {
            ack_received_flag = 1;
            return ACTION_SHOW_ACK;
        }

        return ACTION_NONE;
    }

    // ACK is not for this node, so retransmit it
    lora_uart_send(raw_frame);
    return ACTION_NONE;
}

int handle_incoming_message(const char *type, int sender_id, int dst_id, const char *payload, const char *raw_frame)
{
    if (strcmp(type, "MSG") == 0) {
        return handle_msg(sender_id, payload, raw_frame);
    }

    if (strcmp(type, "ACK") == 0) {
        return handle_ack(dst_id, payload, raw_frame);
    }

    return ACTION_NONE;
}