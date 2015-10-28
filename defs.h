#ifndef DEFS_H_
#define DEFS_H_

#include <stdlib.h>

// one less than the greatest port number shown here:
// https://en.wikipedia.org/wiki/List_of_TCP_and_UDP_port_numbers
static const int PORT = 53639;
static const char SERVER_HELLO[] = "server-hello";
static const size_t DATAGRAM_MAX_READ = 81;
static const int MAX_ERRORS = 10;

#endif
