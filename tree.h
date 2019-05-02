#ifndef TREE_H
#define TREE_H
#include "main.h"
#include "list.h"

typedef struct Tree Tree;
typedef struct TreeList TreeList;
typedef struct TreeListNode TreeListNode;

struct Tree {
    int nullable; IList * firstPos, * lastPos;
    char * content; Tree * leftChild, * rightChild;
};

struct TreeList { int size; TreeListNode * start, * end; };
struct TreeListNode { Tree * content; TreeListNode * next, * prev; };

Tree * Tree_New(char * content);
TreeList *  TreeList_New();
TreeListNode *  TreeListNode_New(Tree *);

TreeList * List2TreeList(List *);
TreeList * Tree2TreeList(Tree *);

void    TreeList_Push(TreeList *, Tree *);
void    TreeList_Add(TreeList *, Tree *);
Tree *  TreeList_Peek(TreeList *);
Tree *  TreeList_Pop(TreeList *);

void    Tree_Pos(Tree *);
void    Tree_Nullable(Tree *);
void    Tree_FirstPos(Tree *);
void    Tree_LastPos(Tree *);

#endif
