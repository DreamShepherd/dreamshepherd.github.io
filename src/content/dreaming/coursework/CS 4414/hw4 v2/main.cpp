#include "bignum.hpp"
#include "threadpool.hpp"

#include <iostream>
#include <string>
#include <vector>

const std::string whitespace = "032";
constexpr int WORDLENGTH = 51;
constexpr int WORDCHARLENGTH = 3 * WORDLENGTH;

template <typename T>
void resizeInsert(std::vector<T>& arr, size_t index, const T& value) {
    if (index >= arr.size()) {
        arr.resize(2 * (index+1)); // exponential resizing
    }
    arr[index] = value;
}

inline void addChar(std::string& buf, char c) {
    std::string converted = std::to_string(static_cast<int>(c));
    for (int i = 0; i < 3 - converted.length(); ++i) {
        buf += '0';
    }
    buf += converted;
}

inline void updateLineCount(std::string& lc, long long linecount) {
    std::string linecountStr = std::to_string(linecount);
    lc.clear();
    for (int i = 0; i < 3 - linecountStr.length(); ++i) {
        lc += whitespace;
    }
    for (char c : linecountStr) {
        addChar(lc, c);
    }
}

inline void addChars(std::string& buf, std::string& line, std::string::iterator& it) {
    int charCount = 3;
    while (it != line.end() && charCount < WORDLENGTH) {
        addChar(buf, *it);
        ++it;
        ++charCount;
    }
    while (charCount < WORDLENGTH) {
        buf += whitespace;
        ++charCount;
    }
}

void encrypt() {
    std::string line;
    std::vector<BigNum> code;
    long long linecount = 1;
    std::string convertedLC;
    std::string curr;
    curr.reserve(WORDCHARLENGTH);
    while (std::getline(std::cin, line)) {
        auto it = line.begin();
        updateLineCount(convertedLC, linecount);
        
        curr.clear();
        curr += convertedLC;
        addChars(curr, line, it);
        code.emplace_back(curr);

        curr.clear();
        addChars(curr, line, it);
        curr += convertedLC;
        code.emplace_back(curr);

        ++linecount;
    }

    for (BigNum& num : code) {
        std::cout << num.encrypt() << std::endl;
    }
}

std::string decryptUpperLine(const std::string& line) {
    BigNum message = BigNum(line);
    std::string decrypted = message.decrypt();
    std::string paddedDecrypted = std::string(WORDCHARLENGTH - decrypted.length(), '0') + decrypted;
    std::string res;
    res.reserve(WORDLENGTH);
    for (int i = 0; i < WORDCHARLENGTH - 9; i += 3) {
        res += static_cast<char>(std::stoi(paddedDecrypted.substr(i, 3)));
    }
    while (res.back() == ' ') res.pop_back();
    return res;
}

std::string decryptLowerLine(const std::string& line) {
    BigNum message = BigNum(line);
    std::string decrypted = message.decrypt();
    std::string paddedDecrypted = std::string(WORDCHARLENGTH - decrypted.length(), '0') + decrypted;
    std::string res;
    res.reserve(WORDLENGTH);
    for (int i = 9; i < WORDCHARLENGTH; i += 3) {
        res += static_cast<char>(std::stoi(paddedDecrypted.substr(i, 3)));
    }
    while (res.back() == ' ') res.pop_back();
    return res;
}
    

void decrypt() {
    BigNum message("0");
    std::string line;
    std::vector<std::string> decryptedLines;
    ThreadPool pool(std::thread::hardware_concurrency());
    int linecount = 0;
 
    while (std::getline(std::cin, line)) {
        // little endian
        pool.enqueue([line, linecount, &decryptedLines]() { resizeInsert(decryptedLines, 2 * linecount, decryptLowerLine(line)); });
        std::getline(std::cin, line);
        pool.enqueue([line, linecount, &decryptedLines]() { resizeInsert(decryptedLines, 2 * linecount+1, decryptUpperLine(line)); });
        ++ linecount;
    }

    pool.finish();
    for (auto it = decryptedLines.begin(); it != decryptedLines.end(); ++it) {
        if (*it == "") break;
        std::string& lowerWord = *it;
        ++it;
        std::string& upperWord = *it;
        std::cout << lowerWord << upperWord << '\n';
    }
    
}

inline void printArr(BigNum::data arr) {
    for (auto i : arr) {
        std::cout << i << ' ';
    }
    std::cout << std::endl;
}

int main(int argc, char* argv[]) {

    const std::string binOps = "+-*/%";
    std::string operation = argv[1];
    if (operation == "e") {
        encrypt();
        return 0;
    } else if (operation == "d") {
        decrypt();
        return 0;
    }

    std::string num1 = "0";
    std::string num2 = "0";
    std::string num3 = "0";
    if (argc > 3) {
        num1 = argv[2];
        num2 = argv[3];
    }
    if (argc > 4) {
        num3 = argv[4];
    }

    if (binOps.find(operation) != std::string::npos && argc != 4 ) {
        std::cout << "Error: +-*/% requires two numbers" << std::endl;
        return 1;
    } 

    if (operation == "^" && argc != 5) {
        std::cout << "Error: Exponent requires three numbers" << std::endl;
        return 1;
    }
    try {
        if (!BigNum::isValidNumber(num1)) {
            std::cout << "Error: \"" + num1 + "\" is not an unsigned integer\n";
        }
        if (!BigNum::isValidNumber(num2)) {
            std::cout << "Error: \"" + num2 + "\" is not an unsigned integer\n";
        }
        if (!BigNum::isValidNumber(num3)) {
            std::cout << "Error: \"" + num3 + "\" is not an unsigned integer\n";
        }
        BigNum n1(num1);
        BigNum n2(num2);
        BigNum n3(num3);
        BigNum result("0");

        if (operation == "+") {
            result = n1 + n2;
        } else if (operation == "-") {
            result = n1 - n2;
        } else if (operation == "*") {
            result = n1 * n2;
        } else if (operation == "/") {
            result = n1 / n2;
        } else if (operation == "%") {
            result = n1 % n2;
        } else if (operation == "^") {
            result = BigNum(BigNum::modExpNums(n1.value, n2.value, n3.value));
        } else {
            std::cout << "Error: \"" + operation + "\" is not a supported operator";
            throw std::invalid_argument("");
        }



        for (int i = 0 ; i < argc ; i ++) {
            if (i == 0) {
                std::cout << argv[i] << ' ';
            } else if (i < argc - 1) { 
                std::cout << argv[i] <<  ' ';
            } else {
                std::cout << argv[i];
            }
        }


        std::cout << std::endl;
        BigNum::print(result.value);

    } catch (const std::invalid_argument& e) {
        return 1;
    } catch (const std::runtime_error& e) {
        for (int i = 0 ; i < argc ; i ++) {
            if (i == 0) {
                std::cout << argv[i] + 2 << ' ';
            } else if (i < argc - 1) { 
                std::cout << argv[i] <<  ' ';
            } else {
                std::cout << argv[i];
            }
        }
 
        std::cout << std::endl;
        std::cout << e.what() << "\n0" << std::endl;
        return 1;
    }

    return 0;
}

