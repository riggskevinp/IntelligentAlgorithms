#pragma once
#include <algorithm>
#include <iostream>
#include <memory>
#include <utility>
#include <numeric>
#include <vector>

template <typename T>
struct fail : std::false_type
        {
        };

class MembershipFunction{
public:
    virtual double getValue(double _x) = 0;
    double operator()(double _x) {return getValue(_x);}
    virtual double center() = 0;
    virtual double area() {return 0;};
    virtual ~MembershipFunction() = default;
};

class TriangleMF: public MembershipFunction{
public:
    TriangleMF(double _a, double _b, double _c): a(_a),b(_b),c(_c){}
    double getValue(double _x) override {return std::max(std::min((_x-a)/(b-a), (c-_x)/(c-b)),0.0);}
    double center() override {return (a+c)/2;}
    double area() override {return (c-a)/2;}

private:
    double a;
    double b;
    double c;
};

class TrapezoidalMF: public MembershipFunction{
public:
    TrapezoidalMF(double _a, double _b, double _c, double _d): a(_a),b(_b),c(_c),d(_d){}
    double getValue(double _x) override {return std::max(std::min({(_x-a)/(b-a), 1.0, (d-_x)/(d-c)}),0.0);}
    double center() override {return (a+d)/2;}
    double area() override {return (a+d + b+c)/2;}


private:
    double a;
    double b;
    double c;
    double d;
};

class GaussianMF: public MembershipFunction{
public:
    GaussianMF(double _c, double _sigma): c(_c),sigma(_sigma){}
    double getValue(double _x) override {return exp(-.5*pow((_x-c)/sigma,2));}
    double center() override {return c;}
    template<typename T = bool>
    double area() {static_assert (fail<T>::value, "Not implemented");}

private:
    double c;
    double sigma;
};

class GeneralizedBellMF: public MembershipFunction{
public:
    GeneralizedBellMF(double _a, double _b, double _c): a(_a),b(_b),c(_c){}
    double getValue(double _x) override {return pow(1+pow(abs((_x-c)/a),2*b),-1);}
    double center() override {return c;}
    template<typename T = bool>
    double area() {static_assert (fail<T>::value, "Not implemented");}

private:
    double a;
    double b;
    double c;
};

class SigmoidalMF: public MembershipFunction{
public:
    SigmoidalMF(double _a, double _c): a(_a),c(_c){}
    double getValue(double _x) override {return 1 / (1+exp(-a*(_x-c)));}
    double center() override {return c;}
    template<typename T = bool>
    double area() {static_assert (fail<T>::value, "Not implemented");}

private:
    double a;
    double c;
};


class FuzzySet{
public:
    explicit FuzzySet(MembershipFunction* _mf):mf(_mf){}
    double operator()(double _x) {return mf->getValue(_x);}
    double center() {return mf->center();}
    double area() {return mf->area();}
    void fuzzify(double crispInput){fuzzValue = mf->getValue(crispInput);}
    double getFuzzyValue(){return fuzzValue;}

private:
    std::shared_ptr<MembershipFunction> mf;
    double fuzzValue{0.0};
};


class FuzzyRule{
public:
    explicit FuzzyRule(std::vector<FuzzySet*> _in, FuzzySet _out): in(std::move(_in)),out(std::move(_out)){}
    std::vector<double> getWeights(){
        std::vector<double> _weights;
        _weights.reserve(in.size());
        for(auto set : in){
            _weights.push_back(set->getFuzzyValue());
        }
        return _weights;
    }
    double getCenter(){return out.center();}
    double getArea(){return out.area();}
private:
    std::vector<FuzzySet*> in;
    FuzzySet out;
};

class Fuzzifier{
public:
    void addValue(FuzzySet* _in){inputs.push_back(_in);}
    void fuzz(double crispInput){
        for(auto input : inputs){
            input->fuzzify(crispInput);
        }
    }
private:
    std::vector<FuzzySet*> inputs;
};

class SimplifiedIEwCA{
public:
    explicit SimplifiedIEwCA(std::vector<FuzzyRule> _rules, std::vector<Fuzzifier> _fuzzer):rules(std::move(_rules)), inputFuzzers(std::move(_fuzzer)){}

protected:
    void fuzzify(const std::vector<double>& crispInputs){
        for(int i = 0; i < inputFuzzers.size(); i++){
            inputFuzzers.at(i).fuzz(crispInputs.at(i));
        }
    }
    std::vector<FuzzyRule> rules;
    std::vector<Fuzzifier> inputFuzzers;
};


class SimplifiedPIECA: public SimplifiedIEwCA{
public:
    explicit SimplifiedPIECA(std::vector<FuzzyRule> _rules, std::vector<Fuzzifier> _fuzzer):SimplifiedIEwCA(std::move(_rules),std::move(_fuzzer)){}

    double fuzzDefuzz(const std::vector<double>& crispInputs){
        fuzzify(crispInputs);
        double num = 0;
        double den = 0;
        for(auto rule : rules){
            auto weights = rule.getWeights();
            double weight = std::accumulate(weights.begin(), weights.end(), 1.0, std::multiplies<>());
            den += weight;
            num += weight*rule.getCenter();
        }
        if(den == 0){
            return 0;
        }
        return num/den;
    }
};

class SimplifiedMIECA: public SimplifiedIEwCA{
public:
    explicit SimplifiedMIECA(std::vector<FuzzyRule> _rules, std::vector<Fuzzifier> _fuzzer):SimplifiedIEwCA(std::move(_rules),std::move(_fuzzer)){}

    double fuzzDefuzz(const std::vector<double>& crispInputs){
        fuzzify(crispInputs);
        double num = 0;
        double den = 0;
        for(auto rule : rules){
            auto weights = rule.getWeights();
            double weight = *(std::max_element(weights.begin(),weights.end()));
            den += weight;
            num += weight*rule.getCenter();
        }
        return num/den;
    }
};

class SumProdIECoA: public SimplifiedIEwCA{
public:
    explicit SumProdIECoA(std::vector<FuzzyRule> _rules, std::vector<Fuzzifier> _fuzzer):SimplifiedIEwCA(std::move(_rules),std::move(_fuzzer)){}

    double fuzzDefuzz(const std::vector<double>& crispInputs){
        fuzzify(crispInputs);
        double num = 0;
        double den = 0;
        for(auto rule : rules){
            auto weights = rule.getWeights();
            double weight = std::accumulate(weights.begin(), weights.end(), 1.0, std::multiplies<>());
            den += weight*rule.getArea();
            num += weight*rule.getArea()*rule.getCenter();
        }
        return num/den;
    }
};