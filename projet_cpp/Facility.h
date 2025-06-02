#ifndef FACILITY_H
#define FACILITY_H

#include <vector>
#include <string>
#include "Lender.h"
#include "Part.h"

class Facility {
public:
    Facility(const std::string& startDate, const std::string& endDate, 
             double amount, const std::string& currency,
             const std::vector<Lender>& lenders, double annualInterestRate);
    
    void addPart(const Part& part);
    void payPart(double amount, const std::string& date);
    
    double getRemainingAmount() const;
    double getAmount() const;
    double calculateInterest(const Part& part) const;
    
    // Getters pour les autres attributs
    const std::string& getStartDate() const;
    const std::string& getEndDate() const;
    const std::string& getCurrency() const;
    const std::vector<Lender>& getLenders() const;
    double getAnnualInterestRate() const;
    
    // Accès en lecture seule aux parts
    const std::vector<Part>& getParts() const;
    size_t getPartsCount() const;

private:
    std::string startDate;
    std::string endDate;
    std::string currency;
    std::vector<Lender> lenders;
    double annualInterestRate;
    double amount;
    std::vector<Part> parts;
    
    // Méthodes de validation privées
    void validateAmount(double amount) const;
    void validateInterestRate(double rate) const;
    void validateDates(const std::string& startDate, const std::string& endDate) const;
};

#endif