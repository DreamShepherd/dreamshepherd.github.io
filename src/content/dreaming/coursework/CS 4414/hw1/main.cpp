#include <fstream> // std::ifstream (Input File Stream)
#include <iostream> // std::cout (Standard Output)
#include <string> // std::string
#include <vector>

int main(int argc, char** argv)
{
    /*** TODO: implement main(), which should read in the provided CSV file and output according to the homework spec */
    int n = std::atoi(argv[2]);
    std::fstream f (argv[1], std::fstream::in);
    std::string name;
    std::string line;
    long long val = 0;
    long long sm = 0;


    std::cout << "==========" << std::endl;
    if (!f.is_open()) {
        return 1;
    }
    while (f >> line) {
        int idx = line.find(',');
        name = line.substr(0, idx);
        val = std::stoi(line.substr(idx+1));

        if (name.size() >= n) {
            sm += val;
            if (name.size() == n) {
                std::cout << name << std::endl;
            }
        }
    }
    std::cout << sm << std::endl;
    std::cout << "==========" << std::endl;
    f.close();
    return 0;
}
