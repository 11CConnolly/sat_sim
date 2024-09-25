#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#include "common.h"

/* Space for thread and activation indicator */
typedef struct task_t {
  pthread_t thread;
  bool active;
} task_t;

extern struct task_t tasks[TOTAL_TASKS];

/* Public Function Definitions */

void manage_tasks(TSK_TYPES task_id);

void manage_instrument_tasks(TSK_TYPES task_id, int id, int operation);

#endif /* TASK_MANAGER_H */
