#include "tree.h"

/**
 * @brief CONSTRUCTORES
 */

#define initNodePointers    (n->next = n->prev = NULL)
#define initListPointers    (l->start = l->end = NULL)
#define initNodeContent     (n->content = content)
#define initListSize        (l->size = 0)
#define isLeaf(t)           (t->leftChild == NULL && t->rightChild == NULL)
#define equals(x,y)         (strcmp(x,y) == 0 ? 1:0)

Tree * Tree_New(char * content){

    Tree * tree = malloc(sizeof(Tree));
    tree->nullable = 1;
    tree->firstPos = IList_New();
    tree->lastPos = IList_New();
    tree->content = content;
    tree->leftChild = tree->rightChild = NULL;
    return tree;

}

TreeList * TreeList_New(){

    TreeList * l = malloc(sizeof(TreeList));
    initListPointers; initListSize;
    return l;

}

TreeListNode * TreeListNode_New(Tree * content){

    TreeListNode  * n = malloc(sizeof(TreeListNode));
    initNodePointers; initNodeContent;
    return n;

}

/**
 * @brief List2TreeList, se usa para poder aplicar el algoritmo
 * de transformación de ER a árbol.
 */

TreeList * List2TreeList(List * l){

    TreeList * list = TreeList_New();
    Node * tmp = l->start;
    while(tmp != NULL){
        TreeList_Add(list, Tree_New(tmp->content));
        tmp = tmp->next;
    }   return list;

}

/**
 * @brief Tree2TreeList, se usa para poder aplicar el algoritmo
 * followpos.
 */

void Tree2TreeListAux(Tree * tree, TreeList * list){

    if(tree != NULL){
        TreeList_Add(list, tree);
        Tree2TreeListAux(tree->leftChild, list);
        Tree2TreeListAux(tree->rightChild, list);
    }

}

TreeList * Tree2TreeList(Tree * tree){

    TreeList * list = TreeList_New();
    Tree2TreeListAux(tree, list);
    return list;

}

/**
 * @brief TreeList_Push, Agrega al INICIO de la lista un puntero de un árbol
 */

void TreeList_Push(TreeList * l, Tree * str){

    TreeListNode * n = TreeListNode_New(str);
    if(l->size == 0) l->start = l->end = n;
    else{
        n->next = l->start;
        n->next->prev = n;
        l->start = n;
    }   l->size++;

}

/**
 * @brief TreeList_Add, Agrega al FINAL de la lista un puntero de un árbol
 */

void TreeList_Add(TreeList * l, Tree * str){

    TreeListNode * n = TreeListNode_New(str);
    if(l->size == 0) l->start = l->end = n;
    else{
        l->end->next = n;
        n->prev = l->end;
        l->end = n;
    }   l->size++;

}

/**
 * @brief TreeList_Peek, Se obtiene un puntero del primer árbol sin desvincularlo
 */

Tree * TreeList_Peek(TreeList * l){

    if(l->size == 0) return NULL;
    else return l->start->content;

}

/**
 * @brief TreeList_Peek, Se obtiene un puntero del primer árbol desvinculandolo de este
 */

Tree * TreeList_Pop_Aux(TreeList * l){

    TreeListNode * pop = l->start;
    if(l->size == 1) initListPointers;
    else{
        l->start = l->start->next;
        pop->next->prev = NULL;
        pop->next = NULL;
    }   l->size--;
    return pop->content;

}

Tree * TreeList_Pop(TreeList * l){

    if(l->size == 0) return NULL;
    else return TreeList_Pop_Aux(l);

}

/**
 * @brief Tree_Pos, asigna un número a cada una de las hojas del arbol
 */

static int pos = 0;

void Tree_PosCaseLeaf(Tree * t){

    IList_Add(t->firstPos, pos);
    IList_Add(t->lastPos, pos);
    pos++;

}

void Tree_PosCaseNotLeaf(Tree * t){

    Tree_Pos(t->leftChild);
    Tree_Pos(t->rightChild);

}

void Tree_Pos(Tree * t){

    if(t != NULL){
        if(isLeaf(t)) Tree_PosCaseLeaf(t);
        else Tree_PosCaseNotLeaf(t);
    }

}

/**
 * @brief Tree_Nullable, asigna a cada nodo si se puede omitir
 * Ej; En un OR se puede omitir uno de los de parámetros
 */

void Tree_NullableCaseOr(Tree * t){

    Tree_Nullable(t->leftChild);
    Tree_Nullable(t->rightChild);
    t->nullable = t->leftChild->nullable || t->rightChild->nullable;

}

void Tree_NullableCaseConcat(Tree * t){

    Tree_Nullable(t->leftChild);
    Tree_Nullable(t->rightChild);
    t->nullable = t->leftChild->nullable && t->rightChild->nullable;

}

void Tree_NullableCasePlus(Tree * t){
    
    Tree_Nullable(t->leftChild);
    t->nullable = t->leftChild->nullable;
    
}

void Tree_NullableCaseDefault(Tree * t){
    
    t->nullable = 1;
    Tree_Nullable(t->leftChild);
    
}

void Tree_Nullable(Tree * t){

    if(t != NULL){
        if(isLeaf(t)) t->nullable = 0;
        else if(equals(t->content, SOr)) Tree_NullableCaseOr(t);
        else if(equals(t->content, SConcat)) Tree_NullableCaseConcat(t);
        else if(equals(t->content, SKleene)) Tree_NullableCaseDefault(t);
        else if(equals(t->content, SPlus)) Tree_NullableCasePlus(t);
        else if(equals(t->content, SQuestion)) Tree_NullableCaseDefault(t);
    }

}

/**
 * @brief Tree_FirstPos, Requisito para armar el automata.
 */

void Tree_FirstPosCaseOr(Tree * t){

    Tree_FirstPos(t->leftChild);
    Tree_FirstPos(t->rightChild);
    IList_Union(t->firstPos, t->leftChild->firstPos);
    IList_Union(t->firstPos, t->rightChild->firstPos);

}

void Tree_FirstPosCaseUnaryOp(Tree * t){

    Tree_FirstPos(t->leftChild);
    Tree_FirstPos(t->rightChild);
    IList_Union(t->firstPos, t->leftChild->firstPos);

}

void Tree_FirstPosCaseConcat(Tree * t){

    if(t->leftChild->nullable){
        Tree_FirstPosCaseOr(t);
    }
    else {
        Tree_FirstPosCaseUnaryOp(t);
    }

}

void Tree_FirstPos(Tree * t){

    if(t != NULL ){
        if(equals(t->content, SOr)) Tree_FirstPosCaseOr(t);
        else if(equals(t->content, SConcat)) Tree_FirstPosCaseConcat(t);
        else if(equals(t->content, SKleene)) Tree_FirstPosCaseUnaryOp(t);
        else if(equals(t->content, SPlus)) Tree_FirstPosCaseUnaryOp(t);
        else if(equals(t->content, SQuestion)) Tree_FirstPosCaseUnaryOp(t);
    }

}

/**
 * @brief Tree_LastPos, Requisito para armar el automata
 */

void Tree_LastPosCaseOr(Tree * t){

    Tree_LastPos(t->leftChild);
    Tree_LastPos(t->rightChild);
    IList_Union(t->lastPos, t->leftChild->lastPos);
    IList_Union(t->lastPos, t->rightChild->lastPos);

}

void Tree_LastPosCaseConcat(Tree * t){

    if(t->rightChild->nullable){
        Tree_LastPosCaseOr(t);
    }
    else{
        Tree_LastPos(t->leftChild);
        Tree_LastPos(t->rightChild);
        IList_Union(t->lastPos, t->rightChild->lastPos);
    }

}

void Tree_LastPosCaseUnaryOp(Tree * t){

    Tree_LastPos(t->leftChild);
    Tree_LastPos(t->rightChild);
    IList_Union(t->lastPos, t->leftChild->lastPos);

}

void Tree_LastPos(Tree * t){

    if(t != NULL){
        if(equals(t->content, SOr)) Tree_LastPosCaseOr(t);
        else if(equals(t->content, SConcat)) Tree_LastPosCaseConcat(t);
        else if(equals(t->content, SKleene)) Tree_LastPosCaseUnaryOp(t);
        else if(equals(t->content, SPlus)) Tree_LastPosCaseUnaryOp(t);
        else if(equals(t->content, SQuestion)) Tree_LastPosCaseUnaryOp(t);
    }

}



















