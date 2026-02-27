#pragma once

#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

#include "Gene.hpp"

class Animal
{
public:
    Animal();
    Animal(std::ifstream& file);

    friend bool operator<(const Animal& a, const Animal& b);
    
private:

};