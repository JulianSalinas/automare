#include "scanner.h"
#define equals(x,y) (strcmp(x,y) == 0 ? 1:0)

/**
 * @brief addPointBetweenConcats
 * recibe la ER en forma de lista en formato infijo
 * se encarga de agregar puntos en lugares donde hay concatenacion implicita
 */

void addPointBetweenConcats(List * list){

    if(list->size > 1){

        Node * currentElement = list->end;
        int index = list->size - 2;

        while(currentElement != NULL && currentElement->prev != NULL){
            char * currentChar = currentElement->content;
            char * prevChar = currentElement->prev->content;

            if( (!IsOperator(currentChar) && !IsOperator(prevChar)) ||
                (equals(prevChar, SQuestion) && !IsOperator(currentChar)) ){

                if( !equals(prevChar, SLeft) && !equals(currentChar, SRight) ){
                    List_InsertAfter(list, index, ".");
                    currentElement = currentElement->prev->prev;
                } else {
                    currentElement = currentElement->prev;
                }

            } else {

                if(equals(prevChar, SQuestion) && IsOperatorOrBracket(prevChar)){
                    List_InsertAfter(list, index, ".");
                    currentElement = currentElement->prev->prev;
                }
                else currentElement = currentElement->prev;
            }

            index = index - 1;

        }
    }
}

/**
 * @brief validateExpression, verifica que la expresion cumpla las restricciones de una ER,
 * recibe la ER en forma de lista en formato infijo
 * Y agrega algunos cambios de formato en caso de ser necesarios y aceptables.
 * Retorna 1 si es valida o 0 de lo contrario.
 */

int validateExpression(List * list){

    // Se encarga de agregar un punto entre cada caracter no operador consecutivo.
    addPointBetweenConcats(list);

    int parenthesisCount = 0; // aumenta en 1 si es parentesis para abrir y en -1 si es para cerrar

    Node * currentElement = list->start;

    if(currentElement != NULL) {
    if(IsOperator(currentElement->content) && !equals(currentElement->content, SLeft) ) return 0; }

    while(currentElement != NULL){

        char * currentChar = currentElement->content;

        if( IsUnaryOperator(currentChar) ){
            char * prevChar = currentElement->prev->content;
            if(IsOperator(prevChar) || equals(prevChar, "(") ) return 0;
        }

        if( IsBinaryOperator(currentChar) ){
            char * prevChar = currentElement->prev->content;
            char * nextChar = currentElement->next->content;
            if(equals(prevChar, "(") || IsOperator(nextChar) || equals(nextChar,")") ) return 0;
        }

        if(equals(currentChar, SLeft) ){
            parenthesisCount++;
        }

        if(equals(currentChar, SRight) ){
            parenthesisCount--;
        }

        if(parenthesisCount<0){ return 0; }

        currentElement = currentElement->next;
    }

    if (parenthesisCount != 0){ return 0; }

    return 1;
}
