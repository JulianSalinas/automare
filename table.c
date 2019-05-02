#include "table.h"

#define isLeaf(t) (t->leftChild == NULL && t->rightChild == NULL)

/**
 * @brief CONSTRUCTORES, Esta tabla es usada por la función follopos,
 * obtener follopos(i) es igual a obtener una fila del arreglo en la
 * posición i
 */

Row * Row_New(int n){

    Row * row = malloc(sizeof(Row));
    row->symbol = NULL;
    row->n = n;
    row->followpos = IList_New();
    return row;

}

Table * Table_New(int rows){

    Table * table = malloc(sizeof(Table));
    int pos = table->amount = rows;
    for(int i = 0; i < pos; i++)
        table->rows[i] = Row_New(i);
    return table;

}

/**
 * @brief Table_Add, añade un elemento (número) en una posición de la tabla.
 * Es lo mismo que decir; followpos(pos) = followpos(pos) U { element }
 */

void Table_Add(Table * table, int pos, int element){

    if(!IList_Search(table->rows[pos]->followpos, element))
    IList_SortInsert(table->rows[pos]->followpos, element);

}

/**
 * @brief Table_SetSymbols, Esta función realmente no es necesaria, pero
 * crea un panorama más claro a la hora de imprimirla y poder realizar arreglos.
 */

void Table_SetSymbols(Table * table, TreeList * list){

    TreeListNode * n = list->start;
    while(n != NULL){
        if(isLeaf(n->content)){
            int pos = n->content->firstPos->start->content;
            table->rows[pos]->symbol = n->content->content;
        }   n = n->next;
    }

}

/**
 * @brief Table_Print
 */

void Table_Print(Table * table){

    printf("\tn\tfollopos(n)\n");
    int pos = table->amount;
    for(int i = 0; i < pos; i++){
        INode * n = table->rows[i]->followpos->start;
        printf("\t%s->%d: \t[ ",table->rows[i]->symbol,i);
        if( n == NULL) printf("Ø");
        while(n != NULL){
            printf("%d", n->content);
            n = n->next;
            if(n != NULL) printf(", ");
        }   printf(" ]\n");
    }

}
