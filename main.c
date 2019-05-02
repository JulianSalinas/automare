
#include "list.h"
#include "tree.h"
#include "dfa.h"
#include "funct.h"
#include "stringGenerator.h"
#include "scanner.h"

void PrintLine(){ printf("\t"); for(int i = 0; i <= 60; i++) printf("."); printf("\n\n"); }

int GetFileSize(FILE * file){

    fseek(file, 0, SEEK_END);
    int size = ftell(file) - 1;
    rewind(file);
    return size;

}

char * GetFileContent(FILE * file, int size){

    char * string = (char*)malloc(size);
    for(int i = 0; i < size; i++){
        fseek(file, i, SEEK_SET);
        string[i] = fgetc(file);
    }
    fclose(file);
    return string;

}

void Welcome(){

    printf("\n\n");
    printf("\t╔═════════════════════════════════════════╗\n");
    printf("\t║                                         ║\n");
    printf("\t║         »AutoER version 25.2.1«         ║\n");
    printf("\t║                hecho por                ║\n");
    printf("\t║            Julian & Brandon             ║\n");
    printf("\t║                                         ║\n");
    printf("\t╚═════════════════════════════════════════╝\n");

}

void Help(){

    PrintLine();
    printf("\tLos primeros 2 parámetros son obligatorios: ");
    printf("\n\t* El 1° debe ser -r ó -g");
    printf("\n\t* El 2° debe ser la expresión regular a usar");

    printf("\n\n\tEn caso de la opción -r se debe cumplir que: ");
    printf("\n\t* El 3° parámetro es la cadena (o archivo) a procesar");
    printf("\n\t* El 4° parámetro (Opcional) es el archivo de configuración");

    printf("\n\n\tEn caso de la opción -g se debe cumplir que: ");
    printf("\n\t* El 3° parámetro es el largo máximo de las cadenas a generar");
    printf("\n\t* El 4° parámetro es la cantidad máxima de cadenas que se mostrarán en la consola");
    printf("\n\t* El 5° parámetro es 1 para generación aleatoria ó 0 para generar en orden");
    printf("\n\t* El 6° parámetro (Opcional) es el archivo de configuración");

    printf("\n\n\t* Ejemplo >> -r \"abc|aaa|c+\" \"abababcacbaaacbccbbbaacaaabababab\" \"home/config.txt\"\n");
    printf("\t* Ejemplo >> -r \"a|(b.ca)\" \"home/string.txt\" \"home/config.txt\"\n");
    printf("\t* Ejemplo >> -g \"cb*caa\" 25 5 \"home/config.txt\"\n");

}

List * LoadConfigFile(char * filename){

    PrintLine();
    List * alpha = List_New();

    FILE * file = fopen(filename, "r");


    if(file != NULL) {

        /* Se guarda el contenido del archivo*/
        int fileSize = GetFileSize(file);
        char * fileContent = GetFileContent(file, fileSize);

        /* Se recorre cada caracter de la cadena en busca de operadores y luego el alfabeto*/
        int currentSymbol = 0;
        for(int i = 0; i < strlen(fileContent); i++){

            /*Si se encuentra un entere, el caracter siguiente es un operador*/
            if(fileContent[i] == '\n' && currentSymbol <= 7){
                symbols[currentSymbol] = Char2Str(fileContent[i+1]);
                currentSymbol++;
            }

            /*Al finalizar los operadores se procesa la linea con el alfabeto*/
            else if(fileContent[i] == '\n'){
                for(int j = i+1; j< fileSize; j++){
                    List_Add(alpha, Char2Str(fileContent[j]));
                }   break;
            }

        }
    }

    /*Si no se específica el archivo de configuración, se cargan los datos por default*/

    else{

        symbols[0] = "*"; symbols[1] = "?";
        symbols[2] = "+"; symbols[3] = "|";
        symbols[4] = "."; symbols[5] = "(";
        symbols[6] = ")"; symbols[7] = "#";

        List_Add(alpha, "a"); List_Add(alpha, "b"); List_Add(alpha, "c");
        List_Add(alpha, "d"); List_Add(alpha, "e"); List_Add(alpha, "f");

    }
    return alpha;

}

List * LoadRegex(char * expression){

        List * infix = Str2List(expression);
        List_Push(infix, SLeft);
        List_Add(infix, SRight);
        List_Add(infix, SConcat);
        List_Add(infix, SFinal);
        return infix;

}

void Recognizer(char * expression, char * stringOrFilename, List * alpha){

    List * regex = LoadRegex(expression);
    if(validateExpression(regex)){
        List * postfix = ShuntingYard(regex);
        Tree * tree = BuildTree(postfix);
        DFA * dfa = BuildDFA(tree, alpha);
        printf("\tDFA: \n"); DFA_Print(dfa); printf("\n"); PrintLine();
        printf("\tCadenas encontradas en \"%s\": \n", stringOrFilename);

        /*Aquí es donde se toma el tiempo*/
        clock_t begin, end;
        begin = clock();
        DFA_Process(dfa, stringOrFilename);
        end = clock();

        PrintLine();
        printf("\tTiempo transcurrido: %f segundos\n", (end-begin)/(double) CLOCKS_PER_SEC);
    }
    else printf("\tError en la expresión\n");

}

void Generator(char * expression, int maxSize, int maxAmount, int random){

    List * regex = Str2List(expression);
    if(validateExpression(regex)){
        List * postfix = ShuntingYard(regex);
        printf("\tCadenas generadas: \n");

        /*Aquí es donde se toma el tiempo*/
        clock_t begin, end;
        begin = clock();
        List * result = generateRegexLanguage(postfix, maxSize);
        end = clock();


        int amount = 0;
        if(!random){
            Node * tmp = result->start;
            while(tmp != NULL && amount <= maxAmount){
                printf("\t%s\n", tmp->content);
                tmp = tmp->next; amount++;
            }
        }
        else{
            while(amount++ < maxAmount){
                int randomNumber = rand() % result->size;
                Node * tmp = result->start;
                while(randomNumber > 0){
                    tmp = tmp->next;
                    randomNumber--;
                }   printf("\t%s\n", tmp->content);
            }
        }
        PrintLine();
        printf("\tTiempo transcurrido: %f segundos\n", (end-begin)/(double) CLOCKS_PER_SEC);
    }
    else printf("\tError en la expresión\n");


}

int main(int argc, char *argv[]) {
    Welcome();
    if(argc < 3 ) Help();
    else {

        PrintLine();
        printf("\tExpresión ingresada: %s\n", argv[2]);

        if(strcmp(argv[1], "-r") == 0 && (argc == 4 || argc == 5)){
            List * alphabet = NULL;
            if(argc == 4) alphabet = LoadConfigFile(NULL);
            else alphabet = LoadConfigFile(argv[4]);
            Recognizer(argv[2], argv[3], alphabet );
        }

        else if(strcmp(argv[1], "-g") == 0 && (argc == 6 || argc == 7)){
            if(argc == 6) LoadConfigFile(NULL);
            else LoadConfigFile(argv[6]);
            Generator(argv[2], atoi(argv[3]), atoi(argv[4]), atoi(argv[5]));
        }

        else{
            PrintLine();
            printf("\tPárametros incorrectos\n");
            Help();
        }

    }
    PrintLine();
    printf("\t"); return 0;
}
