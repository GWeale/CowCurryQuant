#include "TradingStrategy.h"
#include "Utils.h"
#include <cmath>
#include <random>

TradingStrategy::TradingStrategy(const Eigen::MatrixXd& features, const std::vector<std::string>& feature_names)
    : features_(features), feature_names_(feature_names) {
    trainModel();
}

std::vector<TradingSignal> TradingStrategy::generateSignals() {
    std::vector<TradingSignal> signals;
    for (int i = 0; i < features_.rows(); ++i) {
        double prediction = sigmoid(weights_.dot(features_.row(i)));
        TradingSignal signal;
        signal.date = ""; // Assign date appropriately
        if (prediction > 0.6) {
            signal.buy = true;
            signal.sell = false;
        } else if (prediction < 0.4) {
            signal.buy = false;
            signal.sell = true;
        } else {
            signal.buy = false;
            signal.sell = false;
        }
        signals.push_back(signal);
    }
    return signals;
}


void TradingStrategy::trainModel() {
    
    std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution(-1.0, 1.0);
    weights_ = Eigen::VectorXd(features_.cols());
    for (int i = 0; i < weights_.size(); ++i) {
        weights_(i) = distribution(generator); // Random initialization
    }

   
    double learning_rate = 0.01;
    int epochs = 1000;
    for (int epoch = 0; epoch < epochs; ++epoch) {
        Eigen::VectorXd predictions = (features_ * weights_).unaryExpr(&sigmoid);
        Eigen::VectorXd labels = Eigen::VectorXd::Ones(features_.rows()); // Placeholder
        Eigen::VectorXd error = predictions - labels;
        Eigen::VectorXd gradient = features_.transpose() * error / static_cast<double>(features_.rows());
        weights_ -= learning_rate * gradient;
    }
}

double TradingStrategy::sigmoid(double x) {
    return 1.0 / (1.0 + std::exp(-x));
}
