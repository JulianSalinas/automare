#ifndef STRINGGENERATOR_H
#define STRINGGENERATOR_H
#include "main.h"
#include "list.h"

List * generateRegexLanguage( List * postfixRegex, int maxRepetitions);

void OrOperation(LList *);
void ConcatOperation(LList *);
void KleeneOperation(LList *, int);
void PlusOperation(LList *, int);
void QuestionOperation(LList *);

#endif // STRINGGENERATOR_H
