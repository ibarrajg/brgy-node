#ifndef NODE_LOGIC_H
#define NODE_LOGIC_H

#ifdef __cplusplus
extern "C" {
#endif

void handle_msg(const char *payload, const char *raw_frame);
void handle_ack(int dst_id, const char *payload, const char *raw_frame);

#ifdef __cplusplus
}
#endif

#endif
