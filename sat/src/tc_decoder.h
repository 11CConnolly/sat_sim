#ifndef TC_DECODER_H
#define TC_DECODER_H

#include <stdint.h>

/*
 * Buffer size defines
 */
#define DATA_LEN_TC_101 (sizeof(uint32_t))
#define DATA_LEN_TC_102 (sizeof(uint32_t) + (3 * sizeof(float)))
#define DATA_LEN_TC_103 ((3 * sizeof(uint32_t)))

/*
 * ENUM for the TeleCommand Type
 */
typedef enum TC_TYPES {
    TC_101 = 101,
    TC_102 = 102,
    TC_103 = 103
} TC_TYPES;

/*
 * TeleCommand Parameters themselves
 */
typedef struct tc_101_t {
  uint32_t command_id;
  uint8_t checksum;
} tc_101;

typedef struct tc_102_t {
  uint32_t command_id;
  float deltaVx;
  float deltaVy;
  float deltaVz;
  uint8_t checksum;
} tc_102;

typedef struct tc_103_t {
  uint32_t command_id;
  uint32_t operation_code;
  uint32_t payload_id;
  uint8_t checksum;
} tc_103;

/* Public Function Definitions */

void handle_tc_101(char *buff);

void handle_tc_102(char *buff);

void handle_tc_103(char *buff);

#endif /* TC_DECODER_H */
