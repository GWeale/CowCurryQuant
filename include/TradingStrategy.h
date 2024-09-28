#ifndef TRADING_STRATEGY_H
#define TRADING_STRATEGY_H

#include <Eigen/Dense>
#include <vector>
#include <string>

struct TradingSignal {
    std::string date;
    bool buy;
    bool sell;
};

class TradingStrategy {
public:
    TradingStrategy(const Eigen::MatrixXd& features, const std::vector<std::string>& feature_names);
    std::vector<TradingSignal> generateSignals();

private:
    Eigen::MatrixXd features_;
    std::vector<std::string> feature_names_;
    Eigen::VectorXd weights_;

    void trainModel();
    double sigmoid(double x);
};

#endif // TRADING_STRATEGY_H
