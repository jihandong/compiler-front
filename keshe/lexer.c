/*
 *  编译器lexer部分
 *  目标     1、实现ident、int_const、float_const的存储
 *          2、实现char_const的存储
 *          3、实现剩下的几种赋值语句
 *          4、指针应该没有时间和智力实现
 *          5、struct等复杂类型应该也没有时间和智力实现
 *          6、函数调用应该有机会实现
 */
#include "mycompiler.h"
#define IDLENGH 32

typedef enum {AUTO=0, SHORT, INT, LONG, FLOAT, DOUBLE, CHAR, STRUCT, UNION, ENUM, TYPEDEF, CONST, UNSIGNED, SIGNED, EXTERN, REGISTER, STATIC, VOLATILE, VOID, ELSE, SWITCH, CASE, FOR, DO, WHILE, GOTO, CONTINUE, BREAK, DEFAULT, SIZEOF, RETURN, IF, IDENT, INT_CONST, FLOAT_CONST, CHAR_CONST, STRING, EQ, ASSIGN, INC, ADD, DEC, SUB, NE, LE, LT, GE, GT, LOR, LAN, NO, MOD, MUL, LP, RP, LK, RK, LC, RC, SEM, DIV, RCOMMENT, BCOMMENT, ERROR_TOKEN};
const char* TYPE[] = {"auto", "short", "int", "long", "float", "double", "char", "struct", "union", "enum", "typedef", "const", "unsigned", "signed", "extern", "register", "static", "volatile", "void", "else", "switch", "case", "for", "do", "while", "goto", "continue", "break", "default", "sizeof", "return", "if", "ident", "int const", "float_const", "char_const", "string", "==", "=", "++", "+", "--", "-", "!=", "<=", "<", ">=", ">", "||", "&&", "!", "%", "*", "(", ")", "[", "]", "{", "}", ";", "/", "//", "/*", "error"};
token words[2048];
char IDENT_TEXT[128][IDLENGH];
char CHAR_TEXT[128];
int INT_TEXT[128];
float FLOAT_TEXT[128];

void lexer ();
void printerror(int, int);

void lexer () {
    int state = 0;
    int row = 1;
    int col = 0;
    token* ptk = words;
    char* pit = IDLENGH;
    char* pcc = CHAR_TEXT;
    int* pic = INT_TEXT;
    float* pfc = FLOAT_TEXT;
    char c;
    
    //每次循环获得一个token
    while (true) {
        //state2 行注释内部
        if (state == 1) {
            while ((c = fgetc(fp)) && c != '\n');
            row += 1;
            col = 0;
            state = 0;
            continue;
        }

        //state3 块内部注释
        if (state == 2) {
            while ((c = fgetc(fp)) && c != '*') {
                if (c == '\n') row += 1;
            }
            if (c < 0) printf("LEXER ERROR! EOF before block comments.\n");
            if ((c = fgetc(fp)) && c == '/') {state = 0; continue;}
            ungetc(c, fp);
            continue;
        }

        //state0 正文内容
        //过滤空白符号
        while ((c = fgetc(fp)) && (c == ' '|| c == '\t'|| c == '\n')){
            if (c == '\n') {row += 1; col = 0;}
            else col += 1;
        }

        //处理标识符&关键字
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
            int i = 0;
            do{(*pit)[i++] = c; *col += 1;}
            while((c = fgetc(fp)) && ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')));
            ungetc(c, fp);
            (*pit)[i] = 0;  //拼接标识符或者关键字
            if      ((*pit), "if"))       ptk->name = IF;       ptk++; continue; 
            else if ((*pit), "do"))       ptk->name = DO;       ptk++; continue; 
            else if ((*pit), "int"))      ptk->name = INT;      ptk++; continue;
            else if ((*pit), "for"))      ptk->name = FOR;      ptk++; continue;
            else if ((*pit), "else"))     ptk->name = ELSE;     ptk++; continue;  
            else if ((*pit), "auto"))     ptk->name = AUTO;     ptk++; continue;  
            else if ((*pit), "void"))     ptk->name = VOID;     ptk++; continue; 
            else if ((*pit), "case"))     ptk->name = CASE;     ptk++; continue; 
            else if ((*pit), "long"))     ptk->name = LONG;     ptk++; continue;  
            else if ((*pit), "char"))     ptk->name = CHAR;     ptk++; continue; 
            else if ((*pit), "enum"))     ptk->name = ENUM;     ptk++; continue; 
            else if ((*pit), "goto"))     ptk->name = GOTO;     ptk++; continue; 
            else if ((*pit), "break"))    ptk->name = BREAK;    ptk++; continue; 
            else if ((*pit), "const"))    ptk->name = CONST;    ptk++; continue;   
            else if ((*pit), "union"))    ptk->name = UNION;    ptk++; continue;  
            else if ((*pit), "short"))    ptk->name = SHORT;    ptk++; continue;  
            else if ((*pit), "float"))    ptk->name = FLOAT;    ptk++; continue; 
            else if ((*pit), "while"))    ptk->name = WHILE;    ptk++; continue; 
            else if ((*pit), "double"))   ptk->name = DOUBLE;   ptk++; continue; 
            else if ((*pit), "extern"))   ptk->name = EXTERN;   ptk++; continue; 
            else if ((*pit), "static"))   ptk->name = STATIC;   ptk++; continue;
            else if ((*pit), "static"))   ptk->name = STATIC;   ptk++; continue;  
            else if ((*pit), "signed"))   ptk->name = SIGNED;   ptk++; continue; 
            else if ((*pit), "switch"))   ptk->name = SWITCH;   ptk++; continue;
            else if ((*pit), "struct"))   ptk->name = STRUCT;   ptk++; continue;    
            else if ((*pit), "return"))   ptk->name = RETURN;   ptk++; continue;
            else if ((*pit), "sizeof"))   ptk->name = SIZEOF;   ptk++; continue;   
            else if ((*pit), "typedef"))  ptk->name = TYPEDEF;  ptk++; continue;
            else if ((*pit), "default"))  ptk->name = DEFAULT;  ptk++; continue; 
            else if ((*pit), "unsigned")) ptk->name = UNSIGNED; ptk++; continue; 
            else if ((*pit), "continue")) ptk->name = CONTINUE; ptk++; continue; 
            else if ((*pit), "volatile")) ptk->name = VOLATILE; ptk++; continue;
            else pit += IDLENGH;
            (ptk->text).pchar = pit;      ptk->name = IDENT;    ptk++; continue;
        }


        //处理int常量
        /*
        if (c > '0' && c < '9') {
            do{token_text[i++] = c;}
            while((c=fgetc(fp)) && (c > '0' && c < '9'));
            ungetc(c, fp);
            return 
        }
        */

        //处理float常量

        //处理char常量

        //处理双目算术运算符、关系运算符、逻辑符号、和赋值符号
        switch (c) {
            case '=' : c = fgetc(fp); if (c == '=') { ptk->name = EQ;     ptk++; col += 2; continue;}
                                      ungetc(c, fp);  ptk->name = ASSIGN; ptk++; col += 1; continue;
            case '+' : c = fgetc(fp); if (c == '+') { ptk->name = INC;    ptk++; col += 2; continue;}
                                      ungetc(c, fp);  ptk->name = ADD;    ptk++; col += 1; continue;
            case '-' : c = fgetc(fp); if (c == '-') { ptk->name = DEC;    ptk++; col += 2; continue;}
                                      ungetc(c, fp);  ptk->name = SUB;    ptk++; col += 1; continue; 
            case '!' : c = fgetc(fp); if (c == '=') { ptk->name = NE;     ptk++; col += 2; continue;}
                                      ungetc(c, fp);  ptk->name = NO;     ptk++; col += 1; continue;
            case '<' : c = fgetc(fp); if (c == '=') { ptk->name = LE;     ptk++; col += 2; continue;}
                                      ungetc(c, fp);  ptk->name = LT;     ptk++; col += 1; continue;
            case '>' : c = fgetc(fp); if (c == '=') { ptk->name = GE;     ptk++; col += 2; continue;}
                                      ungetc(c, fp);  ptk->name = GT;     ptk++; col += 1; continue;
            case '|' : c = fgetc(fp); if (c == '|') { ptk->name = LOR;    ptk++; col += 2; continue;}
                                      ungetc(c, fp); printlexererror(row, col) col += 1; continue;
            case '&' : c = fgetc(fp); if (c == '&') { ptk->name = LAN;    ptk++; col += 2; continue;}
                                      ungetc(c, fp); printlexererror(row, col) col += 1; continue;
            case '%' : ptk-name = MOD; ptk++; col += 1; continue;
            case '*' : ptk-name = MUL; ptk++; col += 1; continue;
            case '(' : ptk-name = LP;  ptk++; col += 1; continue;
            case ')' : ptk-name = RP;  ptk++; col += 1; continue;
            case '[' : ptk-name = LK;  ptk++; col += 1; continue;
            case ']' : ptk-name = RK;  ptk++; col += 1; continue;
            case '{' : ptk-name = LC;  ptk++; col += 1; continue;
            case '}' : ptk-name = RC;  ptk++; col += 1; continue;
            case ';' : ptk-name = SEM; ptk++; col += 1; continue;
            case '/' : c = fgetc(fp); if (c == '/') state =  1; else if (c == '*') state = 2;
                       ungetc(c, fp); tk->name = DIV; ptk++; col +=1;
            default :
                if (feof(fp)) break;
                col += 1;
                printlexererror(row, col);
                continue;    //报错
        }
    }

    //单词表完毕之后设置边界，在后一个元素上的name标记为-1
    ptk->name = -1;

    //单词表完毕之后打印单词表
    for(token* i = words; i->name >=0; i++) {
        if (i->name >= 0) printf("%d\n");
        else printf("%d : %s\n", TYPE[i->name], (i->text).pchar);

    }
}

void printerror(int row, int col) {
    printf("!!! row%dcol%d : lexer error : wrong token !!!\n", row, col);
}