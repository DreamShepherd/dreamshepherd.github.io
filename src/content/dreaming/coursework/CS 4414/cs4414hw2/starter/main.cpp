#include "SinglyLinkedList.hpp"
#include <iostream>

int main(int argc, char* argv[]){
    /*** TODO: implement main function
     * 1. parse the argument to get input csv directory, output csv directory, and the range number (start position, end position) via flag.
     *    (e.g. ./CityPopulationAnalysis -o output.csv -s 10 -e 12 -i world_population_by_city_2024.csv )
     * 2. construct singly linked list by loading it from CSV
     * 3. write the output to the csv
     */
    
    std::string input_csv;
    std::string output_csv;
    int start_index = -1;
    int end_index = -1;


    for (int i = 1; i < argc; i += 2) {
        std::string arg = argv[i];
        if (arg == "-i") {
            input_csv = argv[i + 1];
        } else if (arg == "-o") {
            output_csv = argv[i + 1];
        } else if (arg == "-s") {
            start_index = std::stoi(argv[i + 1]);
        } else if (arg == "-e") {
            end_index = std::stoi(argv[i + 1]);
        }
    }
    SinglyLinkedList cityList;
    cityList.loadFromCSV(input_csv);
    
    std::cout << '\n' << '\n';
    std::string a = "tst";
    std::string& curr = a;
    std::cout << &curr << '\n';
    std::cout << curr << '\n';

    curr = "hi2";
    std::cout << &curr << '\n';
    std::cout << curr << '\n';

    cityList.writeRangeOfCitiesToCSV(output_csv, start_index, end_index);

    return 0;
}
