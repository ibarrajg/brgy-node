#include <stdio.h>
#include <string.h>

#include "reception.h"
#include "lora_uart.h"
#include "frame_parser.h"

int reception_process(char *payload_out,
                      char *raw_out,
                      int *sender_id,
                      int *dst_id,
                      char *type_out)
{
    char rx_buffer[1024];

    // Step 1: Receive data from UART LoRa
    int len = lora_uart_receive(rx_buffer, sizeof(rx_buffer) - 1);

    if (len > 0) {
        // Step 2: Null-terminate
        rx_buffer[len] = '\0';

        // Step 3: Print raw data
        //printf("Raw Received: %s\n", rx_buffer);

        // Step 4: Copy raw frame for retransmission use
        strcpy(raw_out, rx_buffer);

        // Step 5: Parse frame to extract payload, sender, destination, and type
        if (parse_message_frame(rx_buffer, payload_out, sender_id, dst_id, type_out)) {
            //printf("%s\n", payload_out);
            return 1;
        } else {
            printf("Invalid frame or CRC error\n");
        }
    }

    return 0;
}
