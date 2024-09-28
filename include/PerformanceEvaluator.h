#ifndef PERFORMANCE_EVALUATOR_H
#define PERFORMANCE_EVALUATOR_H

#include "Backtester.h"
#include <map>
#include <string>

struct PerformanceMetrics {
    double total_return;
    double annualized_return;
    double max_drawdown;
    double sharpe_ratio;
};

class PerformanceEvaluator {
public:
    PerformanceEvaluator(const BacktestResult& result, const std::vector<double>& daily_returns);
    PerformanceMetrics evaluate();

private:
    BacktestResult result_;
    std::vector<double> daily_returns_;

    double calculateTotalReturn() const;
    double calculateAnnualizedReturn() const;
    double calculateMaxDrawdown() const;
    double calculateSharpeRatio() const;
};

#endif // PERFORMANCE_EVALUATOR_H
