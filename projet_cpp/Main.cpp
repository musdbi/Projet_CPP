#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>

#include "Facility.h"
#include "Lender.h"
#include "Part.h"
#include "Borrower.h"
#include "Deal.h"
#include "Portfolio.h"

std::vector<Lender> pool;
std::vector<Lender> lenders;
std::vector<Borrower> borrowers;
std::vector<Facility> facilities;
std::vector<Deal> deals;

void createDeal();
Borrower createBorrower();
Lender createLender();
void addLenderToPool();
void createFacility();
void pay();
void displayFacilities();
void displayParts();
Deal* findDealByNumber(const std::string& dealNum);

int main() {
    try {
        // Création des emprunteurs
        borrowers.push_back(Borrower("Air France"));
        borrowers.push_back(Borrower("Orange"));

        // Création des prêteurs
        lenders.push_back(Lender("BNP"));
        lenders.push_back(Lender("SG"));

        // Création d'un deal avec le nouveau constructeur
        Deal deal("S1234", "BNP", lenders, borrowers[0], 10000000, "USD", 
                  "2025-06-26", "2035-06-26", Deal::Status::CLOSED);
        deals.push_back(deal);

        // Création d'une facility avec validation
        Facility facility1("2025-06-26", "2035-06-26", 5000000, "USD", 
                          {lenders[0], lenders[1]}, 0.05);
        
        // Ajout de remboursements
        facility1.payPart(1000000, "2027-06-25");
        facility1.payPart(2000000, "2030-06-25");
        facility1.payPart(1000000, "2032-06-25");
        
        facilities.push_back(facility1);
        deals[0].addFacility(facility1);

        // Création du portfolio avec pointeur
        Portfolio portfolio(&facility1);

        while (true) {
            std::cout << "\n---------------------------------------\n";
            std::cout << "Structured Financing Application\n";
            std::cout << "---------------------------------------\n";
            std::cout << "1. Create a new deal\n";
            std::cout << "2. Create a new borrower\n";
            std::cout << "3. Create a new lender\n";
            std::cout << "4. Add a lender to the pool\n";
            std::cout << "5. Create a new facility\n";
            std::cout << "6. Pay a part of a facility\n";
            std::cout << "7. Calculate interest\n";
            std::cout << "8. Manage portfolio\n";
            std::cout << "9. Terminate a deal\n";
            std::cout << "10. Display details of a deal\n";
            std::cout << "11. Display list of borrowers and lenders\n";
            std::cout << "12. Display the facilities\n";
            std::cout << "13. Display the parts\n";
            std::cout << "0. Exit\n";
            std::cout << "---------------------------------------\n";
            std::cout << "Enter your choice: ";

            int choice;
            std::cin >> choice;

            switch (choice) {
                case 1:
                    createDeal();
                    break;
                case 2:
                    borrowers.push_back(createBorrower());
                    break;
                case 3:
                    lenders.push_back(createLender());
                    break;
                case 4:
                    addLenderToPool();
                    break;
                case 5:
                    createFacility();
                    break;
                case 6:
                    pay();
                    break;
                case 7:
                    if (!facilities.empty() && facilities[0].getPartsCount() > 0) {
                        const std::vector<Part>& parts = facilities[0].getParts();
                        std::cout << "Calculate the interest of first part\n";
                        std::cout << "The interest is: " << facilities[0].calculateInterest(parts[0]) << std::endl;
                    } else {
                        std::cout << "No parts available for interest calculation.\n";
                    }
                    break;
                case 8:
                    std::cout << "The total interest accumulated is: " << portfolio.getTotalInterest() << std::endl;
                    std::cout << "The total amount is: " << portfolio.getTotalAmount() << std::endl;
                    std::cout << "Amount should be repaid: " << portfolio.getTotalRemboursement() << std::endl;
                    break;
                case 9:
                    if (!deals.empty()) {
                        deals[0].setStatus(Deal::Status::TERMINATED);
                        std::cout << "Deal terminated successfully.\n";
                    } else {
                        std::cout << "No deals to terminate.\n";
                    }
                    break;
                case 10:
                    if (!deals.empty()) {
                        deals[0].displayDetails();
                    } else {
                        std::cout << "No deals to display.\n";
                    }
                    break;
                case 11:
                    std::cout << "List of lenders:\n";
                    for (const auto& lender : lenders) {
                        std::cout << "- " << lender.getName() << std::endl;
                    }
                    std::cout << "\nList of borrowers:\n";
                    for (const auto& borrower : borrowers) {
                        std::cout << "- " << borrower.getName() << std::endl;
                    }
                    break;
                case 12:
                    displayFacilities();
                    break;
                case 13:
                    displayParts();
                    break;
                case 0:
                    std::cout << "Goodbye!\n";
                    return 0;
                default:
                    std::cout << "Invalid choice. Please enter a number between 0 and 13.\n";
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}

void pay() {
    if (facilities.empty()) {
        std::cout << "No facilities available for payment.\n";
        return;
    }

    double amount;
    std::string date;
    std::cout << "Enter the amount to pay: ";
    std::cin >> amount;
    std::cout << "Enter the date of payment (YYYY-MM-DD): ";
    std::cin >> date;

    try {
        if (facilities[0].getRemainingAmount() < amount) {
            std::cout << "Payment amount exceeds remaining facility amount.\n";
        } else {
            facilities[0].payPart(amount, date);
            std::cout << "Payment successful!\n";
            std::cout << "Remaining amount: " << facilities[0].getRemainingAmount() << std::endl;
        }
    } catch (const std::exception& e) {
        std::cout << "Payment failed: " << e.what() << std::endl;
    }
}

Deal* findDealByNumber(const std::string& dealNum) {
    for (auto& deal : deals) {
        if (deal.getContractNum() == dealNum) {  
            return &deal;
        }
    }
    return nullptr;
}

void createFacility() {
    if (deals.empty()) {
        std::cout << "No deals available. Create a deal first.\n";
        return;
    }

    std::string currency;
    double amount;
    double annualInterestRate;
    std::vector<Lender> selectedLenders;

    std::cout << "Enter the amount: ";
    std::cin >> amount;
    std::cout << "Enter the currency: ";
    std::cin >> currency;
    std::cout << "Enter the annual interest rate (e.g., 0.05 for 5%): ";
    std::cin >> annualInterestRate;

    // Sélection des lenders
    int lenderCount;
    std::cout << "How many lenders would you like to add to this facility? ";
    std::cin >> lenderCount;

    for (int i = 0; i < lenderCount; i++) {
        std::string lenderName;
        std::cout << "Enter the name of lender " << (i+1) << ": ";
        std::cin >> lenderName;

        auto it = std::find_if(lenders.begin(), lenders.end(), 
                              [&lenderName](const Lender& lender) {
            return lender.getName() == lenderName;
        });

        if (it != lenders.end()) {
            selectedLenders.push_back(*it);
        } else {
            std::cout << "Lender " << lenderName << " not found.\n";
        }
    }

    try {
        Facility facility(deals[0].getContractSignDate(), deals[0].getContractEndDate(), 
                         amount, currency, selectedLenders, annualInterestRate);
        
        facilities.push_back(facility);
        deals[0].addFacility(facility);
        std::cout << "Facility created successfully!\n";
    } catch (const std::exception& e) {
        std::cout << "Failed to create facility: " << e.what() << std::endl;
    }
}

void createDeal() {
    std::string contract_num;
    std::string agent;
    double project_amount;
    std::string currency;
    std::string contract_sign_date;
    std::string contract_end_date;

    std::cout << "\nEnter contract number (Format: Letter + 4 digits, e.g., S1234): ";
    std::cin >> contract_num;
    std::cout << "\nEnter agent: ";
    std::cin >> agent;
    std::cout << "\nEnter project amount: ";
    std::cin >> project_amount;
    std::cout << "\nEnter currency (3 letters, e.g., EUR): ";
    std::cin >> currency;
    std::cout << "\nEnter contract sign date (YYYY-MM-DD): ";
    std::cin >> contract_sign_date;
    std::cout << "\nEnter contract end date (YYYY-MM-DD): ";
    std::cin >> contract_end_date;

    try {
        if (borrowers.empty()) {
            std::cout << "No borrowers available. Create a borrower first.\n";
            return;
        }

        Deal deal(contract_num, agent, lenders, borrowers[0], project_amount, 
                 currency, contract_sign_date, contract_end_date, Deal::Status::CLOSED);
        deals.push_back(deal);
        std::cout << "Deal created successfully!\n";
    } catch (const std::exception& e) {
        std::cout << "Failed to create deal: " << e.what() << std::endl;
    }
}

Borrower createBorrower() {
    std::string name;
    std::cout << "Enter borrower's name: ";
    std::cin >> name;

    try {
        return Borrower(name);
    } catch (const std::exception& e) {
        std::cout << "Failed to create borrower: " << e.what() << std::endl;
        return Borrower();  // Retourne le constructeur par défaut
    }
}

Lender createLender() {
    std::string name;
    std::cout << "Enter lender's name: ";
    std::cin >> name;

    try {
        return Lender(name);
    } catch (const std::exception& e) {
        std::cout << "Failed to create lender: " << e.what() << std::endl;
        throw;  // Re-lancer l'exception car Lender n'a pas de constructeur par défaut
    }
}

void addLenderToPool() {
    std::string lenderName;
    std::cout << "Enter the lender's name: ";
    std::cin >> lenderName;

    auto it = std::find_if(lenders.begin(), lenders.end(), 
                          [&lenderName](const Lender& lender) {
        return lender.getName() == lenderName;
    });

    if (it != lenders.end()) {
        pool.push_back(*it);
        std::cout << "Lender " << lenderName << " added to the pool.\n";
    } else {
        std::cout << "Error: Lender " << lenderName << " does not exist.\n";
    }
}

void displayFacilities() {
    if (facilities.empty()) {
        std::cout << "No facilities to display.\n";
        return;
    }

    std::cout << "\n=== FACILITIES ===\n";
    for (size_t i = 0; i < facilities.size(); i++) {
        std::cout << "Facility " << (i+1) << ":\n";
        std::cout << "  Amount: " << facilities[i].getAmount() << " " << facilities[i].getCurrency() << std::endl;
        std::cout << "  Remaining: " << facilities[i].getRemainingAmount() << std::endl;
        std::cout << "  Interest Rate: " << (facilities[i].getAnnualInterestRate() * 100) << "%\n";
        std::cout << "  Parts Count: " << facilities[i].getPartsCount() << std::endl;
        std::cout << "  Start Date: " << facilities[i].getStartDate() << std::endl;
        std::cout << "  End Date: " << facilities[i].getEndDate() << std::endl;
        std::cout << "-------------------\n";
    }
}

void displayParts() {
    if (facilities.empty()) {
        std::cout << "No facilities available.\n";
        return;
    }

    std::cout << "\n=== PARTS ===\n";
    for (size_t i = 0; i < facilities.size(); i++) {
        std::cout << "Facility " << (i+1) << " Parts:\n";
        const std::vector<Part>& parts = facilities[i].getParts();
        
        if (parts.empty()) {
            std::cout << "  No parts in this facility.\n";
        } else {
            for (size_t j = 0; j < parts.size(); j++) {
                std::cout << "  Part " << (j+1) << ": " << parts[j].getAmount() 
                         << " on " << parts[j].getDate() << std::endl;
            }
        }
        std::cout << "-------------------\n";
    }
}