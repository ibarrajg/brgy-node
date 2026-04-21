#include <string.h>
#include <driver/uart.h>
#include "lora_uart.h"

#define LORA_UART UART_NUM_1

#define TXD_PIN 18 // change if needed, uart 1 rx on esp32s3 is pin 17
#define RXD_PIN 17 // change if needed, uart 1 tx on esp32s3 is pin 18


void lora_uart_init(void)
{
    const uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
#if ESP_IDF_VERSION_MAJOR >= 6
        .source_clk = UART_SCLK_DEFAULT,
#endif
    };

    uart_driver_install(LORA_UART, 1024, 0, 0, NULL, 0);
    uart_param_config(LORA_UART, &uart_config);
    uart_set_pin(LORA_UART, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    uart_flush_input(LORA_UART);
    vTaskDelay(pdMS_TO_TICKS(200));
    uart_flush_input(LORA_UART);
}


void lora_uart_send(const char *data)
{
    uart_write_bytes(LORA_UART, data, strlen(data));
}


int lora_uart_receive(char *buffer, int max_len)
{
    int len = uart_read_bytes(LORA_UART, (uint8_t *)buffer, max_len - 1, 50 / portTICK_PERIOD_MS);

    if (len > 0) {
        return len;
    }

    return 0;
}