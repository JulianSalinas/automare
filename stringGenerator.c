#include "stringGenerator.h"

#define equals(x,y) (strcmp(x,y) == 0 ? 1:0)

List * generateRegexLanguage(List * postfixRegex, int maxRepetitions){

    LList * stack = LList_New();

    while (postfixRegex->start != NULL){

        char * nextRegexElmnt = List_Pop(postfixRegex);

        //Si lo que sigue en la expresión no es operador, se agrega a la pila.
        if(!IsOperator(nextRegexElmnt)){
            LList_Push(stack, Str2List(nextRegexElmnt));
        }
        //Si sigue una operación que ocupa dos operandos (or, concat) se sacan
        //los dos últimos operandos de la pila, se realiza la operación y se introduce el resultado en la pila.

        else if (equals(nextRegexElmnt, SOr)){ OrOperation(stack); }

        else if (equals(nextRegexElmnt, SConcat)){ ConcatOperation(stack); }

        //Si es una función que ocupa un solo operando (kleene, plus) entonces se saca un operando de la pila,
        //se evalúa y se guarda el resultado en la pila.
        else if (equals(nextRegexElmnt, SQuestion)) { QuestionOperation(stack); }

        else if (equals(nextRegexElmnt, SKleene)){ KleeneOperation(stack, maxRepetitions); }

        else if (equals(nextRegexElmnt, SPlus)){ PlusOperation(stack, maxRepetitions); }

    }

    if(stack->start != NULL){        
        return List_Union(List_New(), stack->start->content);
    }
    else {
        return List_New();
    }

}

void OrOperation(LList * stack){

    List * pop2 = LList_Pop(stack);
    List * pop1 = LList_Pop(stack);
    LList_Push(stack, List_Union(pop1, pop2));
}

void QuestionOperation(LList * stack){

    LList_Push(stack, Str2List(" "));
    OrOperation(stack);
}

void ConcatOperation(LList * stack){

    List * pop2 = LList_Pop(stack);
    List * pop1 = LList_Pop(stack);
    LList_Push(stack, List_Multiply(pop1, pop2));
}

void KleeneOperation(LList * stack, int maxRepetitions){

    List * pop1 = LList_Pop(stack);
    List * tempStrings = List_Concat(List_New(), pop1);
    List * result = List_Concat(List_New(), pop1);

    int count = 1;
    while ( count < maxRepetitions )   {

        result = List_Combination(tempStrings, pop1);
        tempStrings = List_Concat(List_New(), result);

        count = count + 1;
    }

    LList_Push(stack, List_Concat(Str2List(" "), result) );
}

void PlusOperation(LList * stack, int maxRepetitions){

    List * pop1 = LList_Pop(stack);
    List * tempStrings = List_Concat(List_New(), pop1);
    List * result = List_Concat(List_New(), pop1);

    int count = 1;
    while ( count < maxRepetitions )   {

        result = List_Combination(tempStrings, pop1);
        tempStrings = List_Concat(List_New(), result);

        count = count + 1;
    }

    LList_Push(stack, result);
}




