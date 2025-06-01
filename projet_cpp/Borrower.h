#ifndef BORROWER_H
#define BORROWER_H

#include <string>

class Borrower {
public:
    Borrower();
    explicit Borrower(const std::string& name);
    
    const std::string& getName() const;
    void setName(const std::string& name);

private:
    std::string name;
    
    void validateName(const std::string& name) const;
};

#endif