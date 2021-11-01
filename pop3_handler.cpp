/** pop3_handler.cpp
* Created by Martin Novotny Mlinarcsik (xnovot1r) on 03.10.21.
* ISA 2021/2022 project - POP3 client
 *
* @file pop3_handler class methods and variables.
* @author Martin Novotny Mlinarcsik <xnovot1r@stud.fit.vutbr.cz>
*/

#include <string.h>
#include <iostream>
#include <utility>
#include <unistd.h>
#include <fstream>
#include <regex>
#define PRINT(text) std::cout << text << std::endl

/** OPEN SSH HEADERS **/
#include <openssl/ssl.h>
#include <openssl/bio.h>

#include "pop3_handler.h"

/* Constructor and class variables */
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
/* SETTERS (KAGEYAMAs) */
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
            exit(-1);
    }
}
/* GETTERS (NISHINOYAs) */
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
            exit(-1);
    }
}

/* HELPER METHODS */
/* Reads recv_buffer and return its value */
std::string POP3_handler::read_recv_buffer() {
    std::string string_buffer;
    string_buffer.append(recv_buffer);
    return string_buffer;
}
/* Sets the entire buffer to '\0' */
void POP3_handler::flush_recv_buffer(){
    if (strlen(recv_buffer) > 0) {
        memset(recv_buffer, '\0', sizeof(recv_buffer));
    }
}
/* First, we designate a default port, if not given in input parameters.
 * Using OpenSSL BIO functions, we create a non-secured connection to
 * server. Program receives confirmation message and proceeds
 * to verify user.
 */
int POP3_handler::establish_connection()
{

    if (this->get_flag(PORT_FLAG) == false)
    {
        this->set_port("110");
    }

    std::string connection_string = this->get_address() + ":" + this->get_port();
    const char* connection_char_array = connection_string.c_str();
    this->pop3_bio = BIO_new_connect(connection_char_array);
    static std::regex err_regex("-ERR .*\r\n");

    if (this->get_handler_file_descriptor() == nullptr)
    {
            return -1;
    }

    if (BIO_do_connect(this->get_handler_file_descriptor()) <= 0)
    {
         return -1;
    }


    BIO_read(this->get_handler_file_descriptor(), this->recv_buffer, 60);
    if (std::regex_search(this->read_recv_buffer(), err_regex) == true)
    {
        exit(-2);
    }

    this->authenticate();

    BIO_free_all(pop3_bio);
    return 0;
}

/* First, we designate a default port, if not given in input parameters.
 * Using OpenSSL BIO functions, we create a secured connection to
 * server. We load security certificates and after connection, verify
 * certificates presented by server. Program receives confirmation message
 * and proceeds to verify user.
 */

int POP3_handler::establish_ssl_connection()
{
    static std::regex err_regex("-ERR .*\r\n");

    SSL_CTX *ctx = SSL_CTX_new(TLS_method());
    SSL *ssl;
    this->pop3_bio = BIO_new_ssl_connect(ctx);
    BIO_get_ssl(pop3_bio, &ssl);
    SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);

    if (this->get_flag(PORT_FLAG) == false)
    {
        this->set_port("995");
    }

    std::string hostname_port = this->get_address() + ":" + this->get_port();
    this->load_certs(ctx);

    BIO_set_conn_hostname(pop3_bio, hostname_port.c_str());
    if (BIO_do_connect(pop3_bio) <= 0)
    {
        exit(-1);
    }

    if(SSL_get_verify_result(ssl) != X509_V_OK)
    {
        exit(-1);
    }

    BIO_read(this->get_handler_file_descriptor(), this->recv_buffer, 1024);
    if (std::regex_search(this->read_recv_buffer(), err_regex) == true)
    {
        exit(-2);
    }
    this->authenticate();

    SSL_CTX_free(ctx);
    return 0;
}

/* First, we designate a default port, if not given in input parameters.
 * Using OpenSSL BIO functions, we first create a non-secured connection to
 * server, which we afterwards promote to secured using STLS command.
 * We load security certificates and after connection, verify
 * certificates presented by server. Program receives confirmation message
 * and proceeds to verify user.
 */
int POP3_handler::establish_tls_connection()
{
    static std::regex err_regex("-ERR .*\r\n");

    int send_buffer_length;
    std::string connection_string = this->get_address() + ":" + this->get_port();
    const char* connection_char_array = connection_string.c_str();
    this->pop3_bio = BIO_new_connect(connection_char_array);

    if (this->get_flag(PORT_FLAG) == false)
    {
        this->set_port("110");
    }

    if (this->get_handler_file_descriptor() == nullptr)
    {
        return -1;
    }

    if (BIO_do_connect(this->get_handler_file_descriptor()) <= 0)
    {
        return -1;
    }

    memset(recv_buffer, '\0', strlen(recv_buffer));
    BIO_read(this->get_handler_file_descriptor(), this->recv_buffer, 60);
    if (std::regex_search(this->read_recv_buffer(), err_regex) == true)
    {
        exit(-2);
    }
    this->flush_recv_buffer();

    /* SEND STLS MESSAGE */
    send_buffer_length = this->set_send_buffer("STLS\r\n");
    BIO_write(this->get_handler_file_descriptor(), (const void*) send_buffer, send_buffer_length);
    BIO_read(this->get_handler_file_descriptor(), this->recv_buffer, 60);
    if (std::regex_search(this->read_recv_buffer(), err_regex) == true)
    {
        exit(-2);
    }

    this->flush_recv_buffer();

    BIO* ret = NULL, *ssl = NULL;

    /* Create context */
    SSL_CTX *ctx = SSL_CTX_new(TLS_method());

    this->load_certs(ctx);

    /* Promote non-secured connection to secured */
    if ((ssl = BIO_new_ssl(ctx, 1)) == NULL)
    {
        exit(-2);
    }

    if ((ret = BIO_push(ssl, this->pop3_bio)) == NULL)
    {
        exit(-2);
    }

    this->pop3_bio = ret;
    this->authenticate();

    return 0;
}

/* First, we load username and password from authentication file.
 * Using OpenSSL BIO functions, we communicate with POP3 server
 * and using USER and PASS commands, we verify the user and proceed to
 * receive the message. After receiving designated number of messages,
 * programs sends QUIT message.
 */


int POP3_handler::authenticate()
{
    std::string username;
    std::string password;
    std::string pop3_reply;
    int msg_count;
    int send_buffer_length;
    std::string msg_text;
    static std::regex err_regex("-ERR .*\r\n");

    memset(recv_buffer, '\0', strlen(recv_buffer)); // Vynulujeme buffer

    std::ifstream authentication_file(this->get_auth_file());
    getline(authentication_file, username);
    getline(authentication_file, password);

    /* Delete "password =" and "username =" */
    username.erase(0, 11);
    password.erase(0, 11);

    send_buffer_length = this->set_send_buffer("USER " + username + "\r\n");

    BIO_write(this->get_handler_file_descriptor(), (const void*) send_buffer, send_buffer_length);
    BIO_read(this->get_handler_file_descriptor(), this->recv_buffer, 60);
    if (std::regex_search(this->read_recv_buffer(), err_regex) == true)
    {
        exit(-2);
    }

    this->flush_recv_buffer();

    send_buffer_length = this->set_send_buffer("PASS " + password + "\r\n");

    BIO_write(this->get_handler_file_descriptor(), (const void*) send_buffer, send_buffer_length);
    BIO_read(this->get_handler_file_descriptor(), this->recv_buffer, 60);
    if (std::regex_search(this->read_recv_buffer(), err_regex) == true)
    {
        exit(-2);
    }
    this->flush_recv_buffer();

    send_buffer_length = this->set_send_buffer("STAT\r\n");

    BIO_write(this->get_handler_file_descriptor(), (const void*) send_buffer, send_buffer_length);
    BIO_read(this->get_handler_file_descriptor(), this->recv_buffer, 60);
    if (std::regex_search(this->read_recv_buffer(), err_regex) == true)
    {
        exit(-2);
    }

    /* Calculate number of emails do download */
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

    /* If we have 0 emails, exit */
    if (msg_count == 0)
    {
        send_buffer_length = this->set_send_buffer("QUIT\r\n");

        BIO_write(this->get_handler_file_descriptor(), (const void*) send_buffer, send_buffer_length);
        BIO_read(this->get_handler_file_descriptor(), this->recv_buffer, 60);
        if (std::regex_search(this->read_recv_buffer(), err_regex) == true)
        {
            exit(-2);
        }
        this->flush_recv_buffer();

        std::cout << "Downloaded 0 messages" << std::endl;
        return 0;
    }


    /* If not, proceed to receive */
    this->receive(msg_count);
    send_buffer_length = this->set_send_buffer("QUIT\r\n");

    BIO_write(this->get_handler_file_descriptor(), (const void*) send_buffer, send_buffer_length);
    BIO_read(this->get_handler_file_descriptor(), this->recv_buffer, 60);
    if (std::regex_search(this->read_recv_buffer(), err_regex) == true)
    {
        exit(-2);
    }
    this->flush_recv_buffer();

    authentication_file.close();
    return 0;
}

/*
 * In for loop, we send RETR messages according to number
 * of emails program is to receive.
 */
int POP3_handler::receive(int msg_count)
{
    /* Download mailov */
    int send_buffer_length = 0;
    int downloaded_msgs = 0;
    static std::regex err_regex("-ERR .*\r\n");


    for(int msg_id = 1; msg_id <= msg_count; ++msg_id)
    {
        send_buffer_length = this->set_send_buffer("RETR " + std::to_string(msg_id) + "\r\n");
        BIO_write(this->get_handler_file_descriptor(), (const void *) send_buffer, send_buffer_length);
        if (std::regex_search(this->read_recv_buffer(), err_regex) == true)
        {
            break;
        }
        this->flush_recv_buffer();
        downloaded_msgs += msg_parser(msg_id);
    }

    std::cout << "Downloaded " << downloaded_msgs << " messages." << std::endl;
    flush_recv_buffer();
    return 0;
}


/*
 * Function parses messages, removes +OK response, replaces any byte-stuffed
 * '.' characters and removes termination octet. Afterwards, program creates
 * new email text files, depending on if -n parameter was inputted.
 * It also deletes downloaded messages from the server by sending
 * DELE messages, depending on if -d parameter was inputted.
 * If both -n and -d are inputted, only new downloaded messages
 * will be downloaded.
 */
int POP3_handler::msg_parser(int msg_id)
{

    int send_buffer_length;
    int downloaded_msgs = 0;
    std::string msg_text;
    std::smatch match;
    bool delete_msg = false;
    bool new_msg = false;

    static const std::regex ID_regex("Message-Id: <.+@.+>", std::regex_constants::icase);
    static const std::regex termination_rgx ("\r\n\\.\r\n");
    static std::regex dotByteStuff_rgx ("\r\n\\.\\.");
    static std::regex ok_rgx("\\+OK (.*)\r\n");
    static std::regex err_regex("-ERR .*\r\n");


    /* Finds out, how many emails we have in out dir */
    int emails_n = count_emails(this->out_dir);
    emails_n++;

    while (42) {
        memset(this->recv_buffer, '\0', strlen(recv_buffer));
        BIO_read(this->get_handler_file_descriptor(), this->recv_buffer, 1023);
        msg_text.append(this->recv_buffer);

        /* Termination octet found */
        if (std::regex_search(msg_text.begin(), msg_text.end(), termination_rgx)) {
            std::regex_search(msg_text, match, ID_regex);
            downloaded_msgs++;

            /* If -n is inputted, we check, if the file was already downloaded */
            if (this->get_flag(NEW_FLAG) == true)
            {
                if (this->msgID_lookup(match.str()) == true)
                {
                    downloaded_msgs--;
                    new_msg = false;
                    break;
                }
            }

            /* If not, download */
            if (this->msgID_lookup(match.str()) == false)
            {
                add_ID(match.str());
                new_msg = true;
            }

            /* Removing +OK line */
            if (std::regex_search(msg_text.begin(), msg_text.end(), ok_rgx, std::regex_constants::format_first_only))
            {
                msg_text = std::regex_replace(msg_text, ok_rgx, "");
            }

            /* Removing termination octet and CRLF */
            msg_text.erase(msg_text.length() - 3, 3);
            flush_recv_buffer();

            delete_msg = true;
            break;
        }
        this->flush_recv_buffer();
    }

    /* Writing msg body into files */
    if (std::regex_search(msg_text.begin(), msg_text.end(), dotByteStuff_rgx))
    {
        if (new_msg == true) {

            std::string mail_name = match.str().substr(0, match.str().length());
            std::string replaced_text = std::regex_replace(msg_text, dotByteStuff_rgx, "\r\n.");
            create_mail_file(this->get_out_dir(), replaced_text, mail_name);
            msg_text.clear();
        }
    } else {
        if (new_msg == true) {
            std::string mail_name = match.str().substr(0, match.str().length());
            create_mail_file(this->get_out_dir(), msg_text, mail_name);
            msg_text.clear();
        }
    }

    /* Deleting emails from pop3 server */
    if (this->get_flag(DELETE_FLAG) == true && delete_msg == true)
    {
        send_buffer_length = this->set_send_buffer("DELE " + std::to_string(msg_id) + "\r\n");
        BIO_write(this->get_handler_file_descriptor(), (const void *) send_buffer, send_buffer_length);
        if (std::regex_search(this->read_recv_buffer(), err_regex) == true)
        {
            exit(-2);
        }
        this->flush_recv_buffer();
    }

    return downloaded_msgs;
}

/* Function finds message id in msg_IDs file. Uses case-insesitive
 * regex.
 */
bool POP3_handler::msgID_lookup(std::string id)
{
    bool ID_found = false;
    std::ifstream ID_file("msg_IDs.txt");
    const static std::regex plus_regex ("\\+");


    if (std::regex_search(id, plus_regex) == true)
    {
        id = std::regex_replace(id, plus_regex, "\\+");
    }

    std::regex msg_ID_regex (id);
    std::string line;

    while (std::getline(ID_file, line))
    {
        if (std::regex_search(line, msg_ID_regex) == true)
        {
            ID_found = true;
            break;
        }
    }
    ID_file.close();
    return ID_found;
}

/* Function loads security certificates depending on if -c/-C
 * parameters were inputted.
 */
void POP3_handler::load_certs(SSL_CTX *ctx) {
    if (this->get_flag(CERT_LOCATION_FLAG) == true) {
       verify_path(this->get_cert_path(), 'C');

        if (!SSL_CTX_load_verify_locations(ctx, NULL, this->get_cert_path().c_str()))
        {
            exit(-1);
        }
    }

    if (this->get_flag(CERT_FLAG) == true)
    {
        std::cerr << this->get_cert_file() << std:: endl;
        verify_path(this->get_cert_file(), 'c');
        if (!SSL_CTX_load_verify_locations(ctx, this->get_cert_file().c_str(), NULL))
        {
            exit(-1);
        }
    }

    if (this->get_flag(CERT_FLAG) == false && this->get_flag(CERT_LOCATION_FLAG) == false)
    {
        SSL_CTX_set_default_verify_paths(ctx);
    }
}


