//---------------------------------------------------------------------------
//#include <vcl.h>
#pragma hdrstop

#include "semantic.h"

bool flag_err;
bool flag_err_comp;

extern int pos;
extern int pos1;

tree *root;
tree *mynode;

void clearThree() {
  root = NULL;
  mynode = NULL;
}

FILE *ff;
void semprint(const char *data, char *lex) {
  char *s = new char[200];
  char *s1 = new char[200];

  strcpy(s, data);
  strcat(s, " в с. %d(%d)!\n");
  // CharToOem(s,s1) ;
  printf(s, lex, pos, pos1);
  delete[] s;
  delete[] s1;
  flag_err = true;
  flag_err_comp = true;

  // getch() ;
  // exit(1) ;
};

void SetLeft(data new_data)
//создать левого потомка от текущей вершины
{
  tree *a;
  a = mynode;
  if (mynode == NULL) {
    mynode = new tree;
    root = mynode;
  } else {
    mynode->lp = new tree;
    mynode = mynode->lp;
  };
  mynode->lp = NULL;
  mynode->rp = NULL;
  mynode->p = a;
  memcpy(&mynode->data, &new_data, sizeof(data));
  return;
};

void SetRight(data new_data) {
  tree *a;
  a = mynode;
  if (mynode == NULL) {
    mynode = new tree;
    root = mynode;
  } else {
    mynode->rp = new tree;
    mynode = mynode->rp;
  };
  mynode->lp = NULL;
  mynode->rp = NULL;
  mynode->p = a;
  memcpy(&mynode->data, &new_data, sizeof(data));
  return;
};

data *FindUpFun(Tlex l)
//найти Идентификатор в таблице , двигаясь вверх и останавливаясь до первой
//функции
{
  tree *tmp, *tp;

  tmp = mynode;
  while (tmp != NULL) {
    if (!strcmp(tmp->data.id, l)) {
      //нашли
      return &tmp->data;
      // break ;
    };
    tp = tmp;
    tmp = tmp->p;
    if (tmp != NULL)
      if (tmp->rp != NULL)
        if (tp == tmp->rp) break;
  };  // wh
  return NULL;
};

data *FindUp(Tlex l)
//найти Идентификатор в таблице , двигаясь вверх
{
  tree *tmp;
  tmp = mynode;
  while (tmp != NULL) {
    if (!strcmp(tmp->data.id, l)) {
      //нашли
      return &tmp->data;
      // break ;
    };
    tmp = tmp->p;
  };  // wh
  return NULL;
};

int a() {
  data new_data;
  Tlex l;
  strcpy(l, "my_id");
  new_data.t = typevar;
  new_data.datatype = typeint;
  strcpy(new_data.id, l);

  SetLeft(new_data);
  SetLeft(new_data);

  strcpy(l, "olya_id");
  strcpy(new_data.id, l);
  SetLeft(new_data);

  data *myd;

  myd = FindUp("myid");

  // int c=2+3 ;
};

tree *Sem13() { return mynode; }

//занести в таблицу без типа
void Sem14(Tlex name) {
  data new_data;
  //проверить

  data *myd = FindUpFun(name);

  if (myd != NULL) {
    //уже есть !
    semprint("Повторный Идентификатор '%s'", name);
    return;
  };
  new_data.t = typevar;
  new_data.datatype = typenone;
  strcpy(new_data.id, name);
  SetLeft(new_data);
};

//определить семантический тип по лексическому
int Sem10(int t) {
  if (t == Tint) return typeint;
  if (t == Tbool) return typebool;
  return typenone;
};

//изменить тип typenone на нужный тип, поднимаясь вверх по дереву
void Sem15(tree *n, int tt) {
  //   |(n)
  // |
  tree *tmp = mynode;

  while (tmp != n) {
    tmp->data.datatype = tt;
    tmp = tmp->p;
  };
};

//занести в таблицу константу без типа
void Sem14c(Tlex name) {
  data new_data;
  //проверить
  data *myd = FindUp(name);
  if (myd != NULL) {
    //уже есть !
    semprint("Повторный Идентификатор '%s'", name);
    return;
  };
  new_data.t = typeconst;
  new_data.datatype = typenone;
  strcpy(new_data.id, name);
  SetLeft(new_data);
};

void Sem15c(Tlex l, bool flag_minus) {
  int z;
  int tip;
  //изменить и тип и значение
  // tree *tmp=mynode ;

  if (l[0] == 'T' || l[0] == 't') {  // bool value
    z = 1;
    tip = typebool;
  } else if (l[0] == 'F' || l[0] == 'f') {
    z = 0;
    tip = typebool;
  } else {
    //пытаемся преобразовать
    char s[40];
    strcpy(s, "0000000000000000000");
    int ofs = 19 - strlen(l);
    for (int j = 0; j < strlen(l); j++) {
      s[ofs + j] = l[j];
    };
    if (strcmp(s, "0000000002147483647") > 0) {
      semprint("Слишком большая числовая константа - %s", l);
      tip = typenone;
    };

    z = atoi(l);

    if (flag_minus) z = -z;
    tip = typeint;
  };
  //теперь занести

  mynode->data.datatype = tip;
  mynode->data.znach = z;
};

//фунции
tree *Sem16(Tlex l) {
  //создать потомка - функцию
  tree *u;
  data new_data;
  //проверить

  data *myd = FindUpFun(l);

  if (myd != NULL) {
    //уже есть !
    semprint("Повторный Идентификатор '%s'", l);
    return NULL;
  };
  new_data.t = typefunc;
  new_data.datatype = typenone;
  new_data.param = 0;
  strcpy(new_data.id, l);
  SetLeft(new_data);
  u = mynode;
  //занесли
  //создать пустого правого потомка
  new_data.t = typenone;
  new_data.datatype = typenone;
  strcpy(new_data.id, "");
  SetRight(new_data);
  return u;
};

//изменить номер параметров у функции
void Sem17(tree *k) {
  if (k == NULL) return;
  k->data.param++;
};

//добавить параметр в функцию
void Sem14p(Tlex name, bool fr, bool var) {
  data new_data;
  //проверить

  data *myd = FindUpFun(name);

  if (myd != NULL) {
    //уже есть !
    semprint("Повторный Идентификатор '%s'", name);
    return;
  };

  new_data.t = typeparam;
  new_data.datatype = typenone;
  if (var)
    new_data.type_param = param_var;
  else
    new_data.type_param = param_;

  strcpy(new_data.id, name);

  if (fr == true) {  //первый вызов - добавить вправо
    memcpy(&mynode->data, &new_data, sizeof(data));
  } else  //иначе - влево
    SetLeft(new_data);
};

//изменить неопред. тип функции
void Sem15f(tree *n, int tt) { n->data.datatype = tt; };

void Sem18(tree *k) { mynode = k; };

int Sem1(Tlex l, bool *var) {
  data *myd = FindUp(l);
  if (myd == NULL)  //не найден
  {
    semprint("Идентификатор %s не описан, ", l);
    return typenone;
  };
  if (myd->t == typefunc) {
    semprint("Неверный идентификатор '%s'", l);
    return typenone;
  };
  if (myd->t == typevar)
    *var = true;
  else
    *var = false;
  return myd->datatype;
};

int Sem2(Tlex l) {
  // 2147483647
  if (!isdigit(l[0])) {
    // true/false
    return typebool;
  };
  char s[40];
  strcpy(s, "0000000000000000000");
  int ofs = 19 - strlen(l);
  for (int j = 0; j < strlen(l); j++) {
    s[ofs + j] = l[j];
  };
  if (strcmp(s, "0000000002147483647") > 0) {
    semprint("Слишком большая числовая константа - %s", l);
    return typenone;
  };
  return typeint;
};

//проверить на функцию
int Sem5(Tlex l, tree **n) {
  *n = NULL;
  data *myd = FindUp(l);
  if (myd == NULL)  //не найден
  {
    semprint("Идентификатор '%s' не описан, ", l);

    return typenone;
  };

  if (myd->t != typefunc) {
    semprint("Идентификатор '%s' не является именем функции, ", l);
    return typenone;
  };

  tree *tmp;
  *n = NULL;

  //найти n

  tmp = mynode;
  while (tmp != NULL) {
    if (!strcmp(tmp->data.id, l)) {
      //нашли
      *n = tmp;
      break;
    };
    tmp = tmp->p;
  };  // wh

  return myd->datatype;
};

//проверить тип параметра с номером k в функции * n на тип t
void Sem6(int t, tree *n, int k, bool var) {
  int j;
  tree *tmp = n;
  if (n == NULL) return;
  if (k > tmp->data.param) {
    semprint("Превышено число параметров у функции '%s'", tmp->data.id);
    return;
  };
  tmp = tmp->rp;  //первый параметр
  for (j = 2; j <= k; j++) tmp = tmp->lp;

  //проверить тип
  if (t != tmp->data.datatype) {
    semprint("Неверный тип параметра '%s'", tmp->data.id);
    return;
  };
  if (tmp->data.type_param == param_var)
    if (!var) {
      semprint("Параметр var '%s' не может быть выражением", tmp->data.id);
      return;
    };
};

void Sem7(tree *n, int k) {
  if (n == NULL) return;
  if (n->data.param != k)
    semprint("Не хватает параметров у функции '%s'", n->data.id);
};

int Sem71(tree *n) {
  if (n == NULL) return typenone;
  return n->data.datatype;
};

int Sem3(int t1, int t2, int oper) {
  switch (oper) {
    case Tplus:
    case Tminus:
    case Tmod:
    case Tdiv:
    case Tmult: {
      if (t1 == typeint && t2 == typeint) return typeint;
      if (t1 == typenone && t2 == typenone) return typenone;
      if (t1 == typenone && t2 == typeint) return typenone;
      if (t1 == typeint && t2 == typenone) return typenone;
      semprint("Несовпадение типов%s", "");
      return typenone;
      break;
    };
    case Tand:
    case Tor:
    case Txor:
    case Tassign: {
      if (t1 == typeint && t2 == typeint) return typeint;
      if (t1 == typebool && t2 == typebool) return typebool;
      if (t1 == typenone && t2 == typenone) return typenone;
      if (t1 == typeint && t2 == typenone) return typenone;
      if (t1 == typenone && t2 == typeint) return typenone;
      if (t1 == typebool && t2 == typenone) return typenone;
      if (t1 == typenone && t2 == typebool) return typenone;
      semprint("Несовпадение типов%s", "");
      return typenone;
    };
    case Tbolee:
    case Tmenee:
    case Tbolequ:
    case Tmenequ:
    case Tequ:
    case Tnonequ: {
      if (t1 == typeint && t2 == typeint) return typebool;
      if (t1 == typebool && t2 == typebool) return typebool;
      if (t1 == typenone && t2 == typenone) return typenone;
      if (t1 == typeint && t2 == typenone) return typenone;
      if (t1 == typenone && t2 == typeint) return typenone;
      if (t1 == typebool && t2 == typenone) return typenone;
      if (t1 == typenone && t2 == typebool) return typenone;
      semprint("Несовпадение типов%s", "");
      return typenone;
    };
  };
};
int Sem31(int t1, int oper) {
  if (oper == Tplus || oper == Tminus) {
    if (t1 == typeint) return typeint;
    if (t1 == typenone) return typenone;
    semprint("Несовпадение типов в унарной операции%s", "");
    return typenone;
  };

  if (oper == Tnot) {
    if (t1 == typeint) return typeint;
    if (t1 == typebool) return typebool;
    if (t1 == typenone) return typebool;
    semprint("Несовпадение типов в унарной операции%s", "");
    return typenone;  //никогда не вып.
  };
  return t1;
};

//***************************************************
void _printtree(tree *t) {
  if (t) {
    fprintf(ff, "%d %s ", t, t->data.id);
    if (t->data.t == typeconst)
      fprintf(ff, "c %d %d", t->data.znach, t->data.datatype);
    if (t->data.t == typevar) fprintf(ff, "v %d 0", t->data.datatype);
    if (t->data.t == typefunc)
      fprintf(ff, "f %d %d", t->data.param, t->data.datatype);
    if (t->data.t == typeparam)
      fprintf(ff, "p %d %d", t->data.type_param, t->data.datatype);
    fprintf(ff, " %d %d %d\n", t->p, t->lp, t->rp);
    if (t->lp) _printtree(t->lp);
    if (t->rp) _printtree(t->rp);
  };
};

int Sem9(Tlex l) {
  data *myd = FindUp(l);
  if (myd == NULL)  //не найден
  {
    semprint("Идентификатор '%s' не описан, ", l);
    return typenone;
  };

  if (myd->t != typevar && myd->t != typefunc) {
    semprint("Идентификатор '%s' не является именем переменной", l);
    return typenone;
  };
  //поиск функции

  if (myd->t == typefunc) {
    tree *tmp, *tp;

    tmp = mynode;
    while (tmp != NULL) {
      tp = tmp;
      tmp = tmp->p;
      if (tmp != NULL)
        if (tmp->rp != NULL)
          if (tp == tmp->rp) {
            if (!strcmp(tmp->data.id, l)) {
              //нашли
              // return &tmp->data ;
              break;
            } else {
              semprint("Неверный оператор возврата значения '%s'", l);
              return typenone;
              break;
            };
          };
    };
  };
  return myd->datatype;
};

void Sem91(int T1, int T2) {
  if (T1 != T2) semprint("Типы данных при присваивании должны совпадать%s", "");
};

void Sem8(int t) {
  if (t != typebool) semprint("Для while нужно выражение типа boolean%s", "");
};

void printtree() {
  ff = fopen("tree.tree", "w");
  _printtree(root);
  fclose(ff);
};

//---------------------------------------------------------------------------
#pragma package(smart_init)
