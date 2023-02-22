import argparse
import sys
import re
import xml.etree.ElementTree as ET
import os.path
from os import path

class Argument:
    def __init__(self, type, value):
        self.type = type
        self.value = value


class Instruction:
    def __init__(self, order, opcode):
        self.order = order
        self.opcode = opcode
        self.args = []

    def addArgument(self, type, value):
        self.args.append(Argument(type, value))


class Frame:
    def __init__(self, type):
        self.values = {}
        self.type = type

    def add(self, key, value):
        self.values[key] = value

    def isEmpty(self):
        return len(self.values) == 0

class Stack:
    def __init__(self):
        self.frames = []

    def push(self, Frame):
        self.frames.append(Frame)

    def pop(self):
        if self.isEmpty():
            print("prádzný zásobník - neexistující rámec", file=sys.stderr)
            exit(55)
        self.frames.pop()

    def isEmpty(self):
        return len(self.frames) == 0

    def top(self):
        if self.isEmpty():
            print("prádzný zásobník - neexistující rámec", file=sys.stderr)
            exit(55)
        return self.frames[len(self.frames)-1]


#globální proměnné
GF = Frame("GF")
stack = Stack()
callStack = Stack()
dataStack = Stack()


#----------------------------
#      POMOCNÉ FUNKCE
#----------------------------

def checkProgramParams():
    missingArg = 0

    if (programArgs.source == None):
        missingArg += 1
        programArgs.source = sys.stdin
    else:
        if not(path.exists(programArgs.source)):
            print("zadaný soubor nelze otevřít", file=sys.stderr)
            exit(11)

    if (programArgs.input == None):
        missingArg += 1
        programArgs.input = sys.stdin
    else:
        if not(path.exists(programArgs.source)):
            print("zadaný soubor nelze otevřít", file=sys.stderr)
            exit(11)

    if (missingArg > 1):
        print("Alespoň jeden z argumetnů source a input musí být vyplněn", file=sys.stderr)
        exit(10)

def checkNumArguments(args, number):
    if (len(args) != number):
        print("špatný počet argumetnů", file=sys.stderr)
        exit(32)

def checkInt(string):
    string = str(string)
    # https://appdividend.com/2021/03/23/how-to-check-if-string-is-integer-in-python/
    if string[0] in ('-', '+'):
        return string[1:].isdigit()
    return string.isdigit()

def checkBool(arg):
    if((arg.type.upper() == "bool".upper()) and (arg.value.upper() == "true".upper() or arg.value.upper() == "false".upper())):
        return 1
    else:
        return 0

def checkString(arg):
    if ((arg.type.upper() == "string".upper())):
        return 1
    else:
        return 0

def checkFrame(string):
    if (string == "LF"):
        frame = stack.top()
        return 1
    elif (string == "GF"):
        return 1
    elif (string == "TF"):
        try:
            TF
        except NameError:
            print("rámec neexistuje", file=sys.stderr)
            exit(55)

#vrátí požadovaný rámec. Pokud string neurčuje typ rámce, vyhodí chybu
def getFrame(string, shutdown):
    if (string == "LF"):
        checkFrame(string)
        frame = stack.top()
    elif (string == "GF"):
        frame = GF
    elif (string == "TF"):
        checkFrame(string)
        frame = TF
    else:
        if(shutdown == 1):
            print("špatný typ rámce", file=sys.stderr)
            exit(55)
        else:
            if(string=="nil"):
                return "nil"
            else:
                return "konst"

    return frame

def checkVarExistence(frame, var, shutdown):
    if not (var in frame.values):
        if(shutdown == 1):
            print("proměnná v daném rámci neexistuje", file=sys.stderr)
            exit(54)
        else:
            return 0
    else:
        return 1

def escapeToChars(inputString):
    inputString = str(inputString)
    numbers = re.findall('\\\[0-9][0-9][0-9]', inputString)
    numbers = list(dict.fromkeys(numbers))  #odstranění duplikátních dat

    fixedChars = []

    for number in numbers:
        number = re.sub('\\\\0+', '', number) #převedení na celočíselnou hdnotu bez \ a 0 na začátku
        fixedChars.append(chr(int(number)))


    i = 0
    for char in fixedChars:
        inputString = inputString.replace(numbers[i],char)
        i += 1

    return inputString

#rozpozná, jestli je argument instrukce konstanta nebo proměnná a vrátí hodnotu a typ
def getElement(arg):
    arg.value.strip()
    splitted = arg.value.split("@")
    element = getFrame(splitted[0], 0)

    if (element == "konst"):
        element = arg
    elif(element == "nil"):
        element = Argument("nil", "nil")
    else:
        checkVarExistence(element, splitted[1], 1)
        element = element.values[splitted[1]]

    return element


#----------------------------
#      PRÁCE S RÁMCI
#----------------------------

def move(args):
    checkNumArguments(args, 2)

    varSrc = getElement(args[1])


    #cílový rámec
    args[0].value.strip()
    splitted = args[0].value.split("@")

    frameDest = getFrame(splitted[0], 1)

    checkVarExistence(frameDest, splitted[1],1)

    if (not varSrc.type):
        print("MOVE: symboly musí obsahovat hodnoty", file=sys.stderr)
        exit(56)

    frameDest.add(splitted[1],varSrc)


def createframe():
    global TF
    TF = Frame("TF")

def pushframe():

    global TF
    checkFrame("TF")

    TF.type = "LF"
    stack.push(TF)
    del(TF)

def popframe():
    frame = stack.top()
    stack.pop()

    global TF

    TF = frame
    TF.type = "TF"

def defvar(args):
    checkNumArguments(args, 1)

    args[0].value.strip()
    splitted = args[0].value.split("@")


    global TF

    frame = getFrame(splitted[0],1)


    if (not(frame.isEmpty()) and splitted[1] in frame.values):
        print("DEFVAR: proměnná je již v rámci definovaná", file=sys.stderr)
        exit(52)

    res = Argument("", "")
    frame.add(splitted[1], res)

def call(args, index,labels):
    checkNumArguments(args, 1)

    #index += 1
    callStack.push(index)
    return jump(args, labels)

def myReturn():
    if(callStack.isEmpty()):
        print("RETURN: prádzný zásobník volání", file=sys.stderr)
        exit(56)

    index = callStack.top()
    callStack.pop()

    return index


# ------------------------------
#        DATOVÝ ZÁSOBNÍK
# ------------------------------
def pushs(args):
    checkNumArguments(args, 1)

    symb = getElement(args[0])
    if(not symb.type):
        print("PUSHS: neinicializovaná hodnota", file=sys.stderr)
        exit(56)
    dataStack.push(symb)



def pops(args):
    if (dataStack.isEmpty()):
        print("POPS: prádzný zásobník", file=sys.stderr)
        exit(56)

    var = dataStack.top()
    dataStack.pop()

    args[0].value.strip()
    splitted = args[0].value.split("@")

    frame = getFrame(splitted[0], 1)
    checkVarExistence(frame, splitted[1],1)
    frame.add(splitted[1],var)



#---------------------------------------------------------
#      ARITMETICKÉ, RELAČNÍ, BOOL A KONVERZNÍ FUNKCE
#---------------------------------------------------------
def add(args):
    checkNumArguments(args, 3)
    args[0].value.strip()
    splitted = args[0].value.split("@")

    frame = getFrame(splitted[0], 1)

    symb1 = getElement(args[1])
    symb2 = getElement(args[2])

    if (not symb1.value or not symb2.value):
        print("ADD: alespoň jeden z argumentů má chybějící hodnotu", file=sys.stderr)
        exit(56)


    if(symb1.type == "int" and symb2.type == "int" and checkInt(symb1.value) and checkInt(symb2.value)):
        checkVarExistence(frame, splitted[1], 1)
        res = Argument("int", str(int(symb1.value) + int(symb2.value)))
        frame.add(splitted[1], res)

    else:
        if(not symb1.value or not symb2.value):
            print("ADD: alespoň jeden z argumentů má chybějící hodnotu", file=sys.stderr)
            exit(56)

        print("ADD: alespoň jeden z argumentů není int", file=sys.stderr)
        exit(53)

def sub(args):
    checkNumArguments(args, 3)
    args[0].value.strip()
    splitted = args[0].value.split("@")

    frame = getFrame(splitted[0], 1)

    symb1 = getElement(args[1])
    symb2 = getElement(args[2])

    if (not symb1.value or not symb2.value):
        print("SUB: alespoň jeden z argumentů má chybějící hodnotu", file=sys.stderr)
        exit(56)

    if(symb1.type == "int" and symb2.type == "int" and checkInt(symb1.value) and checkInt(symb2.value)):
        checkVarExistence(frame, splitted[1],1)
        res = Argument("int", str(int(symb1.value) - int(symb2.value)))
        frame.add(splitted[1], res)

    else:

        print("SUB: alespoň jeden z argumentů není int", file=sys.stderr)
        exit(53)


def mul(args):
    checkNumArguments(args, 3)
    args[0].value.strip()
    splitted = args[0].value.split("@")

    frame = getFrame(splitted[0], 1)

    symb1 = getElement(args[1])
    symb2 = getElement(args[2])

    if (not symb1.value or not symb2.value):
        print("MUL: alespoň jeden z argumentů má chybějící hodnotu", file=sys.stderr)
        exit(56)

    if (symb1.type == "int" and symb2.type == "int" and checkInt(symb1.value) and checkInt(symb2.value)):
        checkVarExistence(frame, splitted[1], 1)
        res = Argument("int", int(symb1.value) * int(symb2.value))
        frame.add(splitted[1], res)

    else:
        print("MUL: alespoň jeden z argumentů není int", file=sys.stderr)
        exit(53)


def idiv(args):
    checkNumArguments(args, 3)
    args[0].value.strip()
    splitted = args[0].value.split("@")

    frame = getFrame(splitted[0], 1)
    checkVarExistence(frame, splitted[1], 1)

    symb1 = getElement(args[1])
    symb2 = getElement(args[2])

    if (not symb1.value or not symb2.value):
        print("IDIV: symboly musí obsahovat hodnoty", file=sys.stderr)
        exit(56)

    if (symb1.type == "int" and symb2.type == "int" and checkInt(symb1.value) and checkInt(symb2.value)):
        if(symb2.value == "0"):
            print("IDIV: dělení nulou", file=sys.stderr)
            exit(57)

        checkVarExistence(frame, splitted[1], 1)
        res = Argument("int", int(int(symb1.value) / int(symb2.value)))
        frame.add(splitted[1], res)

    else:
        print("IDIV: alespoň jeden z argumentů není int", file=sys.stderr)
        exit(53)


def lt(args):
    checkNumArguments(args, 3)

    args[0].value.strip()
    splitted = args[0].value.split("@")
    var = getFrame(splitted[0],1)
    checkVarExistence(var, splitted[1], 1)

    symb1 = getElement(args[1])
    symb2 = getElement(args[2])

    if (symb1.type != symb2.type or symb1.type == "" or symb2.type == "" or symb1.type == "nil" or symb2.type == "nil"):
        if (not symb1.value or not symb2.value):
            print("LT: symboly musí obsahovat hodnoty", file=sys.stderr)
            exit(56)
        print("LT: symboly nejsou stejného typu", file=sys.stderr)
        exit(53)

    if(symb1.type == "int" and symb2.type == "int"):
        if (int(symb1.value) < int(symb2.value)):
            help = Argument("bool", "true")
            var.add(splitted[1], help)
        else:
            help = Argument("bool", "false")
            var.add(splitted[1], help)
    else:
        if(symb1.value < symb2.value):
            help = Argument("bool", "true")
            var.add(splitted[1],help)
        else:
            help = Argument("bool", "false")
            var.add(splitted[1], help)



def gt(args):
    checkNumArguments(args, 3)

    args[0].value.strip()
    splitted = args[0].value.split("@")
    var = getFrame(splitted[0],1)
    checkVarExistence(var, splitted[1], 1)

    symb1 = getElement(args[1])
    symb2 = getElement(args[2])

    if(symb1.type != symb2.type or symb1.type == "" or symb2.type == "" or symb1.type == "nil" or symb2.type == "nil"):
        if (not symb1.value or not symb2.value):
            print("GT: symboly musí obsahovat hodnoty", file=sys.stderr)
            exit(56)
        print("GT: symboly nejsou stejného typu", file=sys.stderr)
        exit(53)

    if (symb1.type == "int" and symb2.type == "int"):
        if (int(symb1.value) > int(symb2.value)):
            help = Argument("bool", "true")
            var.add(splitted[1], help)
        else:
            help = Argument("bool", "false")
            var.add(splitted[1], help)
    else:
        if(symb1.value > symb2.value):
            help = Argument("bool", "true")
            var.add(splitted[1],help)
        else:
            help = Argument("bool", "false")
            var.add(splitted[1], help)

def eq(args):
    checkNumArguments(args, 3)

    args[0].value.strip()
    splitted = args[0].value.split("@")
    var = getFrame(splitted[0],1)
    checkVarExistence(var, splitted[1], 1)

    symb1 = getElement(args[1])
    symb2 = getElement(args[2])

    nil = False
    if(symb1.type == "nil" or symb2.type == "nil"):
        nil = True

    if(nil == False and symb1.type != symb2.type):
        if(not symb1.value or not symb2.value):
            print("EQ: symboly musí obsahovat hodnoty", file=sys.stderr)
            exit(56)
        print("EQ: symboly nejsou stejného typu", file=sys.stderr)
        exit(53)

    if (symb1.type == "int" and symb2.type == "int"):
        if (int(symb1.value) == int(symb2.value)):
            help = Argument("bool", "true")
            var.add(splitted[1], help)
        else:
            help = Argument("bool", "false")
            var.add(splitted[1], help)
    else:
        if(symb1.value == symb2.value):
            help = Argument("bool", "true")
            var.add(splitted[1],help)
        else:
            help = Argument("bool", "false")
            var.add(splitted[1], help)


def myAnd(args):
    checkNumArguments(args, 3)

    symb1 = getElement(args[1])
    symb2 = getElement(args[2])
    if(checkBool(symb1) and checkBool(symb2)):

        args[0].value.strip()
        splitted = args[0].value.split("@")

        frame = getFrame(splitted[0], 1)
        checkVarExistence(frame, splitted[1], 1)


        if(symb1.value.upper() == "true".upper() and symb2.value.upper() == "true".upper()):
            res = Argument("bool", "true")
            frame.add(splitted[1], res)
        else:
            res = Argument("bool", "false")
            frame.add(splitted[1], res)

    else:
        if (not symb1.value or not symb2.value):
            print("AND: symboly musí obsahovat hodnoty", file=sys.stderr)
            exit(56)
        print("AND: alespoň jeden z argumentů není bool", file=sys.stderr)
        exit(53)



def myOr(args):
    checkNumArguments(args, 3)

    symb1 = getElement(args[1])
    symb2 = getElement(args[2])
    if (checkBool(symb1) and checkBool(symb2)):

        args[0].value.strip()
        splitted = args[0].value.split("@")

        frame = getFrame(splitted[0], 1)
        checkVarExistence(frame, splitted[1], 1)

        if(symb1.value.upper() == "true".upper() or symb2.value.upper() == "true".upper()):
            res = Argument("bool", "true")
            frame.add(splitted[1], res)
        else:
            res = Argument("bool", "false")
            frame.add(splitted[1], res)

    else:
        if (not symb1.value or not symb2.value):
            print("OR: symboly musí obsahovat hodnoty", file=sys.stderr)
            exit(56)
        print("OR: alespoň jeden z argumentů není bool", file=sys.stderr)
        exit(53)

def myNot(args):
    checkNumArguments(args, 2)

    symb1 = getElement(args[1])
    if (checkBool(symb1)):

        args[0].value.strip()
        splitted = args[0].value.split("@")

        frame = getFrame(splitted[0], 1)
        checkVarExistence(frame, splitted[1], 1)

        if(symb1.value.upper() == "true".upper()):
            res = Argument("bool", "false")
            frame.add(splitted[1], res)
        else:
            res = Argument("bool", "true")
            frame.add(splitted[1], res)

    else:
        if (not symb1.value):
            print("NOT: symboly musí obsahovat hodnoty", file=sys.stderr)
            exit(56)
        print("NOT: alespoň jeden z argumentů není bool", file=sys.stderr)
        exit(53)

def int2char(args):
    checkNumArguments(args, 2)

    args[0].value.strip()
    splitted1 = args[0].value.split("@")
    destFrame = getFrame(splitted1[0],1)
    checkVarExistence(destFrame, splitted1[1], 1)

    src = getElement(args[1])

    if (not src.value):
        print("INT2CHAR: nesmí probíhat nad prázdným řetězcem", file=sys.stderr)
        exit(56)

    if(src.type != "int"):
        print("INT2CHAR: špatný typ druhého argumentu", file=sys.stderr)
        exit(53)

    if (checkInt(src.value) and int(src.value) >= 0 and int(src.value) <= 1114111):
        src.value = chr(int(src.value))
        src.type = "string"
        destFrame.add(splitted1[1], src)
    else:
        print("INT2CHAR: nevalidní hodnota znaku v Unicode", file=sys.stderr)
        exit(58)


def stri2int(args):
    checkNumArguments(args, 3)

    args[0].value.strip()
    splitted1 = args[0].value.split("@")
    destFrame = getFrame(splitted1[0],1)


    src = getElement(args[1])
    index = getElement(args[2])

    checkVarExistence(destFrame, splitted1[1], 1)

    if (not src.value or not index.value):
        print("STRI2INT: nesmí probíhat nad prázdným řetězcem", file=sys.stderr)
        exit(56)

    if (src.type != "string" or index.type != "int"):
        print("STRI2INT: špatný typ druhého argumentu", file=sys.stderr)
        exit(53)

    if (checkInt(index.value) and int(index.value) >= 0 and int(index.value) < len(src.value)):
        src.value = str(ord((src.value[int(index.value)])))
        src.type = "int"
        destFrame.add(splitted1[1], src)
    else:
        print("STRI2INT: nevalidní hodnota znaku v Unicode nebo indexace mimo řetězec", file=sys.stderr)
        exit(58)


#----------------------------
#      IN/OUT FUNKCE
#----------------------------

def read(args):
    checkNumArguments(args, 2)

    args[0].value.strip()
    splitted1 = args[0].value.split("@")

    frame = getFrame(splitted1[0], 1)
    checkVarExistence(frame, splitted1[1],1)
    if(programArgs.input != sys.stdin):
        sys.stdin = open(programArgs.input, "r")

    var = input()
    type = args[1].value
    if (type == "int" or type == "string" or type == "bool"):
        if(type == "int"):
            var = Argument("int", var)
        elif (type == "string"):
            var = Argument("string", var)
        else:
            if(var.upper() == "true".upper()):
                var = Argument("bool", "true")
            else:
                var = Argument("bool", "false")

    else:
        var = Argument("nil", "nil")

    frame.add(splitted1[1], var)

def write(args):
    checkNumArguments(args,1)

    #pokud proměnná -> mrkni do vrcholu stacku jestli existuje -> pokud ano vytiskni její hodnotu || pokud ne error
    args[0].value.strip()
    splitted = args[0].value.split("@")


    if(splitted[0] == "GF" or splitted[0] == "TF" or splitted[0]=="LF"):
            frame = getFrame(splitted[0], 1)
            checkVarExistence(frame, splitted[1],1)
            if(not frame.values[splitted[1]].type):
                print("WRITE: neinicializovaná hodnota", file=sys.stderr)
                exit(56)
            out = escapeToChars(frame.values[splitted[1]].value)
            print(out, end='')
    else:
        if(len(splitted) > 1 and splitted[0] == "nil" and splitted[1]=="nil"):
            print("", end='')
        elif(splitted[0] == "bool"):
            print(splitted[1].lower(), end='')

        else:
            args[0].value = escapeToChars(args[0].value)
            print(args[0].value, end='')

    #pokud symbol -> zkontorluj null -> pokud ano vypiš prázdný string || pokud ne vypiš obsah


#----------------------------
#      PRÁCE S ŘETĚZCI
#----------------------------

def concat(args):
    checkNumArguments(args, 3)

    args[0].value.strip()
    splitted = args[0].value.split("@")
    frame = getFrame(splitted[0], 1)
    checkVarExistence(frame, splitted[1], 1)

    concLeft = getElement(args[1])
    concRight = getElement(args[2])

    if(concRight.type != "string" or concLeft.type != "string" ):
        if(not concRight.value or not concLeft.value):
            print("CONCAT: konkatenace nemsmí probíhat nad prázdným řetězcem", file=sys.stderr)
            exit(56)

        print("CONCAT: konkatenace musí probíhat nad 2 řetězci", file=sys.stderr)
        exit(53)

    res = Argument("string", concLeft.value + concRight.value)
    frame.add(splitted[1], res)


def strlen(args):
    checkNumArguments(args, 2)

    args[0].value.strip()
    splittedDest = args[0].value.split("@")
    destFrame = getFrame(splittedDest[0], 1)

    checkVarExistence(destFrame, splittedDest[1], 1)

    src = getElement(args[1])

    if (not src.type):
        print("STRLEN: neinicializovaný řetězec", file=sys.stderr)
        exit(56)

    if (src.type != "string"):
        print("STRLEN: špatný typ druhého argumentu", file=sys.stderr)
        exit(53)

    res = Argument("int", len(src.value))
    destFrame.add(splittedDest[1], res)


def getchar(args):
    checkNumArguments(args, 3)
    args[0].value.strip()
    splitted1 = args[0].value.split("@")
    destFrame = getFrame(splitted1[0], 1)

    src = getElement(args[1])
    index = getElement(args[2])
    checkVarExistence(destFrame, splitted1[1], 1)

    if (not index.value or not src.value):
        print("GETCHAR: chybějící hodnota", file=sys.stderr)
        exit(56)

    if(index.type != "int" or src.type != "string"):
        print("GETCHAR: špatný typ alespoň jednoho argumentu", file=sys.stderr)
        exit(53)

    if (checkInt(index.value) and int(index.value) >= 0 and int(index.value) < len(src.value)):
        destFrame.add(splitted1[1], args[1].value[int(args[2].value)])
    else:
        print("GETCHAR: indexace mimo řetězec", file=sys.stderr)
        exit(58)

    res = Argument("string", src.value[int(index.value)])
    destFrame.add(splitted1[1], res)


def setchar(args):
    checkNumArguments(args, 3)

    args[0].value.strip()
    splitted1 = args[0].value.split("@")
    destFrame = getFrame(splitted1[0], 1)


    src = getElement(args[2])
    index = getElement(args[1])

    checkVarExistence(destFrame, splitted1[1], 1)

    if (not index.value or not destFrame.values[splitted1[1]].value):
        print("SETCHAR: chybějící hodnota", file=sys.stderr)
        exit(56)

    if (index.type != "int" or src.type != "string" or destFrame.values[splitted1[1]].type != "string"):
        print("SETCHAR: špatný typ alespoň jednoho argumentu", file=sys.stderr)
        exit(53)

    if (checkInt(index.value) and int(index.value) >= 0 and int(index.value) < len(destFrame.values[splitted1[1]].value) and len(src.value) > 0 ):
        destFrame.values[splitted1[1]].value = destFrame.values[splitted1[1]].value[:int(index.value)] + src.value[0] + destFrame.values[splitted1[1]].value[int(index.value) + 1:]
    else:
        print("SETCHAR: indexace mimo řetězec", file=sys.stderr)
        exit(58)



# ------------------------------
#         PRÁCE S TYPY
# ------------------------------
def type(args):
    checkNumArguments(args, 2)

    args[0].value.strip()
    splitted1 = args[0].value.split("@")

    frame = getFrame(splitted1[0],1)
    checkVarExistence(frame, splitted1[1],1)

    #tím, že se musím chovat k neinicializované proměnné jinak, než obvykle, nemůžu použít fci getElement() protože ta by zabila program při neinicializované proměnné
    args[1].value.strip()
    splitted = args[1].value.split("@")
    symb = getFrame(splitted[0], 0)
    if(symb != "nil" and symb != "konst"):
        checkVarExistence(symb, splitted[1], 1)

    if (symb == "konst"):
        symb = args[1]
    elif (symb == "nil"):
        symb = Argument("nil", "nil")
    else:
        if not(symb.values[splitted[1]].type):
            symb.type = ""
        else:
            symb = symb.values[splitted[1]]

    res = Argument("type", symb.type)
    frame.add(splitted1[1], res)

# ------------------------------
#      ŘÍZENÍ TOKU PROGRAMU
# ------------------------------

def jump(args, labels):
    checkNumArguments(args,1)

    if not(args[0].value in labels):
        print("JUMP: daný label neexistuje", file=sys.stderr)
        exit(52)

    return int(labels[args[0].value]) #vrátí index PC



def jumpIfEq(args, index, labels):
    checkNumArguments(args, 3)

    symb1 = getElement(args[1])
    symb2 = getElement(args[2])

    if (not symb1.type or not symb2.type):
        print("JUMPIFEQ: nesmí probíhat s neinicializovanou hodnotou", file=sys.stderr)
        exit(56)

    if not(args[0].value in labels):
        print("JUMP: daný label neexistuje", file=sys.stderr)
        exit(52)

    if(symb1.type == symb2.type or symb1.value == "nil" or symb2.value == "nil"):
        if(symb1.value == symb2.value):
            args.pop()
            args.pop()
            return jump(args, labels)
    else:
        print("argumenty nejsou stejného typu a ani jeden z nich není nil")
        exit(53)


    return index


def jumpIfNotEq(args, index, labels):
    checkNumArguments(args, 3)

    symb1 = getElement(args[1])
    symb2 = getElement(args[2])

    if (not symb1.type or not symb2.type):
        print("JUMPIFNEQ: nesmí probíhat s neinicializovanou hodnotou", file=sys.stderr)
        exit(56)

    if not (args[0].value in labels):
        print("JUMPIFNEQ: daný label neexistuje", file=sys.stderr)
        exit(52)

    if (symb1.type == symb2.type or symb1.value == "nil" or symb2.value == "nil"):
        if not(symb1.value == symb2.value):
            tmpArgs = []
            tmpArgs.append(args[0])
            return jump(tmpArgs, labels)
    else:
        print("argumenty nejsou stejného typu a ani jeden z nich není nil")
        exit(53)

    return index


def myExit(args):
    checkNumArguments(args, 1)
    symb = getElement(args[0])
    if(not symb.value):
        print("EXIT: prázdná hodnota proměnné", file=sys.stderr)
        exit(56)
    if(symb.type != "int"):
        print("EXIT: špatný typ", file=sys.stderr)
        exit(53)
    if(symb.value == "" or not(checkInt(symb.value)) or int(symb.value) < 0 or int(symb.value) > 49):
        print("EXIT: nevalidní celočíselná hodnota", file=sys.stderr)
        exit(57)

    exit(int(symb.value))


#----------------------------
#      LADÍCÍ INSTRUKCE
#----------------------------

def dprint(args):
    checkNumArguments(args, 1)
    symb = getElement(args[0])
    print(symb.value, file=sys.stderr)

def myBreak(index):
    print("momentálně se vykonává "+str((index+1))+". instrukce (počítáno od 1)", file=sys.stderr)
    if not(stack.isEmpty()):
        print("LF: " + stack.top().values, file=sys.stderr)
    if(GF):
        print("GF: ", file=sys.stderr, end='')
        for value in GF.values:
            print(value, file=sys.stderr, end=', ')

    print()
    try:
        TF
    except NameError:
        print("", end='')
    else:
        for value in TF.values:
            print(value, file=sys.stderr, end=', ')
    print()


parser = argparse.ArgumentParser()

parser.add_argument("--source", type=str, help="vstupní soubor s XML reprezentací zdrojového kódu")
parser.add_argument("--input", type=str, help="soubor se vstupy pro samotnou interpretaci zadaného zdrojového kódu")

programArgs = parser.parse_args()


checkProgramParams()

try:
    tree = ET.parse(programArgs.source)
except ET.ParseError:
    print("chybný formát XML", file=sys.stderr)
    exit(31)

root = tree.getroot()


#kontrola validity XML
if (root.tag != "program" or root.attrib["language"].upper() != "IPPcode22".upper()):
    print("neočekávaná struktura XML", file=sys.stderr)
    exit(32)

instructions = []
orders = []

for instructionTmp in root:
    if(instructionTmp.tag != "instruction"):
        print("špatná instrukce", file=sys.stderr)
        exit(32)


    instructionTmpKeys = list(instructionTmp.attrib.keys())
    instructionTmpValues = list(instructionTmp.attrib.values())


    if not("order" in instructionTmpKeys and "opcode" in instructionTmpKeys):
        print("chybi order nebo opcode instrukce", file=sys.stderr)
        exit(32)


    if(checkInt(instructionTmpValues[0])):
        instructionTmpValues[0] = int(instructionTmpValues[0])
    else:
        print("nevalidní order", file=sys.stderr)
        exit(32)

    if (instructionTmpValues[0] in orders or int(instructionTmpValues[0]) < 1):
        print("nevalidní order", file=sys.stderr)
        exit(32)
    orders.append(instructionTmpValues[0])

    instruction = Instruction(instructionTmpValues[0], instructionTmpValues[1])

    args = []
    for arg in instructionTmp:
        if (not(re.match("arg[123]",arg.tag))):
            print("špatný argument",file=sys.stderr)
            exit(32)

        args.append(arg)


    #pokud má instrukce argumenty, zajistí aby byly zpracovány ve správném pořadí. Zároveň kontroluje, aby první argument byl arg1
    if(len(args) > 0):
        args = sorted(args, key=lambda argument: argument.tag[-1]) #https://wiki.python.org/moin/HowTo/Sorting#Sortingbykeys

        if(args[0].tag[-1] != "1"):
            print("chybějící argument", file=sys.stderr)
            exit(32)

        for arg in args:
            if not (arg.text):
                arg.text = ""
            instruction.addArgument(arg.attrib["type"], arg.text)

    instructions.append(instruction)


#stack složení z dictionaries jsou rámce
#konkrétní rámec = konkrétní dictionary


#načtení všech labels
labels = {}
q = 0
while q < len(instructions):
    i = instructions[q]
    if(i.opcode == "LABEL"):
        checkNumArguments(i.args,1)

        if (i.args[0].value in labels):
            print("daný label již existuje", file=sys.stderr)
            exit(52)

        labels[i.args[0].value] = q
    q += 1


q = 0

#zajistí aby se instrukce zpracovávaly ve správném pořadí
instructions = sorted(instructions, key=lambda instruction: instruction.order)


while q < len(instructions):
    i = instructions[q]
    #print(str(i.order) + " " + i.opcode)
    i.opcode = i.opcode.upper()
    for f in range(len(i.args)):
        i.args[f].value = escapeToChars(i.args[f].value)

    if(i.opcode == "MOVE"):
        move(i.args)

    elif(i.opcode == "CREATEFRAME"):
        createframe()

    elif (i.opcode == "PUSHFRAME"):
        pushframe()

    elif (i.opcode == "POPFRAME"):
        popframe()

    elif (i.opcode == "DEFVAR"):
        defvar(i.args)

    elif (i.opcode == "CALL"):
        q = call(i.args, q, labels)

    elif (i.opcode == "RETURN"):
        q = myReturn()

    elif (i.opcode == "PUSHS"):
        pushs(i.args)

    elif (i.opcode == "POPS"):
        pops(i.args)

    elif (i.opcode == "ADD"):
        add(i.args)

    elif (i.opcode == "SUB"):
        sub(i.args)

    elif (i.opcode == "MUL"):
        mul(i.args)

    elif (i.opcode == "IDIV"):
        idiv(i.args)

    elif (i.opcode == "LT"):
        lt(i.args)

    elif (i.opcode == "GT"):
        gt(i.args)

    elif (i.opcode == "EQ"):
        eq(i.args)

    elif (i.opcode == "AND"):
        myAnd(i.args)

    elif (i.opcode == "OR"):
        myOr(i.args)

    elif (i.opcode == "NOT"):
        myNot(i.args)

    elif (i.opcode == "INT2CHAR"):
        int2char(i.args)

    elif (i.opcode == "STRI2INT"):
        stri2int(i.args)

    elif (i.opcode == "READ"):
        read(i.args)

    elif (i.opcode == "WRITE"):
        write(i.args)

    elif (i.opcode == "CONCAT"):
        concat(i.args)

    elif (i.opcode == "STRLEN"):
        strlen(i.args)

    elif (i.opcode == "GETCHAR"):
        getchar(i.args)

    elif (i.opcode == "SETCHAR"):
        setchar(i.args)

    elif (i.opcode == "TYPE"):
        type(i.args)

    elif (i.opcode == "LABEL"):
        q += 1
        continue

    elif (i.opcode == "JUMP"):
        q = jump(i.args, labels)

    elif (i.opcode == "JUMPIFEQ"):
        q = jumpIfEq(i.args, q, labels)

    elif (i.opcode == "JUMPIFNEQ"):
        q = jumpIfNotEq(i.args, q, labels)

    elif (i.opcode == "EXIT"):
        myExit(i.args)

    elif (i.opcode == "DPRINT"):
        dprint(i.args)

    elif (i.opcode == "BREAK"):
        myBreak(q)

    else:
        print("neznámý opcode", file=sys.stderr)
        exit(32)

    q += 1
