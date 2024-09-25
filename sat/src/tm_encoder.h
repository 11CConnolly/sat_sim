#ifndef TM_ENCODER_H
#define TM_ENCODER_H

#include <stdint.h>

/*
 * Buffer size defines
 */
#define DATA_LEN_TM_201 (sizeof(uint32_t) + (3 * sizeof(float)))
#define DATA_LEN_TM_202 (sizeof(uint32_t) + (3 * sizeof(float)))
#define DATA_LEN_TM_203 ((3 * sizeof(uint32_t)) + (3 * sizeof(float)))
#define DATA_LEN_TM_001 (2 * sizeof(uint32_t))
#define DATA_LEN_TM_002 (2 * sizeof(uint32_t))

typedef enum TM_TYPES {
    TM_201 = 201,
    TM_202 = 202,
    TM_203 = 203,
    TM_001 = 001,
    TM_002 = 002
} TM_TYPES;

/*
 * Telemetry Parameters themselves.
 */
typedef struct tm_201_t {
  uint32_t status_code;
  float cpu_usage;
  float memory_usage;
  float battery_level;
  uint8_t checksum;
} tm_201;

typedef struct tm_202_t {
  uint32_t status_code;
  float Vx;
  float Vy;
  float Vz;
  uint8_t checksum;
} tm_202;

typedef struct tm_203_t {
  uint32_t status_code;
  uint32_t payload_id;
  uint32_t operational_status;
  float m_1;
  float m_2;
  float m_3;
  uint8_t checksum;
} tm_203;

/* Error TM for informational / low priority issues */
typedef struct tm_001_t {
  uint32_t status_code; /* Status code containing TM type */
  uint32_t command_id;  /* Command ID of the TC there was an issue with */
  uint8_t checksum;     /* Checksum byte */
} tm_001;

/* Error TM for significant errors */
typedef struct tm_002_t {
  uint32_t status_code; /* Status code containing TM type */
  uint32_t command_id;  /* Command ID of the TC there was an issue with */
  uint8_t checksum;     /* Checksum byte */
} tm_002;

/* Public Function Definitions */

void sendMsg(int sock, void* msg, uint32_t msgsize);

void execute_tm_201();

void execute_tm_202();

void execute_tm_203(int payload_id);

void execute_tm_001(uint32_t command_id);

void execute_tm_002(uint32_t command_id);

#endif /* TM_ENCODER_H */
