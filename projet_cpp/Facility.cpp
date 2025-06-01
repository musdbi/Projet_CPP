#include "Facility.h"
#include <cmath>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <stdexcept>

// Fonction utilitaire pour parser les dates
std::tm parseDate(const std::string& date) {
    std::tm tm = {};
    std::stringstream ss(date);
    ss >> std::get_time(&tm, "%Y-%m-%d");
    
    if (ss.fail()) {
        throw std::invalid_argument("Invalid date format: " + date);
    }
    
    return tm;
}

Facility::Facility(const std::string& startDate, const std::string& endDate, 
                   double amount, const std::string& currency,
                   const std::vector<Lender>& lenders, double annualInterestRate) 
    : startDate(startDate), endDate(endDate), currency(currency), 
      lenders(lenders), annualInterestRate(annualInterestRate), amount(amount) {
    
    // Validation des paramètres
    validateAmount(amount);
    validateInterestRate(annualInterestRate);
    validateDates(startDate, endDate);
    
    if (currency.empty()) {
        throw std::invalid_argument("Currency cannot be empty");
    }
    
    if (lenders.empty()) {
        throw std::invalid_argument("At least one lender is required");
    }
}

void Facility::addPart(const Part& part) {
    // Vérifier que le remboursement ne dépasse pas le montant restant
    if (part.getAmount() > getRemainingAmount()) {
        throw std::invalid_argument("Part amount exceeds remaining facility amount");
    }
    
    parts.push_back(part);
}

void Facility::payPart(double amount, const std::string& date) {
    Part newPart(amount, date);  // Part valide ses propres paramètres
    addPart(newPart);
}

double Facility::getRemainingAmount() const {
    double totalPaid = 0;
    for (const Part& part : parts) {
        totalPaid += part.getAmount();
    }
    return amount - totalPaid;
}

double Facility::getAmount() const {
    return amount;
}

const std::string& Facility::getStartDate() const {
    return startDate;
}

const std::string& Facility::getEndDate() const {
    return endDate;
}

const std::string& Facility::getCurrency() const {
    return currency;
}

const std::vector<Lender>& Facility::getLenders() const {
    return lenders;
}

double Facility::getAnnualInterestRate() const {
    return annualInterestRate;
}

const std::vector<Part>& Facility::getParts() const {
    return parts;
}

size_t Facility::getPartsCount() const {
    return parts.size();
}

double Facility::calculateInterest(const Part& part) const {
    try {
        // Convertir les dates pour calculer la durée en années
        std::tm start = parseDate(startDate);
        std::tm end = parseDate(part.getDate());
        
        std::time_t startTime = std::mktime(&start);
        std::time_t endTime = std::mktime(&end);
        
        if (startTime == (std::time_t)(-1) || endTime == (std::time_t)(-1)) {
            throw std::runtime_error("Failed to convert dates to time_t");
        }
        
        double timeInYears = std::difftime(endTime, startTime) / (60 * 60 * 24 * 365);
        
        if (timeInYears < 0) {
            throw std::invalid_argument("Part date cannot be before facility start date");
        }
        
        // Calcul des intérêts composés
        double interest = getRemainingAmount() * std::pow(annualInterestRate, timeInYears);
        return interest;
        
    } catch (const std::exception& e) {
        throw std::runtime_error("Error calculating interest: " + std::string(e.what()));
    }
}

void Facility::validateAmount(double amount) const {
    if (amount <= 0) {
        throw std::invalid_argument("Facility amount must be positive");
    }
}

void Facility::validateInterestRate(double rate) const {
    if (rate < 0) {
        throw std::invalid_argument("Interest rate cannot be negative");
    }
}

void Facility::validateDates(const std::string& startDate, const std::string& endDate) const {
    // Valider le format des dates
    parseDate(startDate);  // Lève une exception si invalide
    parseDate(endDate);    // Lève une exception si invalide
    
    // Vérifier que la date de fin est après la date de début
    std::tm start = parseDate(startDate);
    std::tm end = parseDate(endDate);
    
    std::time_t startTime = std::mktime(&start);
    std::time_t endTime = std::mktime(&end);
    
    if (endTime <= startTime) {
        throw std::invalid_argument("End date must be after start date");
    }
}