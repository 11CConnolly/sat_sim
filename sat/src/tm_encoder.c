#include <stdlib.h>
#include <static/checksum.h>
#include "common.h"
#include "tc_decoder.h"
#include "tm_encoder.h"

/*
 * Send data down pipe, possibly move to a common
 */
void sendMsg(int sock, void* msg, uint32_t msgsize)
{
    // TODO - Need to add a mutex in this sendMsg call, the calling pthread will have this on it's execution thread
    // TODO - So won't be shared and will be independent
    if (sendto(sock, msg, msgsize, 0, (struct sockaddr*) single_client_addr, sizeof(struct sockaddr_in)) < 0)
    {
        printf("[ERROR]: Can't send message.\n");

        /* 
         * If we cannot send the packet, for simulation purposes we close the socket and exit, however in a real-world situation
         * It may be expected to not be able to connect and send messages due to ephemeris, occlusion, or orbits
         * 
         * In this situation, we consider the error is significant and not caused due to lack of connection with a client, but
         * due to instrumentation failure and exit. In situations of 'expected' data loss the best solution would may be to continue 
         * to send data, without interrupting our program execution, until reply is given, a timeout is hit, or otherwise.
         */
        
        close(sock);
        exit(1);
    }

    printf("[INFO]: Message sent (%d bytes).\n", msgsize);
}

/*
 * Produce a TM(201)
 */
void execute_tm_201()
{
    printf("[INFO]: Trying to send a tm 201\n");
    
    /* Create our data */
    tm_201 tm_data = (tm_201) {
        .status_code = TM_201,
        .cpu_usage = system_status.cpu_usage,
        .memory_usage = system_status.memory_usage,
        .battery_level = system_status.battery_level
    };

    /* Convert to contiguous data to compute checksum */
    char checksum_buff[DATA_LEN_TM_201];
    memcpy(checksum_buff, &tm_data, DATA_LEN_TM_201);

    /* Add on checksum byte */
    tm_data.checksum = compute_XOR_checksum(checksum_buff, DATA_LEN_TM_201);

    /* Send our actual message now */
    sendMsg(sock, &tm_data, sizeof(tm_201));
}

/*
 * Produce a TM(202)
 */
void execute_tm_202()
{
    printf("[INFO]: Trying to send a tm 202\n");

    /* Create our data */
    tm_202 tm_data = (tm_202) {
        .status_code = TM_202,
        .Vx = 10.0f,
        .Vy = 10.0f,
        .Vz = 10.0f
    };

    /* Convert to contiguous data to compute checksum */
    char checksum_buff[DATA_LEN_TM_202];
    memcpy(checksum_buff, &tm_data, DATA_LEN_TM_202);

    /* Add on checksum byte */
    tm_data.checksum = compute_XOR_checksum(checksum_buff, DATA_LEN_TM_202);

    /* Send our actual message now */
    sendMsg(sock, &tm_data, sizeof(tm_202));
}

/*
 * Produce a TM(203)
 */
void execute_tm_203(int payload_id)
{
    printf("[INFO]: Trying to send a tm 203\n");
    
    payload_instr *instr = &payload_instruments[payload_id];

    /* Fetch our data from the Instrumentation array */
    tm_203 tm_data = (tm_203) {
        .status_code = TM_203,
        .operational_status = instr->status,
        .payload_id = payload_id,
        .m_1 = instr->m1,
        .m_2 = instr->m2,
        .m_3 = instr->m3
    };

    /* Convert to contiguous data to compute checksum */
    char checksum_buff[DATA_LEN_TM_203];
    memcpy(checksum_buff, &tm_data, DATA_LEN_TM_203);

    /* Add on checksum byte */
    tm_data.checksum = compute_XOR_checksum(checksum_buff, DATA_LEN_TM_203);

    /* Send our actual message now */
    sendMsg(sock, &tm_data, sizeof(tm_203));
}


/*
 * Produce a TM(001)
 */
void execute_tm_001(uint32_t command_id)
{
    printf("[INFO]: Trying to send a tm 001\n");
    
    /* Create our data */
    tm_001 tm_data = (tm_001) {
        .status_code = TM_001,
        .command_id = command_id
    };

    /* Convert to contiguous data to compute checksum */
    char checksum_buff[DATA_LEN_TM_001];
    memcpy(checksum_buff, &tm_data, DATA_LEN_TM_001);

    /* Add on checksum byte */
    tm_data.checksum = compute_XOR_checksum(checksum_buff, DATA_LEN_TM_001);

    /* Send our actual message now */
    sendMsg(sock, &tm_data, sizeof(tm_001));
}


/*
 * Produce a TM(002)
 */
void execute_tm_002(uint32_t command_id)
{
    printf("[INFO]: Trying to send a tm 002\n");
    
    /* Create our data */
    tm_002 tm_data = (tm_002) {
        .status_code = TM_002,
        .command_id = command_id
    };

    /* Convert to contiguous data to compute checksum */
    char checksum_buff[DATA_LEN_TM_002];
    memcpy(checksum_buff, &tm_data, DATA_LEN_TM_002);

    /* Add on checksum byte */
    tm_data.checksum = compute_XOR_checksum(checksum_buff, DATA_LEN_TM_002);

    /* Send our actual message now */
    sendMsg(sock, &tm_data, sizeof(tm_002));
}
