#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <filesystem>

namespace fs = std::filesystem;

const std::vector<int> search_numbers =  {0,46,5,58,114,55,116,21,56,139,92,78,96,8,2,50,25,98,99,37};
const std::string txt_directory = "labels/train/";
const std::string jpg_directory = "images/train/";
const std::string filtered_txt_directory = "labels_filtered/train/";
const std::string filtered_jpg_directory = "images_filtered/train/";

std::vector<std::string> split(const std::string& line, char delimiter) {
    std::vector<std::string> tokens;
    std::istringstream iss(line);
    std::string token;
    while (std::getline(iss, token, delimiter)) {
        tokens.push_back(std::move(token));
    }
    return tokens;
}

bool isFirstNumberInSearchNumbers(const std::string& line) {
    std::vector<std::string> numbers = split(line, ' ');
    if (!numbers.empty()) {
        int firstNumber = std::stoi(numbers[0]);
            return std::find(search_numbers.begin(), search_numbers.end(), firstNumber) != search_numbers.end();
    }
    return false;
}

void processFiles() {
    std::vector<std::string> tab_names;
    
    for (const auto& entry : fs::directory_iterator(txt_directory)) {
        const std::string& txt_file_name = entry.path().filename().string();
        if (fs::is_regular_file(entry) && txt_file_name.substr(txt_file_name.find_last_of(".") + 1) == "txt") {
            const std::string& txt_file_path = entry.path().string();
            const std::string& jpg_file_name = txt_file_name.substr(0, txt_file_name.find_last_of(".")) + ".jpg";
           // const std::string& jpg_file_path = jpg_directory + jpg_file_name;
            
            std::ifstream txt_file(txt_file_path);
            std::string line;
            
            std::string buffor ="";

            while (std::getline(txt_file, line)) {
                if (isFirstNumberInSearchNumbers(line)) {
                    buffor += line ;
                    buffor += "\n";
                }
            }
            
            if (buffor.size() > 0) {
                tab_names.push_back(jpg_file_name);

                
                std::ofstream filtered_txt_file(filtered_txt_directory + txt_file_name);
                filtered_txt_file << buffor;
                filtered_txt_file.close();
                
            }
            
            txt_file.close();
        }
    }
    for(const auto name: tab_names){
        fs::copy_file(jpg_directory+name, filtered_jpg_directory + name);
    }
}

int main() {
    processFiles();
    return 0;
}
