#include "bignum.hpp"
#include <iostream>
#include <algorithm>
#include <string>


const std::string rsa_n = "9616540267013058477253762977293425063379243458473593816900454019721117570003248808113992652836857529658675570356835067184715201230519907361653795328462699";
const std::string rsa_e = "65537";
const std::string rsa_d = "4802033916387221748426181350914821072434641827090144975386182740274856853318276518446521844642275539818092186650425384826827514552122318308590929813048801";


std::string BigNum::encrypt() const {
    return modExpStrings(this->value, rsa_e, rsa_n);
}

std::string BigNum::decrypt() const {
    return modExpStrings(this->value, rsa_d, rsa_n);
}

int BigNum::compareStrings(const std::string &a, const std::string &b) {
    if (a.size() != b.size()) return a.size() > b.size() ? 1 : -1;
    return a > b ? 1 : (a < b ? -1 : 0);
}

BigNum::BigNum(const std::string& value) : value(value) {
    // if (!isValidNumber(value)) {
    //     throw std::invalid_argument("Error: \"" + value + "\" is not an unsigned integer");
    // }

    this->value.erase(0, this->value.find_first_not_of('0'));
    if (this->value.empty()) this->value = "0";
}

bool BigNum::isValidNumber(const std::string& str) {
    return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
}

BigNum BigNum::operator+(const BigNum& other) const {
    return BigNum(addStrings(this->value, other.value));
}

BigNum BigNum::operator-(const BigNum& other) const {
    return BigNum(subtractStrings(this->value, other.value));
}

BigNum BigNum::operator*(const BigNum& other) const {
    return BigNum(multiplyStrings(this->value, other.value));
}

BigNum BigNum::operator/(const BigNum& other) const {
    return BigNum(divideStrings(this->value, other.value).first);
}

BigNum BigNum::operator%(const BigNum& other) const {
    return BigNum(modulusStrings(this->value, other.value));
}

std::string BigNum::toString() const {
    return value;
}

std::string BigNum::addStrings(const std::string& a, const std::string& b) {
    std::string result;
    int carry = 0, sum;
    int i = a.size() - 1, j = b.size() - 1;

    while (i >= 0 || j >= 0 || carry) {
        sum = carry;
        if (i >= 0) sum += a[i--] - '0';
        if (j >= 0) sum += b[j--] - '0';
        carry = sum / 10;
        result.push_back(sum % 10 + '0');
    }
    std::reverse(result.begin(), result.end());
    return result;
}

inline std::string BigNum::subtractStrings(const std::string& a, const std::string& b) {
    std::string result;
    int borrow = 0, diff;
    int i = a.size() - 1, j = b.size() - 1;

    while (i >= 0) {
        diff = a[i--] - '0' - (j >= 0 ? b[j--] - '0' : 0) - borrow;
        borrow = (diff < 0);
        result.push_back((diff + (borrow ? 10 : 0)) + '0');
    }
    while (result.size() > 1 && result.back() == '0') result.pop_back();
    std::reverse(result.begin(), result.end());
    return result;
}

std::string BigNum::multiplyStrings(const std::string& a, const std::string& b) {
    std::string result(a.size() + b.size(), '0');

    for (int i = a.size() - 1; i >= 0; --i) {
        int carry = 0;
        for (int j = b.size() - 1; j >= 0; --j) {
            int sum = (result[i + j + 1] - '0') + (a[i] - '0') * (b[j] - '0') + carry;
            carry = sum / 10;
            result[i + j + 1] = sum % 10 + '0';
        }
        result[i] += carry;
    }

    auto startPos = result.find_first_not_of('0');
    return startPos == std::string::npos ? "0" : result.substr(startPos);
}

std::pair<std::string, std::string> BigNum::divideStrings(const std::string& a, const std::string& b) {
    if (b == "0") throw std::runtime_error("Error: Divide by zero");
    if (a == "0" || compareStrings(a, b) < 0) return {"0", b};
    
    std::string q = "0", curr = a;
    int shift = a.length() - b.length();
    std::string divisor = b + std::string(shift, '0');
    std::string power = "1" + std::string(shift, '0');
    
    while (shift >= 0) {
        while (compareStrings(curr, divisor) >= 0) {
            curr = subtractStrings(curr, divisor);
            q = addStrings(q, power);
        }
        divisor.pop_back();
        power.pop_back();
        shift--;
    }
    return {q, curr};
}


std::string BigNum::modulusStrings(const std::string &a, const std::string &b) {
    if (b == "0") throw std::runtime_error("Error: Divide by zero");
    if (a == "0") return "0";
    if (compareStrings(a, b) < 0) return a;
    std::string remainder = divideStrings(a, b).second;
    return remainder;
}

// std::string multiplyStringsLimit(const std::string& a, const std::string& b, u_long limit) {
//     std::string result(a.size() + b.size(), '0');
//     for (int i = a.size() - 1; i >= 0; --i) {
//         int carry = 0;
//         for (int j = b.size() - 1; j >= 0; --j) {
//             if (result.size() - (i + j + 1) >= limit) {
//                 continue;
//             }
//             int sum = (result[i + j + 1] - '0') + (a[i] - '0') * (b[j] - '0') + carry;
//             carry = sum / 10;
//             result[i + j + 1] = sum % 10 + '0';
//         }
//         if (result.size() - (i + 1) >= limit) break;
//         result[i] += carry;
//     }

//     auto startPos = result.find_first_not_of('0');
//     return startPos == std::string::npos ? "0" : result.substr(startPos);
// }


std::string BigNum::modExpStrings(const std::string &a, const std::string &b, const std::string &c) {
    if (b == "0") return "1";
    if (b == "1") return modulusStrings(a, c);

    std::string result = "1";
    std::string base = modulusStrings(a, c);
    std::string exponent = b;


    
    while (exponent.size() > 0 && compareStrings(exponent, "0") > 0) {
        if ((exponent.back() - '0' )% 2 == 1) {
            result = modulusStrings(multiplyStrings(result, base), c);
        }
        base = modulusStrings(multiplyStrings(base, base), c);
        exponent = multiplyStrings(exponent, "5");
        exponent.pop_back();
        if (base == "0" || base == "1") {
            return (base == "0") ? "0" : "1";
        }
    }
    
    return result;
}

