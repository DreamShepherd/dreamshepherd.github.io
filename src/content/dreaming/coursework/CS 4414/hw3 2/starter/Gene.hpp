#pragma once

#include <string>

class Gene
{
public:
    Gene();
    Gene(const std::string &gene_);
    Gene(const std::string &gene_, int id_);

private:
    std::string gene;
    int id;
};