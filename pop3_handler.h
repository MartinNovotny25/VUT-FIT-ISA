//
// Created by Martin Novotny Mlinarcsik on 03.10.21.
//


#ifndef POP3_CLIENT_POP3_HANDLER_H
#define POP3_CLIENT_POP3_HANDLER_H

#include <iostream>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    int handler_file_descriptor;
    char recv_buffer[6000];
    std::string send_buffer;


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
    void set_send_buffer(std::string msg);
    int set_file_descriptor();

    std::string get_address();
    std::string get_port();
    bool get_flag(int out_flag);
    std::string get_cert_file();
    std::string get_cert_path();
    std::string get_auth_file();
    std::string get_out_dir();
    int get_handler_file_descriptor();
    std::string read_recv_buffer();

    void flush_recv_buffer();

    int establish_connection();
    int authenticate();

};

int establish_connection(std::string address, std::string port);
int close_socket(int &client_fd);

#endif //POP3_CLIENT_POP3_HANDLER_H


