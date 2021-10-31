/** argument_parser.h
* Created by Martin Novotny Mlinarcsik (xnovot1r) on 03.10.21.
* ISA 2021/2022 project - POP3 client
*
* @file Functions for argument parsing.
* @author Martin Novotny Mlinarcsik <xnovot1r@stud.fit.vutbr.cz>
*/

#ifndef POP3CL_ARGUMENT_PARSER_H
#define POP3CL_ARGUMENT_PARSER_H

#include "pop3_handler.h"

/**
* Function parses input arguments
 *
 * @param argc Number of arguments.
 * @param *argv[] Value of argument.
 * @param *pop3_client Pointer to pop3 client object.
 * @return 0 if successful.
*/
int arg_parse(int argc, char **argv, POP3_handler* pop3_client);

#endif //POP3CL_ARGUMENT_PARSER_H