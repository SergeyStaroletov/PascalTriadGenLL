//---------------------------------------------------------------------------

#ifndef triadeH
#define triadeH
#define MAXTRIAD 10000
#include "defs.h"
#include "semantic.h"
//---------------------------------------------------------------------------
typedef struct {
  char op[MAXLEX];
  char op1[MAXLEX];
  char op2[MAXLEX];
} triada;

triada triads[MAXTRIAD];
int K;  //номер триады

typedef struct {
  union {
    int value;
    Tlex ident;
  };
  int tip;  //ссылка или нет
} rs;

rs r[MAXTRIAD];  //магазин значений
int rr;          //указатель

int nmet;

int T[MAXTRIAD];  //магазин типов
int tt;

int par1[MAXTRIAD];           //маг. параметров целых
char par2[MAXTRIAD][MAXLEX];  //маг параметров лексем

int p1;
int p2;

int cp;  //число параметров для функции
int cpv;  //число параметров для функции вызываемой
//тип - ссылка() или нет
#define Link 1984
#define Norma 1808

#define DeltPM 10000
#define DeltV1 10001
#define DeltA11 10002
#define DeltPri 10003
#define DeltaNUllP 10004
#define DeltaSaveP 10005
#define DeltaFunc 10006
#define DeltaGetID 10007
#define DeltaEndFunc 10008
#define DeltaPpp 10009
#define DeltaWhile1 10010
#define DeltaWhile2 10011

#define DeltaWhile3 10012
#define DeltaCHR0 10013
#define DeltCall 10014
#define DeltPush 10015
#define DeltGetCurrTree 10016
#define DeltaZaID 10017
#define DeltaSetTip 10018
#define DeltaSetConst1 10019
#define DeltaSetConst2 10020
#define DeltCreateTreeFun 10021
#define DeltaChangeFun 10022
#define DeltaExitFun 10023
#define DeltaChangeParam 10024
#define DeltaGetTipID 10025

tree *tek;  //текущаяя вершина
// tree *fun ;//указатель на вновь созд. функцию

tree *par3[MAXTRIAD];  //параметры-узлы дерева
int p3;
bool flag_minus;  //для констант
bool is_var;
#endif
