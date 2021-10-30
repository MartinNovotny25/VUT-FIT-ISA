// file_parser.h
// Created by Martin Novotny Mlinarcsik (xnovot1r) on 03.10.21.
// ISA 2021/2022 project - POP3 client


#ifndef POP3CL_FILE_PARSER_H
#define POP3CL_FILE_PARSER_H

#include <string.h>
#include <iostream>
#include "pop3_handler.h"


int verify_path(std::string argPath, int type);
int create_mail_file(std::string out_dir, std::string msg_text, int msg_num);
int create_mail_ID_file();
int add_ID(std::string id);
int count_emails(std::string path);
//bool startup_ID_check(std::string path);

#endif //POP3CL_FILE_PARSER_H
