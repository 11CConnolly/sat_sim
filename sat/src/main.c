#include <linux/limits.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <unistd.h>    
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "common.h"
#include "tc_decoder.h"

/* Map handler to executions */
void (*handler_tc_mapping[])(char *buff) = {
    [TC_101] = &handle_tc_101,
    [TC_102] = &handle_tc_102,
    [TC_103] = &handle_tc_103,
};

/* Generic TC packet to decode just the command and pass to TC handler */
typedef struct recv_generic_tc_t {
  uint32_t command_id;
} recv_generic_tc;

/* 
 * Global define of our system parameters 
 */

/* System Params */
system_params_t system_status;
/* Instrument Data */
payload_instr payload_instruments[TOTAL_INSTRUMENTS];
/* Global sock to send data */
int sock;
/* Assuming only a singly connecting client(simulator) */
struct sockaddr_in *single_client_addr;


static void handle_tc(char *buff);
static int create_server_socket(int port);

/*
 * Private Function Definitions 
 */

/* Decode TC, validate, and call handling function as appropraite */
static void handle_tc(char *buff) 
{    
    /* Check the received data and cast as appropriate */
    recv_generic_tc *buff_ptr = (recv_generic_tc *) buff;
    
    TC_TYPES tc_type = buff_ptr->command_id;

    printf("[INFO] Received packet=%d \n", tc_type);

    /* Validate type, could probably check parameters as well */
    bool isValidTcType = ((tc_type != TC_101) || 
                          (tc_type != TC_102) ||
                          (tc_type != TC_103));

    if (isValidTcType) 
    {
        /* Handle TC as appropriate for the handler */
        handler_tc_mapping[tc_type](buff);
    }
    else
    {
        printf("[ERROR]: Invalid tc received \n");
    }
}

/* create sockets() server */
static int create_server_socket(int port)
{
    int server_sock;
    struct sockaddr_in server = {0};

    // Internet Protocol IPv4 Address family
    // SOCK_STREAM mimicking TCP (TODO - Make Datagram Packet)
    if ((server_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        printf("[ERROR]: Socket creation failed\n");
        exit(1);
    }
    
    /* Set reuse parameter to avoid TIME_WAIT */
    int reuse = 1;
    int result = setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, (void *)&reuse, sizeof(reuse));
    if ( result < 0 ) {
        perror("[ERROR]: SO_REUSEADDR:");
    }

    printf("[DEBUG]: Socket created\n");

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);

    if (bind(server_sock, (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        printf("ERROR: Bind failed\n");
        exit(1);
    }
    printf("[DEBUG]: Bind done\n");

    return server_sock;
}


/*
 * Main entry point
 */
int main(void)
{
    const int PORT = 2300;

    /* Define local buffer var */
    char buff[BUFFSIZE] = {0};
    
    /* Declare our net vars */
    struct sockaddr_in client_addr;
    int client_struct_length = sizeof(client_addr);
    int ssock;
    
    ssock = create_server_socket(PORT);
    sock = ssock;
    
    printf("[DEBUG]: Server socket created on port %d\n", PORT);

    /* Main loop of program, pend on reading from socket data until event */
    while (true) 
    {
        /* Successful connection, print, zero out buffer, and set global sock to out server sock */
        bzero(buff, BUFFSIZE);

        // Wait for data from client
        ssize_t n = recvfrom(ssock, (char *)buff, BUFFSIZE, MSG_WAITALL, (struct sockaddr *)&client_addr, &client_struct_length);
        buff[n] = '\0';  // Null-terminate received string
        printf("Received: %lu bytes\n", n);

        /* Set the client address */
        single_client_addr = &client_addr;

        handle_tc(buff);
    }
    
    /* close server socket */
    close(ssock);

    return 0;
}
