#include <iostream>
#include <iomanip>
#include <string>

  class Account {
  protected:
      std::string accountNumber;
      std::string accountHolder;
      double balance;

  public:
      Account(const std::string& number, const std::string& holder, double initialBalance)
          : accountNumber(number), accountHolder(holder), balance(initialBalance) {}

      virtual void displayDetails () const {
          std::cout << "Account Details for " << getType() << " (ID: " << accountNumber << "):" << std::endl;
          std::cout << "   Holder: " << accountHolder << std::endl;
          std::cout << "   Balance: $" << std::fixed << std::setprecision(2) << balance << std::endl;
      }

      virtual void deposit(double amount) {
          balance += amount;
      }

      virtual void withdraw(double amount) {
          if (amount > balance) {
              std::cout << "Amount greater than the balance!" << std::endl;
          } else {
              balance -= amount;
          }
      }

      virtual std::string getType() const {
          return "Account";
      }

      double getBalance() const {
          return balance;
      }
      friend std::ostream& operator<<(std::ostream& os, const Account& acc);
  };

std::ostream& operator<<(std::ostream& os, const Account& acc) {
    acc.displayDetails();
    return os;
}

  class SavingsAccount : public Account {
  private:
      double interestRate;

  public:
      SavingsAccount(const std::string& number, const std::string& holder, double initialBalance, double rate)
          : Account(number, holder, initialBalance), interestRate(rate) {}

      void displayDetails() const override {
          Account::displayDetails();
          std::cout << "   Interest Rate: " << std::fixed << std::setprecision(2) << (interestRate * 100) << "%" << std::endl;
      }

      void withdraw(double amount) override {
          const double minimumBalance = 100; // example minimum balance for savings account
          if (balance - amount < minimumBalance) {
              std::cout << "Not possible. Minimum balance should be maintained." << std::endl;
          } else {
              balance -= amount;
          }
      }

      std::string getType() const override {
          return "Savings Account";
      }
  };

  class CurrentAccount : public Account {
  private:
      double overdraftLimit;

  public:
      CurrentAccount(const std::string& number, const std::string& holder, double initialBalance, double limit)
          : Account(number, holder, initialBalance), overdraftLimit(limit) {}

      void displayDetails() const override {
          Account::displayDetails();
          std::cout << "   Overdraft Limit: $" << std::fixed << std::setprecision(2) << overdraftLimit << std::endl;
      }

      void withdraw(double amount) override {
          if (amount > (balance + overdraftLimit)) {
              std::cout << "Not possible. Amount exceeds overdraft limit." << std::endl;
          } else {
              balance -= amount;
          }
      }

      std::string getType() const override {
          return "Current Account";
      }
  };

  CurrentAccount operator+(CurrentAccount& current, SavingsAccount& savings) {
      double transferAmount = 300; // example transfer amount
      if (savings.getBalance() >= transferAmount) {
          current.deposit(transferAmount);
          savings.withdraw(transferAmount);
      } else {
          std::cout << "Transfer not possible. Insufficient amount in savings account." << std::endl;
      }

      return current;
  }

  int main() {
      SavingsAccount savings("S123", "John Doe", 1000, 0.02);
      CurrentAccount current("C456", "Jane Doe", 2000, 500);

      std::cout << savings;
      std::cout << " " << std::endl;
      std::cout << current;
      std::cout << " " << std::endl;

      savings.deposit(500);
      current.withdraw(1000);

      std::cout << "Account Details after deposit and withdrawal:" << std::endl;
      std::cout << savings;
      std::cout << " " << std::endl;
      std::cout << current;
      std::cout << " " << std::endl;

      // Transfer 300 from savings to current
      current = current + savings;

      std::cout << "Account Details after transfer:" << std::endl;
      std::cout << savings;
      std::cout << " " << std::endl;
      std::cout << current;
      std::cout << " " << std::endl;

      return 0;
  }
