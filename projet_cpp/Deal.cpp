#include "Deal.h"
#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <cctype>

// Fonction utilitaire pour parser les dates (réutilisée de Facility)
std::tm parseDealDate(const std::string& date) {
    std::tm tm = {};
    std::stringstream ss(date);
    ss >> std::get_time(&tm, "%Y-%m-%d");
    
    if (ss.fail()) {
        throw std::invalid_argument("Invalid date format: " + date);
    }
    
    return tm;
}

Deal::Deal(const std::string& contract_num,
           const std::string& agent,
           const std::vector<Lender>& pool,
           const Borrower& borrower,
           double project_amount,
           const std::string& currency,
           const std::string& contract_sign_date,
           const std::string& contract_end_date,
           Status status)
    : contract_num(contract_num), agent(agent), pool(pool), borrower(borrower),
      project_amount(project_amount), currency(currency),
      contract_sign_date(contract_sign_date), contract_end_date(contract_end_date),
      status(status) {
    
    // Validation de tous les paramètres
    validateContractNum(contract_num);
    validateAgent(agent);
    validatePool(pool);
    validateAmount(project_amount);
    validateCurrency(currency);
    validateDates(contract_sign_date, contract_end_date);
}

void Deal::addFacility(const Facility& facility) {
    // Vérifier que la facility est compatible avec le deal
    if (facility.getCurrency() != currency) {
        // Permettre des devises différentes - RAS
    }
    
    // Vérifier que le total des facilities ne dépasse pas le montant du projet
    double totalFacilities = getTotalFacilitiesAmount() + facility.getAmount();
    if (totalFacilities > project_amount) {
        throw std::invalid_argument("Total facilities amount cannot exceed project amount");
    }
    
    facilities.push_back(facility);
}

const std::vector<Facility>& Deal::getFacilities() const {
    return facilities;
}

size_t Deal::getFacilitiesCount() const {
    return facilities.size();
}

const std::string& Deal::getContractNum() const {
    return contract_num;
}

const std::string& Deal::getAgent() const {
    return agent;
}

const std::vector<Lender>& Deal::getPool() const {
    return pool;
}

const Borrower& Deal::getBorrower() const {
    return borrower;
}

double Deal::getProjectAmount() const {
    return project_amount;
}

const std::string& Deal::getCurrency() const {
    return currency;
}

const std::string& Deal::getContractSignDate() const {
    return contract_sign_date;
}

const std::string& Deal::getContractEndDate() const {
    return contract_end_date;
}

Deal::Status Deal::getStatus() const {
    return status;
}

std::string Deal::getStatusString() const {
    switch (status) {
        case Status::TERMINATED:
            return "terminated";
        case Status::CLOSED:
            return "closed";
        default:
            return "unknown";
    }
}

void Deal::setStatus(Status status) {
    this->status = status;
}

void Deal::setAgent(const std::string& agent) {
    validateAgent(agent);
    this->agent = agent;
}

void Deal::displayDetails() const {
    std::cout << "=== DEAL DETAILS ===" << std::endl;
    std::cout << "Contract Number: " << contract_num << std::endl;
    std::cout << "Agent: " << agent << std::endl;
    
    std::cout << "Pool: ";
    for (const Lender& lender : pool) {
        std::cout << lender.getName() << " ";
    }
    std::cout << std::endl;
    
    std::cout << "Borrower: " << borrower.getName() << std::endl;
    std::cout << "Project Amount: " << project_amount << " " << currency << std::endl;
    std::cout << "Contract Sign Date: " << contract_sign_date << std::endl;
    std::cout << "Contract End Date: " << contract_end_date << std::endl;
    std::cout << "Status: " << getStatusString() << std::endl;
    std::cout << "Facilities Count: " << facilities.size() << std::endl;
    std::cout << "Total Facilities Amount: " << getTotalFacilitiesAmount() << " " << currency << std::endl;
    std::cout << "===================" << std::endl;
}

double Deal::getTotalFacilitiesAmount() const {
    double total = 0;
    for (const Facility& facility : facilities) {
        total += facility.getAmount();
    }
    return total;
}

bool Deal::isCompleted() const {
    return status == Status::TERMINATED;
}

void Deal::validateContractNum(const std::string& contract_num) const {
    if (contract_num.empty()) {
        throw std::invalid_argument("Contract number cannot be empty");
    }
    
    // Validation du format : une lettre (S,Z,B) suivi par 4 chiffres
    if (contract_num.length() != 5) {
        throw std::invalid_argument("Contract number must be 5 characters (1 letter + 4 digits)");
    }
    
    char firstChar = std::toupper(contract_num[0]);
    if (firstChar != 'S' && firstChar != 'Z' && firstChar != 'B') {
        throw std::invalid_argument("Contract number must start with S, Z, or B");
    }
    
    for (size_t i = 1; i < contract_num.length(); ++i) {
        if (!std::isdigit(contract_num[i])) {
            throw std::invalid_argument("Contract number must have 4 digits after the letter");
        }
    }
}

void Deal::validateAmount(double amount) const {
    if (amount <= 0) {
        throw std::invalid_argument("Project amount must be positive");
    }
}

void Deal::validateDates(const std::string& sign_date, const std::string& end_date) const {
    // Valider le format des dates
    parseDealDate(sign_date);  // Lève une exception si invalide
    parseDealDate(end_date);   // Lève une exception si invalide
    
    // Vérifier que la date de fin est après la date de signature
    std::tm start = parseDealDate(sign_date);
    std::tm end = parseDealDate(end_date);
    
    std::time_t startTime = std::mktime(&start);
    std::time_t endTime = std::mktime(&end);
    
    if (endTime <= startTime) {
        throw std::invalid_argument("Contract end date must be after sign date");
    }
}

void Deal::validateCurrency(const std::string& currency) const {
    if (currency.empty()) {
        throw std::invalid_argument("Currency cannot be empty");
    }
    
    // Validation basique : 3 caractères majuscules
    if (currency.length() != 3) {
        throw std::invalid_argument("Currency must be 3 characters (e.g., EUR, USD)");
    }
    
    for (char c : currency) {
        if (!std::isupper(c)) {
            throw std::invalid_argument("Currency must be uppercase letters");
        }
    }
}

void Deal::validateAgent(const std::string& agent) const {
    if (agent.empty()) {
        throw std::invalid_argument("Agent name cannot be empty");
    }
}

void Deal::validatePool(const std::vector<Lender>& pool) const {
    if (pool.empty()) {
        throw std::invalid_argument("Pool must contain at least one lender");
    }
}