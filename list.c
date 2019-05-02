#include "list.h"

/**
 * @brief CONSTRUCTORES
 */

#define initNodePointers      (n->next = n->prev = NULL)
#define initListPointers      (l->start = l->end = NULL)
#define initNodeContent       (n->content = content)
#define initListSize          (l->size = 0)
#define equals(x,y) (strcmp(x,y) == 0 ? 1:0)

Node * Node_New(char * content){

    Node * n = malloc(sizeof(Node));
    initNodePointers; initNodeContent;
    return n;

}

List * List_New(){

    List * l = malloc(sizeof(List));
    initListPointers; initListSize;
    return l;

}

INode * INode_New(int content){

    INode * n = malloc(sizeof(INode));
    initNodePointers; initNodeContent;
    return n;

}

IList * IList_New(){

    IList * l = malloc(sizeof(IList));
    initListPointers; initListSize;
    return l;

}

LNode * LNode_New(List * content){

    LNode * n = malloc(sizeof(LNode));
    initNodePointers; initNodeContent;
    return n;
}

LList * LList_New(){

    LList * l = malloc(sizeof(LList));
    initListPointers; initListSize;
    return l;
}

/**
 * @brief Char2Str, Convierte un char a un char *
 * Esto por el motivo que pueden existir carácteres que se representen anteponiendo otro.
 */

char * Char2Str(char c){

    char * str = (char*) malloc(2);
    str[0] = c; str[1] = '\0';
    return str;

}


/**
 * @brief Str2List, Al convertir el char * a una lista se facilita
 * su manejo.
 */

List * Str2List(char * str){

    List * list = List_New();
    char * tmp = str;
    int size =  strlen(str);
    for (int i = 0; i < size; i++){
        List_Add(list,Char2Str(tmp[0])); *tmp++;
    }   return list;

}

/**
 * @brief List_Add, Añade un nuevo nodo al FINAL de la lista
 */

void List_Add(List * l, char * str){

    Node * n = Node_New(str);
    if(l->size == 0) l->start = l->end = n;
    else{
        l->end->next = n;
        n->prev = l->end;
        l->end = n;
    }   l->size++;

}

/**
 * @brief List_Push, Añade un nuevo nodo al INICIO de la lista
 */

void List_Push(List * l, char * str){

    Node * n = Node_New(str);
    if(l->size == 0) l->start = l->end = n;
    else{
        n->next = l->start;
        n->next->prev = n;
        l->start = n;
    }   l->size++;

}

/**
 * @brief List_Peek, Retorna una copia del inicio de la lista
 */

char * List_Peek(List * l){

    if(l->size == 0) return NULL;
    else{
        Node * node = l->start;
        int size = strlen(node->content);
        char * content = (char*) malloc(size);
        strcpy(content, node->content);
        return content;
    }

}

/**
 * @brief List_Pop, Devuelve el contenido del ultimo nodo y lo elimina
 */

char * List_Pop_Aux(List * l){

    Node * pop = l->start;
    if(l->size == 1) l->start = l->end = NULL;
    else{
        l->start = l->start->next;
        pop->next->prev = NULL;
        pop->next = NULL;
    }   l->size--;
    return pop->content;

}

char * List_Pop(List * l){

    if(l->size == 0) return NULL;
    else return List_Pop_Aux(l);

}

/**
 * @brief List_Search, Retorna 1 si el elemento se encuentra en la lista
 */

int List_Search(List * l, char * content){

    Node * tmp = l->start;
    while(tmp!=NULL){
        if(strcmp(tmp->content, content) == 0) return 1;
        tmp = tmp->next;
    }   return 0;

}

/**
 * @brief List_Append, Agrega todos los valores de una lista a otra
 */

List * List_Concat(List * l1, List * l2){

    Node * tmp = l2->start;
    while(tmp != NULL){
        List_Add(l1, tmp->content);
        tmp = tmp->next;
    }   return l1;

}

List * List_Union(List * l1, List * l2){

    List * l3 = List_New();

    Node * tmp = l1->start;
    while(tmp != NULL){
        if(!List_Search(l3, tmp->content))
        List_Add(l3, tmp->content);
        tmp = tmp->next;
    }

    tmp = l2->start;

    while(tmp != NULL){
        if(!List_Search(l3, tmp->content))
        List_Add(l3, tmp->content);
        tmp = tmp->next;
    }

    return l3;
}
/**
 * @brief IList_Add, Añade un nuevo nodo con un entero al final de la lista
 */

void IList_Add(IList * l, int content){

    INode * n = INode_New(content);
    if(l->size == 0) l->start = l->end = n;
    else{
        l->end->next = n;
        n->prev = l->end;
        l->end = n;
    }   l->size++;

}

/**
 * @brief IList_Push, Añade un entero al inicio de la lista
 */

void IList_Push(IList * l, int content){

    INode * n = INode_New(content);
    if(l->size == 0) l->start = l->end = n;
    else{
        n->next = l->start;
        n->next->prev = n;
        l->start = n;
    }   l->size++;

}

/**
 * @brief IList_SortInsert, Inserta un elemento en orden ascendente
 */

void IList_SortInsertAux(IList * l, INode * n){

    INode * tmp = l->start;
    while(tmp != NULL){
        if(tmp->content <= n->content && n->content < tmp->next->content){
            n->next = tmp->next;
            n->prev = tmp->next->prev;
            tmp->next->prev = n;
            tmp->next = n;
            break;
        }   tmp = tmp->next;
    }   l->size++;

}

void IList_SortInsertCaseZero(IList * l, INode * n){

    l->start = l->end = n;
    l->size++;

}

void IList_SortInsertCaseNotZero(IList * l, INode * n){

    if(n->content <= l->start->content) IList_Push(l,n->content);
    else if(n->content > l->end->content) IList_Add(l,n->content);
    else IList_SortInsertAux(l, n);

}

void IList_SortInsert(IList * l, int content){

    INode * n = INode_New(content);
    if(l->size == 0) IList_SortInsertCaseZero(l,n);
    else if(l->size > 0) IList_SortInsertCaseNotZero(l,n);

}

/**
 * @brief IList_Union, Añade los elementos de l2 a l1, siempre
 * que el elemento no se encuentre ya en l1.
 */

IList *  IList_Union(IList * l1, IList *l2){

    INode * tmp = l2->start;
    while(tmp != NULL){
        if(!IList_Search(l1, tmp->content))
        IList_Add(l1, tmp->content);
        tmp = tmp->next;
    }   return l1;

}

/**
 * @brief IList_Concat, Añade los elementos de l2 a l1.
 */

IList * IList_Concat(IList * l1, IList *l2){

    INode * tmp = l2->start;
    while(tmp != NULL){
        IList_Add(l1, tmp->content);
        tmp = tmp->next;
    }   return l1;

}

/**
 * @brief List_Multiply
 */

List * List_Multiply(List * a, List * b){
    List * c = List_New();
    Node * i = a->start;
    while( i != NULL){

        Node * j = b->start;
        while(j != NULL){

            char * contentA = i->content;
            char * contentB = j->content;
            int size = strlen(contentA) + strlen(contentB)+1;
            char * newStr = (char*) malloc(size);
            strcpy(newStr, "");
            if(!equals(contentA, " ")) {strcat(newStr, contentA);}
            if(!equals(contentB, " ")) {strcat(newStr, contentB);}
            List_Add(c, newStr);

            j = j->next;
        }

        i = i->next;

    }

    return c;
}


/**
 * @brief List_Combination
 */

List * List_Combination(List * a, List * b){

    List * union1 = List_Union(a,b);
    List * crossConcat = List_Multiply(a,b);

    return List_Union(union1, crossConcat);
}


/**
 * @brief IList_Search, Retorna 1 si el elemento esta presente en la lista
 */

int IList_Search(IList * l, int content){
    INode * tmp = l->start;
    while(tmp!=NULL){
        if(tmp->content == content)
            return 1;
        tmp = tmp->next;
    }   return 0;
}

/**
 * @brief IList_Equals, Revisa uno por uno los elementos de la lista
 */

int IList_Equals(IList * l1, IList * l2){

    INode * n1 = l1->start;
    INode * n2 = l2->start;
    if(l1->size == l2->size){
        while(n1 != NULL && n2 != NULL){
            if(n1->content != n2->content) return 0;
            n1 = n1->next; n2 = n2->next;
        }   return 1;
    }   return 0;

}

/**
 * @brief IList_Print
 */

void IList_PrintCaseNotNull(IList *l){

    INode * tmp = l->start;
    printf("[ ");
    while (tmp != NULL) {
        printf("%d",tmp->content);
        tmp = tmp->next;
        if(tmp != NULL) printf(", ");
    }   printf(" ]");

}

void IList_Print(IList * l){

    if(l->size == 0) printf("[ Ø ]");
    else IList_PrintCaseNotNull(l);

}

/**
 * @brief List_Print
 */

void List_PrintCaseNotNull(List *l){

    Node * tmp = l->start;
    while (tmp != NULL) {
        printf("%s ",tmp->content);
        tmp = tmp->next;
    }

}

void List_Print(List * l){

    if(l->size == 0) printf("[ Ø ]");
    else List_PrintCaseNotNull(l);

}

/**
 * @brief List_InsertAfter, inserta el elemento recibido en la lista, en el indice recibido + 1
 */

void List_InsertAfter(List * l, int index, char * str){

    Node * newNode = Node_New(str);

    Node * tmp = l->start;

    int i = 0;
    while(i != index) {
        tmp = tmp->next; i++;
    }

    Node * aux = tmp->next;
    tmp->next = newNode;
    aux->prev = newNode;
    newNode->next = aux;
    newNode->prev = tmp;

}

/**
 * @brief LList_Push, Inserta al inicio de la lista de listas
 */

void LList_Push(LList * l, List * list){

    LNode * n = LNode_New(list);
    if(l->size == 0) l->start = l->end = n;
    else{
        n->next = l->start;
        n->next->prev = n;
        l->start = n;
    }   l->size++;

}

/**
 * @brief LList_Pop, Devuelve el contenido del ultimo nodo insertado y lo elimina
 */

List * LList_Pop_Aux(LList * l){

    LNode * pop = l->start;
    if(l->size == 1) l->start = l->end = NULL;
    else{
        l->start = l->start->next;
        pop->next->prev = NULL;
        pop->next = NULL;
    }   l->size--;
    return pop->content;

}

List * LList_Pop(LList * l){

    if(l->size == 0) return NULL;
    else return LList_Pop_Aux(l);

}
