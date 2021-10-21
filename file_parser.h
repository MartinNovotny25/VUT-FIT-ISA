//
// Created by dekker on 21.10.21.
//


#ifndef POP3CL_FILE_PARSER_H
#define POP3CL_FILE_PARSER_H

#include <string.h>
#include <iostream>
#include "pop3_handler.h"



int verify_path(std::string argPath, int type);
int create_mail_file(std::string out_dir, std::string msg_text, int msg_num);



#endif //POP3CL_FILE_PARSER_H
