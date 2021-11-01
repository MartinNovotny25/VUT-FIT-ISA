/** file_parser.h
* Created by Martin Novotny Mlinarcsik (xnovot1r) on 03.10.21.
* ISA 2021/2022 project - POP3 client
 *
* @file Functions  for handling files.
* @author Martin Novotny Mlinarcsik <xnovot1r@stud.fit.vutbr.cz>
*/


#ifndef POP3CL_FILE_PARSER_H
#define POP3CL_FILE_PARSER_H

#include <string.h>
#include <iostream>
#include "pop3_handler.h"

/**
* Verifies, if inputted path to file is valid.
 *
 * @param argPath String path to the file.
 * @param type What input parameter file program is verifying.
 * @return 0 if successful, -1 if not.
*/
int verify_path(std::string argPath, int type);

/**
* Creates mail file for given email
 *
 * @param out_dir String path to the out directory.
 * @param msg_text Email content (headers and body).
 * @param mail_name Substring of email id used to name the file.
 * @return 0 if successful.
*/
int create_mail_file(std::string out_dir, std::string msg_text, std::string mail_name);

/**
* Creates a file for email IDs, if not created already.
 *
 * @return 0 if successful.
*/
int create_mail_ID_file();

/**
* Adds ID string to msg_IDs.txt file, if not present already.
 *
 * @return 0 if successful.
*/
int add_ID(std::string id);

/**
* Counts email files already present in the out directory.
 *
 * @return Number of emails.
*/
int count_emails(std::string path);

/**
*  Function removes IDs of email files, that were deleted from client PC.
 *
 * @return Number of emails.
*/
void delete_removed_IDs(std::string path_to_dir);

#endif //POP3CL_FILE_PARSER_H
