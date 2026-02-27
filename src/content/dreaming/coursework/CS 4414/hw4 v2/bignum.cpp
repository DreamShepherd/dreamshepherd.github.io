#include "bignum.hpp"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <string>
#include <sstream>



using data = BigNum::data;

constexpr data zero = {};
constexpr data one = {1};
constexpr data fiveHundredMillion= {500000000};

inline void printArr(BigNum::data arr) {
    for (auto i : arr) {
        std::cout << i << ' ';
    }
    std::cout << std::endl;
}


constexpr data parse(const char* input) {
    data result = {};
    size_t len = 0;
    while (input[len]) ++len;
    int digit = 0;
    for (int i = len; i > 0; i -= BigNum::basePower) {
        uint64_t chunk = 0;
        size_t chunk_end = (i > BigNum::basePower) ? i - BigNum::basePower : 0;

        for (int j = chunk_end; j < i; ++j) {
            chunk = chunk * 10 + (input[j] - '0');
        }
        result[digit++] = chunk;
    }
    
    return result;
}

BigNum::BigNum(const std::string& input) {
    // if (!isValidNumber(value)) {
    //     throw std::invalid_argument("Error: \"" + value + "\" is not an unsigned integer");
    // }
    value = zero;
    int digit = 0;

    for (int i = input.size(); i > 0; i -= BigNum::basePower) {
        size_t start = (i < BigNum::basePower) ? 0 : i - BigNum::basePower;
        value[digit++] = std::stoull(input.substr(start, i - start)); // Little Endian
    }
}
inline int getDigits(const data& a) {
    for (int i = BigNum::digitsMinus; i >= 0; --i) {
        if (a[i] != 0) return i + 1;
    }
    return 0;
}
BigNum::BigNum(const data& inpt) : value(inpt) {}

const uint64_t base = 1e9;
constexpr data rsa_n = parse("9616540267013058477253762977293425063379243458473593816900454019721117570003248808113992652836857529658675570356835067184715201230519907361653795328462699");
constexpr data rsa_e = parse("65537");
constexpr data rsa_d = parse("4802033916387221748426181350914821072434641827090144975386182740274856853318276518446521844642275539818092186650425384826827514552122318308590929813048801");


std::string BigNum::encrypt() const {
    return BigNum::toString(modExpNums(this->value, rsa_e, rsa_n));
}

std::string BigNum::decrypt() const {
    return BigNum::toString(modExpNums(this->value, rsa_d, rsa_n));
}

int BigNum::compare(const data &a, const data &b) {
    for (int i = digitsMinus; i >= 0; --i) {
        if (a[i] != b[i]) return a[i] > b[i] ? 1 : -1;
    }
    return 0;
}


bool BigNum::isValidNumber(const std::string& str) {
    return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
}

BigNum BigNum::operator+(const BigNum& other) const {
    return BigNum(addNums(this->value, other.value));
}

BigNum BigNum::operator-(const BigNum& other) const {
    return BigNum(subtractNums(this->value, other.value));
}

BigNum BigNum::operator*(const BigNum& other) const {
    return BigNum(multiplyNums(this->value, other.value));
}

BigNum BigNum::operator/(const BigNum& other) const {
    return BigNum(divideNums(this->value, other.value).first);
}

BigNum BigNum::operator%(const BigNum& other) const {
    return BigNum(modulusNums(this->value, other.value));
}
void BigNum::print() {
    if (value == zero) {
        std::cout << 0 << std::endl;
        return;
    }
    bool started = false;
    for (int i = digitsMinus; i >= 0; --i) {
        if (started) {
            std::cout << std::setw(9) << std::setfill('0') << value[i];
        } else if (value[i] != 0) {
            started = true;
            std::cout << value[i];
        }     
    }
    std::cout << std::endl;
}

void BigNum::print(data input) {
    if (input == zero) {
        std::cout << 0 << std::endl;
        return;
    }
    bool started = false;
    for (int i = digitsMinus; i >= 0; --i) {
        if (started) {
            std::cout << std::setw(9) << std::setfill('0') << input[i];
        } else if (input[i] != 0) {
            started = true;
            std::cout << input[i];
        } 
    }
    std::cout << std::endl;
}
inline std::string BigNum::toString(data input) {
    std::ostringstream res;
    bool started = false;
    for (int i = digitsMinus; i >= 0; --i) {
        if (started) {
            res << std::setw(9) << std::setfill('0') << input[i];
        } else if (input[i] != 0) {
            started = true;
            res << input[i];
        }
    }
    return res.str() == "" ? "0" : res.str();
}

std::string BigNum::toString() const {

    std::ostringstream res;
    bool started = false;
    for (int i = digitsMinus; i >= 0; --i) {
        if (started) {
            res << std::setw(9) << std::setfill('0') << value[i];
        } else if (value[i] != 0) {
            started = true;
            res << value[i];
        }
    }
    return res.str() == "" ? "0" : res.str();
}

inline data BigNum::addNums(const data& a, const data& b) {
    data result;
    uint64_t carry = 0;
   
    for (size_t i = 0; i < digits; ++i) {
        uint64_t sum = a[i] + b[i] + carry;
        result[i] = sum % base;
        carry = sum / base;
    }
    
   return result;
}

inline data BigNum::subtractNums(const data& a, const data& b) {
    
    data result = {};  
    int borrow = 0;  

    for (size_t i = 0; i < digits; ++i) {
        if (a[i] >= b[i] + borrow) {
            result[i] = a[i] - b[i] - borrow;
            borrow = 0;
        } else {
            result[i] = a[i] + base - b[i]-borrow;
            borrow = 1;
        }
    }
    return result;  
}



data BigNum::multiplyNums(const data& a, const data& b) {
    data result = {};
    if (getDigits(a) > workspace || getDigits(b) > workspace) {
        std::cout << "Too big" << std::endl;
    }
    for (int i = 0; i < workspace; ++i) {
        int carry = 0;
        for (int j = 0; j < workspace; ++j) {
            uint64_t sum = a[i] * b[j] + result[i + j] + carry;
            carry = sum / base;
            result[i+j] = sum % base;
        }
        result[i + workspace] += carry;
    }
    return result;
}


//gets smaller
inline void shiftRight(data& a, int shift) {
    for (int i = 0; i < BigNum::digits - shift; ++i) {
        a[i] = a[i + shift];
    }
    for (int i = BigNum::digits - shift; i < BigNum::digits; ++i) {
        a[i] = 0;
    }
}

// for shift > 1
inline data shiftLeft(data& a, int shift) {
    data result = {};
    for (int i = BigNum::digitsMinus; i >=shift ; --i) {
        a[i] = a[i - shift];
    }
    for (int i = 0; i < shift; ++i) {
        a[i] = 0;
    }
    return result;
}


data scaleConst(const data& a, const uint64_t& b) {
    data result = {};


    int carry = 0;
    int end = getDigits(a);
    for (int i = 0; i < end; ++i) {
        uint64_t sum = a[i] * b + result[i] + carry;
        carry = sum / base;
        result[i] = sum % base;
    }
    result[end] = carry;
    return result;
}

std::pair<data, data> BigNum::divideNums(const data& a, const data& b) {
    // if (b == "0") throw std::runtime_error("Error: Divide by zero");
    if (compare(a, b) < 0) return {{}, b};
    
    data q = {}, curr = a;
    int shift = getDigits(a) - getDigits(b);
    data divisor = b;
    shiftLeft(divisor, shift);
    while (shift >= 0) {
        uint64_t l = 0, r = base;
        while (l < r) {
            uint64_t mid = (l + r) / 2;
            data midScaled = scaleConst(divisor, mid);

            int comparison = compare(midScaled, curr);
            if (comparison > 0) {
                r = mid;
            } else {
                l = mid + 1;
            }
        }
        --l;
        curr = subtractNums(curr, scaleConst(divisor, l));
        q[shift] = l;
        shiftRight(divisor, 1);
        shift--;
    }

    return {q, curr};
}


data BigNum::modulusNums(const data &a, const data &b) {
    // if (b == "0") throw std::runtime_error("Error: Divide by zero");
    if (compare(a, b) < 0) return a;
    data remainder = divideNums(a, b).second;
    return remainder;
}


data BigNum::modExpNums(const data &a, const data &b, const data &c) {
    data result = one;
    data base = modulusNums(a, c);
    data exponent = b;

    
    while (compare(exponent, zero) > 0) {

        if (exponent[0] % 2 == 1) {
            result = modulusNums(multiplyNums(result, base), c);
        }
        base = modulusNums(multiplyNums(base, base), c);
        exponent = multiplyNums(exponent, fiveHundredMillion);
        shiftRight(exponent, 1);

    }
    
    return result;
}

