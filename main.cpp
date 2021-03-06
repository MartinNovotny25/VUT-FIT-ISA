 /** main.cpp
 * Created by Martin Novotny Mlinarcsik (xnovot1r) on 03.10.21.
 * ISA 2021/2022 project - POP3 client
 * @file main file setting up correct connection
 * @author Martin Novotny Mlinarcsik <xnovot1r@stud.fit.vutbr.cz>
 *
 *
 */

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/bio.h>
#include "pop3_handler.h"
#include "argument_parser.h"

#include "file_parser.h"


int main(int argc, char *argv[]) {

    /* Load openssl libraries */
    SSL_load_error_strings();
    ERR_load_BIO_strings();
    OpenSSL_add_all_algorithms();
    SSL_library_init();

    /* New POP3_handler class object */
    auto popcl = new POP3_handler();

    /* Parses arguments */
    int returnCode = arg_parse(argc, argv, popcl);
    if (returnCode == -1) {
        return 1;
    }

    /* We create mail_ID_file if not created */
    create_mail_ID_file();

    /* Delete IDs of emails removed from client PC in out_dir */
    delete_removed_IDs(popcl->get_out_dir());

    /* Depending on desired type of connection */
    /* SSL -T*/
    if (popcl->get_flag(T_FLAG) == true) {
        if (popcl->establish_ssl_connection() != 0) {
            exit(1);
        }

    /* TLS COMMAND -S */
    } else if(popcl->get_flag(S_FLAG) == true)
    {
        if (popcl->establish_tls_connection() != 0) {
            exit(1);
        }
    }
    /* UNSECURED */
    else {

        if (popcl->establish_connection() != 0) {
            exit(1);
        }
    }
    return 0;
}



