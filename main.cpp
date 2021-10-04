#include <iostream>
#include <fstream>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "pop3_handler.h"
#include "argument_parser.h"


/** PORT 110, NAME pop3.centrum.sk, IP 46.255.231.11 **/

int main(int argc, char *argv[]) {
    auto *popcl = new POP3_handler();

    int returnCode = arg_parse(argc, argv, popcl);
    if (returnCode == -1) {
        std::cerr << "BAD OPTION" << std::endl;
        return 1;
    }

    popcl->establish_connection();


    return 0;



}



