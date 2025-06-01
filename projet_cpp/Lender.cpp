#include "Lender.h"

Lender::Lender(const std::string& name) : name(name) {
}

const std::string& Lender::getName() const {
    return name;
}