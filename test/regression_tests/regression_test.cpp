#include <regression_test.h>
#include <fstream>
#include <iostream>
#include <cstdlib>

using std::begin;
using std::end;


int perform_regression_test(const std::string& filename, std::function<void()> test_function)
{
    static const std::string old_directory_name = "regression_test_data/old_data";
    static const std::string new_directory_name = "regression_test_data/new_data";

    int error_code = 0;
    
    {
        std::ofstream newer_stream(new_directory_name + "/" + filename);
        if (!newer_stream) {
            std::cerr << "Unable to open file \"" << filename << "\" (ensure that \"" + new_directory_name + "\" is in the current path)" << std::endl;
            error_code = 2;
        }
        else {
            std::streambuf *coutbuf = std::cout.rdbuf();
            std::cout.rdbuf(newer_stream.rdbuf());
            test_function();
            std::cout.rdbuf(coutbuf);
        }
    }

    if (error_code == 0) {
        std::ifstream older_stream(old_directory_name + "/" + filename);
        std::ifstream newer_stream(new_directory_name + "/" + filename);
        if (!older_stream) {
            std::cerr << "Unable to open file \"" << filename << "\" (ensure that \"" + old_directory_name + "\" is in the current path)" << std::endl;
            error_code = 2;
        }
        else if (!newer_stream) {
            std::cerr << "Unable to open file \"" << filename << "\" (ensure that \"" + new_directory_name + "\" is in the current path)" << std::endl;
            error_code = 2;
        }
        else {
            int line_number = 1;
            auto completed = false;
            while (!completed && error_code == 0) {
                std::string older_line;
                std::string newer_line;
                std::getline(older_stream, older_line);
                std::getline(newer_stream, newer_line);
                if (!older_stream && !newer_stream) {
                    completed = true;
                }
                else if (!older_stream) {
                    std::cout << "===============================================================================" << std::endl;
                    std::cout << "Regression test for \"" << filename << "\" failed due to mismatch on line " << line_number << std::endl;
                    std::cout << "(Older version). (end of file)" << std::endl;
                    std::cout << "(Newer version) " << newer_line << std::endl;
                    std::cout << std::endl;
                    error_code = 1;
                }
                else if (!newer_stream) {
                    std::cout << "===============================================================================" << std::endl;
                    std::cout << "Regression test for \"" << filename << "\" failed due to mismatch on line " << line_number << std::endl;
                    std::cout << "(Older version) " << older_line << std::endl;
                    std::cout << "(Newer version). (end of file)" << std::endl;
                    std::cout << std::endl;
                    error_code = 1;
                }
                else {
                    if (!older_line.empty() && older_line[older_line.size() - 1] == '\r') {
                        older_line.erase(older_line.size() - 1);
                    }
                    if (!newer_line.empty() && newer_line[newer_line.size() - 1] == '\r') {
                        newer_line.erase(newer_line.size() - 1);
                    }
                    if (older_line != newer_line) {
                        std::cout << "===============================================================================" << std::endl;
                        std::cout << "Regression test for \"" << filename << "\" failed due to mismatch on line " << line_number << std::endl;
                        std::cout << "(Older version) " << older_line << std::endl;
                        std::cout << "(Newer version) " << newer_line << std::endl;
                        std::cout << std::endl;
                        error_code = 1;
                    }
                    else {
                        ++line_number;
                    }
                }
            }
        }
    }

    return error_code;
}
