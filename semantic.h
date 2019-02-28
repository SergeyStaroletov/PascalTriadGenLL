//---------------------------------------------------------------------------
#ifndef semanticH
#define semanticH

#include "defs.h"

struct data {
  Tobj t;        //тип
  Tlex id;       //идентефикатор
  int datatype;  //тип или тип возвр. значения
  union {
    int znach;       //значение для const
    int param;       //число парам для func
    int type_param;  //параметр функции - его тип(по ссылке, значению)
  };
};

//в дереве
struct tree {
  data data;
  tree *p;
  tree *lp;
  tree *rp;
};

void clearThree();
//семантика описаний

data *FindUp(Tlex l);
tree *Sem13();
void Sem14(Tlex name);
void Sem14c(Tlex name);
void Sem14p(Tlex name, bool fr, bool var);
int Sem10(int t);
void Sem15(tree *n, int tt);
void Sem15c(Tlex l, bool flag_minus);
tree *Sem16(Tlex l);
void Sem17(tree *k);
void Sem15f(tree *n, int tt);
void Sem18(tree *k);
//семантика вызовов
int Sem1(Tlex l, bool *var);
int Sem2(Tlex l);

int Sem5(Tlex l, tree **n);

void Sem6(int t, tree *n, int k, bool var);
void Sem7(tree *n, int k);
int Sem71(tree *n);
void Sem8(int t);
int Sem9(Tlex l);
void Sem91(int T1, int T2);

int Sem3(int t1, int t2, int oper);
int Sem31(int t1, int oper);

void printtree();
void semprint(const char *data, char *lex);
//---------------------------------------------------------------------------
#endif
