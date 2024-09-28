#include "PerformanceEvaluator.h"
#include <algorithm>
#include <numeric>
#include <cmath>

PerformanceEvaluator::PerformanceEvaluator(const BacktestResult& result, const std::vector<double>& daily_returns)
    : result_(result), daily_returns_(daily_returns) {}

PerformanceMetrics PerformanceEvaluator::evaluate() {
    PerformanceMetrics metrics;
    metrics.total_return = calculateTotalReturn();
    metrics.annualized_return = calculateAnnualizedReturn();
    metrics.max_drawdown = calculateMaxDrawdown();
    metrics.sharpe_ratio = calculateSharpeRatio();
    return metrics;
}

double PerformanceEvaluator::calculateTotalReturn() const {
    return result_.return_percentage;
}

double PerformanceEvaluator::calculateAnnualizedReturn() const {
    int years = 3;
    return std::pow(1 + (result_.return_percentage / 100.0), 1.0 / years) - 1;
}

// Calculate maximum drawdown
double PerformanceEvaluator::calculateMaxDrawdown() const {
    double peak = -std::numeric_limits<double>::infinity();
    double max_drawdown = 0.0;
    double current = result_.initial_capital;

    for (const auto& trade : result_.trades) {
        if (trade.type == "buy") {
            current -= trade.price * trade.quantity;
        } else if (trade.type == "sell") {
            current += trade.price * trade.quantity;
        }
        if (current > peak) {
            peak = current;
        }
        double drawdown = (peak - current) / peak;
        if (drawdown > max_drawdown) {
            max_drawdown = drawdown;
        }
    }
    return max_drawdown * 100.0;
}

double PerformanceEvaluator::calculateSharpeRatio() const {
    double mean = Utils::calculateMean(daily_returns_);
    double stddev = Utils::calculateStdDev(daily_returns_);
    if (stddev == 0.0) return 0.0;
    return mean / stddev * std::sqrt(252); // Annualize
}
