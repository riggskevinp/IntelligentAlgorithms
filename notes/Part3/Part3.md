# Fuzzy Logic

The general goal with Fuzzy Logic is to enable statements to be partially true and then apply those statements to rules that then may only be partially satisfied.

## **Propositional logic** combines propositions with connectives to form sentences.
* Connectives: not,and,or,implies,equivalence
* Pattern is {proposition} {connective} {proposition} {connective} {proposition} ... {conn} {prop}
* Ex: P ^ Q -> R; P and Q implies R; If P AND Q Then R;

## **Modus Ponens** "Affirm by affirming"
* Infer the truth of a proposition from the truth of another proposition through the implication rule A -> B

Ex:
* premise 1 (fact): x is A
* premise 2 (rule, implication rule): if x is A then y is B
* consequence (conclusion): y is B

## **Generalized Modus Ponens** is applying Modus Ponens but in an approximate way.

B' is an approximation of B. A' is an approximation of A

Ex:
* premise 1 (fact): x is A'
* premise 2 (rule, implication): if x is A then y is B
* consequence (conclusion): y is B'

## **Generalized Modus Tolens** is the generalization of Modus Tolens "deny by denying"

Ex:
* premise 1 (fact): y is $\overline{B}$'
* premise 2 (rule, implication): if x is A then y is B
* consequence (conclusion): x is $\overline{A}$'

Note, GMT and GMP are not always true in classical logic but can be used approximately for real applications

## **Generalized Hypothetical Syllogism** is the generalization of Hypothetical Syllogism which is a form of argument where a conditional is created on one or more of its premises: A->B, B->C, gives A->C

Generalization comes in the form A->B and B'->C then we infer fuzzy proposition A->C'


# Fuzzy Rules (If-Then)

Fuzzy System Context:
* Fuzzy Sets: information, expertise, data
* Fuzzy Rules: decision making; articulation of ideas and concepts


A **Fuzzy Rule** is a basic element for capturing knowledge in Fuzzy Systems.

A **Fuzzy If-Then Rule** takes the form IF {antecedent or premise} THEN {consequent or conclusion}

A **antecedent** describes a condition and is comprised of one or more fuzzy sets

A **consequent** describes a conclusion that is drawn when a premise holds and can be crisp or fuzzy

##  Conventional vs Fuzzy Antecedent
* In a conventional rule, the antecedent describes a crisp condition (it is greater than 90 degrees)
* In a fuzzy rule, the antecedent is fuzzy (it is hot)

## Convention vs Fuzzy Consequent

|Consequent|Rule Structure|notes|
|---|---|---|
|Crisp|IF {ante} THEN y = a|a is crisp, numeric or symbolic, computationally more efficient|
|Fuzzy|IF {ante} THEN y = A|A is a fuzzy set, more suitable for capturing imprecise expertise|
|Functional|IF $x_1 is A_1$ AND  $\dots x_n is A_n$ THEN y = $a_0 + \sum_{i=1}^{n}a_ix_i$|$a_i$ are crisp, can be used for the approximation of nonlinear models|

Fuzzy Sets are interpreted differently when used in an antecedent vs a consequent
* In an antecedent, define an inexact region of input space covered
* In a consequent, describes the vagueness of a conclusion
* Fuzzy input variables should cover then entire input space but fuzzy output variables do not need to cover the entire output space

Axiom: a fuzzy inference system contains a set of fuzzy rules that converts inputs to outputs, or, is a functional mapping from an input space to an output space.

The **Fuzzy Approximation Theorem** is that a finite number of fuzzy rules can cover a curve.

An **N-ary Fuzzy Relation** is a fuzzy set defined a Cartesian product space where the tuples of the fuzzy relation are a subset of teh product space
* $R = \{((x_1,x_2,\dots,x_n),\mu_R(x_1,x_2,\dots,x_n))\mid (x_1,x_2,\dots,x_n)\subset X_1 \times X_2 \times \dots \times X_n\}$
* A fuzzy relation is a membership function of a multidimensional fuzzy set which can be thought of as a fuzzy mapping from an input to output space


# Fuzzy Implication
* A coupled with B is a mapping rule
* A entails B is an implication rule

## A coupled with B implication examples
* Employs the Triangular norm: $R=A \text{ -> }B = A \cap B = \int_{X \times Y}\mu_R(x,y)/(x,y)$ where $\mu_R(x,y) = T[\mu_A(x),\mu_B(y)]$

|T-Norm Name|T-Norm Operator|
|---|---|
|Min (Mamdani)|$\mu_{R_m}(x,y) = \mu_A(x)$^$\mu_B(y)$|
|Algebraic Product|$\mu_{R_p}(x,y) = \mu_A(x) * \mu_B(y)$|
|Bounded Product|$\mu_{R_{bp}}(x,y) = 0$ v $[\mu_A(x) + \mu_B(y) - 1]$|
|Drastic Product|$\mu_{R_{dp}}(x,y) \mu_A(x) * \mu_B(y)$; a*b == {a if b = 1;b if a = 1; 0 otherwise}|


## A entails B implication examples
* $R = A -> B = \int_{X \times Y}\mu_R(x,y)/(x,y)$
* Utilizes both T and S norms

### Example Implication Relations (Tautologies)
* Material Implication $\overline{A} \cup B$: $\mu_R(x,y) = S[[1-\mu_A(x)], \mu_B(y)]$
* Propositional Calculus $\overline{A} \cup (A \cap B)$: $\mu_R(x,y) = S[[1 - \mu_A(x)],T[\mu_A(x),\mu_B(y)]]$
* Generalization of Modus Ponens $A \times sup(c), c \subseteq B$: $\mu_R(x,y) = sup\{c \mid [\mu_A(x),c] \le \mu_B(y), 0 \le c \le 1\}$

### Example T/S-norm Implementations
|||
|---|---|
|Zadeh's Arithmetic rule: bounded sum|$\mu_{R_a} = 1$ ^ $[1 - \mu_A(x) + \mu_B(y)]$|
|Boolean Fuzzy Implication|$\mu_{R_s}(x,y) = [1 - \mu_A(x)]$ v $\mu_B(y)$|
|Zadeh's max-min rule|$\mu_{R_{mm}} = [1 - \mu_A(x)]$ v $[\mu_A(x)$ ^ $\mu_B(y)]$|
|Goguen's Fuzzy Implication|$\mu_{R_{\Delta}} = [\mu_A(x) <_?  \mu_B(y)]; a <_? b = 1 \text{ if } a \le b, b/a \text{ if } a > b$|

[Part4](../Part4/Part4.md)