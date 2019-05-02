#ifndef ALGORITHMS_H
#define ALGORITHMS_H
#include "main.h"
#include "tree.h"
#include "list.h"
#include "dfa.h"
#include "table.h"

int IsUnaryOperator(char * str);
int IsBinaryOperator(char * str);
int IsOperator(char * str);
int IsOperatorOrBracket(char * str);
int PrecedenceOf(char *);

List * ShuntingYard(List *);
Tree * BuildTree(List * expression);
DFA * BuildDFA(Tree * tree, List * sigma);

#endif // ALGORITHMS_H
