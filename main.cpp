// main.cpp
// Created by Martin Novotny Mlinarcsik (xnovot1r) on 03.10.21.
// ISA 2021/2022 project - POP3 client

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/bio.h>
#include "pop3_handler.h"
#include "argument_parser.h"


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
    //startup_ID_check(popcl->get_out_dir());

    /** OPEN SSL **/
    if (popcl->get_flag(T_FLAG) == true) {
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



