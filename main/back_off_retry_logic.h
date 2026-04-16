#ifndef BACK_OFF_RETRY_LOGIC_H
#define BACK_OFF_RETRY_LOGIC_H

#ifdef __cplusplus
extern "C" {
#endif

void start_ack_wait(const char *frame);
void stop_ack_wait(void);
int is_waiting_for_ack(void);
int should_retry_ack(unsigned int timeout_ms);
void mark_retry_used(void);
int retry_already_used(void);
const char *get_retry_frame(void);
void backoff_delay(unsigned int min_ms, unsigned int max_ms);

#ifdef __cplusplus
}
#endif

#endif
