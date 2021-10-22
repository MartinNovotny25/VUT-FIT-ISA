//
// Created by dekker on 21.10.21.
//

#include "file_parser.h"
#include <string.h>
#include <iostream>
#include <fstream>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;
int verify_path(std::string argPath, int type) {

    fs::path pathToCheck(argPath);

    if (type == 'o') {
        if (fs::exists(pathToCheck) == 0 || fs::is_directory(pathToCheck) == 0) {
            //std::cerr << "Directory does not exist, creating" << std::endl;
            fs::create_directory(pathToCheck);
        } else {
            //std::cerr << "Directory existing" << std::endl;
        }
    } else if (type == 'c') {
        if (fs::exists(pathToCheck) == 0 || fs::is_directory(pathToCheck) == 0) {
            std::cerr << "Invalid authentication directory, exiting" << std::endl;
            fs::create_directory(pathToCheck);
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