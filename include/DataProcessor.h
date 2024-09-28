#ifndef DATA_PROCESSOR_H
#define DATA_PROCESSOR_H

#include <string>
#include <vector>
#include <map>
#include <Eigen/Dense>

struct StockData {
    std::string date;
    double open;
    double high;
    double low;
    double close;
    double volume;
};

class DataProcessor {
public:
    DataProcessor(const std::string& filepath);
    bool loadData();
    void cleanData();
    void addTechnicalIndicators();
    Eigen::MatrixXd getFeatureMatrix() const;
    std::vector<std::string> getFeatureNames() const;

private:
    std::string filepath_;
    std::vector<StockData> raw_data_;
    std::vector<StockData> cleaned_data_;
    Eigen::MatrixXd feature_matrix_;
    std::vector<std::string> feature_names_;

    void calculateSMA(int period);
    void calculateEMA(int period);
    void calculateRSI(int period);
    void calculateMACD(int short_period, int long_period, int signal_period);
};

#endif // DATA_PROCESSOR_H
