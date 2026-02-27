#include "SinglyLinkedList.hpp" 
#include <iostream>
#include <cassert>
#include <memory> 

void testBasicFunctionality() {
    SinglyLinkedList cityList; 

    // Insert cities
    cityList.insertCity("Tokyo", "Japan", "Asia", 37115035, 37194105, -0.0021);
    cityList.insertCity("Delhi", "India", "Asia", 33807403, 32941309, 0.0263);
    cityList.insertCity("Shanghai", "China", "Asia", 29867918, 29210808, 0.0225);

    // Test first city (1-based index)
    std::shared_ptr<CityNode> firstCity = cityList.getCityAtIndex(1);
    assert(firstCity != nullptr);
    assert(firstCity->name == "Tokyo");

    // Test i-th city (2nd city in 1-based index)
    std::shared_ptr<CityNode> secondCity = cityList.getCityAtIndex(2);
    assert(secondCity != nullptr);
    assert(secondCity->name == "Delhi");

    // Test i-th city (3rd city in 1-based index)
    std::shared_ptr<CityNode> thirdCity = cityList.getCityAtIndex(3);
    assert(thirdCity != nullptr);
    assert(thirdCity->name == "Shanghai");

    // Test getLastCity before deletion
    std::shared_ptr<CityNode> lastCity = cityList.getLastCity();
    assert(lastCity != nullptr);
    assert(lastCity->name == "Shanghai"); 

    // Test deletion
    cityList.deleteCity("Delhi");
    std::shared_ptr<CityNode> deletedCity = cityList.getCityAtIndex(2); 
    assert(deletedCity != nullptr);
    assert(deletedCity->name == "Shanghai");

    // Test getLastCity after deletion
    lastCity = cityList.getLastCity();
    assert(lastCity != nullptr);
    assert(lastCity->name == "Shanghai"); 

    std::cout << "Part2 tests passed successfully." << std::endl;
}

int main() {
    testBasicFunctionality();

    return 0;
}
