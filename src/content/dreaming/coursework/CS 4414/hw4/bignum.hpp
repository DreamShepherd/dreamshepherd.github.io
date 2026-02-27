#include <string>
#include <utility>

class BigNum {
public:
    BigNum(const std::string& value);

    BigNum operator+(const BigNum& other) const;
    BigNum operator-(const BigNum& other) const;
    BigNum operator*(const BigNum& other) const;
    BigNum operator/(const BigNum& other) const;
    BigNum operator%(const BigNum& other) const;
    std::string decrypt() const;
    std::string encrypt() const;
    static std::string modExpStrings(const std::string &a, const std::string &b, const std::string &c);
    static bool isValidNumber(const std::string& str);
    static int compareStrings(const std::string &a, const std::string &b); 

    std::string toString() const;

private:
    std::string value;
    static std::string addStrings(const std::string& a, const std::string& b);
    static std::string subtractStrings(const std::string& a, const std::string& b);
    static std::string multiplyStrings(const std::string& a, const std::string& b);
    static std::pair<std::string, std::string> divideStrings(const std::string& a, const std::string& b);
    static std::string modulusStrings(const std::string &a, const std::string &b);
};
