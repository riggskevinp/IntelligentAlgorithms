#pragma once
#include <algorithm>
#include <iostream>
#include <memory>

class MembershipFunction{
public:
    virtual double operator()(double _x) = 0;
    virtual ~MembershipFunction() = default;
};

class TriangleMF: public MembershipFunction{
public:
    TriangleMF(double _a, double _b, double _c): a(_a),b(_b),c(_c){}
    double operator()(double _x) override {return std::max(std::min((_x-a)/(b-a), (c-_x)/(c-b)),0.0);}

private:
    double a;
    double b;
    double c;
};

class TrapezoidalMF: public MembershipFunction{
public:
    TrapezoidalMF(double _a, double _b, double _c, double _d): a(_a),b(_b),c(_c),d(_d){}
    double operator()(double _x) override {return std::max(std::min({(_x-a)/(b-a), 1.0, (d-_x)/(d-c)}),0.0);}

private:
    double a;
    double b;
    double c;
    double d;
};

class GaussianMF: public MembershipFunction{
public:
    GaussianMF(double _c, double _sigma): c(_c),sigma(_sigma){}
    double operator()(double _x) override {return exp(-.5*pow((_x-c)/sigma,2));}

private:
    double c;
    double sigma;
};

class GeneralizedBellMF: public MembershipFunction{
public:
    GeneralizedBellMF(double _a, double _b, double _c): a(_a),b(_b),c(_c){}
    double operator()(double _x) override {return pow(1+pow(abs((_x-c)/a),2*b),-1);}

private:
    double a;
    double b;
    double c;
};

class SigmoidalMF: public MembershipFunction{
public:
    SigmoidalMF(double _a, double _c): a(_a),c(_c){}
    double operator()(double _x) override {return 1 / (1+exp(-a*(_x-c)));}

private:
    double a;
    double c;
};


class FuzzySet{
public:
    FuzzySet(MembershipFunction* _mf):mf(_mf){}

private:
    std::shared_ptr<MembershipFunction> mf;
};