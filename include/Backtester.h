#ifndef BACKTESTER_H
#define BACKTESTER_H

#include <vector>
#include <string>
#include <map>

struct Trade {
    std::string date;
    std::string type; // "buy" or "sell"
    double price;
    int quantity;
};

struct BacktestResult {
    double initial_capital;
    double final_capital;
    double return_percentage;
    std::vector<Trade> trades;
};

class Backtester {
public:
    Backtester(const std::vector<Trade>& trades, double initial_capital = 100000.0);
    BacktestResult runBacktest();

private:
    std::vector<Trade> trades_;
    double initial_capital_;
    double final_capital_;
    double current_capital_;
    int current_position_;

    void executeTrade(const Trade& trade);
};

#endif // BACKTESTER_H
