#include <stdlib.h>
#include <string.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_system.h>
#include <esp_random.h>
#include "back_off_retry_logic.h"

static int waiting_for_ack = 0;
static int retry_used = 0;
static TickType_t ack_wait_start = 0;
static char last_sent_frame[512] = "";

void start_ack_wait(const char *frame)
{
    waiting_for_ack = 1;
    retry_used = 0;
    ack_wait_start = xTaskGetTickCount();
    strcpy(last_sent_frame, frame);
}

void stop_ack_wait(void)
{
    waiting_for_ack = 0;
}

int is_waiting_for_ack(void)
{
    return waiting_for_ack;
}

int should_retry_ack(unsigned int timeout_ms)
{
    if (!waiting_for_ack) {
        return 0;
    }

    if (retry_used) {
        return 0;
    }

    TickType_t now = xTaskGetTickCount();
    TickType_t elapsed = now - ack_wait_start;

    if (elapsed >= pdMS_TO_TICKS(timeout_ms)) {
        return 1;
    }

    return 0;
}

void mark_retry_used(void)
{
    retry_used = 1;
    ack_wait_start = xTaskGetTickCount();
}

int retry_already_used(void)
{
    return retry_used;
}

const char *get_retry_frame(void)
{
    return last_sent_frame;
}

void backoff_delay(unsigned int min_ms, unsigned int max_ms)
{
    unsigned int delay_ms = min_ms;

    if (max_ms > min_ms) {
        delay_ms += esp_random() % (max_ms - min_ms + 1);
    }

    vTaskDelay(pdMS_TO_TICKS(delay_ms));
}
