#ifndef COMMON_H
#define COMMON_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

/* Buffer and network */
#define BUFFSIZE (64)

/* Tasks */
#define GET_PAYLOAD_TSK_FROM_ID(id) ((TSK_ORB_PATH + 1) + id)
#define TOTAL_TASKS (4)

/* Instruments */
#define TOTAL_INSTRUMENTS (2)
#define ACTIVATE_INSTR (1)
#define DEACTIVATE_INSTR (0)

/* Structure to contain system parameters */
typedef struct system_params_t {
  /* Percentage of CPU in use */
  float cpu_usage;

  /* Percentage of memory in use */
  float memory_usage;

  /* Remaining battery power as a percentage */
  float battery_level;
} system_params_t;

/* Structure for format of payload packet */
typedef struct payload_instr_t {
    uint32_t id;
    uint32_t status;
    float m1;
    float m2;
    float m3;
} payload_instr;

/* Enumeration for different tasks */
typedef enum TSK_TYPES {
  TSK_SYS_CHECK,
  TSK_ORB_PATH,
  TSK_PAYLOAD_INSTR_CAMERA,
  TSK_PAYLOAD_INSTR_THERMAL
} TSK_TYPES;

extern system_params_t system_status;
extern payload_instr payload_instruments[TOTAL_INSTRUMENTS];
extern int sock;
extern struct sockaddr_in *single_client_addr;

#endif /* COMMON_H */
