#include "DataProcessor.h"
#include "Utils.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>


DataProcessor::DataProcessor(const std::string& filepath) : filepath_(filepath) {}


bool DataProcessor::loadData() {
    std::ifstream file(filepath_);
    if (!file.is_open()) {
        std::cerr << "Failed to open data file: " << filepath_ << std::endl;
        return false;
    }

    std::string line;
    // Skip header
    std::getline(file, line);

    while (std::getline(file, line)) {
        std::vector<std::string> tokens = Utils::split(line, ',');
        if (tokens.size() < 6) continue;

        StockData data;
        data.date = tokens[0];
        data.open = std::stod(tokens[1]);
        data.high = std::stod(tokens[2]);
        data.low = std::stod(tokens[3]);
        data.close = std::stod(tokens[4]);
        data.volume = std::stod(tokens[5]);

        raw_data_.push_back(data);
    }

    file.close();
    return true;
}

// Clean data (e.g., remove missing values)
void DataProcessor::cleanData() {
    cleaned_data_ = raw_data_;
    // Example: Remove entries with zero volume
    cleaned_data_.erase(
        std::remove_if(cleaned_data_.begin(), cleaned_data_.end(),
                       [](const StockData& data) { return data.volume == 0; }),
        cleaned_data_.end());
}

// Add technical indicators
void DataProcessor::addTechnicalIndicators() {
    calculateSMA(20);
    calculateEMA(20);
    calculateRSI(14);
    calculateMACD(12, 26, 9);
}

// Generate feature matrix
Eigen::MatrixXd DataProcessor::getFeatureMatrix() const {
    return feature_matrix_;
}


std::vector<std::string> DataProcessor::getFeatureNames() const {
    return feature_names_;
}

// Calculate Simple Moving Average
void DataProcessor::calculateSMA(int period) {
    feature_names_.push_back("SMA_" + std::to_string(period));
    std::vector<double> sma;
    for (size_t i = 0; i < cleaned_data_.size(); ++i) {
        if (i < period - 1) {
            sma.push_back(0.0);
            continue;
        }
        double sum = 0.0;
        for (int j = 0; j < period; ++j) {
            sum += cleaned_data_[i - j].close;
        }
        sma.push_back(sum / period);
    }
    // Append SMA to feature matrix
    Eigen::VectorXd sma_vector = Eigen::Map<Eigen::VectorXd>(sma.data(), sma.size());
    if (feature_matrix_.size() == 0) {
        feature_matrix_ = sma_vector;
    } else {
        feature_matrix_.conservativeResize(feature_matrix_.rows(), feature_matrix_.cols() + 1);
        feature_matrix_.col(feature_matrix_.cols() - 1) = sma_vector;
    }
}


void DataProcessor::calculateEMA(int period) {
    feature_names_.push_back("EMA_" + std::to_string(period));
    std::vector<double> ema;
    double multiplier = 2.0 / (period + 1);
    double prev_ema = cleaned_data_[0].close;
    ema.push_back(prev_ema);

    for (size_t i = 1; i < cleaned_data_.size(); ++i) {
        double current_ema = (cleaned_data_[i].close - prev_ema) * multiplier + prev_ema;
        ema.push_back(current_ema);
        prev_ema = current_ema;
    }

    Eigen::VectorXd ema_vector = Eigen::Map<Eigen::VectorXd>(ema.data(), ema.size());
    feature_matrix_.conservativeResize(feature_matrix_.rows(), feature_matrix_.cols() + 1);
    feature_matrix_.col(feature_matrix_.cols() - 1) = ema_vector;
}

// Find the New strength
void DataProcessor::calculateRSI(int period) {
    feature_names_.push_back("RSI_" + std::to_string(period));
    std::vector<double> rsi;
    double gain = 0.0, loss = 0.0;

    for (size_t i = 1; i < cleaned_data_.size(); ++i) {
        double change = cleaned_data_[i].close - cleaned_data_[i - 1].close;
        if (change > 0) {
            gain += change;
        } else {
            loss -= change;
        }

        if (i >= period) {
            double average_gain = gain / period;
            double average_loss = loss / period;
            double rs = average_loss == 0 ? 0 : average_gain / average_loss;
            double rsi_value = average_loss == 0 ? 100 : 100 - (100 / (1 + rs));
            rsi.push_back(rsi_value);

            // Subtract the oldest gain/loss
            double old_change = cleaned_data_[i - period + 1].close - cleaned_data_[i - period].close;
            if (old_change > 0) {
                gain -= old_change;
            } else {
                loss += old_change;
            }
        } else {
            rsi.push_back(0.0);
        }
    }

    
    std::vector<double> rsi_full(cleaned_data_.size(), 0.0);
    for (size_t i = period; i < cleaned_data_.size(); ++i) {
        rsi_full[i] = rsi[i - period];
    }

    Eigen::VectorXd rsi_vector = Eigen::Map<Eigen::VectorXd>(rsi_full.data(), rsi_full.size());
    feature_matrix_.conservativeResize(feature_matrix_.rows(), feature_matrix_.cols() + 1);
    feature_matrix_.col(feature_matrix_.cols() - 1) = rsi_vector;
}

// Calculate Moving Average Convergence Divergence
void DataProcessor::calculateMACD(int short_period, int long_period, int signal_period) {
    feature_names_.push_back("MACD");
    feature_names_.push_back("Signal_Line");
    feature_names_.push_back("MACD_Hist");

    std::vector<double> ema_short;
    std::vector<double> ema_long;
    std::vector<double> macd;
    std::vector<double> signal;
    std::vector<double> hist;

    // Calculate short EMA
    double multiplier_short = 2.0 / (short_period + 1);
    double prev_ema_short = cleaned_data_[0].close;
    ema_short.push_back(prev_ema_short);
    for (size_t i = 1; i < cleaned_data_.size(); ++i) {
        double current_ema = (cleaned_data_[i].close - prev_ema_short) * multiplier_short + prev_ema_short;
        ema_short.push_back(current_ema);
        prev_ema_short = current_ema;
    }

    
    double multiplier_long = 2.0 / (long_period + 1);
    double prev_ema_long = cleaned_data_[0].close;
    ema_long.push_back(prev_ema_long);
    for (size_t i = 1; i < cleaned_data_.size(); ++i) {
        double current_ema = (cleaned_data_[i].close - prev_ema_long) * multiplier_long + prev_ema_long;
        ema_long.push_back(current_ema);
        prev_ema_long = current_ema;
    }

    
    for (size_t i = 0; i < cleaned_data_.size(); ++i) {
        macd.push_back(ema_short[i] - ema_long[i]);
    }

    // Calculate Signal Line
    double multiplier_signal = 2.0 / (signal_period + 1);
    double prev_signal = macd[0];
    signal.push_back(prev_signal);
    for (size_t i = 1; i < macd.size(); ++i) {
        double current_signal = (macd[i] - prev_signal) * multiplier_signal + prev_signal;
        signal.push_back(current_signal);
        prev_signal = current_signal;
    }

    // Calculate Histogram
    for (size_t i = 0; i < macd.size(); ++i) {
        hist.push_back(macd[i] - signal[i]);
    }

    // Append to feature matrix
    Eigen::VectorXd macd_vector = Eigen::Map<Eigen::VectorXd>(macd.data(), macd.size());
    Eigen::VectorXd signal_vector = Eigen::Map<Eigen::VectorXd>(signal.data(), signal.size());
    Eigen::VectorXd hist_vector = Eigen::Map<Eigen::VectorXd>(hist.data(), hist.size());

    feature_matrix_.conservativeResize(feature_matrix_.rows(), feature_matrix_.cols() + 3);
    feature_matrix_.col(feature_matrix_.cols() - 3) = macd_vector;
    feature_matrix_.col(feature_matrix_.cols() - 2) = signal_vector;
    feature_matrix_.col(feature_matrix_.cols() - 1) = hist_vector;
}
