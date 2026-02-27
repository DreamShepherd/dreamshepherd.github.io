#pragma once

#include "CityNode.hpp"
#include <memory>

class SinglyLinkedList {
public:
    SinglyLinkedList();

    void insertCity(std::string name, std::string country, std::string continent, long population_2024, long population_2023, double growth_rate);
    void deleteCity(std::string name);
    void printList() const;
    void loadFromCSV(const std::string& filename);
    void writeRangeOfCitiesToCSV(const std::string& filename, int start, int end) const;

    std::shared_ptr<CityNode> getFirstCity();
    std::shared_ptr<CityNode> getLastCity();
    std::shared_ptr<CityNode> getCityAtIndex(int index);

private:
    std::shared_ptr<CityNode> head;
};
