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
            fs::create_directory(pathToCheck);
        } else {
        }
    } else if (type == 'c') {
        if (fs::exists(pathToCheck) == 0 && fs::is_directory(pathToCheck) == 0) {
            exit(1);
        }
    } else if (type == 'C') {
        if (fs::exists(pathToCheck) == 0 || fs::is_directory(pathToCheck) == 0) {
            exit(1);
        }
    } else if (type == 'a')
    {
        if (fs::exists(pathToCheck) == 0) {
            exit(1);
        }
    }
    return 0;
}

/*
 * Creates file to hold the message content.
 */
int create_mail_file(std::string out_dir, std::string msg_text, std::string mail_name)
{
    std::string filename = "email_" + mail_name + ".txt";
    fs::path outDir_path(out_dir);
    outDir_path /= filename;

    std::ofstream file(outDir_path);
    file << msg_text;
    file.close();

    return 0;
}

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

/*
 * On startup, this function compares ID of every email in out_dir with every ID in msg_IDs.txt.
 * If we delete a message, we also need to remove its ID from the ID file, so the email can be
 * downloaded again, if needed and if still present on the email server.
 * Function iterates through email files, finds the ID and compares it. If the ID is found, it's
 * printed into tmp.txt. In the end, the old msg_IDs.txt is removed and tmp.txt is renamed to
 * msg_IDs.txt.
 */
void delete_removed_IDs(std::string path_to_dir)
{
    fs::path outDirPath (path_to_dir);
    std::fstream ID_file("msg_IDs.txt");
    std::ofstream tmp_file("tmp.txt");

    std::string IDfile_ID;
    std::smatch currentEmail_ID;

    std::string currentEmail_line;
    bool ID_found = false;

    static const std::regex regex_ID("Message-Id: <.+@.+>", std::regex_constants::icase);

    /* Iterating through email files in out_dir */
    for (auto const& dir_entry: fs::directory_iterator{outDirPath}) {
        ID_file.seekg(0);
        std::fstream email_file(dir_entry.path());

        /* Reading lines from email file until we find email_id */
        while (std::getline(email_file, currentEmail_line))
        {
            /* We found ID and save the match, which is the id */
            if (std::regex_search(currentEmail_line, currentEmail_ID, regex_ID) == true)
            {

                /* We get lines from ID file */
                while (std::getline(ID_file, IDfile_ID))
                {
                    if (strcmp(currentEmail_ID.str().c_str(), IDfile_ID.c_str()) == 0)
                    {
                        ID_found = true;
                        break;
                    } else
                    {
                        ID_found = false;
                    }
                }
                break;
            }
        }

        if (ID_found == true)
        {
            tmp_file << IDfile_ID << std::endl;

        } /*else if (ID_found == false)
        {
            std::cout << "ID NOT FOUND: " << IDfile_ID << ", DELETING"  << std::endl;
        }*/

        IDfile_ID.clear();
        currentEmail_ID.str().clear();
        currentEmail_line.clear();
    }

    tmp_file.close();
    ID_file.close();
    remove("msg_IDs.txt");
    rename("tmp.txt", "msg_IDs.txt");

}