/*
Projekt do předmětů IFJ a IAL (ZS 2021)
Tým 031, varianta I

Autoři:
xploci01: Peter Pločica
xsouce15: Tomáš Souček
xcolog00: Adam Cologna
*/


typedef struct {
	char *attribute;
	int type; //enum vrací číselné hodnoty
    int length;
} Token_t;

typedef enum {
    start,  
    error,  //lexikální chyba
    id,      //identifikátor
    equals,
    number,
    comma,
    multiply,
    divide,
    plus,
    minus,
    lBracket,
    rBracket,
    not,
    dot,
    colon,
    greater,
    lower,
    hash,
    string,
    stringEscape,
    stringNumEscape,
    comment,
    blockComment,
    lineComment,
    greaterEqual,
    lowerEqual,
    concatenate,
    notEqual,
    doesEqual,
    intDivide,
    numberDot,
    numberExp,
    keywordDo,
    keywordElse,
    keywordEnd,
    keywordFunction,
    keywordGlobal,
    keywordIf,
    keywordInteger,
    keywordLocal,
    keywordNil,
    keywordNumber,
    keywordRequire,
    keywordReturn,
    keywordString,
    keywordThen,
    keywordWhile,
    integer
} State_t;

Token_t getToken();
void printResult(Token_t token);
