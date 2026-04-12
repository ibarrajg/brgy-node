#include <stdio.h>
#include <string.h>

#include "transmission.h"
#include "get_message.h"
#include "message_frame.h"
#include "lora_uart.h"


int transmission_process(void)
{
    char msg[256];
    char frame[300];

    // Step 1: Get user message
    if (get_user_message(msg)) {

        // Step 2: Build frame
        create_message_frame(msg, frame);

        // Step 3: Send via LoRa UART
        lora_uart_send(frame);

        //printf("sent: %s\n", frame); 

        return 1;
    }

    return 0; // no message yet
}