# Système de Financement Structuré

**Projet C++ - Master 2 MIAGE IF Apprentissage 2024/2025**

Un système de gestion de financement structuré bancaire développé en C++17, modélisant les relations entre banques prêteuses, emprunteurs, et tranches de financement.

---

## Description

Ce projet implémente un système de financement structuré utilisé dans le secteur bancaire. Lorsqu'une institution financière ne peut assumer seule le risque d'un projet de grande envergure, elle fait appel à un consortium de banques pour partager ce financement.

Le système se compose de six classes principales :
- **Deal** : Contrat principal entre l'emprunteur et le consortium bancaire
- **Borrower** : L'emprunteur (compagnie aérienne, entreprise)
- **Lender** : Les banques prêteuses 
- **Facility** : Les tranches de financement avec leurs spécificités
- **Part** : Les remboursements individuels effectués par l'emprunteur
- **Portfolio** : Vue consolidée des calculs financiers et intérêts

---

## Fonctionnalités

### Gestion des données
- Validation stricte de tous les paramètres d'entrée
- Encapsulation complète avec attributs privés
- Gestion d'exceptions pour les cas d'erreur
- Format de contrat standardisé (S/Z/B + 4 chiffres)

### Calculs financiers
- Intérêts composés avec gestion temporelle précise
- Suivi en temps réel des montants restants
- Consolidation automatique via Portfolio
- Support multi-devise

### Tests et qualité
- 62 tests unitaires couvrant tous les composants
- Tests d'intégration pour les scénarios complets
- Interface utilisateur avec menu interactif
- Compilation sans warnings

---

## Exemples d'utilisation

### Création d'un contrat de financement
```cpp
// Création des acteurs
Borrower borrower("Air France");
std::vector<Lender> pool = {Lender("BNP Paribas"), Lender("Société Générale")};

// Création du contrat principal
Deal deal("S1234", "BNP Paribas", pool, borrower, 
          50000000.0, "EUR", "2024-01-01", "2026-01-01", Deal::Status::CLOSED);

// Ajout d'une tranche de financement
Facility facility("2024-01-01", "2026-01-01", 30000000.0, "EUR", pool, 0.035);
deal.addFacility(facility);
```

### Gestion des remboursements
```cpp
// Remboursements échelonnés
facility.payPart(5000000.0, "2024-06-01");
facility.payPart(10000000.0, "2025-01-01");

// Calculs automatiques via portfolio
Portfolio portfolio(&facility);
double totalRepayment = portfolio.getTotalRemboursement();
```

### Interface utilisateur
```
Structured Financing Application
1. Create a new deal
2. Create a new borrower
3. Pay a part of a facility
4. Manage portfolio
0. Exit
```

---

## Installation

### Prérequis
- Compilateur C++17 (g++, clang++, ou MSVC)
- Système : Windows, Linux, ou macOS

### Compilation rapide
```bash
# Windows
g++ -std=c++17 -Wall -O2 -o projet_cpp.exe main.cpp Borrower.cpp Lender.cpp Part.cpp Facility.cpp Deal.cpp Portfolio.cpp

# Tests
g++ -std=c++17 -Wall -g -o test_suite.exe test_suite.cpp Borrower.cpp Lender.cpp Part.cpp Facility.cpp Deal.cpp Portfolio.cpp
```

### Avec CMake
```bash
mkdir build && cd build
cmake ..
cmake --build .
```

### Structure du projet
```
projet_cpp/
├── main.cpp                    # Programme principal
├── test_suite.cpp              # Tests unitaires
├── Borrower.h/.cpp             # Classe Emprunteur
├── Lender.h/.cpp               # Classe Prêteur
├── Part.h/.cpp                 # Classe Remboursement
├── Facility.h/.cpp             # Classe Tranche
├── Deal.h/.cpp                 # Classe Contrat
├── Portfolio.h/.cpp            # Classe Portfolio
└── CMakeLists.txt              # Configuration build
```

### Vérification
```bash
./test_suite.exe    # Tous les tests doivent passer (100%)
./projet_cpp.exe    # Lancement de l'application
```

## Auteurs

- DOUBABI Mustapha
- HADDAD Chirine
