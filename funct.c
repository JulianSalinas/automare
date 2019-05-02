#include "funct.h"

/**
 * @brief IsOperator, usadas en el archivo de algoritmos
 */

#define equals(x,y) (strcmp(x,y) == 0 ? 1:0)

int IsUnaryOperator(char * str){

    return  equals(str, SKleene) ||
            equals(str, SPlus) ||
            equals(str, SQuestion);

}

int IsBinaryOperator(char * str){

    return  equals(str, SOr) ||
            equals(str, SConcat);

}

int IsOperator(char * str){

    return  IsBinaryOperator(str) ||
            IsUnaryOperator(str);
}

int IsOperatorOrBracket(char *str){

    return  IsBinaryOperator(str) ||
            IsUnaryOperator(str) ||
            equals(str, SLeft) ||
            equals(str, SRight);
}

/**
 * @brief PrecedenceOf, usada para determinar el nivel de importancia
 * del tope de la pila con respecto a la cadena leída
 */

int PrecedenceOf(char * str){

    if(equals(str, SLeft)) return 1;
    if(equals(str, SOr)) return 2;
    if(equals(str, SConcat)) return 3;
    if(equals(str, SQuestion)) return 4;
    if(equals(str, SKleene)) return 4;
    if(equals(str, SPlus)) return 4;
    return 6;

}

/**
 * @brief Definiciones para imprimir los distintos errores que pueden ocurrir
 * durante la ejecución de alguno de los algoritmos.
 */

#define error_1             (printf("\tSe ha encontrado un error en la posición %d de la expresión\n\t", pos));
#define error_2             (printf("\tSe ha producido un error al convertir la expresión infija a postfija\n\t"))
#define error_3             (printf("\tLa cantidad de elementos en la pila es incorrecta\n\t"))

/**
 * @brief Algoritmo Shunting-Yard para traducir ER a notación post-fija
 * Fuente de apoyo https://gist.github.com/gmenard/6161825
 * Los "define" son para hacer la funcion más entendible
 */

#define equals(x,y)         (strcmp(x,y) == 0 ? 1:0)
#define pop                 (List_Pop(stack))
#define push                (List_Push(stack, str))
#define peek                (List_Peek(stack))
#define empty               (stack->size <= 0)
#define isTopPriority       (PrecedenceOf(peek) >= PrecedenceOf(str) ? 1:0)


void ShuntingYardProcessStrCaseSRight(int pos, List * exp, List * stack){

    while(!equals(peek, SLeft) && !empty) List_Add(exp,pop);
    if(equals(peek,SLeft)) pop;
    else{ error_1; exit(1); }

}


void ShuntingYardProcessStr(char * str, Node * pointer, List * exp, List * stack){

    int pos = 0;
    while(pointer != NULL){
        str = pointer->content;
        if(equals(str,SLeft)) push;
        else if(equals(str,SRight)) ShuntingYardProcessStrCaseSRight(pos,exp,stack);
        else { while(!empty && isTopPriority) List_Add(exp,pop); push; }
        pointer = pointer->next; pos++;
    }

}

List * ShuntingYard(List * regex){

    List * exp = List_New();
    List * stack = List_New();
    Node * pointer = regex->start;
    char * str = pointer->content;
    ShuntingYardProcessStr(str, pointer, exp, stack);
    while(!empty) List_Add(exp,pop);
    return exp;

}

/**
 * @brief Algoritmo para traducir de notación postfija a un árbol
 * binario. El algoritmo deja en NULL a los hijos derechos cuando
 * el operador es unario.
 */

void BuildTreeCaseUnaryOp(Tree * subtree, TreeList * stack){

    if(stack->size < 1) { error_3; exit(1); }
    subtree->leftChild = TreeList_Pop(stack);

}

void BuildTreeCaseBinaryOp(Tree *subtree, TreeList * stack){

    if(stack->size < 2) { error_3; exit(1); }
    subtree->rightChild = TreeList_Pop(stack);
    subtree->leftChild = TreeList_Pop(stack);

}

void BuildTreeCaseOp(char * content, TreeList * stack){

    Tree * subtree = Tree_New(content);
    if(IsUnaryOperator(content)) BuildTreeCaseUnaryOp(subtree, stack);
    else if(IsBinaryOperator(content)) BuildTreeCaseBinaryOp(subtree, stack);
    TreeList_Push(stack, subtree);

}

void BuildTreeProccessExp(TreeList * treeList, TreeList * stack){

    TreeListNode * pointer = treeList->start;
    while(pointer != NULL){
        char * content = pointer->content->content;
        if(equals(content, SLeft)) { error_2; exit(1); }
        if(!IsOperator(content)) TreeList_Push(stack, pointer->content);
        else{ BuildTreeCaseOp(content, stack); }
        pointer = pointer->next;
    }

}


Tree * BuildTree(List * expression){

    TreeList * treeList = List2TreeList(expression);
    TreeList * stack = TreeList_New();
    BuildTreeProccessExp(treeList, stack);
    if(stack->size != 1) { error_3; exit(1); }
    else {
        Tree * tree = TreeList_Pop(stack);
        Tree_Pos(tree);
        Tree_Nullable(tree);
        Tree_FirstPos(tree);
        Tree_LastPos(tree);
        return tree;
    }

}

/**
 * @brief FollowPos, Algoritmo para construir la tabla followPos para poder
 * obtener luego los estados transiciones del autómata.
 */

void FollowPosCaseConcatAddToTable(Table * table, TreeListNode * node, INode * i){

    INode * j = node->content->rightChild->firstPos->start;
    while(j != NULL){
        Table_Add(table,i->content,j->content);
        j = j->next;
    }

}

void FollowPosCaseConcat(Table * table, TreeListNode * node){

    INode * i = node->content->leftChild->lastPos->start;
    while(i != NULL){
        FollowPosCaseConcatAddToTable(table, node, i);
        i = i->next;
    }

}

void FollowPosCaseUnaryOpAddTable(Table * table, TreeListNode * node, INode * i){

    INode * j = node->content->firstPos->start;
    while(j != NULL){
        Table_Add(table,i->content,j->content);
        j = j->next;
    }

}

void FollowPosSubcaseUnaryOp(Table * table, TreeListNode * node){

    INode * i = node->content->lastPos->start;
    while(i != NULL){
        FollowPosCaseUnaryOpAddTable(table, node, i);
        i = i->next;
    }

}

void FollowPosCaseUnaryOp(Table * table, TreeListNode * node, int positions){

    for(int k = 0; k < positions; k++){
        FollowPosSubcaseUnaryOp(table, node);
    }

}

void FollowPosExecuteCases(Table *table, TreeListNode * node, int positions){

    while(node != NULL){
        if(equals(node->content->content, SConcat))
            FollowPosCaseConcat(table, node);
        else if(IsUnaryOperator(node->content->content))
            FollowPosCaseUnaryOp(table, node, positions);
        node = node->next;
    }

}

Table * FollowPos(Tree * tree){

    int positions = tree->rightChild->firstPos->start->content + 1;
    TreeList * t = Tree2TreeList(tree);
    Table * table = Table_New(positions);
    FollowPosExecuteCases(table, t->start, positions);
    Table_SetSymbols(table, t);
    return table;

}

/**
 * @brief BuildDFA, Algoritmo para construir el automáta por usando el alfabeto
 * y el arbol contruido previamente. Aqui se crea la tabla FollowPos para usarla
 * y poder crear cada uno de los estados,
 */

IList * BuildDFAMatchSymbols(Table *follows, State *currentState, Node *currentSymbol){

    IList * U = IList_New();
    INode * currentPos = currentState->content->start;
    while(currentPos != NULL){
        int pos = currentPos->content;
        char * followSymbol = follows->rows[pos]->symbol;
        if(equals(followSymbol, currentSymbol->content)){
            IList_Union(U, follows->rows[pos]->followpos);
        }
        currentPos = currentPos->next;
    }   return U;

}

void BuildDFAGoOverTransitions(DFA *dfa, Table *follows, State *currentState, Node *currentSymbol){

    IList * U = BuildDFAMatchSymbols(follows, currentState, currentSymbol);
    State * dstate = DFA_Search(dfa,U);
    if(U->size != 0 && dstate == NULL) {
        dstate = State_New(U);

        if(IList_Search(dstate->content, dfa->qFinal)){
            dstate->final = 1;
        }

        DFA_Add(dfa, dstate);
    }
    State_Add(currentState, dstate, currentSymbol->content);

}

void BuildDFAGoOverAlphabet(DFA *dfa, Table *follows, List * sigma, State *currentState){

    Node * currentSymbol = sigma->start;
    while(currentSymbol != NULL){
        BuildDFAGoOverTransitions(dfa, follows, currentState, currentSymbol);
        currentSymbol = currentSymbol->next;
    }

}

void BuildDFAGoOverDStates(DFA *dfa, Table *follows, List *sigma){

    State * currentState = DFA_GetFirstUnmarked(dfa);
    while(currentState != NULL){
        currentState->marked = 1;
        BuildDFAGoOverAlphabet(dfa, follows, sigma, currentState);
        currentState = DFA_GetFirstUnmarked(dfa);
    }

}

void BuildDFAEnumerateStates(DFA * dfa){

    State * tmp = dfa->start;
    int pos = dfa->size;
    while(tmp != NULL){
        tmp->q = pos;
        if(tmp->next == NULL) dfa->qStart = tmp;
        tmp = tmp->next; pos--;
    }

}

void BuildDFASetTrashTransitions(DFA *dfa ,State * trashState){
    Node * currentSymbol = dfa->alphabet->start;
    while(currentSymbol != NULL){
        int pos = currentSymbol->content[0];
        Transition * trans = Transition_New();
        trans->symbol = currentSymbol->content;
        trans->to = trashState;
        trashState->transitions[pos] = trans;
        currentSymbol = currentSymbol->next;
    }
}

void BuildDFASetTrashStates(DFA * dfa){

    State * trash = State_New(IList_New());
    BuildDFASetTrashTransitions(dfa, trash);

    State * currentState = dfa->start;
    while(currentState != NULL){
        for(int i = 0; i < 256; i++){
            Transition * currentTrans = currentState->transitions[i];
            if(currentTrans != NULL && currentTrans->to == NULL)
                currentTrans->to = trash;
        }
        currentState = currentState->next;
    }
    DFA_Add(dfa,trash);

}

DFA * BuildDFA(Tree * tree, List *sigma){

    Table * follows = FollowPos(tree);

    DFA * dfa = DFA_New();
    dfa->start = State_New(tree->firstPos);
    dfa->start->q = 0;
    dfa->qFinal = tree->rightChild->firstPos->start->content;
    dfa->alphabet = sigma;

    if(IList_Search(dfa->start->content, dfa->qFinal)){
        dfa->start->final = 1;
    }

    BuildDFAGoOverDStates(dfa, follows, sigma);
    BuildDFAEnumerateStates(dfa);
    BuildDFASetTrashStates(dfa);

    return dfa;

}
