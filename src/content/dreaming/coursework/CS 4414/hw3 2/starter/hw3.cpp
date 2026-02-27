#include <list>
#include <vector>
#include <functional>

#include "Animal.hpp"

// from Sagar's wc++
std::vector<std::filesystem::path> find_all_files(
    const std::filesystem::path &dir, std::function<bool(const std::string &)> pred)
{
    std::list<std::filesystem::path> files_to_sweep;
    // iterate recursively to find all files that satisfy pred
    for (auto &entry : std::filesystem::recursive_directory_iterator(dir))
    {
        if (entry.is_regular_file())
        {
            std::filesystem::path cur_file = entry.path();
            std::string type(cur_file.extension());
            if (pred(type))
            {
                files_to_sweep.push_back(std::move(cur_file));
            }
        }
    }
    return std::vector<std::filesystem::path>(std::make_move_iterator(files_to_sweep.begin()), std::make_move_iterator(files_to_sweep.end()));
}

// your program should expect two command-line arguments:
// 1. the program name (as always)
// 2. the name of the directory containing the species data
int main(int argc, char *argv[])
{
    /*** TODO: implement main function
     * 1. read the species-data directory from the command line
     * 2. read all the .dat files in the directory (using Sagar's code, if you like)
     * 3. generate output as specified in the handout
     */
}