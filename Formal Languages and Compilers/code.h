/*
Projekt do předmětů IFJ a IAL (ZS 2021)
Tým 031, varianta I

Autoři:
xploci01: Peter Pločica
xsouce15: Tomáš Souček
xcolog00: Adam Cologna
*/

void genMain_start();
void genNumberInt(char* atr);
void genNumberFloat(char* atr);
void genWrite();
void genCallWrite();
void genString(char* atr);
void genFncCall(char* atr);
void genFncStart(char* atr, int labnum);
void genFncEnd(char* name, int labnum, int cnt);
void genDiscard(int cnt);
void genVarUse(char* name, int blkid);
void genNil();
void genVarDef(char* name, int blkid);
void genVarAssign(char* name, int blkid);
void genParamAsign(char* name);
void genExit();
void genReadi();
void genReads();
void genReadn();
void genTointeger();
void genSubstr();
void genOrd();
void genChr();
void genWhileStart(int lab);
void genWhileExpr(int lab);
void genWhileEnd(int lab);
void genIfExpr(int lab);
void genIfThenElse(int lab);
void genIfEnd(int lab);
void genPlus(char t1, char t2);
void genMinus(char t1, char t2);
void genLength();
void genMultiply(char t1, char t2);
void genDivide(char t1, char t2);
void genIntDivide();
void genConcatenate();
void genNotEqual(char t1, char t2);
void genEqual(char t1, char t2);
void genLower(char t1, char t2);
void genGreater(char t1, char t2);
void genLowerEqual(char t1, char t2);
void genGreaterEqual(char t1, char t2);
void genReturn();
void genIntToFloats();
