#ifndef NODE_LOGIC_H
#define NODE_LOGIC_H

#ifdef __cplusplus

extern "C" {
#endif

#define ACTION_NONE 0
#define ACTION_SHOW_MSG 1
#define ACTION_SHOW_ACK 2

void reset_ack_flag(void);
int is_ack_received(void);
int handle_incoming_message(const char *type, int sender_id, int dst_id, const char *payload, const char *raw_frame);


#ifdef __cplusplus
}
#endif

#endif
