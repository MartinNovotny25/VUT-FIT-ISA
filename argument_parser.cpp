//
// Created by dekker on 03.10.21.
//

#include "argument_parser.h"
#include "unistd.h"

#include <netdb.h>
#include <sys/socket.h>
#include <string.h>
#include <iostream>
#include <arpa/inet.h>

/** NEZABUDNUT NA IPv6 PODPORU **/

/** TODO - Opravit citanie argumentov **/

int arg_parse(int argc, char *argv[], POP3_handler* pop3_client) {

    struct addrinfo hints = {0};
    struct addrinfo *result;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = 0;
    hints.ai_protocol = 0;

    int option;
    opterr = 0;

    std::cerr << argc << std::endl;

    while (optind < argc) {
        if ((option = getopt(argc, argv, "p:TSc:C:dna:o:")) != -1)
        {
            /** Pridat pointer na objekt **/
            switch (option)
            {
                case 'p':
                {
                    if (pop3_client->get_flag(PORT_FLAG) == true)
                    {
                        std::cerr << "Reduntantny -p option" << std::endl;
                        return -1;
                    }
                    else
                    {
                        int optarg_to_int = std::stoi(optarg, nullptr, 10);
                        if (optarg_to_int < 0 or optarg_to_int > 65535) {
                            std::cerr << "Port number out of range" << std::endl;
                            return -1;
                        } else {
                            /** Tu sa prida port do objektu **/
                            pop3_client->set_port(optarg);
                            pop3_client->set_flag(PORT_FLAG);
                            break;
                        }
                    }
                }
                case 'T':
                {
                    if (pop3_client->get_flag(T_FLAG) == true) {
                        std::cout << "Redundantny -T option" << std::endl;
                        return -1;
                    }
                    else
                    {
                        pop3_client->set_flag(T_FLAG);
                        break;
                    }
                }

                case 'S':
                {
                    if (pop3_client->get_flag(S_FLAG) == true) {
                        std::cout << "Redundantny -S option" << std::endl;
                        return -1;
                    }
                    else
                    {
                        pop3_client->set_flag(S_FLAG);
                        break;
                    }
                }

                case 'c':
                {
                    if (pop3_client->get_flag(CERT_FLAG) == true) {
                        std::cout << "Redundantny -c option" << std::endl;
                        return -1;
                    }
                    else
                    {
                        pop3_client->set_cert_file(optarg);
                        pop3_client->set_flag(CERT_FLAG);
                        break;
                    }
                }

                case 'C':
                {
                    if (pop3_client->get_flag(CERT_LOCATION_FLAG) == true) {
                        std::cout << "Redundantny -C option" << std::endl;
                        return -1;
                    }
                    else
                    {
                        pop3_client->set_cert_path(optarg);
                        pop3_client->set_flag(CERT_LOCATION_FLAG);
                        break;
                    }
                }

                case 'd':
                {
                    if (pop3_client->get_flag(DELETE_FLAG) == true) {
                        std::cout << "Redundantny -d option" << std::endl;
                        return -1;
                    }
                    else
                    {
                        pop3_client->set_flag(DELETE_FLAG);
                        break;
                    }
                }

                case 'n':
                {
                    if (pop3_client->get_flag(NEW_FLAG) == true) {
                        std::cout << "Redundantny -n option" << std::endl;
                        return -1;
                    }
                    else
                    {
                        pop3_client->set_flag(NEW_FLAG);
                        break;
                    }
                }

                case 'a':
                {
                    if (pop3_client->get_flag(AUTH_FILE_FLAG) == true) {
                        std::cout << "Redundantny -a option" << std::endl;
                        return -1;
                    }
                    else
                    {
                        pop3_client->set_auth_file(optarg);
                        pop3_client->set_flag(AUTH_FILE_FLAG);
                        break;
                    }
                }

                case 'o':
                {
                    if (pop3_client->get_flag(OUT_DIR_FLAG) == true) {
                        std::cout << "Redundantny -o option" << std::endl;
                        return -1;
                    }
                    else
                    {
                        pop3_client->set_out_dir(optarg);
                        pop3_client->set_flag(OUT_DIR_FLAG);
                        break;
                    }
                }

                case '?':
                {
                    std::cout << "???????" << std::endl;
                    break;
                }

                default:
                {
                    std::cerr << "SWITCH default option" << std::endl;
                    return -1;
                }
            }
        }
        else
        {
            if (pop3_client->get_flag(ADDR_FLAG) == true)
            {
                std::cerr << "Reduntantna adresa" << std::endl;
                return -1;
            }
            else {
                struct in_addr inaddr = {0};
                if (inet_pton(AF_INET, argv[optind], &inaddr) != 1) {
                    std::cerr << argv[optind] << std::endl;
                    std::cerr << "Nespravna IP adresa" << std::endl;
                    return -1;
                } else {
                    pop3_client->set_address(argv[optind]);

                    std::cout << "HERE" << std::endl;
                    std::cout << argv[optind] << std::endl;

                    pop3_client->set_flag(ADDR_FLAG);
                    optind++;
                }
            }
        }
    }
}
