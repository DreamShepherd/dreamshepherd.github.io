#include <fstream>
#include <sstream>
#include <iostream>
#include <memory>
#include <vector>
#include "SinglyLinkedList.hpp"

// Constructor for SinglyLinkedList
SinglyLinkedList::SinglyLinkedList() : head(nullptr) {}

// Insert a new city into the linked list in descending order by population (2024)
void SinglyLinkedList::insertCity(std::string name, std::string country, std::string continent, long population_2024, long population_2023, double growth_rate) {
    /*** TODO: implement insert City method */
    auto added = std::make_shared<CityNode>(name, country, continent, population_2024, population_2023, growth_rate); 
    if (!head) {
        head = added;
        return;
    }
    if (*head < *added) {
        added->next = head;
        head = added;
        return;
    }

    auto curr = head;
    while (curr->next && *added < *curr->next) {
        curr = curr->next;
    }
    added->next = curr->next;
    curr->next = added;
}


// Delete a city by name from the list
void SinglyLinkedList::deleteCity(std::string name) {
    /*** TODO: implement delete city method */
    if (!head) {
        return;
    }
    if (head->name == name) {
        head = head->next;
        return;
    }
    auto curr = head;

    while (curr->next && curr->next->name != name) {
        curr = curr->next;
    }
    curr->next = curr->next->next;
}

// Print all cities in the list
void SinglyLinkedList::printList() const {
    /*** TODO: implement this */
    auto curr = head;
    while (curr) {
        std::cout << curr->name << ' ';
        curr = curr->next;
    }
    std:: cout << '\n';
}

// read city information to construct the Singly Linked list from the file at filename
void SinglyLinkedList::loadFromCSV(const std::string& filename) {
    /*** TODO: implement this */
    std::fstream f (filename, std::fstream::in);
    std::string line;
    std::string item;
    std::vector<std::string> args;

    getline(f, line); // skip first line
    while (getline(f, line)) {
        std::stringstream ss(line);
        while (getline(ss, item, ',')) 
            args.push_back(item);
        insertCity(args[0], args[1], args[2], std::stol(args[3]), std::stol(args[4]), std::stod(args[5]));
        args.clear();
    }
    f.close();

}

// write city from start index to end index in the singly linked list to csv at filename 
void SinglyLinkedList::writeRangeOfCitiesToCSV(const std::string& filename, int start, int end) const {
    /*** TODO: implement this */
    std::ofstream of (filename, std::ofstream::out);
    of << "City,Country,Continent,Population(2024),Population(2023),Growth Rate\n";
    int idx = 1;
    auto curr = head;
    for (; idx < start; idx++) {
        curr = curr->next;
        if (!curr) {
            break;
        }
    }
    while (curr && idx < end) {
        of << curr->name << ',' << curr->country << ',' << curr->continent << ',' << curr->population_2024 << ',' << curr->population_2023 << ',' << curr->growth_rate << '\n';
        curr = curr->next;
        idx++;
    }
    of.close();
}


std::shared_ptr<CityNode> SinglyLinkedList::getFirstCity() {
    return head;
}

// get the shared_ptr of the last city in the SinglyLinkedList.
std::shared_ptr<CityNode> SinglyLinkedList::getLastCity() {
    /*** TODO: implement this. */
    auto curr = head;
    while (curr->next) {
        curr = curr->next;
    }
    return curr;
}

/*** Get the shared_ptr to the node at the index.
 * Nodes are ordered by descending order of 2024 population. Indices start from 1. 
 * The city with the highest 2024 population has index 1, second highest population with index 2, etc. 
*/
std::shared_ptr<CityNode> SinglyLinkedList::getCityAtIndex(int index) {
    /*** TODO: implement this. */
    auto curr = head;
    for (int i = 1; i < index; i++) {
        if (!curr->next) {
            return nullptr;
        }
        curr = curr->next;
    }
    return curr;
}
