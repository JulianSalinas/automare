#ifndef AUTOMATA_H
#define AUTOMATA_H
#include "main.h"
#include "list.h"

typedef struct State State;
typedef struct StateList StateList;
typedef struct Transition Transition;
typedef struct DFA DFA;

struct State{
    int q;
    int marked;
    int final;
    IList * content;
    State * next;
    Transition * transitions[256];
};

struct Transition{
    char * symbol;
    State * to;
};

struct DFA {
    List * alphabet;
    State * start;
    State * qStart;
    int qFinal;
    int size;
};


State * State_New(IList *);
Transition * Transition_New();
DFA * DFA_New();

void DFA_Add(DFA *, State *);
State * DFA_GetFirstUnmarked(DFA *);
State * DFA_Search(DFA *, IList *);

void State_Add(State *, State *, char *);
State * State_GetNext(State *, char *);

List * DFA_Process(DFA *, char * );
void DFA_Print(DFA *);


#endif
