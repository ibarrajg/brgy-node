#include <stdio.h>
#include <string.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "lora_uart.h"
#include "transmission.h"
#include "reception.h"
#include "node_logic.h"
#include "loop_guard.h"

#define NODE_ID 2 // This node's ID

extern "C" void app_main(void)
{
    printf("SYSTEM RUNNING\n");

    lora_uart_init();

    char payload[256];
    char raw[300];
    char type[4];
    int sender_id;
    int dst_id;

    while (1)
    {
        transmission_process();

        if (reception_process(payload, raw, &sender_id, &dst_id, type))
        {
            // Handle normal message packets
            if (strcmp(type, "MSG") == 0)
            {
                // Ignore my own message when it comes back through retransmission
                if (sender_id != NODE_ID)
                {
                    if (loop_guard(raw))
                    {
                        handle_msg(payload, raw);
                    }
                }
            }

            // Handle acknowledgement packets
            if (strcmp(type, "ACK") == 0)
            {
                // ACK is for this node
                if (dst_id == NODE_ID)
                {
                    handle_ack(dst_id, payload, raw);
                }

                // ACK is not for this node
                if (dst_id != NODE_ID)
                {
                    if (loop_guard(raw))
                    {
                        handle_ack(dst_id, payload, raw);
                    }
                }
            }
        }

        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}
