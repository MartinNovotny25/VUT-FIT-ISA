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

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/bio.h>

#include "pop3_handler.h"
#include "argument_parser.h"


/** PORT 110, NAME pop3.centrum.sk, IP 46.255.231.11 **/

int main(int argc, char *argv[]) {

    /** OPEN SSL **/
    SSL_load_error_strings();
    ERR_load_BIO_strings();
    OpenSSL_add_all_algorithms();


    auto *popcl = new POP3_handler();

    int returnCode = arg_parse(argc, argv, popcl);
    if (returnCode == -1) {
        std::cerr << "BAD OPTION" << std::endl;
        return 1;
    }

    if (popcl->establish_connection() != 0)
    {
        std::cerr << "Connection not established, exiting" << std::endl;
        exit(1);
    };


    return 0;



}



