/*
 *------------------------------------------------------------------
 * test_pcp_client_peer_opcode.c
 *
 * March 15th, 2013, mbagljas
 *
 * Copyright (c) 2013-2013 by cisco Systems, Inc.
 * All rights reserved.
 *
 *------------------------------------------------------------------
 */


#include <stdio.h>
#include <stdlib.h>

#ifndef WIN32
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#include "pcp_socket.h"
#include <string.h>
#include "pcp.h"
#include "unp.h"
#include "test_macro.h"

int main(int argc, char *argv[]) {
    struct sockaddr_storage destination;
    struct sockaddr_storage source;
    struct sockaddr_storage ext;

    uint8_t protocol = 6;
    uint32_t lifetime = 10;

    pcp_flow_t flow = NULL;

    PD_SOCKET_STARTUP();
    pcp_log_level = 5;

    memset(&source, 0, sizeof(source));
    memset(&destination, 0, sizeof(destination));
    memset(&ext, 0, sizeof(ext));

    TEST(pcp_add_server(Sock_pton("127.0.0.1:5351"), 2)==0);

    sock_pton("10.10.10.10:1234", (struct sockaddr*) &destination);
    sock_pton("127.0.0.1:1235", (struct sockaddr*) &source);
    sock_pton("0.0.0.0", (struct sockaddr*) &ext);

    printf("\n");
    printf("#########################################\n");
    printf("####   *************************     ####\n");
    printf("####   *    Begin PEER test    *     ####\n");
    printf("####   *************************     ####\n");
    printf("#########################################\n");

    flow = pcp_new_flow((struct sockaddr*)&source,
                        (struct sockaddr*)&destination,
                        (struct sockaddr*)&ext,
                        protocol, lifetime);

    TEST(pcp_wait(flow, 3000, 0) == pcp_state_succeeded);

    pcp_close_flow(flow);
    pcp_delete_flow(flow);
    flow = NULL;

    PD_SOCKET_CLEANUP();
    pcp_terminate(0);

    return 0;
}