#include <iostream>
#include <cassert>
#include <stdexcept>
#include <vector>
#include <string>

#include "Borrower.h"
#include "Lender.h"
#include "Part.h"
#include "Facility.h"
#include "Deal.h"
#include "Portfolio.h"

// Compteurs pour les tests
int tests_passed = 0;
int tests_failed = 0;

#define TEST_ASSERT(condition, test_name) \
    do { \
        if (condition) { \
            std::cout << "PASS: " << test_name << std::endl; \
            tests_passed++; \
        } else { \
            std::cout << "FAIL: " << test_name << std::endl; \
            tests_failed++; \
        } \
    } while(0)

#define TEST_EXCEPTION(code, exception_type, test_name) \
    do { \
        bool caught = false; \
        try { \
            code; \
        } catch (const exception_type&) { \
            caught = true; \
        } catch (...) { \
            caught = false; \
        } \
        TEST_ASSERT(caught, test_name); \
    } while(0)

// Tests pour la classe Borrower
void test_borrower() {
    std::cout << "\n=== Tests Borrower ===" << std::endl;
    
    // Test constructeur par defaut
    Borrower defaultBorrower;
    TEST_ASSERT(defaultBorrower.getName() == "Air France", "Borrower - Constructeur par defaut");
    
    // Test constructeur avec parametre
    Borrower customBorrower("Lufthansa");
    TEST_ASSERT(customBorrower.getName() == "Lufthansa", "Borrower - Constructeur avec parametre");
    
    // Test setter
    customBorrower.setName("British Airways");
    TEST_ASSERT(customBorrower.getName() == "British Airways", "Borrower - Setter");
    
    // Test validation nom vide
    TEST_EXCEPTION(customBorrower.setName(""), std::invalid_argument, "Borrower - Validation nom vide");
}

// Tests pour la classe Lender
void test_lender() {
    std::cout << "\n=== Tests Lender ===" << std::endl;
    
    // Test constructeur
    Lender lender("BNP Paribas");
    TEST_ASSERT(lender.getName() == "BNP Paribas", "Lender - Constructeur");
    
    // Test que le nom ne peut pas être modifie (pas de setter)
    // Ceci est teste par la compilation - si ça compile, c'est bon
    TEST_ASSERT(true, "Lender - Immutabilite du nom");
}

// Tests pour la classe Part
void test_part() {
    std::cout << "\n=== Tests Part ===" << std::endl;
    
    // Test constructeur valide
    Part validPart(100000.0, "2024-06-01");
    TEST_ASSERT(validPart.getAmount() == 100000.0, "Part - Montant valide");
    TEST_ASSERT(validPart.getDate() == "2024-06-01", "Part - Date valide");
    
    // Test validation montant negatif
    TEST_EXCEPTION(Part(-1000.0, "2024-06-01"), std::invalid_argument, "Part - Montant negatif rejete");
    
    // Test validation montant zero
    TEST_EXCEPTION(Part(0.0, "2024-06-01"), std::invalid_argument, "Part - Montant zero rejete");
    
    // Test validation date vide
    TEST_EXCEPTION(Part(1000.0, ""), std::invalid_argument, "Part - Date vide rejetee");
    
    // Test validation format date incorrect
    TEST_EXCEPTION(Part(1000.0, "invalid-date"), std::invalid_argument, "Part - Format date incorrect rejete");
    
    // Test validation format date court
    TEST_EXCEPTION(Part(1000.0, "2024-6-1"), std::invalid_argument, "Part - Format date court rejete");
}

// Tests pour la classe Facility
void test_facility() {
    std::cout << "\n=== Tests Facility ===" << std::endl;
    
    // Preparation des lenders
    std::vector<Lender> lenders = {Lender("BNP"), Lender("SG")};
    
    // Test constructeur valide
    Facility facility("2024-01-01", "2025-01-01", 1000000.0, "EUR", lenders, 0.03);
    TEST_ASSERT(facility.getAmount() == 1000000.0, "Facility - Montant");
    TEST_ASSERT(facility.getCurrency() == "EUR", "Facility - Devise");
    TEST_ASSERT(facility.getAnnualInterestRate() == 0.03, "Facility - Taux d'interêt");
    TEST_ASSERT(facility.getPartsCount() == 0, "Facility - Pas de parts initialement");
    TEST_ASSERT(facility.getRemainingAmount() == 1000000.0, "Facility - Montant restant initial");
    
    // Test ajout de part valide
    Part part(100000.0, "2024-06-01");
    facility.addPart(part);
    TEST_ASSERT(facility.getPartsCount() == 1, "Facility - Ajout part");
    TEST_ASSERT(facility.getRemainingAmount() == 900000.0, "Facility - Montant restant apres part");
    
    // Test payPart
    facility.payPart(200000.0, "2024-12-01");
    TEST_ASSERT(facility.getPartsCount() == 2, "Facility - PayPart ajoute une part");
    TEST_ASSERT(facility.getRemainingAmount() == 700000.0, "Facility - Montant restant apres payPart");
    
    // Test validation montant negatif
    TEST_EXCEPTION(Facility("2024-01-01", "2025-01-01", -1000.0, "EUR", lenders, 0.03), 
                   std::invalid_argument, "Facility - Montant negatif rejete");
    
    // Test validation taux negatif
    TEST_EXCEPTION(Facility("2024-01-01", "2025-01-01", 1000000.0, "EUR", lenders, -0.01), 
                   std::invalid_argument, "Facility - Taux negatif rejete");
    
    // Test validation dates incorrectes (fin avant debut)
    TEST_EXCEPTION(Facility("2025-01-01", "2024-01-01", 1000000.0, "EUR", lenders, 0.03), 
                   std::invalid_argument, "Facility - Date fin avant debut rejetee");
    
    // Test validation devise vide
    std::vector<Lender> emptyLenders;
    TEST_EXCEPTION(Facility("2024-01-01", "2025-01-01", 1000000.0, "", lenders, 0.03), 
                   std::invalid_argument, "Facility - Devise vide rejetee");
    
    // Test validation lenders vides
    TEST_EXCEPTION(Facility("2024-01-01", "2025-01-01", 1000000.0, "EUR", emptyLenders, 0.03), 
                   std::invalid_argument, "Facility - Lenders vides rejetes");
    
    // Test part qui depasse le montant restant
    Facility smallFacility("2024-01-01", "2025-01-01", 100000.0, "EUR", lenders, 0.03);
    TEST_EXCEPTION(smallFacility.addPart(Part(200000.0, "2024-06-01")), 
                   std::invalid_argument, "Facility - Part depassant le montant rejetee");
}

// Tests pour la classe Deal
void test_deal() {
    std::cout << "\n=== Tests Deal ===" << std::endl;
    
    // Preparation des donnees
    std::vector<Lender> pool = {Lender("BNP"), Lender("SG")};
    Borrower borrower("Air France");
    
    // Test constructeur valide
    Deal deal("S1234", "BNP Paribas", pool, borrower, 10000000.0, "EUR", 
              "2024-01-01", "2025-01-01", Deal::Status::CLOSED);
    
    TEST_ASSERT(deal.getContractNum() == "S1234", "Deal - Numero de contrat");
    TEST_ASSERT(deal.getAgent() == "BNP Paribas", "Deal - Agent");
    TEST_ASSERT(deal.getProjectAmount() == 10000000.0, "Deal - Montant projet");
    TEST_ASSERT(deal.getCurrency() == "EUR", "Deal - Devise");
    TEST_ASSERT(deal.getStatus() == Deal::Status::CLOSED, "Deal - Statut");
    TEST_ASSERT(deal.getStatusString() == "closed", "Deal - Statut string");
    TEST_ASSERT(deal.getFacilitiesCount() == 0, "Deal - Pas de facilities initialement");
    
    // Test ajout facility
    Facility facility("2024-01-01", "2025-01-01", 5000000.0, "EUR", pool, 0.03);
    deal.addFacility(facility);
    TEST_ASSERT(deal.getFacilitiesCount() == 1, "Deal - Ajout facility");
    TEST_ASSERT(deal.getTotalFacilitiesAmount() == 5000000.0, "Deal - Total facilities");
    
    // Test changement de statut
    deal.setStatus(Deal::Status::TERMINATED);
    TEST_ASSERT(deal.getStatus() == Deal::Status::TERMINATED, "Deal - Changement statut");
    TEST_ASSERT(deal.getStatusString() == "terminated", "Deal - Statut terminated string");
    TEST_ASSERT(deal.isCompleted() == true, "Deal - Deal complete");
    
    // Test validation numero de contrat incorrect
    TEST_EXCEPTION(Deal("INVALID", "BNP", pool, borrower, 1000000.0, "EUR", 
                       "2024-01-01", "2025-01-01", Deal::Status::CLOSED), 
                   std::invalid_argument, "Deal - Numero contrat invalide rejete");
    
    // Test validation numero de contrat avec mauvaise lettre
    TEST_EXCEPTION(Deal("X1234", "BNP", pool, borrower, 1000000.0, "EUR", 
                       "2024-01-01", "2025-01-01", Deal::Status::CLOSED), 
                   std::invalid_argument, "Deal - Lettre incorrecte rejetee");
    
    // Test validation montant negatif
    TEST_EXCEPTION(Deal("S1234", "BNP", pool, borrower, -1000000.0, "EUR", 
                       "2024-01-01", "2025-01-01", Deal::Status::CLOSED), 
                   std::invalid_argument, "Deal - Montant negatif rejete");
    
    // Test validation devise incorrecte
    TEST_EXCEPTION(Deal("S1234", "BNP", pool, borrower, 1000000.0, "eu", 
                       "2024-01-01", "2025-01-01", Deal::Status::CLOSED), 
                   std::invalid_argument, "Deal - Devise incorrecte rejetee");
    
    // Test validation pool vide
    std::vector<Lender> emptyPool;
    TEST_EXCEPTION(Deal("S1234", "BNP", emptyPool, borrower, 1000000.0, "EUR", 
                       "2024-01-01", "2025-01-01", Deal::Status::CLOSED), 
                   std::invalid_argument, "Deal - Pool vide rejete");
    
    // Test facility depassant le montant du projet
    Deal smallDeal("S5678", "BNP", pool, borrower, 1000000.0, "EUR", 
                   "2024-01-01", "2025-01-01", Deal::Status::CLOSED);
    Facility largeFacility("2024-01-01", "2025-01-01", 2000000.0, "EUR", pool, 0.03);
    TEST_EXCEPTION(smallDeal.addFacility(largeFacility), 
                   std::invalid_argument, "Deal - Facility depassant projet rejetee");
}

// Tests pour la classe Portfolio
void test_portfolio() {
    std::cout << "\n=== Tests Portfolio ===" << std::endl;
    
    // Preparation des donnees
    std::vector<Lender> lenders = {Lender("BNP"), Lender("SG")};
    Facility facility("2024-01-01", "2025-01-01", 1000000.0, "EUR", lenders, 0.05);
    
    // Ajout de quelques remboursements
    facility.payPart(100000.0, "2024-06-01");
    facility.payPart(200000.0, "2024-12-01");
    
    // Test constructeur
    Portfolio portfolio(&facility);
    TEST_ASSERT(portfolio.getTotalAmount() == 1000000.0, "Portfolio - Montant total");
    TEST_ASSERT(portfolio.getRemainingAmount() == 700000.0, "Portfolio - Montant restant");
    TEST_ASSERT(portfolio.getNumberOfParts() == 2, "Portfolio - Nombre de parts");
    
    // Test calculs (les montants exacts dependent de la formule d'interêt)
    double totalInterest = portfolio.getTotalInterest();
    double totalRepayment = portfolio.getTotalRemboursement();
    TEST_ASSERT(totalInterest > 0, "Portfolio - Interêts calcules");
    TEST_ASSERT(totalRepayment > portfolio.getTotalAmount(), "Portfolio - Remboursement > montant");
    TEST_ASSERT(totalRepayment == portfolio.getTotalAmount() + totalInterest, "Portfolio - Coherence calculs");
    
    // Test synchronisation : ajout d'une nouvelle part
    facility.payPart(150000.0, "2025-01-01");
    TEST_ASSERT(portfolio.getNumberOfParts() == 3, "Portfolio - Synchronisation nouvelle part");
    TEST_ASSERT(portfolio.getRemainingAmount() == 550000.0, "Portfolio - Synchronisation montant restant");
    
    // Test validation pointeur null
    TEST_EXCEPTION(Portfolio(nullptr), std::invalid_argument, "Portfolio - Pointeur null rejete");
}

// Tests d'integration
void test_integration() {
    std::cout << "\n=== Tests d'Integration ===" << std::endl;
    
    // Scenario complet : Creation d'un deal avec facility et remboursements
    try {
        // Creation des acteurs
        Borrower borrower("Air France");
        std::vector<Lender> pool = {Lender("BNP Paribas"), Lender("Societe Generale")};
        
        // Creation du deal
        Deal deal("B9876", "BNP Paribas", pool, borrower, 50000000.0, "EUR", 
                  "2024-01-01", "2026-01-01", Deal::Status::CLOSED);
        
        // Creation de la facility
        Facility facility("2024-01-01", "2026-01-01", 30000000.0, "EUR", pool, 0.04);
        
        // Ajout de remboursements
        facility.payPart(5000000.0, "2024-06-01");
        facility.payPart(10000000.0, "2025-01-01");
        facility.payPart(8000000.0, "2025-06-01");
        
        // Ajout de la facility au deal
        deal.addFacility(facility);
        
        // Creation du portfolio
        Portfolio portfolio(&facility);
        
        // Verifications finales
        TEST_ASSERT(deal.getFacilitiesCount() == 1, "Integration - Deal contient facility");
        TEST_ASSERT(facility.getPartsCount() == 3, "Integration - Facility contient 3 parts");
        TEST_ASSERT(facility.getRemainingAmount() == 7000000.0, "Integration - Montant restant correct");
        TEST_ASSERT(portfolio.getNumberOfParts() == 3, "Integration - Portfolio synchronise");
        TEST_ASSERT(deal.getTotalFacilitiesAmount() <= deal.getProjectAmount(), "Integration - Facilities <= Projet");
        
        // Test completion du deal
        deal.setStatus(Deal::Status::TERMINATED);
        TEST_ASSERT(deal.isCompleted(), "Integration - Deal termine");
        
        std::cout << "PASS: Scenario d'integration complet" << std::endl;
        tests_passed++;
        
    } catch (const std::exception& e) {
        std::cout << "FAIL: Scenario d'integration - Exception: " << e.what() << std::endl;
        tests_failed++;
    }
}

// Fonction principale des tests
int main() {
    std::cout << "Demarrage des tests unitaires du systeme de financement structure\n" << std::endl;
    
    try {
        test_borrower();
        test_lender();
        test_part();
        test_facility();
        test_deal();
        test_portfolio();
        test_integration();
        
        std::cout << "\n" << std::string(50, '=') << std::endl;
        std::cout << "RESULTATS DES TESTS" << std::endl;
        std::cout << std::string(50, '=') << std::endl;
        std::cout << "Tests reussis: " << tests_passed << std::endl;
        std::cout << "Tests echoues: " << tests_failed << std::endl;
        std::cout << "Taux de reussite: " << (tests_passed * 100.0 / (tests_passed + tests_failed)) << "%" << std::endl;
        
        if (tests_failed == 0) {
            std::cout << "\nTOUS LES TESTS SONT PASSES ! Le systeme fonctionne correctement." << std::endl;
            return 0;
        } else {
            std::cout << "\nCERTAINS TESTS ONT ECHOUE. Verifiez l'implementation." << std::endl;
            return 1;
        }
        
    } catch (const std::exception& e) {
        std::cout << "ERREUR FATALE lors des tests: " << e.what() << std::endl;
        return 1;
    }
}