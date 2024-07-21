#include "file_management.h"
#include "utils.h"
#include <typeinfo>

int make_directories( const std::string PATH ){
    std::vector<std::string> categories {FG::APPLICATION, FG::IMAGE, FG::VIDEO, FG::DOCUMENT, FG::MISC};
    for( std::string& category : categories ){
        std::filesystem::create_directories( PATH + "/" + "Organized Files/"+category );
    }
    return 0;
}


int is_file( const std::filesystem::directory_entry& entry ){
    std::string entry_ext = entry.path().extension().string();
    entry_ext = lower( entry_ext );
    return ( std::filesystem::is_regular_file(entry) || ( std::filesystem::is_directory(entry) && entry_ext == ".app") );
}

int is_system_file( std::string& fname ){
    return ( fname[0] == '/' && fname[1] == '.');
}

int list_files( std::vector<std::string>& file_names, const std::string PATH ){
    const int PATH_LEN = PATH.length();

    /* C++ will deduce the type of the variable when using the "const auto&" keyword,
        but we can't do this for function definitions like is_mac_application() function we
        have to explicitly state its type.
    */
    for( const auto& entry : std::filesystem::directory_iterator(PATH) ){
        std::string fname_only = entry.path().string().substr( PATH_LEN );

        if( 
            is_file( entry ) &&

            is_system_file( fname_only ) != 1

            ){
            
            std::cout << fname_only << '\n';
            file_names.push_back(fname_only);
        }
    }
    return 0;
}



std::string return_file_ext(std::string fname){
    lower(fname);                                                                       // lowercasing a copy (did not pass by reference in parameter of this function)
    int dot_delim = fname.find_last_of(".");                                            // in case there are '.' in fname ex: fname.txt.pdf
    
    if( dot_delim == std::string::npos ){
        return "";                                                                      // no extension -> application (executable)
    }
    
    std::string file_ext = fname.substr( dot_delim );

    return file_ext;
}

std::string return_file_category(std::string fname){
    std::map<std::string, std::string> file_ext_categories {                            // Load from a file instead of hard-coding this!
        { ".dmg", FG::APPLICATION },
        { ".app", FG::APPLICATION },
        { ".pkg", FG::APPLICATION },
        { ".sh", FG::APPLICATION },
        { ".command", FG::APPLICATION },
        { ".out", FG::APPLICATION },
        { ".py", FG::APPLICATION },
        { ".pyw", FG::APPLICATION },
        { "", FG::APPLICATION },                                                        // Files with no extensions are usually executables

        { ".jpeg", FG::IMAGE },
        { ".png", FG::IMAGE },
        { ".webp", FG::IMAGE },
        { ".jpg", FG::IMAGE },
        { ".gif", FG::IMAGE },

        { ".mp4", FG::VIDEO },
        { ".mp3", FG::VIDEO },
        { ".avi", FG::VIDEO },
        { ".mov", FG::VIDEO },
        { ".wav", FG::VIDEO },


        { ".txt", FG::DOCUMENT },
        { ".pdf", FG::DOCUMENT },
        { ".csv", FG::DOCUMENT },
        { ".rtf", FG::DOCUMENT },
        { ".xlsx", FG::DOCUMENT },
        { ".docx", FG::DOCUMENT },
    };

    std::string file_ext = return_file_ext( fname );
    std::string category = file_ext_categories[ file_ext ];

    return ( category != "" ) ? category : FG::MISC;                                                                                                    // unknown file extensions will be categorized as miscellaneous

}



int move_to_category( std::string fname, std::string PATH ){
    std::string category = return_file_category(fname);
    std::string command = "mv \"" + PATH + fname + "\" \"" +  PATH + "/Organized Files/" + category + "/" + fname + "\"";
    std::cout << command << '\n';
    

    int status = std::system( command.c_str() );                                                                                                        // requires const char*, use .c_str() to get ptr to null terminated string array
    if( status != 0 ){
        std::string err_msg = "File move for command: \"" + command + "\"command failed with status code: " + std::to_string(status);
        log_err(err_msg);
        return 1;
    }
    else{
        std::cout << "[ SUCCESSFULLY MOVED \'" << fname << "\' ]";
    }

    return 0;
}


const std::string normalize_path( std::string PATH ){
    /*
        1) Should have no '/' or '\\' at the end
            EX: /path/to/path = good
            EX: /path/to/path/ = bad
            EX: /path/to/path/\ = bad
            EX: /path/to/path\ = bad
    
    */
   while( PATH.back() == '/' || PATH.back() == '\\' ){
        PATH.pop_back();
   }
   return PATH;
}
