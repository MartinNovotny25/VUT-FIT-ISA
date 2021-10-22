//
// Created by dekker on 03.10.21.
//

/** ZMENIT exit v set_flag a get_flag **/

#include <string.h>
#include <iostream>
#include <utility>
#include <unistd.h>
#include <fstream>
#include <regex>
#include <filesystem>

#define PRINT(text) std::cout << text << std::endl

#include <netdb.h>
#include <sys/socket.h>

/** OPEN SSH HEADERS **/
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/bio.h>


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
int POP3_handler::set_send_buffer(std::string msg) {
    send_buffer = msg.c_str();
    return msg.length();
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
/*int POP3_handler::set_file_descriptor() {
    handler_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    return handler_file_descriptor;
}*/

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
BIO* POP3_handler::get_handler_file_descriptor() {
    return pop3_bio;
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

/** HELPER METHODS **/
std::string POP3_handler::read_recv_buffer() {
    std::string string_buffer;
    string_buffer.append(recv_buffer);
    return string_buffer;
}
void POP3_handler::flush_recv_buffer(){
    //std::cout << std::endl << "****************** FLUSHING CONTENTS ***********************" << std::endl;
    if (strlen(recv_buffer) > 0) {
        memset(recv_buffer, 0, sizeof(recv_buffer));
    }
}

int POP3_handler::establish_connection()
{
    /** Pridat funckionalitu ak sa nezada port **/
    std::string connection_string = this->get_address() + ":" + this->get_port();
    const char* connection_char_array = connection_string.c_str();
    this->pop3_bio = BIO_new_connect(connection_char_array);
    if (this->get_handler_file_descriptor() == nullptr)
    {
            std::cout << "bio.h: Socket not created" << std::endl;
            return -1;
    }

    //std::cout << "bio.h: Socket created" << std::endl;

    if (BIO_do_connect(this->get_handler_file_descriptor()) <= 0)
    {
         std::cerr << "bio.h: Connection failed" << std::endl;
         return -1;
    }

    std::cout << "bio.h: Connection created" << std::endl;

    BIO_read(this->get_handler_file_descriptor(), this->recv_buffer, 50);
    std::cout << this->read_recv_buffer() << std::endl;

    this->authenticate();

    BIO_free_all(pop3_bio);
    return 0;

}

int POP3_handler::establish_ssl_connection()
{
    SSL_CTX *ctx = SSL_CTX_new(SSLv23_client_method());
    SSL *ssl;

    /*if(! SSL_CTX_load_verify_locations(ctx, "TrustStore.pem", NULL))
    {
        std::cerr << "LOADING TRUST STORE FAILED" << std::endl;
        exit(-1);
    }*/

    SSL_CTX_set_default_verify_paths(ctx);

    /** PRIDAT FOLDER PRE DOK **/

   this->pop3_bio = BIO_new_ssl_connect(ctx);
   BIO_get_ssl(pop3_bio, &ssl);
   SSL_set_mode(ssl, SSL_MODE_NO_AUTO_CHAIN);

   std::string hostname_port = this->get_address() + ":" + this->get_port();
//   std::cerr << hostname_port<< std::endl;



   BIO_set_conn_hostname(pop3_bio, hostname_port.c_str());
   if (BIO_do_connect(pop3_bio) <= 0)
   {
       std::cerr << "bio.h: DO_CONNECT FAILED" << std::endl;
       exit(-1);
   }

    if(SSL_get_verify_result(ssl) != X509_V_OK)
    {
        std::cerr << "bio.h: CERT NOT VALID" << std::endl;
        exit(-1);
    }

    //std::cout << "bio.h: CONNECTED AND CERT VALID" << std::endl;

    BIO_read(this->get_handler_file_descriptor(), this->recv_buffer, 1024);
    std::cout << this->read_recv_buffer() << std::endl;
    this->authenticate();

    SSL_CTX_free(ctx);
    return 0;
}

int POP3_handler::establish_tls_connection()
{
    /** Pridat funckionalitu ak sa nezada port **/
    int send_buffer_length;
    std::string connection_string = this->get_address() + ":" + this->get_port();
    const char* connection_char_array = connection_string.c_str();
    this->pop3_bio = BIO_new_connect(connection_char_array);
    if (this->get_handler_file_descriptor() == nullptr)
    {
        std::cout << "bio.h: Socket not created" << std::endl;
        return -1;
    }

    //std::cout << "bio.h: Socket created" << std::endl;

    if (BIO_do_connect(this->get_handler_file_descriptor()) <= 0)
    {
        std::cerr << "bio.h: Connection failed" << std::endl;
        return -1;
    }

    memset(recv_buffer, '\0', strlen(recv_buffer));
    BIO_read(this->get_handler_file_descriptor(), this->recv_buffer, 50);
    std::cout << this->read_recv_buffer() << std::endl;
    this->flush_recv_buffer();
    /** SEND STLS COMMAND **/


    send_buffer_length = this->set_send_buffer("STLS\r\n");
    BIO_write(this->get_handler_file_descriptor(), (const void*) send_buffer, send_buffer_length);
    BIO_read(this->get_handler_file_descriptor(), this->recv_buffer, 50);

    std::cout << this->read_recv_buffer() << std::endl;
    this->flush_recv_buffer();

    std::cout << "bio.h: Connection created" << std::endl;
    BIO* ret = NULL, *con = NULL, *ssl = NULL;

    SSL_CTX *ctx = SSL_CTX_new(SSLv23_client_method());

    BIO* bio = BIO_new_ssl_connect(ctx);

    if ((ssl = BIO_new_ssl(ctx, 1)) == NULL)
    {
        std::cerr << "bio_new_ssl ERROR" << std::endl;
        exit(-2);
    }

    if ((ret = BIO_push(ssl, this->pop3_bio)) == NULL)
    {
        std::cerr << "bio_push ERROR" << std::endl;
        exit(-2);
    }

    this->pop3_bio = ret;

    this->authenticate();
}

int POP3_handler::authenticate()
{
    std::string username;
    std::string password;
    std::string pop3_reply;
    int msg_count;
    int send_buffer_length;
    std::string msg_text;

    memset(recv_buffer, 0, strlen(recv_buffer)); // Vynulujeme buffer

    std::ifstream authentication_file(this->get_auth_file());
    getline(authentication_file, username);
    getline(authentication_file, password);

    send_buffer_length = this->set_send_buffer("USER " + username + "\r\n");

    BIO_write(this->get_handler_file_descriptor(), (const void*) send_buffer, send_buffer_length);
    BIO_read(this->get_handler_file_descriptor(), this->recv_buffer, 50);
    std::cout << this->read_recv_buffer();
    this->flush_recv_buffer();

    send_buffer_length = this->set_send_buffer("PASS " + password + "\r\n");

    BIO_write(this->get_handler_file_descriptor(), (const void*) send_buffer, send_buffer_length);
    BIO_read(this->get_handler_file_descriptor(), this->recv_buffer, 50);
    std::cout << this->read_recv_buffer();
    this->flush_recv_buffer();

    send_buffer_length = this->set_send_buffer("STAT\r\n");

    BIO_write(this->get_handler_file_descriptor(), (const void*) send_buffer, send_buffer_length);
    BIO_read(this->get_handler_file_descriptor(), this->recv_buffer, 50);
    //std::cout << this->read_recv_buffer();

    /** Ziskanie poctu mailov **/
    char container[256];
    for(int i = 0; i < sizeof(recv_buffer); i++) {
        if(isdigit(recv_buffer[i+4]) == true)
        {
            container[i] = recv_buffer[i+4];
        }
        else {
            this->flush_recv_buffer();
            break;
        }
    }

    const std::string msg_count_string(container);
    msg_count = strtol(msg_count_string.c_str(), nullptr, 10);

    this->receive(msg_count);
    authentication_file.close();
    return 0;
}

int POP3_handler::receive(int msg_count)
{
    /** Download mailov **/
    int send_buffer_length;
    std::string msg_text;

    for(int x = 1; x <= msg_count; ++x)
    {
        //std::cout << "MSG " << x << std::endl;
        std::regex termination_rgx ("\r\n.\r\n");
        //std::regex nonTermination_rgx ("\r\n");
        std::regex ok_rgx("+OK [0-9]* octets\r\n", std::regex_constants::basic);

        send_buffer_length = this->set_send_buffer("RETR " + std::to_string(x) + "\r\n");
        BIO_write(this->get_handler_file_descriptor(), (const void *) send_buffer, send_buffer_length);
        this->flush_recv_buffer();

        while (42)
        {
            memset(recv_buffer, '\0', 1000);
            BIO_read(this->get_handler_file_descriptor(), this->recv_buffer, 1000);
            msg_text.append(this->recv_buffer);

            /** Odmazeme 3 posledne znaky **/
            if (std::regex_search(msg_text.begin(), msg_text.end(), termination_rgx))
            {
                if (std::regex_search(msg_text.begin(), msg_text.end(), ok_rgx)) {
                    msg_text = std::regex_replace(msg_text, ok_rgx, "") ;
                }
                msg_text.erase(msg_text.length()-3, 3);
                flush_recv_buffer();
                break;
            }
        }
        //std::cout << "" << msg_text  << "" << std::endl;

        /** Zapis do suboru **/
        create_mail_file(this->get_out_dir(), msg_text, x);
    }

    std::cout << "Downloaded " << msg_count << " messages." << std::endl;
    return 0;
}

