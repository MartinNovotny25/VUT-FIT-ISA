/** pop3_handler.h
* Created by Martin Novotny Mlinarcsik (xnovot1r) on 03.10.21.
* ISA 2021/2022 project - POP3 client
 *
* @file pop3_handler class methods and variables.
* @author Martin Novotny Mlinarcsik <xnovot1r@stud.fit.vutbr.cz>
*
*
*/


#ifndef POP3_CLIENT_POP3_HANDLER_H
#define POP3_CLIENT_POP3_HANDLER_H

#include <iostream>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/bio.h>

#include "file_parser.h"

/* Macros for getters and setters */
#define PORT_FLAG 0
#define ADDR_FLAG 1
#define T_FLAG 2
#define S_FLAG 3
#define CERT_FLAG 4
#define CERT_LOCATION_FLAG 5
#define DELETE_FLAG 6
#define NEW_FLAG 7
#define AUTH_FILE_FLAG 8
#define OUT_DIR_FLAG 9

class POP3_handler {
private:
    /**
     * Server address.
     */
    std::string address;

    /**
     * Server port.
     */
    std::string port;

    /**
     * Designated certification file.
     */
    std::string cert_file;

    /**
     * Designated certification folder.
     */
    std::string cert_location;

    /**
     * Designated authentication file with username and password.
     */
    std::string auth_file;

    /**
     * Designated out directory for downloaded emails.
     */
    std::string out_dir;

    /**
     * Pointer to *BIO object.
     */
    BIO* pop3_bio;

    /**
     * Buffer from messages received from server.
     */
    char recv_buffer[1024];

    /**
     * Buffer for outgoing messages to server.
     */
    const void* send_buffer;

    /**
     * Port number inputted.
     */
    bool portFlag;


    /**
     * Address inputted.
     */
    bool addrFlag;

    /**
     * -T parameter inputted.
     */
    bool T_Flag;

    /**
    * -S parameter inputted.
    */
    bool S_Flag;

    /**
    * Cert file inputted.
    */
    bool certFlag;

    /**
    * Cert folder inputted.
    */
    bool cert_locationFlag;

    /**
    * -d parameter inputted
    */
    bool deleteFlag;

    /**
    * -n parameter inputted.
    */
    bool newFlag;

    /**
    * Auth file inputted.
    */
    bool auth_fileFlag;

    /**
    * Outgoing directory included.
    */
    bool out_dirFlag;

public:

    /**
    * Default constructor.
    */
    POP3_handler();


    /**
    * Server address setter.
    */
    void set_address(std::string arg_address);

    /**
    * Server port setter.
    */
    void set_port (std::string arg_port);

    /**
    * Input parameters flag setter.
    */
    void set_flag(int in_flag);

    /**
    * Cert file variable setter.
    */
    void set_cert_file(std::string in_file);

    /**
    * Cert folder variable setter.
    */
    void set_cert_path(std::string in_path);

    /**
    * Auth file variable setter.
    */
    void set_auth_file(std::string in_file);

    /**
    * Outgoing directory variable setter.
    */
    void set_out_dir(std::string in_dir);

    /**
    * Sets bufferfor outgoing messages to server.
    */
    int set_send_buffer(std::string msg);

    /**
    * Returns server address.
     *
     * @returns address.
    */
    std::string get_address();

    /**
    * Returns server port.
     *
     * @returns port.
    */
    std::string get_port();

    /**
    * Returns designated flag value.
     *
     * @returns flag value.
    */
    bool get_flag(int out_flag);

    /**
    * Returns cert file string path.
     *
     * @returns cert file string path.
    */
    std::string get_cert_file();

    /**
    * Returns cert folder string path.
     *
     * @returns cert folder string path.
    */
    std::string get_cert_path();

    /**
    * Returns auth file string path.
     *
     * @returns auth file string path.
    */
    std::string get_auth_file();

    /**
    * Returns outgoing directory string path.
     *
     * @returns outgoing directory string path.
    */
    std::string get_out_dir();

    /**
    * Returns pointer to file_descriptor.
     *
     * @returns pointer to file_descriptor.
    */
    BIO* get_handler_file_descriptor();

    /**
    * Returns recv_buffer value.
     *
     * @returns recv_buffer value.
    */
    std::string read_recv_buffer();

    void flush_recv_buffer();

    /**
    * Creates non-secured connection to server on port 110.
     *
     * @returns 0 if successful, -1 if not.
    */
    int establish_connection();

    /**
    * Creates secured connection (SSL) to server on port 995.
     *
     * @returns 0 if successful, -1 if not.
    */
    int establish_ssl_connection();

    /**
    * First creates non-secured connection on port 110, then promotes it to secured by using STLS command.
     *
     * @returns 0 if successful, -1 if not.
    */
    int establish_tls_connection();

    /**
    * Function verifies inputted username and password in auth_file.txt.
     *
     * @returns 0 if successful.
    */
    int authenticate();

    /**
    * Receives messages and sends it to msg_parser
     *
     * @returns 0 if successful.
    */
    int receive(int msg_count);

    /**
    * Parses messages and creates email files
     *
     * @returns number of downloaded messages.
    */
    int msg_parser(int msg_id);

    /**
    * Looks up email IDs in msg_IDs.txt.
     *
     * @returns true if found, false if not;
    */
    bool msgID_lookup(std::string id);

    /**
    * Loads SSL certificates depending on input parameters
    */
    void load_certs(SSL_CTX *ctx);

};

#endif //POP3_CLIENT_POP3_HANDLER_H


