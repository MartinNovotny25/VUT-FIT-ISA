//
// Created by dekker on 03.10.21.
//

/** ZMENIT exit v set_flag a get_flag **/

#include <string.h>
#include <iostream>
#include <utility>
#include <unistd.h>
#include <fstream>

#include <netdb.h>
#include <sys/socket.h>

#include "pop3_handler.h"

/** Prototypy fukcii **/

POP3_handler::POP3_handler() {
    address = "";
    port = "";
    cert_file= "";
    cert_location= "";
    auth_file= "";
    out_dir= "";

    portFlag = false;
    addrFlag = false;
    T_Flag = false;
    S_Flag = false;
    certFlag = false;
    cert_locationFlag = false;
    deleteFlag = false;
    newFlag = false;
    auth_fileFlag = false;
    out_dirFlag = false;
}
/** SETTERS (KAGEYAMAs) **/
void POP3_handler::set_address(std::string arg_address) {
    address = std::move(arg_address);
}
void POP3_handler::set_port (std::string arg_port) {
    port = std::move(arg_port);
}
void POP3_handler::set_cert_file(std::string in_file){
    cert_file = std::move(in_file);
}
void POP3_handler::set_cert_path(std::string in_path){
    cert_location = std::move(in_path);
}
void POP3_handler::set_auth_file(std::string in_file){
    auth_file = std::move(in_file);
}
void POP3_handler::set_out_dir(std::string in_dir){
    out_dir = std::move(in_dir);
}
void POP3_handler::set_send_buffer(std::string msg) {
    //char *temp= new char [msg.length()+1];
    //strcpy (temp, msg.c_str());
    send_buffer = msg;
}
void POP3_handler::set_flag(int in_flag){
    switch (in_flag)
    {
        case PORT_FLAG:
            portFlag = true;
            break;
        case ADDR_FLAG:
            addrFlag = true;
            break;
        case T_FLAG:
            T_Flag = true;
            break;
        case S_FLAG:
            S_Flag = true;
            break;
        case CERT_FLAG:
            certFlag = true;
            break;
        case CERT_LOCATION_FLAG:
            cert_locationFlag = true;
            break;
        case DELETE_FLAG:
            deleteFlag = true;
            break;
        case NEW_FLAG:
            newFlag = true;
            break;
        case AUTH_FILE_FLAG:
            auth_fileFlag = true;
            break;
        case OUT_DIR_FLAG:
            out_dirFlag = true;
            break;
        default:
            std::cerr << "POP3_handler::set_flag() error" << std::endl;
            exit(69);
    }
}
int POP3_handler::set_file_descriptor() {
    handler_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    return handler_file_descriptor;
}

/** GETTERS (NISHINOYAs) **/
std::string POP3_handler::get_address() {
    return address;
}
std::string POP3_handler::get_port() {
    return port;
}
std::string POP3_handler::get_cert_file(){
    return cert_file;
}
std::string POP3_handler::get_cert_path(){
    return cert_location;
}
std::string POP3_handler::get_auth_file(){
    return auth_file;
}
std::string POP3_handler::get_out_dir(){
    return out_dir;
}

std::string POP3_handler::read_recv_buffer() {
    std::string string_buffer;
    string_buffer.append(recv_buffer);
    //memset(recv_buffer, 0, 100);
    return string_buffer;
}
int POP3_handler::get_handler_file_descriptor() {
    return handler_file_descriptor;
}
bool POP3_handler::get_flag(int out_flag){
    switch (out_flag)
    {
        case PORT_FLAG:
            return portFlag;
        case ADDR_FLAG:
            return addrFlag;
        case T_FLAG:
            return T_Flag;
        case S_FLAG:
            return S_Flag;
        case CERT_FLAG:
            return certFlag;
        case CERT_LOCATION_FLAG:
            return cert_locationFlag;
        case DELETE_FLAG:
            return deleteFlag;
        case NEW_FLAG:
            return newFlag;
        case AUTH_FILE_FLAG:
            return auth_fileFlag;
        case OUT_DIR_FLAG:
            return out_dirFlag;
        default:
            std::cerr << "POP3_handler:get_flag() error" << std::endl;
            exit(420);
    }
}
void POP3_handler::flush_recv_buffer(){

    std::cout << "****************** FLUSHING CONTENTS ***********************" << std::endl;
    if (strlen(recv_buffer) > 0) {
        memset(recv_buffer, 0, sizeof(recv_buffer));
    }
}

int POP3_handler::establish_connection() {

    // Vytvorenie socketu
    int client_fd = this->set_file_descriptor();
    if (client_fd < 0) {
        std::cout << "*** Nevytvoril sa socket" << std::endl;
        return -1;
    }

    struct addrinfo hints = {0};
    struct addrinfo *result;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = 0;
    hints.ai_protocol = 0;

    std::string method_address = this->get_address();
    std::string method_port = this->get_port();

    //std::cout << this->get_address() << std::endl;
    //std::cout << this->get_port() << std::endl;

    int returnCode = getaddrinfo(method_address.c_str(), method_port.c_str(), &hints, &result);
    if (returnCode != 0) {
        std::cerr << "Getaddrinfo error" << std::endl;
        return -1;
    }

    if (connect(client_fd, result->ai_addr, result->ai_addrlen) == -1) {
        std::cerr << "*** Pripojenie zlyhalo ***" << std::endl;
        return -1;
    }

    std::cout << "*** Connection established ***" << std::endl;

    /** Teoreticky mozem dat do handleru **/
    /* Premenne pre odosielanie a prijmanie sprav */


    if (recv(client_fd, this->recv_buffer, 6000, 0) < 0) {
        std::cout << "Receive failed" << std::endl;
    }

    std::cout << std::endl << "** Data received **" << std::endl;
    std::cout << this->recv_buffer;

    this->authenticate();

    freeaddrinfo(result);

    if (close_socket(client_fd) == 0)
    {
        this->handler_file_descriptor = 0;
    }
    return 0;
}
int POP3_handler::authenticate()
{
    std::string username;
    std::string password;
    std::string pop3_reply;

    memset(recv_buffer, 0, 300); // Vynulujeme buffer

    int client_fd = this->get_handler_file_descriptor();

    std::ifstream authentication_file(this->get_auth_file());
    getline(authentication_file, username);
    getline(authentication_file, password);

    this->set_send_buffer("USER " + username + "\r\n");

    std::cout << this->send_buffer;
    send(this->get_handler_file_descriptor(), send_buffer.c_str(), send_buffer.length(), 0);
    recv(this->get_handler_file_descriptor(), this->recv_buffer, 20, 0);
    pop3_reply = this->read_recv_buffer();
    std::cout << pop3_reply;
    this->flush_recv_buffer();

    this->set_send_buffer("PASS " + password + "\r\n");

    std::cout << this->send_buffer;
    send(this->get_handler_file_descriptor(), this->send_buffer.c_str(), send_buffer.length(), 0);
    recv(this->get_handler_file_descriptor(), this->recv_buffer, 60, 0);
    pop3_reply = this->read_recv_buffer();
    std::cout << pop3_reply;
    this->flush_recv_buffer();

    authentication_file.close();
    return 0;
}



int close_socket(int &client_fd) {
    if (close(client_fd) == -1){
        std::cout << "Failed to close socket file descriptor" << std::endl;
        return -1;
    }
    std::cout << "Socked closed" << std::endl;

    return 0;
}
