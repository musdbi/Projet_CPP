#include "Borrower.h"
#include <stdexcept>

Borrower::Borrower() : name("Air France") {
}

Borrower::Borrower(const std::string& name) : name(name) {
    validateName(name);
}

const std::string& Borrower::getName() const {
    return name;
}

void Borrower::setName(const std::string& name) {
    validateName(name); 
    this->name = name;
}

void Borrower::validateName(const std::string& name) const {
    if (name.empty()) {
        throw std::invalid_argument("Borrower name cannot be empty");
    }
}