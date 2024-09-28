#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

namespace Utils {
    std::vector<std::string> split(const std::string& s, char delimiter);
    double calculateMean(const std::vector<double>& data);
    double calculateStdDev(const std::vector<double>& data);
}

#endif // UTILS_H
