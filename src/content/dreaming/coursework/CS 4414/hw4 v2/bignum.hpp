#include <string>
#include <utility>
#include <array>

class BigNum {
public:
    constexpr static size_t digits = 38;
    constexpr static size_t digitsMinus = digits - 1;
    constexpr static size_t workspace = 18;
    typedef std::array<uint64_t, digits> data;
    BigNum(const std::string& value);
    BigNum(const data& value);
    constexpr static uint64_t base = 1e9;
    constexpr static uint64_t basePower = 9;

    BigNum operator+(const BigNum& other) const;
    BigNum operator-(const BigNum& other) const;
    BigNum operator*(const BigNum& other) const;
    BigNum operator/(const BigNum& other) const;
    BigNum operator%(const BigNum& other) const;
    std::string decrypt() const;
    std::string encrypt() const;
    static data modExpNums(const data& a, const data& b, const data& c);
    static bool isValidNumber(const std::string& str);
    static int compare(const data &a, const data &b); 

    std::string toString() const;
    static inline std::string toString(data d);
    static void print(data d);
    void print();
    data value; // Little endian 35 digit, base 1e9

private:

    static data addNums(const data& a, const data& b);
    static data subtractNums(const data& a, const data& b);
    static data multiplyNums(const data& a, const data& b);
    static std::pair<data, data> divideNums(const data& a, const data& b);
    static data modulusNums(const data& a, const data& b);
};
