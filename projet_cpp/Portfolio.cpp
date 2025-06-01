#include "Portfolio.h"
#include <stdexcept>

Portfolio::Portfolio(const Facility* facility) : facility(facility) {
    validateFacility(facility);
}

double Portfolio::getTotalAmount() const {
    return facility->getAmount();
}

double Portfolio::getTotalInterest() const {
    double total = 0;
    const std::vector<Part>& parts = facility->getParts();
    
    for (const Part& part : parts) {
        total += facility->calculateInterest(part);
    }
    return total;
}

double Portfolio::getTotalRemboursement() const {
    return getTotalAmount() + getTotalInterest();
}

size_t Portfolio::getNumberOfParts() const {
    return facility->getPartsCount();
}

double Portfolio::getRemainingAmount() const {
    return facility->getRemainingAmount();
}

void Portfolio::validateFacility(const Facility* facility) const {
    if (facility == nullptr) {
        throw std::invalid_argument("Facility cannot be null");
    }
}