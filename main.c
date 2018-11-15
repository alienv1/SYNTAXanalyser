#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


/*
* DECLARATION DES CLASSES LEXICALES
*/
typedef enum {
PROGRAM_TOKEN, CONST_TOKEN,VAR_TOKEN, BEGIN_TOKEN,
END_TOKEN, IF_TOKEN,THEN_TOKEN,WHILE_TOKEN, DO_TOKEN, READ_TOKEN,
WRITE_TOKEN, ID_TOKEN, NUM_TOKEN,ERREUR_TOKEN,
PV_TOKEN, PT_TOKEN, PLUS_TOKEN, MOINS_TOKEN, MULT_TOKEN,
DIV_TOKEN, VIR_TOKEN, AFF_TOKEN,EGAL_TOKEN, INF_TOKEN, INFEG_TOKEN,
SUP_TOKEN, SUPEG_TOKEN, DIFF_TOKEN, PO_TOKEN, PF_TOKEN,
FIN_TOKEN
} CODE_LEX ;


typedef struct {
    CODE_LEX CODE;
    char NOM[20];
} Token;


Token tabToken[] = {
    {PROGRAM_TOKEN, "program"},
    {CONST_TOKEN, "const"},
    {VAR_TOKEN, "var"},
    {BEGIN_TOKEN, "begin"},
    {END_TOKEN, "end"},
    {IF_TOKEN, "if"},
    {THEN_TOKEN, "then"},
    {WHILE_TOKEN, "while"},
    {DO_TOKEN, "Do"},
    {READ_TOKEN, "read"},
    {WRITE_TOKEN, "write"},
    //les caractere speciaux
    {PV_TOKEN ,";"},
    {PT_TOKEN ,"."},
    {PLUS_TOKEN ,"+"},
    {MOINS_TOKEN ,"-"},
    {MULT_TOKEN ,"*"},
    {DIV_TOKEN ,"/"},
    {VIR_TOKEN ,","},
    {EGAL_TOKEN ,"="},
    {AFF_TOKEN ,":="},
    {INF_TOKEN ,"<"},
    {INFEG_TOKEN ,"<="},
    {SUP_TOKEN ,">"},
    {SUPEG_TOKEN ,">="},
    {DIFF_TOKEN ,"<>"},
    {PO_TOKEN ,"("},
    {PF_TOKEN ,")"},
    //{FIN_TOKEN ,EOF},
    //{ID_TOKEN, ID},
    //{NUM_TOKEN, NUM}
    //,{ERREUR_TOKEN, RESTE}
};

int nbMotRes = sizeof tabToken / sizeof tabToken[0];
char Car_Cour;
Token SYM_COUR;

/*
* GESTION DES ERREURS
*/

typedef enum{
    ERR_CAR_INC,
    ERR_FICH_VID,
    ERR_ID_LONG,
    ERR_ID_INV,
    ERR_NBR_LONG,
    ERR_OP_INC,
    ERR_COM_INC,

    PROGRAM_ERR,
    ID_ERR,
    PV_ERR,
    PT_ERR,
    EGAL_ERR,
    NUM_ERR,
    CONST_VAR_BEGIN_ERR,
    BEGIN_ERR,
    END_ERR,
    INST_END_ERR,
    AFF_ERR,
    PO_ERR,
    PF_ERR,
    IF_THEN_ERR,
    OP_COMPAR_ERR,
    FACT_NOT_FOUND_ERR,
    WHILE_DO_ERR,
    WRITE_ERR,
    READ_ERR
}Erreurs_t;

typedef struct  {
    Erreurs_t CODE_ERR;
    char mes[40];
} Erreurs;

Erreurs MES_ERR[] = {
    { ERR_CAR_INC, "CARACRETE: inconnu"},
    { ERR_FICH_VID, "FICHIER: vide"},
    {ERR_ID_LONG, "IDF: tres long"},
    {ERR_ID_INV, "IDF: non valide "},
    {ERR_NBR_LONG, "NUMBER: tres long"},
    {ERR_OP_INC, "OPERATEUR: non complet"},
    {ERR_COM_INC, "COMMENTAIRE: delimiteur absent"},
    {PROGRAM_ERR, "PROGRAM_ERR"},
    {ID_ERR, "ID_ERR"},
    {PV_ERR, "PV_ERR"},
    {PT_ERR, "PT_ERR"},
    {EGAL_ERR, "EGAL_ERR"},
    {NUM_ERR, "NUM_ERR"},
    {CONST_VAR_BEGIN_ERR, "CONST_VAR_BEGIN_ERR"},
    {BEGIN_ERR, "BEGIN_ERR"},
    {END_ERR, "END_ERR"},
    {INST_END_ERR, "INST_END_ERR"},
    {AFF_ERR, "AFF_ERR"},
    {PO_ERR, "PO_ERR"},
    {PF_ERR, "PF_ERR"},
    {IF_THEN_ERR, "IF_THEN_ERR"},
    {OP_COMPAR_ERR, "OP_COMPAR_ERR"},
    {FACT_NOT_FOUND_ERR, "FACT_NOT_FOUND_ERR"}, 
    {WHILE_DO_ERR, "WHILE_DO_ERR"}, 
    {WRITE_ERR, "WRITE_ERR"},
    {READ_ERR, "READ_ERR"}
};
int tailleERR = sizeof MES_ERR / sizeof MES_ERR[0];

//signatures des fonctoins utilisees
void showCodeToken(Token token);
int estBlanc(char c);
int isAccentLettre();
void lire_Car();
void lire_mot();
void lire_nombre();
void lire_spcial();
void lire_commentaire(void );
int detectError( Erreurs_t er);
void nextToken(void);

// Signatures des fonctions utilisées dans l'analyseur syntaxique
void Test_Symbole (CODE_LEX cl, Erreurs_t COD_ERR);
void PROGRAM( void);
void BLOCK();
void CONSTS();
void VARS();
void INSTS();
void INST();
void AFFEC();
void SI();
void TANTQUE();
void ECRIRE();
void LIRE();
void COND();
void EXPR();
void TERM();
void FACT();
FILE *fl = NULL;
int main(int argc, char* argv[]){
    if (!argv[1]) {
        printf("fichier n'exste pas\n");
        exit(EXIT_FAILURE);
    }
    fl = fopen(argv[1], "r");
    nextToken();
    PROGRAM();
    fclose(fl);

    getchar();
    return 0;
}


void showCodeToken(Token token){

    switch(token.CODE){
        case PROGRAM_TOKEN:
            puts("PROGRAM_TOKEN");
            break;
        case ID_TOKEN:
            puts("ID_TOKEN");
            break;
        case NUM_TOKEN:
            puts("NUM_TOKEN");
            break;
        case CONST_TOKEN:
            puts("CONST_TOKEN");
            break;
        case VAR_TOKEN:
            puts("VAR_TOKEN");
            break;
        case BEGIN_TOKEN:
            puts("BEGIN_TOKEN");
            break;
        case END_TOKEN:
            puts("END_TOKEN");
            break;
        case IF_TOKEN:
            puts("IF_TOKEN");
            break;
        case THEN_TOKEN:
            puts("THEN_TOKEN");
            break;
        case WHILE_TOKEN:
            puts("WHILE_TOKEN");
            break;
        case DO_TOKEN:
            puts("DO_TOKEN");
            break;
        case READ_TOKEN:
            puts("READ_TOKEN");
            break;
        case WRITE_TOKEN:
            puts("WRITE_TOKEN");
            break;
        case PV_TOKEN:
            puts("PV_TOKEN");
            break;
        case PT_TOKEN:
            puts("PT_TOKEN");
            break;
        case PLUS_TOKEN:
            puts("PLUS_TOKEN");
            break;
        case MOINS_TOKEN:
            puts("MOINS_TOKEN");
            break;
        case MULT_TOKEN:
            puts("MULT_TOKEN");
            break;
        case DIV_TOKEN:
            puts("DIV_TOKEN");
            break;
        case VIR_TOKEN:
            puts("VIR_TOKEN");
            break;
        case EGAL_TOKEN:
            puts("EGAL_TOKEN");
            break;
        case AFF_TOKEN:
            puts("AFF_TOKEN");
            break;
        case INF_TOKEN:
            puts("INF_TOKEN");
            break;
        case INFEG_TOKEN:
            puts("INFEG_TOKEN");
            break;
        case SUP_TOKEN:
            puts("SUP_TOKEN");
            break;
        case SUPEG_TOKEN:
            puts("SUPEG_TOKEN");
            break;
        case DIFF_TOKEN:
            puts("DIFF_TOKEN");
            break;
        case PO_TOKEN:
            puts("PO_TOKEN");
            break;
        case PF_TOKEN:
            puts("PF_TOKEN");
            break;
        case FIN_TOKEN:
            puts("FIN_TOKEN");
            break;
        case ERREUR_TOKEN:
            puts("ERREUR_TOKEN");
            break;
    }
}

// ================== syntaxique ============================
void Test_Symbole (CODE_LEX cl, Erreurs_t COD_ERR){
    printf("SYM_cur %s___________", SYM_COUR.NOM);
    showCodeToken(SYM_COUR);
    if (SYM_COUR.CODE == cl){
        nextToken();
    }
    else{
        detectError(COD_ERR);
    }
}

void PROGRAM( void){
    // PROGRAM ::= program ID ; BLOCK .
    // printf("program:\n");
    Test_Symbole(PROGRAM_TOKEN, PROGRAM_ERR);
    Test_Symbole(ID_TOKEN, ID_ERR);
    Test_Symbole(PV_TOKEN, PV_ERR);
    BLOCK();
    Test_Symbole(PT_TOKEN, PT_ERR);
    // fin du programme
    if (SYM_COUR.CODE==FIN_TOKEN)
        printf("BRAVO: le programme est correcte!!!");
    else
        printf("PAS BRAVO: fin de programme erronée!!!!");
}

void BLOCK(){
    // BLOCK ::=CONSTS VARS INSTS
    CONSTS();
    VARS();
    INSTS();
}
void CONSTS(){
    // CONSTS ::= const ID = NUM ; { ID = NUM ; } | ep

    switch (SYM_COUR.CODE){
        case CONST_TOKEN:
            nextToken();
            Test_Symbole(ID_TOKEN, ID_ERR);
            Test_Symbole(EGAL_TOKEN, EGAL_ERR);
            Test_Symbole(NUM_TOKEN, NUM_ERR);
            Test_Symbole(PV_TOKEN, PV_ERR);
            
            while(SYM_COUR.CODE == ID_TOKEN){
                nextToken();
                Test_Symbole(EGAL_TOKEN, EGAL_ERR);
                Test_Symbole(NUM_TOKEN, NUM_ERR);
                Test_Symbole(PV_TOKEN, PV_ERR);
            }
            break;
        case VAR_TOKEN: break;
        case BEGIN_TOKEN: break;
        default :
            detectError(CONST_VAR_BEGIN_ERR);
            break;
    }
}
void VARS(){
    // VARS ::= var ID { , ID } ; | eps
    switch (SYM_COUR.CODE){
        case VAR_TOKEN:
            nextToken();
            Test_Symbole(ID_TOKEN, ID_ERR);
            while(SYM_COUR.CODE == VIR_TOKEN){
                nextToken();
                Test_Symbole(ID_TOKEN, ID_ERR);
            }
            Test_Symbole(PV_TOKEN, PV_ERR);
            break;
        case BEGIN_TOKEN: break;
        default :
            detectError(CONST_VAR_BEGIN_ERR);
            break;
    }
}
void INSTS(){
    // INSTS ::= begin INST { ; INST } end
    Test_Symbole(BEGIN_TOKEN, BEGIN_ERR);
    INST();
    while(SYM_COUR.CODE == PV_TOKEN){
        nextToken();
        INST();
    }
    Test_Symbole(END_TOKEN, END_ERR);
}
void INST(){
    // INST ::= INSTS | AFFEC | SI | TANTQUE | ECRIRE | LIRE | eps
    
    switch (SYM_COUR.CODE){
        case BEGIN_TOKEN:
            INSTS();
            break;
        case ID_TOKEN:
            AFFEC();
            break;
        case IF_TOKEN:
            SI();
            break;
        case WHILE_TOKEN:
            TANTQUE();
            break;
        case WRITE_TOKEN:
            ECRIRE();
            break;
        case READ_TOKEN:
            LIRE();
            break;
        case END_TOKEN: break;
        default:
            detectError(INST_END_ERR);
            break;
    }
}
void AFFEC(){
    // ID := EXPR
    Test_Symbole(ID_TOKEN, ID_ERR);
    Test_Symbole(AFF_TOKEN, AFF_ERR);
    EXPR();
}
void SI(){
    // SI ::= if COND then INST
    Test_Symbole(IF_TOKEN, IF_THEN_ERR);
    COND();
    Test_Symbole(THEN_TOKEN, IF_THEN_ERR);
    INST();
}
void TANTQUE(){
    // TANTQUE::= while COND do INST
    Test_Symbole(WHILE_TOKEN, WHILE_DO_ERR);
    COND();
    Test_Symbole(DO_TOKEN, WHILE_DO_ERR);
    INST();
}
void ECRIRE(){
    // ECRIRE ::= write ( EXPR { , EXPR } )
    Test_Symbole(WRITE_TOKEN, WRITE_ERR);
    Test_Symbole(PO_TOKEN, PO_ERR);
    EXPR();
    while(SYM_COUR.CODE == VIR_TOKEN){
        nextToken();
        EXPR();
    }
    Test_Symbole(PF_TOKEN, PF_ERR);
}
void LIRE(){
    // LIRE ::= read ( ID { , ID } )
    Test_Symbole(READ_TOKEN, READ_ERR);
    Test_Symbole(PO_TOKEN, PO_ERR);
    Test_Symbole(ID_TOKEN, ID_ERR);
    while(SYM_COUR.CODE == VIR_TOKEN){
        nextToken();
        Test_Symbole(ID_TOKEN, ID_ERR);
    }
    Test_Symbole(PF_TOKEN, PF_ERR);
}
void COND(){
    // COND ::= EXPR [= | <> | < | > | <= | >=] EXPR
    EXPR();
    if ( (SYM_COUR.CODE != EGAL_TOKEN) && (SYM_COUR.CODE != DIFF_TOKEN) && (SYM_COUR.CODE != INF_TOKEN) && (SYM_COUR.CODE != SUP_TOKEN) && (SYM_COUR.CODE != INFEG_TOKEN)  && (SYM_COUR.CODE != SUPEG_TOKEN)  ) {
        detectError(OP_COMPAR_ERR);
    }
    nextToken();
    EXPR();
}
void EXPR(){
    // EXPR ::= TERM { [+ | -] TERM }
    TERM();
    while( (SYM_COUR.CODE == PLUS_TOKEN)||(SYM_COUR.CODE == MOINS_TOKEN) ){
        printf("SYM_cur %s___________--\n", SYM_COUR.NOM);
        nextToken();
        TERM();
    }
}
void TERM(){
    // TERM ::= FACT { [* | /] FACT }
    FACT();
    while( (SYM_COUR.CODE == MULT_TOKEN)||(SYM_COUR.CODE == DIV_TOKEN) ){
        printf("SYM_cur %s___________---\n", SYM_COUR.NOM);
        nextToken();
        FACT();
    }
}
void FACT(){
    // FACT ::= ID | NUM | ( EXPR )
    if (SYM_COUR.CODE == ID_TOKEN) {
        Test_Symbole(ID_TOKEN, ID_ERR);
    }
    else if (SYM_COUR.CODE == NUM_TOKEN) {
        Test_Symbole(NUM_TOKEN, NUM_ERR);
    }
    else if (SYM_COUR.CODE == PO_TOKEN) {
        nextToken();
        EXPR();
        Test_Symbole(PF_TOKEN, PF_ERR);
    }
    else{
        detectError(FACT_NOT_FOUND_ERR);
    }
    
}

//=================================== lexical================

int estBlanc(char c) {
    return c == ' ' || c == '\t' || c == '\n';
}
int isAccentLettre(){
    // if(-128<Car_Cour && Car_Cour <= -102){ // FROM Ç TO Ü
    if(-96<= Car_Cour && Car_Cour <= -61 ){ //éèçàùûâêîô
        return 1;
    }
    return 0;
}

void lire_Car(){
    Car_Cour = getc(fl);
}


void lire_mot(){
    int lonLex = 0;
    SYM_COUR.NOM[lonLex++] = Car_Cour;
    lire_Car();
    while (isalpha(Car_Cour) || isdigit(Car_Cour)) {
        SYM_COUR.NOM[lonLex++] = Car_Cour;
        lire_Car();
    }
    
    if ( isAccentLettre() ) { //é à ...
        while (isalpha(Car_Cour) || isdigit(Car_Cour) || isAccentLettre() ) {
            SYM_COUR.NOM[lonLex++] = Car_Cour;
            lire_Car();
        }
        detectError(ERR_ID_INV);
        SYM_COUR.CODE = ERREUR_TOKEN;
    }
    else if (lonLex <= 20) {
        // on cherche de symbole
        for(int i = 0; i<nbMotRes; i++){
            if( ! strcmpi(SYM_COUR.NOM, tabToken[i].NOM ) ){
                SYM_COUR.CODE = tabToken[i].CODE;
                break;
            }
            else{
                SYM_COUR.CODE = ID_TOKEN;
            }
        }
    }
    else{
        detectError(ERR_ID_LONG);
        SYM_COUR.CODE = ERREUR_TOKEN;
    }
    //fin du mot
    SYM_COUR.NOM[lonLex++] = '\0';
    ungetc(Car_Cour, fl);
    // printf("SYM_cur %s___________", SYM_COUR.NOM);
    // showCodeToken(SYM_COUR);
}


void lire_nombre(){
    int lonLex = 0;
    SYM_COUR.NOM[lonLex++] = Car_Cour;
    lire_Car();
    while ( isdigit(Car_Cour) ) {
        SYM_COUR.NOM[lonLex++] = Car_Cour;
        lire_Car();
    }

    if ( isalpha(Car_Cour) || isAccentLettre() ) { // identificateur qui commence par des digits
        while ( isalnum(Car_Cour) || isAccentLettre() ) {
            SYM_COUR.NOM[lonLex++] = Car_Cour;
            lire_Car();
        }
        detectError(ERR_ID_INV);
        SYM_COUR.CODE = ERREUR_TOKEN;
    }
    else if (lonLex <= 11) {
        SYM_COUR.CODE = NUM_TOKEN;
    }
    else{ // Nombre est tres long
        detectError(ERR_NBR_LONG);
        SYM_COUR.CODE = ERREUR_TOKEN;
    }
    //fin du mot
    SYM_COUR.NOM[lonLex++] = '\0';
    ungetc(Car_Cour, fl);
}

void lire_spcial(){

        switch( Car_Cour ){
            case ';':
                SYM_COUR.NOM[0] = ';';
                SYM_COUR.NOM[1] = '\0';
                SYM_COUR.CODE = PV_TOKEN;
                break;
            case '.':
                SYM_COUR.NOM[0] = '.';
                SYM_COUR.NOM[1] = '\0';
                SYM_COUR.CODE = PT_TOKEN;
                break;
            case '+':
                SYM_COUR.NOM[0] = '+';
                SYM_COUR.NOM[1] = '\0';
                SYM_COUR.CODE = PLUS_TOKEN;
                break;
            case '-':
                SYM_COUR.NOM[0] = '-';
                SYM_COUR.NOM[1] = '\0';
                SYM_COUR.CODE = MOINS_TOKEN;
                break;
            case '*':
                SYM_COUR.NOM[0] = '*';
                SYM_COUR.NOM[1] = '\0';
                SYM_COUR.CODE = MULT_TOKEN;
                break;
            case '/':
                SYM_COUR.NOM[0] = '/';
                SYM_COUR.NOM[1] = '\0';
                SYM_COUR.CODE = DIV_TOKEN;
                break;
            case ',':
                SYM_COUR.NOM[0] = ',';
                SYM_COUR.NOM[1] = '\0';
                SYM_COUR.CODE = VIR_TOKEN;
                break;
            case '(':
                SYM_COUR.NOM[0] = '(';
                SYM_COUR.NOM[1] = '\0';
                SYM_COUR.CODE = PO_TOKEN;
                break;
            case ')':
                SYM_COUR.NOM[0] = ')';
                SYM_COUR.NOM[1] = '\0';
                SYM_COUR.CODE = PF_TOKEN;
                break;
            case ':':
                lire_Car();
                if( Car_Cour == '='){
                    SYM_COUR.NOM[0] = ':';
                    SYM_COUR.NOM[1] = '=';
                    // SYM_COUR.NOM[2] = '\0';
                    SYM_COUR.CODE = AFF_TOKEN;
                }
                else{
                    //erreur et on a lu un caracter de plus
                    detectError(ERR_OP_INC);
                    ungetc(Car_Cour, fl);
                }
                break;
            case '<':
                lire_Car();
                if( Car_Cour == '='){ // inferieur ou egal
                    SYM_COUR.NOM[0] = '<';
                    SYM_COUR.NOM[1] = '=';
                    // SYM_COUR.NOM[2] = '\0';
                    SYM_COUR.CODE = INFEG_TOKEN;
                }else if( Car_Cour == '>'){ // different
                    SYM_COUR.NOM[0] = '<';
                    SYM_COUR.NOM[1] = '>';
                    // SYM_COUR.NOM[2] = '\0';
                    SYM_COUR.CODE = DIFF_TOKEN;
                }
                else{ // inferieur strictement + caractere lu de plus
                    SYM_COUR.NOM[0] = '<';
                    // SYM_COUR.NOM[1] = '\0';
                    SYM_COUR.CODE = INF_TOKEN;
                    //on a lu un caractere de plus
                    ungetc(Car_Cour, fl);
                }
                break;
            case '>':
                lire_Car();
                if( Car_Cour == '='){ // superieur ou egal
                    SYM_COUR.NOM[0] = '>';
                    SYM_COUR.NOM[1] = '=';
                    // SYM_COUR.NOM[2] = '\0';
                    SYM_COUR.CODE = SUPEG_TOKEN;
                }
                else{ // superieur strictement + caractere lu de plus
                    SYM_COUR.NOM[0] = '>';
                    // SYM_COUR.NOM[1] = '\0';
                    SYM_COUR.CODE = SUP_TOKEN;
                    //on a lu un caractere de plus
                    ungetc(Car_Cour, fl);
                }
                break;
                case '=':
                    SYM_COUR.NOM[0] = '=';
                    SYM_COUR.NOM[1] = '\0';
                    SYM_COUR.CODE = EGAL_TOKEN;
                break;
                case EOF :
                    SYM_COUR.CODE = FIN_TOKEN;
                    break;
            default :
                detectError(ERR_CAR_INC);
                SYM_COUR.CODE = ERREUR_TOKEN;
        }
}
void lire_commentaire(void ){
    int finComment = 0;
    lire_Car();
    if( Car_Cour == '*'){
        lire_Car();
        while(finComment == 0){
            while( (Car_Cour != '*') && (Car_Cour != EOF) ){lire_Car();}
            if( Car_Cour == '*' ){
                lire_Car();
                if( Car_Cour == '}'){ // fin du commentaire
                    finComment = 1;
                    SYM_COUR.NOM[0] = '{';
                    SYM_COUR.NOM[1] = '*';
                    SYM_COUR.NOM[2] = '*';
                    SYM_COUR.NOM[3] = '}';
                    SYM_COUR.NOM[4] = '\0';
                }
            }
            else{ // EOF mais pas fin du commentaire
                detectError(ERR_COM_INC);
                exit(EXIT_FAILURE);
            }
        }
    }
}

int detectError( Erreurs_t er){
    int i = 0;
    for(; i < tailleERR, er != MES_ERR[i].CODE_ERR; i++);
    printf("erreur %s\n", MES_ERR[i].mes);
    exit(EXIT_FAILURE);
}

void nextToken(void) {
    lire_Car();
    memset(SYM_COUR.NOM, '\0', 20); //new word(token)
    while (estBlanc(Car_Cour))
        lire_Car();
    if( isalpha( Car_Cour) || isAccentLettre() ){
        lire_mot();
    }
    else if( isdigit( Car_Cour) ){
            lire_nombre();
    }
    else if( Car_Cour == '{'){ // on supose qu'on va lire un commentaire
            lire_commentaire();
    }
    else{ // caractere speciaux
            lire_spcial();
    }

}

