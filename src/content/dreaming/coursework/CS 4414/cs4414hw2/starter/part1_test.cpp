#include "CityNode.hpp"
#include <iostream>
#include <cassert>
#include <memory> 

int main() {
    std::shared_ptr<CityNode> tokyo = std::make_shared<CityNode>("Tokyo", "Japan", "Asia", 37115035, 37194105, -0.0021);

    // Assertions to verify parameterized constructor behavior
    assert(tokyo->name == "Tokyo");
    assert(tokyo->country == "Japan");
    assert(tokyo->continent == "Asia");
    assert(tokyo->population_2024 == 37115035);
    assert(tokyo->population_2023 == 37194105);
    assert(tokyo->growth_rate == -0.0021);
    assert(tokyo->next == nullptr);

    std::shared_ptr<CityNode> delhi = std::make_shared<CityNode>("Delhi", "India", "Asia", 33807403, 32941309, 0.0263);

    // Link Tokyo to Delhi
    tokyo->next = delhi; 

    // Verify that tokyo->next points to delhi and check field values
    assert(tokyo->next != nullptr); // Ensure that next is not nullptr
    assert(tokyo->next->name == "Delhi");
    assert(tokyo->next->country == "India");
    assert(tokyo->next->continent == "Asia");
    assert(tokyo->next->population_2024 == 33807403);
    assert(tokyo->next->population_2023 == 32941309);
    assert(tokyo->next->growth_rate == 0.0263);
    assert(tokyo->next->next == nullptr); // Ensure Delhi does not point to another node

    std::cout << "Part1 tests passed successfully." << std::endl;

    return 0;
}
