#ifndef DEAL_H
#define DEAL_H

#include <string>
#include <vector>
#include <iostream>
#include "Lender.h"
#include "Borrower.h"
#include "Facility.h"

class Deal {
public:
    // Énumération pour les statuts valides
    enum class Status {
        TERMINATED,  // Le montant du prêt a été utilisé et remboursé
        CLOSED       // La gestion du client est en cours
    };

    Deal(const std::string& contract_num,
         const std::string& agent,
         const std::vector<Lender>& pool,
         const Borrower& borrower,
         double project_amount,
         const std::string& currency,
         const std::string& contract_sign_date,
         const std::string& contract_end_date,
         Status status = Status::CLOSED);

    // Destructeur
    ~Deal() = default;

    // Gestion des facilities
    void addFacility(const Facility& facility);
    const std::vector<Facility>& getFacilities() const;
    size_t getFacilitiesCount() const;

    // Getters
    const std::string& getContractNum() const;
    const std::string& getAgent() const;
    const std::vector<Lender>& getPool() const;
    const Borrower& getBorrower() const;
    double getProjectAmount() const;
    const std::string& getCurrency() const;
    const std::string& getContractSignDate() const;
    const std::string& getContractEndDate() const;
    Status getStatus() const;
    std::string getStatusString() const;

    // Setters avec validation
    void setStatus(Status status);
    void setAgent(const std::string& agent);

    // Méthodes utilitaires
    void displayDetails() const;
    double getTotalFacilitiesAmount() const;
    bool isCompleted() const;

private:
    std::string contract_num;
    std::string agent;
    std::vector<Lender> pool;
    Borrower borrower;
    double project_amount;
    std::string currency;
    std::string contract_sign_date;
    std::string contract_end_date;
    Status status;
    std::vector<Facility> facilities;

    // Méthodes de validation privées
    void validateContractNum(const std::string& contract_num) const;
    void validateAmount(double amount) const;
    void validateDates(const std::string& sign_date, const std::string& end_date) const;
    void validateCurrency(const std::string& currency) const;
    void validateAgent(const std::string& agent) const;
    void validatePool(const std::vector<Lender>& pool) const;
};

#endif