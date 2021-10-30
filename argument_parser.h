// argument_parser.h
// Created by Martin Novotny Mlinarcsik (xnovot1r) on 03.10.21.
// ISA 2021/2022 project - POP3 client

#ifndef POP3CL_ARGUMENT_PARSER_H
#define POP3CL_ARGUMENT_PARSER_H

#include "pop3_handler.h"

int arg_parse(int argc, char *argv[], POP3_handler* pop3_client);

#endif POP3CL_ARGUMENT_PARSER_H