# Complex Quantitative Trading Model

## Overview

This project implements a quantitative trading model in C++. It uses mathematical models and complex algorithms to analyze US equity data from the past three years.

## Features

- **Data Processing:** Load your own data, I got mine from FirstRate Data
- **Feature Engineering:** Compute advanced technical indicators such as SMA, EMA, RSI, and MACD.
- **Trading Strategy:** Implement a logistic regression-based trading strategy using machine learning.
- **Backtesting:** Simulate trading strategies on historical data to evaluate performance.
- **Performance Evaluation:** Calculate key metrics including total return, annualized return, maximum drawdown, and Sharpe ratio.
- **Modular Design:** Organized codebase with clear separation of concerns for maintainability and scalability.

## Installation

### Prerequisites

- **C++ Compiler:** Ensure you have a C++17 compatible compiler installed (e.g., `g++`).
- **Eigen Library:** Install the Eigen library for linear algebra operations.

```bash
sudo apt-get update
sudo apt-get install libeigen3-dev
