#include "Utils.h"
#include <sstream>
#include <cmath>

std::vector<std::string> Utils::split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::stringstream ss(s);
    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

double Utils::calculateMean(const std::vector<double>& data) {
    if (data.empty()) return 0.0;
    double sum = std::accumulate(data.begin(), data.end(), 0.0);
    return sum / data.size();
}

double Utils::calculateStdDev(const std::vector<double>& data) {
    if (data.size() < 2) return 0.0;
    double mean = calculateMean(data);
    double accum = 0.0;
    for (const auto& val : data) {
        accum += (val - mean) * (val - mean);
    }
    return std::sqrt(accum / (data.size() - 1));
}
