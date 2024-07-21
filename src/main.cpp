#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <string>
#include <typeinfo>
#include <vector>
#include <map>
#include <unistd.h>

#include "utils.h"
#include "file_management.h"


int main(){
    while ( 1 ){
        sleep(5);                                                                                                 // every 5 seconds files will be organized
        std::vector<std::string> file_names;
        

        const std::string PATH = normalize_path("ABSOLUTE_FILE_PATH_HERE\\\\/");                                  // just to demonstrate file path normalization

        if( std::filesystem::is_directory( PATH ) != 1 ){                                                         // making sure the file path exists as well
            print_err( "An error occured, please check your log file \'ERRORS.log\' ! " );
            
            std::string err_msg = "[ File path is not a directory ] --> " + PATH;
            log_err( err_msg );
            return 1;
        }
        

        make_directories(PATH);                                                                                 // category directories are being made here (application, docs...)
        list_files(file_names, PATH);                                                                           // saves all files in the path 
        
        for( std::string fname : file_names ){
            if( fname == getprogname() ){                                                                       // checking the name of current executable (don't move it)
                continue;
            }
            
            move_to_category( fname, PATH );
        }

    }


    return 0;
}

