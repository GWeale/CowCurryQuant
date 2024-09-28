#include <iostream>
#include "DataProcessor.h"
#include "TradingStrategy.h"
#include "Backtester.h"
#include "PerformanceEvaluator.h"

int main() {
    DataProcessor processor("data/us_equity_data.csv");
    if (!processor.loadData()) {
        std::cerr << "Data loading failed." << std::endl;
        return -1;
    }
    processor.cleanData();
    processor.addTechnicalIndicators();

    Eigen::MatrixXd features = processor.getFeatureMatrix();
    std::vector<std::string> feature_names = processor.getFeatureNames();

    TradingStrategy strategy(features, feature_names);
    std::vector<TradingSignal> signals = strategy.generateSignals();

    std::vector<Trade> trades;
    for (const auto& signal : signals) {
        Trade trade;
        trade.date = signal.date;
        if (signal.buy) {
            trade.type = "buy";
            trade.price = 100.0; // cant put my price here :)
            trade.quantity = 10;
            trades.push_back(trade);
        } else if (signal.sell) {
            trade.type = "sell";
            trade.price = 100.0; // cant put my price here :)
            trade.quantity = 10;
            trades.push_back(trade);
        }
    }

    Backtester backtester(trades);
    BacktestResult backtest_result = backtester.runBacktest();

    std::vector<double> daily_returns = { /* cant put my price here :) */ };
    PerformanceEvaluator evaluator(backtest_result, daily_returns);
    PerformanceMetrics metrics = evaluator.evaluate();

    std::cout << "Total Return: " << metrics.total_return << "%" << std::endl;
    std::cout << "Annualized Return: " << metrics.annualized_return * 100 << "%" << std::endl;
    std::cout << "Max Drawdown: " << metrics.max_drawdown << "%" << std::endl;
    std::cout << "Sharpe Ratio: " << metrics.sharpe_ratio << std::endl;

    return 0;
}
