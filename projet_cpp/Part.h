#ifndef PART_H
#define PART_H

#include <string>
#include <stdexcept>

class Part {
public:
    Part(double amount, const std::string& date);
    
    double getAmount() const;
    const std::string& getDate() const;

private:
    double amount;
    std::string date;
    
    void validateAmount(double amount) const;
    void validateDate(const std::string& date) const;
};

#endif // PART_H