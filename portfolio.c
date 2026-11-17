#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

typedef struct {
    char symbol[20];
    int quantity;
    double buy_price;
    double current_price;
} Stock;

Stock *portfolio = NULL;
int count = 0;

int findStock(char symbol[]) {
    for (int i = 0; i < count; i++) {
        if (strcmp(portfolio[i].symbol, symbol) == 0)
            return i;
    }
    return -1;
}

void saveData() {
    FILE *fp = fopen("portfolio.dat", "wb");
    if (fp == NULL) {
        printf("Unable to save data.\n");
        return;
    }
    fwrite(&count, sizeof(int), 1, fp);
    fwrite(portfolio, sizeof(Stock), count, fp);
    fclose(fp);
}

void loadData() {
    FILE *fp = fopen("portfolio.dat", "rb");
    if (fp == NULL)
        return;

    int savedCount;
    if (fread(&savedCount, sizeof(int), 1, fp) != 1 ||
        savedCount < 0 || savedCount > 100000) {
        fclose(fp);
        return;
    }

    Stock *temp = savedCount > 0 ? malloc((size_t)savedCount * sizeof(Stock)) : NULL;
    if (savedCount > 0 && temp == NULL) {
        fclose(fp);
        return;
    }

    if (savedCount > 0 &&
        fread(temp, sizeof(Stock), (size_t)savedCount, fp) != (size_t)savedCount) {
        free(temp);
        fclose(fp);
        return;
    }

    free(portfolio);
    portfolio = temp;
    count = savedCount;
    fclose(fp);
}

void buyStock() {
    char symbol[20];
    int quantity;
    double price;

    printf("\nEnter stock symbol: ");
    if (scanf("%19s", symbol) != 1) return;

    for (int i = 0; symbol[i]; i++)
        symbol[i] = (char)toupper((unsigned char)symbol[i]);

    printf("Enter quantity: ");
    if (scanf("%d", &quantity) != 1 || quantity <= 0) {
        printf("Invalid quantity.\n");
        return;
    }

    printf("Enter buying price: ");
    if (scanf("%lf", &price) != 1 || !isfinite(price) || price <= 0) {
        printf("Invalid price.\n");
        return;
    }

    int index = findStock(symbol);
    if (index != -1) {
        Stock *s = &portfolio[index];
        s->buy_price = ((s->quantity * s->buy_price) +
                        (quantity * price)) / (s->quantity + quantity);
        s->quantity += quantity;
    } else {
        Stock *temp = realloc(portfolio, (size_t)(count + 1) * sizeof(Stock));
        if (temp == NULL) {
            printf("Memory allocation failed.\n");
            return;
        }
        portfolio = temp;
        strcpy(portfolio[count].symbol, symbol);
        portfolio[count].quantity = quantity;
        portfolio[count].buy_price = price;
        portfolio[count].current_price = price;
        count++;
    }

    saveData();
    printf("\nStock purchased successfully!\n");
}

void sellStock() {
    char symbol[20];
    int quantity;

    printf("\nEnter stock symbol: ");
    if (scanf("%19s", symbol) != 1) return;

    for (int i = 0; symbol[i]; i++)
        symbol[i] = (char)toupper((unsigned char)symbol[i]);

    int index = findStock(symbol);
    if (index == -1) {
        printf("Stock not found in portfolio.\n");
        return;
    }

    printf("Available quantity: %d\n", portfolio[index].quantity);
    printf("Enter quantity to sell: ");
    if (scanf("%d", &quantity) != 1 || quantity <= 0 ||
        quantity > portfolio[index].quantity) {
        printf("Invalid quantity.\n");
        return;
    }

    portfolio[index].quantity -= quantity;
    if (portfolio[index].quantity == 0) {
        for (int i = index; i < count - 1; i++)
            portfolio[i] = portfolio[i + 1];
        count--;

        if (count == 0) {
            free(portfolio);
            portfolio = NULL;
        } else {
            Stock *temp = realloc(portfolio, (size_t)count * sizeof(Stock));
            if (temp != NULL)
                portfolio = temp;
        }
    }

    saveData();
    printf("\nStock sold successfully!\n");
}

void updatePrice() {
    char symbol[20];
    double price;

    printf("\nEnter stock symbol: ");
    if (scanf("%19s", symbol) != 1) return;

    for (int i = 0; symbol[i]; i++)
        symbol[i] = (char)toupper((unsigned char)symbol[i]);

    int index = findStock(symbol);
    if (index == -1) {
        printf("Stock not found.\n");
        return;
    }

    printf("Enter current market price: ");
    if (scanf("%lf", &price) != 1 || !isfinite(price) || price <= 0) {
        printf("Invalid price.\n");
        return;
    }

    portfolio[index].current_price = price;
    saveData();
    printf("\nMarket price updated successfully!\n");
}

void viewPortfolio() {
    if (count == 0) {
        printf("\nYour portfolio is empty.\n");
        return;
    }

    printf("\n================ PORTFOLIO HOLDINGS ================\n");
    printf("%-12s %-10s %-12s %-12s %-12s\n",
           "Symbol", "Quantity", "Buy Price", "Market Price", "P/L");

    for (int i = 0; i < count; i++) {
        double profit = (portfolio[i].current_price -
                         portfolio[i].buy_price) * portfolio[i].quantity;
        printf("%-12s %-10d %-12.2f %-12.2f %-12.2f\n",
               portfolio[i].symbol, portfolio[i].quantity,
               portfolio[i].buy_price, portfolio[i].current_price, profit);
    }
    printf("=====================================================\n");
}

void portfolioSummary() {
    double invested = 0.0, value = 0.0;

    for (int i = 0; i < count; i++) {
        invested += portfolio[i].quantity * portfolio[i].buy_price;
        value += portfolio[i].quantity * portfolio[i].current_price;
    }

    double profit = value - invested;
    printf("\n============= PORTFOLIO PERFORMANCE =============\n");
    printf("Total Investment : Rs. %.2f\n", invested);
    printf("Current Value    : Rs. %.2f\n", value);
    printf("Total Profit/Loss: Rs. %.2f\n", profit);
    printf("Return Percentage: %.2f%%\n",
           invested > 0 ? (profit / invested) * 100.0 : 0.0);

    if (profit > 0)
        printf("Status           : PROFIT\n");
    else if (profit < 0)
        printf("Status           : LOSS\n");
    else
        printf("Status           : NO PROFIT NO LOSS\n");

    printf("=================================================\n");
}

int main() {
    int choice;
    loadData();

    printf("\n========================================\n");
    printf("       STOCK MARKET PORTFOLIO SIMULATOR\n");
    printf("========================================\n");

    do {
        printf("\n1. Buy Stock");
        printf("\n2. Sell Stock");
        printf("\n3. View Portfolio");
        printf("\n4. Update Current Stock Price");
        printf("\n5. Portfolio Performance");
        printf("\n6. Save Portfolio");
        printf("\n7. Exit");
        printf("\n\nEnter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) {}
            continue;
        }

        switch (choice) {
            case 1: buyStock(); break;
            case 2: sellStock(); break;
            case 3: viewPortfolio(); break;
            case 4: updatePrice(); break;
            case 5: portfolioSummary(); break;
            case 6:
                saveData();
                printf("\nPortfolio saved successfully!\n");
                break;
            case 7:
                saveData();
                printf("\nThank you for using Stock Market Portfolio Simulator!\n");
                break;
            default:
                printf("\nInvalid choice. Please try again.\n");
        }
    } while (choice != 7);

    free(portfolio);
    return 0;
}