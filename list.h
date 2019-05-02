#ifndef LIST_H
#define LIST_H
#include "main.h"

typedef struct Node Node;
typedef struct List List;

typedef struct INode INode;
typedef struct IList IList;

typedef struct LNode LNode;
typedef struct LList LList;

struct Node { Node * next, * prev; char * content; };
struct List { Node * start, * end; int size; };

struct INode { INode * next, * prev; int content; };
struct IList { INode * start, * end; int size; };

struct LNode { LNode * next, * prev; List * content; };
struct LList { LNode * start, * end; int size; };

Node * Node_New(char *);
List * List_New();

INode * INode_New(int);
IList * IList_New();

LNode * LNode_New(List *);
LList * LList_New();

char * Char2Str(char c);
List * Str2List(char * str);

void List_Add(List *, char *);
void List_Push(List *, char *);
char * List_Peek(List *);
char * List_Pop(List *);
int List_Search(List *, char *);
List * List_Concat(List *, List *);
List * List_Multiply(List *, List *);
List * List_Union(List *, List *);
List * List_Combination(List *, List *);

void List_InsertAfter(List *, int, char *);

void IList_Add(IList *, int);
void IList_Push(IList *, int);
void IList_SortInsert(IList *, int);
IList * IList_Union(IList *, IList *);
IList * IList_Concat(IList *, IList *);
int IList_Search(IList *, int);
int IList_Equals(IList *, IList *);

void LList_Push(LList *, List *);
List * LList_Pop(LList *);

void List_Print(List *);
void IList_Print(IList *);

#endif // LIST_H
