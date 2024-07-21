#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <string>
#include <typeinfo>
#include <vector>
#include <map>


namespace FileCategories{                                                                   // these are our categories
    const std::string APPLICATION = "applications";
    const std::string IMAGE = "images";
    const std::string DOCUMENT = "documents";
    const std::string VIDEO = "videos";
    const std::string MISC = "misc";
   
}
namespace FG = FileCategories;


int make_directories( const std::string PATH );                                             // making the OrganizedFiles directories from FileCategories namespace variables
int list_files( std::vector<std::string>& file_names, const std::string PATH );             // files we're interested in moving (not system or directories)
std::string return_file_ext(std::string fname);
std::string return_file_category(std::string fname);
int move_to_category( std::string fname, std::string PATH );                                // using system command mv file will be moved to correct category
const std::string normalize_path( std::string PATH );                                       // path_here/\/ -> path_here 
int is_file( const std::filesystem::directory_entry& entry );                               // .app and any regular binary or ASCII file
int is_system_file( std::string& fname );                                                   // files beginning '.' such as .DS_STORE
