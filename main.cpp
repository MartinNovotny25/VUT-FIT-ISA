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

#include <experimental/filesystem>
#include <iostream>
#include <fstream>



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

    create_mail_ID_file();

    /** OPEN SSL **/
    if (popcl->get_flag(T_FLAG) == true) {
        if (popcl->get_flag(PORT_FLAG) == false)
        {
            /** Ak nie je zadany port, pripajame sa na 995 **/
            popcl->set_port("995");
        }
        if (popcl->establish_ssl_connection() != 0) {
            std::cerr << "Secure Connection not established, exiting" << std::endl;
            exit(1);
        }

    /** TLS COMMAND -S **/
    } else if(popcl->get_flag(S_FLAG) == true)
    {
        if (popcl->establish_tls_connection() != 0) {
            std::cerr << "TLS connection not established, exiting" << std::endl;
            exit(1);
        }
    }
    /** UNSECURED **/
    else {

        if (popcl->establish_connection() != 0) {
            std::cerr << "Unsecure connection not established, exiting" << std::endl;
            exit(1);
        }
    }
    return 0;
}



