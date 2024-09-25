#include <stdlib.h>
#include <static/checksum.h>
#include "common.h"
#include "task_manager.h"
#include "tc_decoder.h"
#include "tm_encoder.h"

/*
 * Handle a TC(101) casting from the given buffer
 */
void handle_tc_101(char *buff)
{
    tc_101 *tc = (tc_101 *)buff;
    
    // TODO - Verify and Validate TC Params

    /* Check checksum against received buffer */
    if (tc->checksum == compute_XOR_checksum(buff, DATA_LEN_TC_101))
    {
        /* Checksum is okay, continue with task creation */
        printf("[INFO]: Checksum OK. Handle TC command ID: %u \n\n", tc->command_id);

        /* Send request to start the TC */
        manage_tasks(TSK_SYS_CHECK);
    }
    else
    {
        // Issue error TM 
        printf("[ERROR]: Checksum failed for %u \n", tc->command_id);
        execute_tm_002(tc->command_id);
    }
}

/*
 * Handle a TC(102) casting from the given buffer
 */
void handle_tc_102(char *buff)
{
    tc_102 *tc = (tc_102 *)buff;
    
    // TODO Verify and Validate TC Params

    printf("[INFO]: Value of command is: %u \n\n", tc->command_id);
    printf("[INFO]: Value of command is: %f \n\n", tc->deltaVx);
    printf("[INFO]: Value of command is: %f \n\n", tc->deltaVy);
    printf("[INFO]: Value of command is: %f \n\n", tc->deltaVz);

    /* Check checksum against received buffer */
    if (tc->checksum == compute_XOR_checksum(buff, DATA_LEN_TC_102))
    {
        /* Checksum is okay, continue with task creation */
        printf("[INFO]: Checksum OK. Handle TC command ID: %u \n\n", tc->command_id);

        /* Send request to start the TC */
        manage_tasks(TSK_ORB_PATH);
    }
    else
    {
        // Issue error TM
        printf("[ERROR]: Checksum failed for %u \n", tc->command_id);
        execute_tm_002(tc->command_id);
    }
}

/*
 * Handle a TC(103) casting from the given buffer
 */
void handle_tc_103(char *buff)
{
    tc_103 *tc = (tc_103 *)buff;

    // TODO Verify and Validate TC Params

    printf("[INFO]: Value of command is: %u \n", tc->command_id);
    printf("[INFO]: Value of operation code is: %u \n", tc->operation_code);
    printf("[INFO]: Value of payload id is: %u \n", tc->payload_id);
    
    /* Check checksum against received buffer */
    if (tc->checksum == compute_XOR_checksum(buff, DATA_LEN_TC_103))
    {
        /* Checksum is okay, continue with task creation */
        printf("[INFO]: Checksum OK. Handle TC command ID: %u \n\n", tc->command_id);

        /* Either activate or deactivate the task appropriately */
        manage_instrument_tasks(GET_PAYLOAD_TSK_FROM_ID(tc->payload_id), 
                                tc->payload_id,
                                tc->operation_code);
    }
    else
    {
        // Issue error TM 
        printf("[ERROR]: Checksum failed for %u \n", tc->command_id);
        execute_tm_002(tc->command_id);
    }
}
