#include "CityNode.hpp"



CityNode::CityNode() : 
name(""),  country(""),  continent(""),  population_2024(0),  population_2023(0),  growth_rate(0.0),  next(nullptr)
{
}

CityNode::CityNode(std::string name, std::string country, std::string continent, long population_2024, long population_2023, double growth_rate) : 
name(name),  country(country),  continent(continent),  population_2024(population_2024),  population_2023(population_2023),  growth_rate(growth_rate),  next(nullptr)

{
    /*** TODO: implement this parametrized constructor. */

}

bool CityNode::operator<(const CityNode& other) const {
    if (population_2024 != other.population_2024) {
        return population_2024 < other.population_2024;
    }
    if (population_2023 != other.population_2023) {
        return population_2023 < other.population_2023;
    }
    return name < other.name;
}
