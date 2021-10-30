// pop3_handler.h
// Created by Martin Novotny Mlinarcsik (xnovot1r) on 03.10.21.
// ISA 2021/2022 project - POP3 client


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

/** Makra pre flagy na gettery **/
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
    std::string address;
    std::string port;
    std::string cert_file;
    std::string cert_location;
    std::string auth_file;
    std::string out_dir;
    BIO* pop3_bio;
    char recv_buffer[1024];
    const void* send_buffer;

    bool portFlag;
    bool addrFlag;
    bool T_Flag;
    bool S_Flag;
    bool certFlag;
    bool cert_locationFlag;
    bool deleteFlag;
    bool newFlag;
    bool auth_fileFlag;
    bool out_dirFlag;

public:
    POP3_handler();

    void set_address(std::string arg_address);
    void set_port (std::string arg_port);
    void set_flag(int in_flag);
    void set_cert_file(std::string in_file);
    void set_cert_path(std::string in_path);
    void set_auth_file(std::string in_file);
    void set_out_dir(std::string in_dir);
    int set_send_buffer(std::string msg);
    //int set_file_descriptor();

    std::string get_address();
    std::string get_port();
    bool get_flag(int out_flag);
    std::string get_cert_file();
    std::string get_cert_path();
    std::string get_auth_file();
    std::string get_out_dir();
    BIO* get_handler_file_descriptor();
    std::string read_recv_buffer();

    void flush_recv_buffer();

    int establish_connection();
    int establish_ssl_connection();
    int establish_tls_connection();
    int authenticate();
    int receive(int msg_count);
    int msg_parser(int msg_id);
    bool msgID_lookup(std::string id);
    void load_certs(SSL_CTX *ctx);

};

#endif //POP3_CLIENT_POP3_HANDLER_H


