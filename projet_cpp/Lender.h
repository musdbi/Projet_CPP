#ifndef LENDER_H
#define LENDER_H

#include <string>

class Lender {
public:
    explicit Lender(const std::string& name);
    
    const std::string& getName() const;

private:
    std::string name;
};

#endif