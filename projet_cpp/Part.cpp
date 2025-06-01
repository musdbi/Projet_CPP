#include "Part.h"
#include <sstream>
#include <iomanip>

Part::Part(double amount, const std::string& date) : amount(amount), date(date) {
    validateAmount(amount);
    validateDate(date);
}

double Part::getAmount() const {
    return amount;
}

const std::string& Part::getDate() const {
    return date;
}

void Part::validateAmount(double amount) const {
    if (amount <= 0) {
        throw std::invalid_argument("Part amount must be positive");
    }
}

void Part::validateDate(const std::string& date) const {
    if (date.empty()) {
        throw std::invalid_argument("Date cannot be empty");
    }
    
    // Validation basique du format YYYY-MM-DD
    if (date.length() != 10 || date[4] != '-' || date[7] != '-') {
        throw std::invalid_argument("Date must be in YYYY-MM-DD format");
    }
    
    // Test de parsing pour vérifier la validité
    std::tm tm = {};
    std::stringstream ss(date);
    ss >> std::get_time(&tm, "%Y-%m-%d");
    
    if (ss.fail()) {
        throw std::invalid_argument("Invalid date format");
    }
}