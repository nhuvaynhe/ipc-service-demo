#ifndef __IPC_SOCK_H__
#define __IPC_SOCK_H__

#include <libposix.h>
#include <stdint.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

#define SOCK_ADDR_LEN   (INET_ADDRSTRLEN)

//socket structs

enum sock_type {
    SOCK_TYPE_UDP = 0,
    SOCK_TYPE_TCP,
    SOCK_TYPE_UNIX,

    SOCK_TYPE_MAX,
};

typedef struct sock_addr {
    char ip_str[SOCK_ADDR_LEN];
    uint32_t ip;
    uint16_t port;
} sock_addr_t;

typedef struct sock_addr_list {
    sock_addr_t addr;
    struct sock_addr_list *next;
} sock_addr_list_t;

typedef struct sock_connection {
    int fd;
    uint64_t fd64;
    int type;
    struct sock_addr local;
    struct sock_addr remote;
} sock_connection_t;


#endif