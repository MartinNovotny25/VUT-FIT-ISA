// file_parser.cpp
// Created by Martin Novotny Mlinarcsik (xnovot1r) on 03.10.21.
// ISA 2021/2022 project - POP3 client

#include "file_parser.h"
#include <string.h>
#include <iostream>
#include <fstream>
#include <experimental/filesystem>
#include <bits/stdc++.h>
#include <regex>

namespace fs = std::experimental::filesystem;
int verify_path(std::string argPath, int type) {
    fs::path pathToCheck(argPath);

    if (type == 'o') {
        if (fs::exists(pathToCheck) == 0 || fs::is_directory(pathToCheck) == 0) {
           // std::cerr << "Directory does not exist, creating" << std::endl;
            fs::create_directory(pathToCheck);
        } else {
           // std::cerr << "Directory existing" << std::endl;
        }
    } else if (type == 'c') {
        if (fs::exists(pathToCheck) == 0 && fs::is_directory(pathToCheck) == 0) {
            std::cerr << "Invalid certification file, exiting" << std::endl;
            exit(2);
        }
    } else if (type == 'C') {
        if (fs::exists(pathToCheck) == 0 || fs::is_directory(pathToCheck) == 0) {
            std::cerr << "Invalid certification directory, exiting" << std::endl;
            exit(2);
        }
    } else if (type == 'a')
    {
        if (fs::exists(pathToCheck) == 0) {
            std::cerr << "Invalid auth directory, exiting" << std::endl;
            exit(2);
        }
    }
    return 0;
}

int create_mail_file(std::string out_dir, std::string msg_text, int msg_num)
{
    std::string filename = "email_" + std::to_string(msg_num) + ".txt";
    fs::path outDir_path(out_dir);
    outDir_path /= filename;

    std::ofstream file(outDir_path);
    file << msg_text;
    file.close();

    return 0;
}

/** kod prevzaty z https://stackoverflow.com/questions/41304891/how-to-count-the-number-of-files-in-a-directory-using-standard/41305019 **/
int count_emails(std::string string_path)
{
        fs::path path(string_path);
        using fs::directory_iterator;
        using fp = bool (*)( const fs::path&);
        return std::count_if(directory_iterator(path), directory_iterator{}, (fp)fs::is_regular_file);

}

int create_mail_ID_file()
{
    if (fs::exists("msg_IDs.txt") == false) {
        fs::path msgID_dir("msg_IDs.txt");
        std::ofstream file(msgID_dir);
        file.close();
    }
    return 0;
}

int add_ID(std::string id)
{
    std::ofstream file;
    file.open("msg_IDs.txt", std::ios::app);
    file << id + "\n";
    file.close();
    return 0;
}

/*bool startup_ID_check(std::string path)
{
    bool found = false;
    bool add_to_temp = true;

    const fs::path out_dir(path);
    std::string line, line_2, tmp_line;
    std::smatch match;

    std::regex current_email_regex;
    std::regex general_email_regex("Message-Id: <.+@.+>", std::regex_constants::icase);
    std::regex temp_line_regex;

    std::ifstream id_file("msg_IDs.txt");
    std::fstream temp("temp.txt");

    for (auto const& email_iter: fs::directory_iterator{out_dir})
    {
        const fs::path& email_path = email_iter;
        std::fstream email_file(email_path);

        std::cerr << "CURRENT FILE: " << email_path << std::endl;

        while(std::getline(email_file, line)) {
            if (std::regex_search(line, match, general_email_regex))
            {
                current_email_regex.assign(match.str());
                std::cerr << "CURRENT EMAIL REGEX: " << match.str() << std::endl;

                while(std::getline(id_file, line_2))
                {
                    if (std::regex_search(line_2, current_email_regex))
                    {
                        std::cerr << "ID FOUND IN ID_FILE:  " << line_2 << std::endl;
                        found = true;

                        std::cerr << "IS LINE IN TMP?: ";
                        while (std::getline(temp, tmp_line))
                        {
                            if (tmp_line.compare(line_2) == true)
                            {
                                std::cerr << "YES" << std::endl;
                                add_to_temp = false;
                            }
                        }

                        if (add_to_temp == true)
                        {
                            std::cerr << "NO, ADDING" << std::endl;
                            temp << line_2 << "\r\n";
                        }
                        break;
                    }
                }

                if (found == false)
                {
                    std::cerr << "ID NOT FOUND, REMOVING FROM MSG_IDs" << std::endl;
                    temp << "" << "\r\n";
                }
            }

            if (found == true)
            {
                std::cerr << "ID FOUND, NOT REMOVING FROM MSG_IDs" << std::endl;
                break;
            }
        }
    }


    remove("msg_IDs.txt");
    rename("tmp.txt", "msg_IDs.txt");

    return found;
}*/