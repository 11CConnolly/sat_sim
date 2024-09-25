#include "task_manager.h"
#include "tm_encoder.h"
#include "tc_decoder.h"

/*
 * Static Function Declarations 
 */
static void *system_check_task(void *arg);
static void *orbital_path_task(void *arg);
static void *payload_operation_task(void *arg);

/* Array of tasks with static space for keeping track */
task_t tasks[TOTAL_TASKS];

/* Map handler to map task executions */
void (*task_mapping[])(void *arg) = {
    [TSK_SYS_CHECK] = system_check_task,
    [TSK_ORB_PATH] = orbital_path_task,
    [TSK_PAYLOAD_INSTR_CAMERA] = payload_operation_task,
    [TSK_PAYLOAD_INSTR_THERMAL] = payload_operation_task,
};

/* Task IDs to TC type */
TC_TYPES task_ids_to_TCs[TOTAL_TASKS] = {TC_101, TC_102, TC_103, TC_103};

/*
 * Report back health statuses of all major systems every 2 seconds for 10
 * times (eg a System Health Status TM)
 */
static void *system_check_task(void *arg)
{
    /* Set Task Params */
    int limit = 10;
    int wait = 2;

    for (int i = 0; i < limit; i++) {
        execute_tm_201();
        sleep(wait);
    }

    /* Deactive this task once complete */
    printf("[INFO]: Finished executing System Check Task, disabling task \n");
    tasks[TSK_SYS_CHECK].active = false;

    pthread_exit(NULL);
}

/*
 * Sends its Orbital Data Report every 5 seconds for 5 times (e.g. a Orbital Data Report TM)
 */
static void *orbital_path_task(void *arg)
{
    /* Set Task Params */
    int limit = 5;
    int wait = 5;

    for (int i = 0; i < limit; i++) {
        execute_tm_202();
        sleep(wait);
    }

    /* Deactive this task once complete */
    printf("[INFO]: Finished executing Orbital Path Task, disabling task \n");
    tasks[TSK_ORB_PATH].active = false;

    pthread_exit(NULL);
}

/*
 * In case of Payload activation, the satellite sends the payload Data
 * corresponding to the given Payload ID every 10 for 1min30 (e.g. a
 * Payload Data TM)
 * 
 * In case of deactivation, the satellite stops immediately the Payload
 * Data associated to the given Payload ID
 */
static void *payload_operation_task(void *arg)
{
    /* Get payload ID */
    int payload_id = * (int *)arg;
    task_t *task = &tasks[GET_PAYLOAD_TSK_FROM_ID(payload_id)];

    printf("[INFO]: passed payload id is: %u \n", payload_id);

    /* Set Task Params */    
    int limit = 9;
    int wait = 10;

    for (int i = 0; ((i < limit) && (true == task->active)); i++) {
        execute_tm_203(payload_id);
        sleep(wait);
    }

    /* Deactive this task once complete */
    printf("[INFO]: Finished executing Payload Operations Task, disabling task \n");
    task->active = false;

    pthread_exit(NULL);
}

/*
 * Manage our task array from the received task type from TC
 */
void manage_tasks(TSK_TYPES task_id)
{
    /* Now we've received the TC for the task, examine if the task is already running */
    if (false == tasks[task_id].active)
    {
        /* Task isn't running, activate the task */
        /* In event that task isn't activated, update the task list accordingly */
        tasks[task_id].active = true;

        bool task_create_result = pthread_create(&tasks[task_id].thread, NULL, task_mapping[task_id], NULL);
                
        if (0 != task_create_result)
        {
            /* Task wasn't created successfully */
            printf("[ERROR]: Task_id %u unable to start \n", task_id);
        }
    }
    else
    {
        /* If task already started, ignore the duplicate TC Pass back informational TM */
        execute_tm_001(task_ids_to_TCs[task_id]);
    }
}

/*
 * Manage our task array from the received task type for instrument TCs
 */
void manage_instrument_tasks(TSK_TYPES task_id, int id, int operation_code)
{
    /* If task isn't active */
    if (false == tasks[task_id].active)
    {
        /* Only if the operation code is to activate, then try and create the task */
        if (ACTIVATE_INSTR == operation_code) 
        {
            /* Task isn't running, activate the task */
            /* In event that task isn't activated, update the task list accordingly */
            tasks[task_id].active = true; 
            
            bool task_create_result = pthread_create(&tasks[task_id].thread, NULL, task_mapping[task_id], &id);
            
            if (0 != task_create_result)
            {
                /* Task wasn't created successfully */
                printf("[ERROR]: Task_id %u unable to start \n", task_id);
            }
        }
        else 
        {
            /* Puplicate TC Pass back informational TM */
            execute_tm_001(task_ids_to_TCs[task_id]);
        }
    }
    else
    {
        /* 
         * If task has been started, check the operation code of the TC. 
         * If operation code is deactive, reset space
         */

        if (DEACTIVATE_INSTR == operation_code)
        {
            tasks[task_id].active = false;
        }
        else
        {
            /* Puplicate TC Pass back informational TM */
            execute_tm_001(task_ids_to_TCs[task_id]);
        }
    }
}
