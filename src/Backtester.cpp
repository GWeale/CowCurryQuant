#include "Backtester.h"
#include <iostream>

Backtester::Backtester(const std::vector<Trade>& trades, double initial_capital)
    : trades_(trades), initial_capital_(initial_capital), final_capital_(initial_capital),
      current_capital_(initial_capital), current_position_(0) {}

BacktestResult Backtester::runBacktest() {
    BacktestResult result;
    result.initial_capital = initial_capital_;
    result.final_capital = initial_capital_;
    result.return_percentage = 0.0;

    for (const auto& trade : trades_) {
        executeTrade(trade);
        result.trades.push_back(trade);
    }

    result.final_capital = current_capital_;
    result.return_percentage = ((final_capital_ - initial_capital_) / initial_capital_) * 100.0;
    return result;
}

void Backtester::executeTrade(const Trade& trade) {
    if (trade.type == "buy" && current_capital_ >= trade.price * trade.quantity) {
        current_capital_ -= trade.price * trade.quantity;
        current_position_ += trade.quantity;
    } else if (trade.type == "sell" && current_position_ >= trade.quantity) {
        current_capital_ += trade.price * trade.quantity;
        current_position_ -= trade.quantity;
    }
}
