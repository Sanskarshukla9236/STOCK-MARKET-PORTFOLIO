# Stock Market Portfolio Simulator

**Project 58 – PPS / C Programming**

A menu-driven C program to simulate stock portfolio management.

## Features

1. Buy stocks and calculate the average buying price when adding to an existing holding.
2. Sell stocks while checking available quantity.
3. View portfolio holdings and per-stock profit/loss.
4. Manually update simulated current market prices.
5. Calculate total investment, current portfolio value, profit/loss, and return percentage.
6. Save and load portfolio data using a binary data file.

## Requirements

- GCC or another C11-compatible C compiler.

## Compile and run

### Windows

```bash
gcc portfolio.c -o portfolio.exe -std=c11
portfolio.exe
```

### Linux / macOS

```bash
gcc portfolio.c -o portfolio -std=c11
./portfolio
```

The program creates `portfolio.dat` automatically in the current working directory to store portfolio data.

## Note

This is an educational simulation. Current stock prices are entered manually; the program does not connect to a live stock-market API.
