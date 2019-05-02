#ifndef TABLE_H
#define TABLE_H
#define MAXSTATES 1000
#include "main.h"
#include "tree.h"
#include "list.h"

typedef struct Row Row;
typedef struct Table Table;

struct Row { int n; char * symbol; IList * followpos; };
struct Table { Row * rows [MAXSTATES]; int amount; };

Row * Row_New(int n);
Table * Table_New(int rows);

void Table_Add(Table * table, int pos, int element);
void Table_SetSymbols(Table * table, TreeList * list);
void Table_Print(Table * table);

#endif // TABLE_H
