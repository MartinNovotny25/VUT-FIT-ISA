/** file_parser.cpp
* Created by Martin Novotny Mlinarcsik (xnovot1r) on 03.10.21.
* ISA 2021/2022 project - POP3 client
*
* @file Functions  for handling files.
* @author Martin Novotny Mlinarcsik <xnovot1r@stud.fit.vutbr.cz>
*
*
*/

#include "file_parser.h"
#include <string.h>
#include <iostream>
#include <fstream>
#include <experimental/filesystem>
#include <bits/stdc++.h>
#include <regex>

namespace fs = std::experimental::filesystem;

/* Verify, if the path is valid and the file/folder exists and, depending on type,
 * exits the program if the folder does not exist, or creates it.
 */
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
            exit(1);
        }
    } else if (type == 'C') {
        if (fs::exists(pathToCheck) == 0 || fs::is_directory(pathToCheck) == 0) {
            std::cerr << "Invalid certification directory, exiting" << std::endl;
            exit(1);
        }
    } else if (type == 'a')
    {
        if (fs::exists(pathToCheck) == 0) {
            std::cerr << "Invalid auth directory, exiting" << std::endl;
            exit(1);
        }
    }
    return 0;
}

/*
 * Creates file to hold the message content.
 */
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

/* kod prevzaty z https://stackoverflow.com/questions/41304891/how-to-count-the-number-of-files-in-a-directory-using-standard/41305019 */
/*
 * Counts number of emails in out directory
 */
int count_emails(std::string string_path)
{
        fs::path path(string_path);
        using fs::directory_iterator;
        using fp = bool (*)( const fs::path&);
        return std::count_if(directory_iterator(path), directory_iterator{}, (fp)fs::is_regular_file);

}

/* Creates ID file */
int create_mail_ID_file()
{
    if (fs::exists("msg_IDs.txt") == false) {
        fs::path msgID_dir("msg_IDs.txt");
        std::ofstream file(msgID_dir);
        file.close();
    }
    return 0;
}

/* Adds ID to ID text file */
int add_ID(std::string id)
{
    std::ofstream file;
    file.open("msg_IDs.txt", std::ios::app);
    file << id + "\n";
    file.close();
    return 0;
}