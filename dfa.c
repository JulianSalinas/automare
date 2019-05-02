#include "dfa.h"

/**
 * @brief CONSTRUCTORES
 */

State * State_New(IList * content){

    State * state = malloc(sizeof(State));
    state->q = -1;
    state->marked = 0;
    state->final = 0;
    state->content = content;
    state->next = NULL;
    return state;

}

DFA * DFA_New(){

    DFA * dfa = malloc(sizeof(DFA));
    dfa->start = NULL;
    dfa->size = 0;
    dfa->qFinal = 0;
    dfa->alphabet = NULL;
    return dfa;

}


Transition * Transition_New(){

    Transition * trans = malloc(sizeof(Transition));
    trans->to = NULL;
    trans->symbol = NULL;
    return trans;

}

/**
 * @brief DFA_Add, agrega un estado al conjunto Q del autómata
 */

void DFA_Add(DFA * dfa, State * state){

    State * n = state;
    if(dfa->start == NULL) dfa->start = n;
    else{ n->next = dfa->start; dfa->start = n; }
    dfa->size ++;

}

/**
 * @brief DFA_GetFirstUnmarked, Obtiene uno de los estado que todavía
 * no posea transiciones
 */

State * DFA_GetFirstUnmarked(DFA * dfa){

    State * tmp = dfa->start;
    while(tmp != NULL){
        if(tmp->marked == 0) return tmp;
        tmp = tmp->next;
    }   return NULL;

}

/**
 * @brief DFA_Search, retorna true si la lista que representa el estado, ya
 * esta en la lista de estados, usada para crear el DFA.
 */

State * DFA_Search(DFA * dfa, IList * state){

    State * tmp = dfa->start;
    while(tmp != NULL){
        if(IList_Equals(tmp->content, state)) return tmp;
        tmp = tmp->next;
    }   return NULL;

}

/**
 * @brief State_Add, añade una transición
 */

void State_Add(State * from, State * to, char * symbol){

    int pos = symbol[0];
    Transition * trans = Transition_New();
    trans->to = to;
    trans->symbol = symbol;
    from->transitions[pos] = trans;

}

/**
 * @brief State_GetNext, es equivalente a realizar un paso del autómata
 */

State * State_GetNext(State * state, char * symbol){

    int pos = symbol[0];
    State * next = state->transitions[pos]->to;
    return next;

}

/**
 * @brief DFA_Process, recorre la hilera y va cambiando el número de estado.
 * La lista es para luego mostrar el proceso.
 */

#define error_1 (printf("\tSe ha interrumpido el proceso porque existe un cáracter no definido en el alfabeto\n", symbol, i));

char * substr(char* cadena, int comienzo, int longitud){

    if (longitud == 0) longitud = strlen(cadena) - comienzo;
    char * nuevo = (char*)malloc(sizeof(char) * (longitud+1));
    nuevo[longitud] = '\0';
    strncpy(nuevo, cadena + comienzo, longitud);
    return nuevo;

}

int getFileSize(FILE * file){

    fseek(file, 0, SEEK_END);
    int size = ftell(file) - 1;
    rewind(file);
    return size;

}

char * getFileContent(FILE * file, int size){

    char * string = (char*)malloc(size);
    for(int i = 0; i < size; i++){
        fseek(file, i, SEEK_SET);
        string[i] = fgetc(file);
    }   fclose(file);
    return string;

}

int DFA_ProcessAcceptString(DFA * dfa, char * string){

    State * currentState = dfa->qStart;
    for(int i = 0; i < strlen(string); i++){
        char * symbol = Char2Str(string[i]);
        if(!List_Search(dfa->alphabet, symbol)) { error_1; exit(1); }
        if(currentState->q == -1) return 0;
        currentState = State_GetNext(currentState, symbol);
    }
    return currentState->final;

}

List * DFA_ProcessGetAllSubstrings(DFA * dfa, char * string){

    if(dfa->qStart->final) printf("\t[ cadena vacía ] \n");
    List * substrings = List_New();
    int size =  strlen(string);
    for(int i = 0; i < size; i++){
        int large = size - i;
        for(int j = 1; j <= large; j++){
                char * sub = substr(string, i,j);
                if(DFA_ProcessAcceptString(dfa, sub)){
                    printf("\t\"%s\" : [%d,%d] \n", sub, i, j+i);
                    List_Add(substrings, sub);
                }
        }
    }
    return substrings;
}


List * DFA_Process(DFA * dfa, char * string){

    FILE * file = fopen(string, "r");
    if(file != NULL) {
        int fileSize = getFileSize(file);
        string = getFileContent(file, fileSize);
    }
    return DFA_ProcessGetAllSubstrings(dfa, string);
}


/**
 * @brief DFA_Print, Imprime el autómata usando para ello la representación
 * de la función de transición.
 * Si una transición es hacia el estado Basura, entonces no se imprime
 */

void DFA_Print(DFA * dfa){

    State * currentState = dfa->start;
    while(currentState != NULL){
        Node * symbol = dfa->alphabet->start;
        while(symbol != NULL){
            State * nextState = State_GetNext(currentState, symbol->content);
            if(nextState != NULL && nextState->q != -1)
                printf("\tδ( q%c, %s ) ├ q%c %s\n",
                   currentState->q == -1 ? 'T' : '0' + currentState->q,
                   symbol->content,
                   nextState->q == -1 ? 'T' : '0' + nextState->q,
                   nextState->final ? "Final" : "");
            symbol = symbol->next;
        }   currentState = currentState->next;
    }

}
