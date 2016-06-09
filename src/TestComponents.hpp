#ifndef TESTCOMPONENTS_HPP
#define TESTCOMPONENTS_HPP


#include "ComponentBase.hpp"


class TCA : public ComponentBase {
public:
    friend class TVA;

    TCA(int a, int b) : a_(a), b_(b) {}

private:
    int a_, b_;
};


class TCB : public ComponentBase {
public:
    friend class TVB;

    TCB(float a, double b) : a_(a), b_(b) {}

private:
    float a_;
    double b_;
};


#endif // TESTCOMPONENTS_HPP
