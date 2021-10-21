//
// Created by dekker on 03.10.21.
//

#include "argument_parser.h"
#include "file_parser.h"
#include <unistd.h>

#include <netdb.h>
#include <sys/socket.h>
#include <string.h>
#include <iostream>
#include <arpa/inet.h>

/** NEZABUDNUT NA IPv6 PODPORU **/


int arg_parse(int argc, char *argv[], POP3_handler* pop3_client) {

    struct addrinfo hints = {0};
    struct in_addr inaddr = {0};

    struct addrinfo *result;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = 0;
    hints.ai_protocol = 0;

    int index;
    int tmp_int;

    int option;
    opterr = 0;

    /** Kod na spracovanie nonoption arguemntov prebraty z
     * http://www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html#Example-of-Getopt
     */

    for (int i = 1; i < argc; i++) {

        struct in_addr inaddr = {0};

        if (strcmp(argv[i], "-p") == 0)
        {
            if (pop3_client->get_flag(PORT_FLAG) == true)
            {
                std::cerr << "Redundantny option -p" << std::endl;
                exit(1);
            }
            try {
                tmp_int = std::stoi(argv[i + 1], nullptr, 10);
            }
            catch (std::invalid_argument &error) {
                std::cerr << "Invalid port" << std::endl;
                return -1;
            }

            if (tmp_int < 0 or tmp_int > 65535)
            {
                std::cerr << "Port number out of range" << std::endl;
                return -1;
            } else
            {
                /** Tu sa prida port do objektu **/
                pop3_client->set_port(std::to_string(tmp_int));
                pop3_client->set_flag(PORT_FLAG);
                ++i;
            }

        } else if (strcmp(argv[i], "-T") == 0)
        {
            if (pop3_client->get_flag(T_FLAG) == true)
            {
                std::cout << "Redundantny -T option" << std::endl;
                return -1;
            } else
            {
                pop3_client->set_flag(T_FLAG);
            }
        } else if (strcmp(argv[i], "-S") == 0)
        {
            if (pop3_client->get_flag(S_FLAG) == true) {
                std::cout << "Redundantny -S option" << std::endl;
                return -1;
            }
            else
            {
                pop3_client->set_flag(S_FLAG);
            }
        } else if (strcmp(argv[i], "-c") == 0)
        {
            if (pop3_client->get_flag(CERT_FLAG) == true) {
                std::cout << "Redundantny -c option" << std::endl;
                return -1;
            }
            else
            {
                pop3_client->set_cert_file(argv[i+1]);
                ++i;
                pop3_client->set_flag(CERT_FLAG);
            }
        } else if (strcmp(argv[i], "-C") == 0)
        {
            if (pop3_client->get_flag(CERT_LOCATION_FLAG) == true) {
                std::cout << "Redundantny -C option" << std::endl;
                return -1;
            }
            else
            {
                pop3_client->set_cert_path(argv[i+1]);
                ++i;
                pop3_client->set_flag(CERT_LOCATION_FLAG);
            }
        } else if (strcmp(argv[i], "-d") == 0)
        {
            if (pop3_client->get_flag(DELETE_FLAG) == true) {
                std::cout << "Redundantny -d option" << std::endl;
                return -1;
            }
            else
            {
                pop3_client->set_flag(DELETE_FLAG);
            }
        }

        else if (strcmp(argv[i], "-n") == 0)
        {
            if (pop3_client->get_flag(NEW_FLAG) == true) {
                std::cout << "Redundantny -n option" << std::endl;
                return -1;
            }
            else
            {
                pop3_client->set_flag(NEW_FLAG);
            }
        }

        else if (strcmp(argv[i], "-a") == 0)
        {
            if (pop3_client->get_flag(AUTH_FILE_FLAG) == true) {
                std::cout << "Redundantny -a option" << std::endl;
                return -1;
            }
            else
            {
                pop3_client->set_auth_file(argv[i+1]);
                ++i;
                pop3_client->set_flag(AUTH_FILE_FLAG);
            }
        } else if (strcmp(argv[i], "-o") == 0)
        {
            if (pop3_client->get_flag(OUT_DIR_FLAG) == true) {
                std::cout << "Redundantny -o option" << std::endl;
                return -1;
            }
            else
            {
                pop3_client->set_out_dir(argv[i+1]);
                verify_path(argv[i+1], 'o');
                ++i;
                pop3_client->set_flag(OUT_DIR_FLAG);
            }
        } else if (inet_pton(AF_INET, argv[i], &inaddr) == 1)
        {
            if (pop3_client->get_flag(ADDR_FLAG) == true) {
                std::cerr << "Nespravna IP adresa" << std::endl;
                return -1;
            }
            else
            {
                pop3_client->set_address(argv[i]);
                pop3_client->set_flag(ADDR_FLAG);
            }
        } else {
            std::cerr << "Unknown argument/Bad IP address" << std::endl;
            exit(-1);
        }

    }

    return 0;
}




/*while ((option = getopt(argc, argv, "p:TSc:C:dna:o:")) != -1)

     switch (option)
     {
         case 'p':
         {
             int optarg_to_int = 0;
             if (pop3_client->get_flag(PORT_FLAG) == true)
             {
                 std::cerr << "Reduntantny -p option" << std::endl;
                 return -1;
             }
             else
             {
                 try
                 {
                     optarg_to_int = std::stoi(optarg, nullptr, 10);
                 }
                 catch (std::invalid_argument& error)
                 {
                     std::cerr << "Invalid port" << std::endl;
                     return -1;
                 }


                 if (optarg_to_int < 0 or optarg_to_int > 65535) {
                     std::cerr << "Port number out of range" << std::endl;
                     return -1;
                 } else {
                     /** Tu sa prida port do objektu **/
                           /* pop3_client->set_port(optarg);
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
                    if (optopt == 'p') {std::cerr << "Option -p requires a port number as argument" << std::endl;}
                    else if (optopt == 'c') {std::cerr << "Option -c requires a certification file as argument" << std::endl;}
                    else if (optopt == 'C') {std::cerr << "Option -C requires a path to the certification file as argument" << std::endl;}
                    else if (optopt == 'a') {std::cerr << "Option -a requires an auth file as argument" << std::endl;}
                    else if (optopt == 'o') {std::cerr << "Option -o requires an out dir as argument" << std::endl;}
                    else {std::cerr << "Unknown option character" << std::endl;}
                    return -1;
                }

                default:
                    abort();
            }

            for (index = optind; index < argc; index++)
            {
                if (pop3_client->get_flag(ADDR_FLAG) == true)
                {
                    std::cerr << "Reduntantna adresa" << std::endl;
                    return -1;
                }
                else
                {
                    struct in_addr inaddr = {0};
                    if (inet_pton(AF_INET, argv[optind], &inaddr) != 1) {
                        //std::cerr << argv[optind] << std::endl;
                        std::cerr << "Nespravna IP adresa" << std::endl;
                        return -1;
                    }
                    else
                    {
                        pop3_client->set_address(argv[optind]);

                        //std::cout << argv[optind] << std::endl;

                        pop3_client->set_flag(ADDR_FLAG);
                    }
                }
            }

            return 0;

    //}*/





