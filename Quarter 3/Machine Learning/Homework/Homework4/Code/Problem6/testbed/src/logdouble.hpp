#pragma once
#include <cmath>
#include <iostream>

class logdouble {
public:
    logdouble() : _val(0.0) {}
    logdouble(double val) : _val(val) {}

    // Copy constructor
    logdouble(const logdouble& other) : _val(other._val) {}

    // Move constructor
    logdouble(logdouble&& other) noexcept : _val(std::move(other._val)) {}

    // Assignment operator
    logdouble& operator=(const logdouble& other) {
        if (this != &other) {
            _val = other._val;
        }
        return *this;
    }

    // Move assignment operator
    logdouble& operator=(logdouble&& other) noexcept {
        if (this != &other) {
            _val = std::move(other._val);
        }
        return *this;
    }

    // Multiplication operator
    logdouble operator*(const logdouble& other) const {
        return logdouble(_val + other._val);
    }

    // Addition operator
    logdouble operator+(const logdouble& other) const {
        double logx = std::log(other._val);
        double logy = std::log(_val);

        if (_val <= other._val) {
            return logdouble(logx + std::log(1.0 + std::exp(logy - logx)));
        }
        return logdouble(logy + std::log(1.0 + std::exp(logx - logy)));
    }

    // Greater than operator
    bool operator>(const logdouble& other) const {
        return _val > other._val;
    }

    // Greater than operator
    bool operator<(const logdouble& other) const {
        return _val < other._val;
    }

    // Insertion operator
    friend std::ostream& operator<<(std::ostream& os, const logdouble& ld) {
        os << ld._val;
        return os;
    }

    double getVal() const { return _val; }

private:
    double _val;
};
