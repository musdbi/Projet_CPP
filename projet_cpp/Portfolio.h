#ifndef PORTFOLIO_H
#define PORTFOLIO_H

#include "Facility.h"

class Portfolio {
public:
    explicit Portfolio(const Facility* facility);
    
    double getTotalAmount() const;
    double getTotalInterest() const;
    double getTotalRemboursement() const;
    
    size_t getNumberOfParts() const;
    double getRemainingAmount() const;

private:
    const Facility* facility;
    
    void validateFacility(const Facility* facility) const;
};

#endif