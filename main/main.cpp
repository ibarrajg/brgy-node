#include <stdio.h>
#include <string.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "lora_uart.h"
#include "transmission.h"
#include "reception.h"
#include "node_logic.h"
#include "back_off_retry_logic.h"

static void transmission_task(void *pvParameters)
{
    char last_sent_frame[300];

    while (1)
    {
        if (transmission_process(last_sent_frame))
        {
            reset_ack_flag();
            start_ack_wait(last_sent_frame);
        }

        if (is_waiting_for_ack() && is_ack_received())
        {
            stop_ack_wait();
        }
        else if (should_retry_ack(2000))
        {
            backoff_delay(1000, 3000);
            lora_uart_send(get_retry_frame());
            reset_ack_flag();
            mark_retry_used();
        }

        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

static void reception_task(void *pvParameters)
{
    char payload[256];
    char raw[300];
    char type[4];
    int sender_id;
    int dst_id;
    int action;

    while (1)
    {
        if (reception_process(payload, raw, &sender_id, &dst_id, type))
        {
            action = handle_incoming_message(type, sender_id, dst_id, payload, raw);

            if (action != ACTION_NONE)
            {
                printf("%s\n", payload);
            }
        }

        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

extern "C" void app_main(void)
{
    printf("BARANGAY NODE ACTIVE\n");

    lora_uart_init();

    xTaskCreatePinnedToCore(
        transmission_task,
        "tx_task",
        4096,
        NULL,
        5,
        NULL,
        0
    );

    xTaskCreatePinnedToCore(
        reception_task,
        "rx_task",
        4096,
        NULL,
        5,
        NULL,
        1
    );
}
