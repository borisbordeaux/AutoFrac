#ifndef AUTOFRAC_UTILS_H
#define AUTOFRAC_UTILS_H

#include <algorithm>
#include <sstream>
#include <vector>
#include <iomanip>

namespace frac::utils {

inline std::string to_string(float value) {
    std::stringstream stream;
    stream << std::fixed << std::setprecision(4) << value;
    std::string res = stream.str();
    std::replace(res.begin(), res.end(), ',', '.');
    return res;
}

template<typename T>
T mod(T a, T b) {
    return (a % b + b) % b;
}

template<typename T>
std::vector<T> shiftVector(std::vector<T> const& vec) {
    std::vector<T> res;
    res.reserve(vec.size());
    for (std::size_t i = 1; i < vec.size(); ++i) {
        res.emplace_back(vec[i]);
    }
    res.emplace_back(vec[0]);
    return res;
}

inline std::vector<std::string> split(std::string const& str, char const& delimiter) {
    std::vector<std::string> strings;
    std::istringstream f(str);
    std::string s;
    while (std::getline(f, s, delimiter)) {
        strings.push_back(s);
    }
    return strings;
}

inline std::vector<std::string> split(std::string const& str, std::string const& delimiter) {
    std::vector<std::string> strings;
    std::size_t start = 0, end;
    while ((end = str.find(delimiter, start)) != std::string::npos) {
        strings.push_back(str.substr(start, end - start));
        start = end + delimiter.length();
    }
    strings.push_back(str.substr(start));
    return strings;
}

inline std::vector<float> get_bezier_linear_transformation(unsigned int i, unsigned int n) {
    float denominator = static_cast<float>(n);
    return {
        static_cast<float>(n - i) / denominator,
        static_cast<float>(n - i - 1) / denominator,

        static_cast<float>(i) / denominator,
        static_cast<float>(i + 1) / denominator
    };
}

inline std::vector<float> get_bezier_quadratic_transformation(unsigned int i, unsigned int n) {
    float denominator = static_cast<float>(n * n);
    return {
        static_cast<float>((i - n) * (i - n)) / denominator,
        static_cast<float>((i - n) * (1 + i - n)) / denominator,
        static_cast<float>((1 + i - n) * (1 + i - n)) / denominator,

        static_cast<float>(2 * i * (n - i)) / denominator,
        static_cast<float>(n + 2 * i * n - 2 * i * (i + 1)) / denominator,
        static_cast<float>(-2 * (1 + i - n) * (1 + i)) / denominator,

        static_cast<float>(i * i) / denominator,
        static_cast<float>(i * (1 + i)) / denominator,
        static_cast<float>((i + 1) * (i + 1)) / denominator
    };
}

inline std::vector<float> get_bezier_cubic_transformation(unsigned int i, unsigned int n) {
    float denominator = static_cast<float>(n * n * n);
    return {
        static_cast<float>(-(i - n) * (i - n) * (i - n)) / denominator,
        static_cast<float>(-(i - n) * (i - n) * (i - n + 1)) / denominator,
        static_cast<float>(-(i - n) * (i - n + 1) * (i - n + 1)) / denominator,
        static_cast<float>(-(i - n + 1) * (i - n + 1) * (i - n + 1)) / denominator,

        static_cast<float>(3 * i * (i - n) * (i - n)) / denominator,
        static_cast<float>((i - n) * (3 * i * (i - n + 1) - n)) / denominator,
        static_cast<float>((i - n + 1) * (3 * i * (i - n + 1) - 2 * n)) / denominator,
        static_cast<float>(3 * (1 + i) * (i - n + 1) * (i - n + 1)) / denominator,

        static_cast<float>(3 * i * i * (n - i)) / denominator,
        static_cast<float>(i * (n * (3 * i + 2) - 3 * i * (i + 1))) / denominator,
        static_cast<float>((i + 1) * (3 * i * n + n - 3 * i * (i + 1))) / denominator,
        static_cast<float>(-3 * (i + 1) * (i + 1) * (i - n + 1)) / denominator,

        static_cast<float>(i * i * i) / denominator,
        static_cast<float>(i * i * (i + 1)) / denominator,
        static_cast<float>(i * (i + 1) * (i + 1)) / denominator,
        static_cast<float>((i + 1) * (i + 1) * (i + 1)) / denominator,
    };
}

inline std::vector<float> get_cantor_linear_transformation(unsigned int i, unsigned int n) {
    return get_bezier_linear_transformation(2 * i, 2 * n - 1);
}

inline std::vector<float> get_cantor_quadratic_transformation(unsigned int i, unsigned int n) {
    return get_bezier_quadratic_transformation(2 * i, 2 * n - 1);
}

inline std::vector<float> get_cantor_cubic_transformation(unsigned int i, unsigned int n) {
    return get_bezier_cubic_transformation(2 * i, 2 * n - 1);
}

} // frac::utils

#endif //AUTOFRAC_UTILS_H
