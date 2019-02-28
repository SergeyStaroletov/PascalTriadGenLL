//---------------------------------------------------------------------------
extern bool flag_err;
extern bool flag_err_comp;

void printorfo(const char *text, const char *etalon);
int orfocmp(const char *text, const char *etalon, bool pr);

#define big_macro                            \
  if (t1 == Tid) {                           \
    if (orfocmp(lex, "var", true))           \
      t1 = Tvar;                             \
    else if (orfocmp(lex, "const", true))    \
      t1 = Tconst;                           \
    else if (orfocmp(lex, "function", true)) \
      t1 = Tfun;                             \
    else if (orfocmp(lex, "begin", true))    \
      t1 = Tbegin;                           \
    else if (orfocmp(lex, "end", true))      \
      t1 = Tend;                             \
    else if (orfocmp(lex, "while", true))    \
      t1 = Twhile;                           \
    else if (orfocmp(lex, "integer", true))  \
      t1 = Tint;                             \
    else if (orfocmp(lex, "boolean", true))  \
      t1 = Tbool;                            \
    else if (orfocmp(lex, "true", true))     \
      t1 = Ttrue;                            \
    else if (orfocmp(lex, "false", true))    \
      t1 = Tfalse;                           \
  };

#include "ll.h"
#include "defs.h"
#include "scaner.h"
#include "semantic.h"
#include "triade.h"

//---------------------------------------------------------------------------

int *mag;
int z;
Tlex lex;
int t1;
int pos = 1, col, pos1;

extern bool orfo = true;

void neytro() {
  //входная строка
  int savez;

got:
  savez = z;
  while (t1 != Teof && t1 != Ttz && t1 != Tbegin && t1 != Tend && t1 != Tvar &&
         t1 != Tfun && t1 != Tconst && t1 != Twhile) {
    t1 = scaner(lex);

    big_macro;
  };

  if (t1 == Teof) {
    exit(1);
  };
  //магазин
  // 1) символы из first1
  //
  // while (z!=-1&&mag[z]!=B && mag[z]!=D && mag[z]!=E && mag[z]!=F &&
  // (mag[z]!=G )&& mag[z]!=R1 &&mag[z]!=S &&mag[z]!=C && mag[z]!=U1 &&mag[z]!=V
  // &&mag[z]!=W/*first*/  &&mag[z]!=J && mag[z]!=M) z-- ; last - выкидываем
  // first - нет

  switch (t1) {
    case Ttz:
      do {
        z--;
      } while (z != -1 && mag[z + 1] != U && mag[z + 1] != U1 &&
               mag[z + 1] != J && mag[z + 1] != M);
      if (mag[z + 1] == U || mag[z + 1] == U1)
        z++;
      else if (mag[z + 1] == M || mag[z + 1] == J) {
        t1 = scaner(lex);
        big_macro;
      } else
      // if z==-1
      {  //вернуться назад
        z = savez;
        t1 = scaner(lex);
        big_macro;
        goto got;
      };
      break;

    case Tbegin:
      do {
        z--;
      } while (z != -1 && mag[z + 1] != C && mag[z + 1] != U &&
               mag[z + 1] != V && mag[z + 1] != A);

      if (mag[z + 1] == C || mag[z + 1] == U || mag[z + 1] == V ||
          mag[z + 1] == A)
        z++;
      else {
        z = savez;
        t1 = scaner(lex);
        big_macro;
        goto got;
      };
      break;

    case Tend:
      do
        z--;
      while (z != -1 && mag[z + 1] != C);
      if (z != -1) {
        t1 = scaner(lex);
        big_macro;
      } else {
        z = savez;
        t1 = scaner(lex);
        big_macro;
        goto got;
      };
      break;

    case Tvar:
      do
        z--;
      while (z != -1 && mag[z + 1] != B && mag[z + 1] != D && mag[z + 1] != E &&
             mag[z + 1] != R && mag[z + 1] != S && mag[z + 1] != A);
      if (mag[z + 1] == B || mag[z + 1] == D || mag[z + 1] == E ||
          mag[z + 1] == R || mag[z + 1] == S || mag[z + 1] == A)
        z++;
      else {
        z = savez;
        t1 = scaner(lex);
        big_macro;
        goto got;
      };
      break;

    case Tconst:
      do
        z--;
      while (z != -1 && mag[z + 1] != B && mag[z + 1] != D && mag[z + 1] != F &&
             mag[z + 1] != A);
      if (mag[z + 1] == B || mag[z + 1] == D || mag[z + 1] == F ||
          mag[z + 1] == A)
        z++;
      else {
        z = savez;
        t1 = scaner(lex);
        big_macro;
        goto got;
      };
      break;

    case Tfun:
      do
        z--;
      while (z != -1 && mag[z + 1] != B && mag[z + 1] != D && mag[z + 1] != A);
      if (mag[z + 1] == B || mag[z + 1] == D || mag[z + 1] == A)
        z++;
      else {
        z = savez;
        t1 = scaner(lex);
        big_macro;
        goto got;
      };
      break;

    case Twhile:
      do
        z--;
      while (z != -1 && mag[z + 1] != U && mag[z + 1] != V && mag[z + 1] != W);
      if (mag[z + 1] == U || mag[z + 1] == V || mag[z + 1] == W)
        z++;
      else {
        z = savez;
        t1 = scaner(lex);
        big_macro;
        goto got;
      };
      break;
  };
};

void printorfo(const char *text, const char *etalon) {
  char *s = new char[200];
  char *s1 = new char[200];

  strcpy(s, "[Орфография] Вероятно,вместо '");
  strcat(s, text);
  strcat(s, "' надо было писать '");
  strcat(s, etalon);
  strcat(s, "' в ст.");
  // CharToOem(s,s1) ;
  printf("%s %d(%d)\n", s, pos, pos1);
  flag_err_comp = true;
  delete[] s;
  delete[] s1;
  // getch() ;
  // exit(1) ;
};

int orfocmp(const char *text, const char *etalon, bool pr) {
  int i, j, k;

  if (orfo == false) return 0;
  if (!strcmp(text, etalon)) return 0;

  char *cop = new char[100];
  char *cop2 = new char[100];
  //пропущена одна буква (j-я)

  if (strlen(text) + 1 == strlen(etalon))
    for (j = 0; j < strlen(etalon); j++) {
      k = 0;
      strcpy(cop, "");
      for (i = 0; i < j; i++) cop[k++] = etalon[i];
      for (i = j + 1; i < strlen(etalon); i++) cop[k++] = etalon[i];
      cop[k] = 0;
      if (!strcmp(cop, text))

      {
        delete[] cop2;
        delete[] cop;
        if (pr) printorfo(text, etalon);
        return 1;
      };
    };

  //неверно написана одна буква
  if (strlen(text) == strlen(etalon)) {
    for (j = 0; j < strlen(text); j++) {
      k = 0;
      strcpy(cop, "");

      for (i = 0; i < j; i++) {
        cop[k] = etalon[i];
        cop2[k++] = text[i];
      };

      for (i = j + 1; i < strlen(text); i++) {
        cop[k] = etalon[i];
        cop2[k++] = text[i];
      };

      cop[k] = 0;
      cop2[k] = 0;

      if (!strcmp(cop, cop2)) {
        delete[] cop2;
        delete[] cop;
        if (pr) printorfo(text, etalon);
        return 1;
      };
    };
  };

  //вставлена одна буква
  if (strlen(text) == strlen(etalon) + 1) {
    for (j = 0; j < strlen(text); j++) {
      k = 0;
      strcpy(cop, "");
      for (i = 0; i < j; i++) cop[k++] = text[i];
      for (i = j + 1; i < strlen(text); i++) cop[k++] = text[i];
      cop[k] = 0;

      if (!strcmp(cop, etalon)) {
        delete[] cop2;
        delete[] cop;
        if (pr) printorfo(text, etalon);
        return 1;
      };
    };
  };

  //две соседние буквы переставлены
  if (strlen(text) == strlen(etalon)) {
    strcpy(cop, text);
    for (j = 0; j < strlen(text) - 1; j++) {
      char c = cop[j];
      cop[j] = cop[j + 1];
      cop[j + 1] = c;
      if (!strcmp(cop, etalon)) {
        delete[] cop2;
        delete[] cop;
        if (pr) printorfo(text, etalon);
        return 1;
      }
      c = cop[j];
      cop[j] = cop[j + 1];
      cop[j + 1] = c;
    };
  };
  delete[] cop2;
  delete[] cop;
  return 0;
};

const char *t2lex(int t) {
  switch (t) {
    case Teof:
      return "конец файла";
    case Tnone:
      return "ошибочный символ";
    case Tconst:
      return "const";
    case Tbegin:
      return "begin";
    case Tend:
      return "end";
    case Tvar:
      return "var";
    case Tor:
      return "or";
    case Tnot:
      return "not";
    case Txor:
      return "xor";
    case Tfun:
      return "function";
    case Tand:
      return "and";
    case Tmod:
      return "mod";
    case Tdiv:
      return "div";
    case Tint:
      return "integer";
    case Tbool:
      return "boolean";
    case Ttrue:
      return "true";
    case Tfalse:
      return "false";
    case Twhile:
      return "while";
    case Tdo:
      return "do";
    case Tid:
      return "идентификатор";
    case Tnumber:
      return "целая константа";
    case Tmult:
      return "*";
    case Tpause:
      return ",";
    case Tpoint:
      return ".";
    case Tassign:
      return ":=";
    case Tequ:
      return "=";
    case Ttz:
      return ";";
    case Tdv:
      return ":";
    case Tos:
      return "(";
    case Tzs:
      return ")";
    case Tplus:
      return "+";
    case Tminus:
      return "-";
    case Tmenee:
      return "<";
    case Tbolee:
      return ">";
    case Tbolequ:
      return ">=";
    case Tmenequ:
      return "<=";
    case Tnonequ:
      return "<>";
    default:
      return "ошибочный символ";
  };
};

void printerror(int t, char *lex) {
  char *data = new char[50];
  flag_err_comp = true;
  strcpy(data, t2lex(t));
  char *s = new char[200];
  char *s1 = new char[200];
  if (!strcmp(lex, "")) {
    strcpy(s, "Ошибка! Лишний текст за '.' !");
  } else {
    strcpy(s, "Ошибка! Ожидалось: '");
    strcat(s, data);
    strcat(s, "', найдено: '");
    strcat(s, lex);
    strcat(s, "' в с.");
  };
  // CharToOem(s, s1);
  printf("%s %d(%d)\n", s, pos, pos1);
  delete[] data;
  delete[] s;
  delete[] s1;
  // getch() ;
  // exit(1) ;
  if (lex[0] != 0)
    neytro();
  else
    exit(1);
};

void printerror(const char *data, const char *lex) {
  flag_err_comp = true;
  char *s = new char[200];
  char *s1 = new char[200];
  if (!strcmp(lex, "")) {
    strcpy(s, "Ошибка! Лишний текст за '.' !");
  } else {
    strcpy(s, "Ошибка! Ожидалось: '");
    strcat(s, data);
    strcat(s, "', найдено: '");
    strcat(s, lex);
    strcat(s, "' в с.");
  };
  // CharToOem(s, s1);
  printf("%s %d(%d)\n", s, pos, pos1);
  delete[] s;
  delete[] s1;
  // getch() ;
  // exit(1) ;
  if (lex[0] != 0)
    neytro();
  else
    exit(1);
};

inline void eps() { z--; };

//******************************************************
bool syntax() {
  flag_err_comp = false;
  clearThree();
  mag = new int[30000];  // magazin (big)
  z = -1;
  K = 1;
  tt = 0;
  rr = 0;
  p1 = 1;
  p2 = 1;
  p3 = 1;
  nmet = 0;

  char flag = 1;
  mag[++z] = A;
  t1 = scaner(lex);
  while (flag) {
    if (z == -1 || mag[z] < 6000)  //терминал
    {
      if (z == -1 || mag[z] == t1) {  //
        if (t1 == Teof)
          flag = 0;
        else {
          t1 = scaner(lex);

          if (t1 == Tid) {
            if (orfocmp(lex, "var", true))
              t1 = Tvar;
            else if (orfocmp(lex, "const", true))
              t1 = Tconst;
            else if (orfocmp(lex, "function", true))
              t1 = Tfun;
            else if (orfocmp(lex, "begin", true))
              t1 = Tbegin;
            else if (orfocmp(lex, "end", true))
              t1 = Tend;
            else if (orfocmp(lex, "while", true))
              t1 = Twhile;
            else if (orfocmp(lex, "integer", true))
              t1 = Tint;
            else if (orfocmp(lex, "boolean", true))
              t1 = Tbool;
            else if (orfocmp(lex, "true", true))
              t1 = Ttrue;
            else if (orfocmp(lex, "false", true))
              t1 = Tfalse;
          }
          z--;
        }
      } else
        printerror(mag[z], lex);
    } else {  //нетерминал
      switch (mag[z]) {
        case A:
          if (t1 == Tbegin || t1 == Tvar || t1 == Tconst || t1 == Tfun) {
            mag[z] = Tpoint;
            mag[++z] = C;
            mag[++z] = B;
          } else
            printerror("begin|var|const|function", lex);
          break;
        case B:
          if (t1 == Tvar || t1 == Tconst || t1 == Tfun) {
            mag[z] = B;
            mag[++z] = D;
          } else if (t1 == Tbegin)
            eps();
          else
            printerror("var|const|function", lex);
          break;
        case D:
          if (t1 == Tvar)
            mag[z] = E;
          else if (t1 == Tconst)
            mag[z] = F;
          else if (t1 == Tfun)
            mag[z] = G;
          else
            printerror("var|const|function", lex);
          break;
        case E:
          if (t1 == Tvar) {
            mag[z] = H;
            mag[++z] = Tvar;
          } else
            printerror(Tvar, lex);
          break;
        case H:
          if (t1 == Tid) {
            mag[z] = H1;
            mag[++z] = J;
          } else
            printerror(Tid, lex);
          break;
        case H1:
          if (t1 == Tid) {
            mag[z] = H1;
            mag[++z] = J;
          } else
            eps();
          break;
        case J:
          if (t1 == Tid) {
            //
            mag[z] = DeltaSetTip;
            //
            mag[++z] = Ttz;
            mag[++z] = P;
            mag[++z] = Tdv;
            mag[++z] = I;
            //
            mag[++z] = DeltGetCurrTree;
            //
          } else
            printerror(Tid, lex);
          break;
        case I:
          if (t1 == Tid) {
            mag[z] = DeltaZaID;  //занести
            //
            mag[++z] = I1;
            //
            mag[++z] = Tid;
            mag[++z] = DeltaGetID;
          } else
            printerror(Tid, lex);
          break;
        case Ifun:
          if (t1 == Tid) {
            //
            mag[z] = DeltaPpp;
            //
            mag[++z] = I1fun;
            mag[++z] = Tid;
            mag[++z] = DeltaGetID;
          } else
            printerror(Tid, lex);
          break;

        case I1:
          if (t1 == Tpause) {
            mag[z] = I1;
            mag[++z] = Tid;
            //???
            mag[++z] = DeltaGetID;
            //
            mag[++z] = DeltaZaID;
            //
            mag[++z] = Tpause;
          } else if (t1 == Tdv)
            eps();
          else
            printerror(",|:", lex);
          break;
        case I1fun:
          if (t1 == Tpause) {
            mag[z] = I1fun;
            mag[++z] = Tid;
            mag[++z] = DeltaGetID;
            //
            mag[++z] = DeltaPpp;  //число парам
            //
            mag[++z] = Tpause;
          } else if (t1 == Tdv)
            eps();
          else
            printerror(",|:", lex);
          break;

        case F:
          if (t1 == Tconst) {
            mag[z] = L;
            mag[++z] = Tconst;
          } else
            printerror(Tconst, lex);
          break;
        case L:
          if (t1 == Tid) {
            mag[z] = L1;
            mag[++z] = M;
          } else
            printerror(Tid, lex);
          break;
        case L1:
          if (t1 == Tid) {
            mag[z] = L1;
            mag[++z] = M;
          } else
            eps();
          break;
        case M:
          if (t1 == Tid) {
            //
            mag[z] = DeltaSetConst2;
            //
            mag[++z] = Ttz;
            mag[++z] = Con;
            mag[++z] = Tequ;
            mag[++z] = DeltaSetConst1;
            mag[++z] = Tid;
            //
            mag[++z] = DeltaGetID;  // bl
            //
          } else
            printerror(Tid, lex);
          break;
        case G:
          if (t1 == Tfun) {
            mag[z] = DeltaEndFunc;
            mag[++z] = Ttz;
            mag[++z] = DeltaExitFun;
            mag[++z] = C;
            //
            mag[++z] = DeltaFunc;  //вызвать триаду формирования функции
            //
            mag[++z] = B;
            //
            mag[++z] =
                DeltaSaveP;  //сохранить число параметров, и.к. далее-вызов
            //
            mag[++z] = Ttz;
            mag[++z] = DeltaChangeFun;  //изменить неопред. тип
            mag[++z] = P;
            mag[++z] = Tdv;
            mag[++z] = Tzs;
            mag[++z] = R;
            //
            mag[++z] = DeltaNUllP;  //обнулить число параметров
            //
            //     mag[++z]=DeltGetCurrTree ;
            //
            mag[++z] = Tos;
            mag[++z] = Tid;
            //
            //
            mag[++z] = DeltCreateTreeFun;  //созд. функцию
            mag[++z] = DeltaGetID;  //сохранить в стеке идент.
                                    //

            mag[++z] = Tfun;

          } else
            printerror(Tfun, lex);
          break;
        case P:
          if (t1 == Tint) {
            mag[z] = Tint;
            par1[++p1] = Tint;
          } else if (t1 == Tbool) {
            mag[z] = Tbool;
            par1[++p1] = Tbool;
          } else
            printerror("integer|boolean", lex);
          break;
        case R:
          if (t1 == Tvar || t1 == Tid) {
            //
            //    mag[z]=DeltaPpp ; //увеличить число парамктров
            //
            mag[z] = R1;
            mag[++z] = S;
          } else
            printerror("идентификатор|var", lex);
          break;
        case R1:
          if (t1 == Ttz) {
            //
            //                mag[z]=DeltaPpp ;
            //
            mag[z] = R1;
            mag[++z] = S;
            mag[++z] = Ttz;
          } else if (t1 == Tzs)
            eps();
          else
            printerror(";|)", lex);
          break;
        case S:
          if (t1 == Tvar) {
            is_var = true;
            mag[z] = DeltaChangeParam;
            mag[++z] = P;
            mag[++z] = Tdv;
            mag[++z] = Ifun;
            mag[++z] = Tvar;
            mag[++z] = DeltGetCurrTree;
          } else if (t1 == Tid) {
            is_var = false;
            mag[z] = DeltaChangeParam;
            mag[++z] = P;
            mag[++z] = Tdv;
            mag[++z] = Ifun;
            mag[++z] = DeltGetCurrTree;
          } else
            printerror("идентификатор|var", lex);
          break;
        case C:
          if (t1 == Tbegin) {
            mag[z] = Tend;
            mag[++z] = U;
            mag[++z] = Tbegin;
          }
          break;
        case U:
          if (t1 == Tbegin || t1 == Ttz || t1 == Tid || t1 == Twhile) {
            mag[z] = U1;
            mag[++z] = V;
          } else if (t1 == Tend)
            eps();
          else
            printerror("begin|идентификатор|;|while|end", lex);
          break;
        case U1:
          if (t1 == Ttz) {
            mag[z] = U1;
            mag[++z] = V;
            mag[++z] = Ttz;
          } else if (t1 == Tend)
            eps();
          else
            printerror("end|;", lex);
          break;
        case V:
          if (t1 == Tbegin)
            mag[z] = C;
          else if (t1 == Tid)
            mag[z] = X;
          else if (t1 == Twhile)
            mag[z] = W;
          else if (t1 == Ttz || t1 == Tend)
            eps();
          else
            printerror("begin|идентификатор|while|;|end", lex);
          break;
        case W:
          if (t1 == Twhile) {
            //
            mag[z] = DeltaWhile3;
            //
            mag[++z] = V;
            mag[++z] = Tdo;
            //
            mag[++z] = DeltaWhile2;
            //
            mag[++z] = A0;
            //
            mag[++z] = DeltaWhile1;
            sprintf(par2[++p2], "_m%d", nmet++);  //записать номер позиции
            //
            mag[++z] = Twhile;
          } else
            printerror(Twhile, lex);
          break;
        case X:
          if (t1 == Tid) {
            mag[z] = DeltPri;
            mag[++z] = A0;
            mag[++z] = Tassign;
            //  mag[++z]=Delt15 ;//проверка в дереве
            //лексему- в стек
            // strcpy(par2[++p2],lex) ;
            //
            mag[++z] = DeltaGetTipID;
            mag[++z] = Tid;
            mag[++z] = DeltaGetID;
          } else
            printerror(Tid, lex);
          break;
        case A0:
          if (t1 == Tid || t1 == Tnot || t1 == Tnumber || t1 == Ttrue ||
              t1 == Tfalse || t1 == Tos) {
            mag[z] = V1;
            mag[++z] = A1;
          } else if (t1 == Tplus) {
            mag[z] = V1;
            //
            par1[++p1] = Tplus;
            mag[++z] = DeltPM;
            //
            mag[++z] = A1;
            mag[++z] = Tplus;
          } else if (t1 == Tminus) {
            mag[z] = V1;
            //
            par1[++p1] = Tminus;
            mag[++z] = DeltPM;
            //
            mag[++z] = A1;

            mag[++z] = Tminus;
          } else
            printerror("операнд выражения", lex);
          break;
        case V1:
          switch (t1) {
            case Tbolee:
              mag[z] = V1;
              //
              mag[++z] = DeltV1;
              par1[++p1] = Tbolee;
              //
              mag[++z] = A1;
              mag[++z] = Tbolee;

              break;
            case Tbolequ:
              mag[z] = V1;
              //
              mag[++z] = DeltV1;
              par1[++p1] = Tbolequ;
              //
              mag[++z] = A1;
              mag[++z] = Tbolequ;
              break;
            case Tmenee:
              mag[z] = V1;
              //
              mag[++z] = DeltV1;
              par1[++p1] = Tmenee;
              //
              mag[++z] = A1;
              mag[++z] = Tmenee;
              break;
            case Tmenequ:
              mag[z] = V1;
              //
              mag[++z] = DeltV1;
              par1[++p1] = Tmenequ;
              //
              mag[++z] = A1;
              mag[++z] = Tmenequ;
              break;
            case Tequ:
              mag[z] = V1;
              //
              mag[++z] = DeltV1;
              par1[++p1] = Tequ;
              //
              mag[++z] = A1;
              mag[++z] = Tequ;
              break;
            case Tnonequ:
              mag[z] = V1;
              //
              mag[++z] = DeltV1;
              par1[++p1] = Tnonequ;
              //
              mag[++z] = A1;
              mag[++z] = Tnonequ;
              break;
            default:
              eps();
          }
          break;
        case A1:
          if (t1 == Tid || t1 == Tnot || t1 == Tnumber || t1 == Ttrue ||
              t1 == Tfalse || t1 == Tos) {
            mag[z] = A11;
            mag[++z] = A2;
          } else
            printerror("операнд выражения", lex);
          break;
        case A11:
          if (t1 == Tplus) {
            mag[z] = A11;
            //
            mag[++z] = DeltA11;
            par1[++p1] = Tplus;
            //
            mag[++z] = A2;
            mag[++z] = Tplus;
          } else if (t1 == Tminus) {
            mag[z] = A11;
            //
            mag[++z] = DeltA11;
            par1[++p1] = Tminus;
            //
            mag[++z] = A2;
            mag[++z] = Tminus;
          } else if (t1 == Tor) {
            mag[z] = A11;
            //
            mag[++z] = DeltA11;
            par1[++p1] = Tor;
            //
            mag[++z] = A2;
            mag[++z] = Tor;
          } else if (t1 == Txor) {
            mag[z] = A11;
            //
            mag[++z] = DeltA11;
            par1[++p1] = Txor;
            //
            mag[++z] = A2;
            mag[++z] = Txor;
          } else
            eps();
          break;
        case A2:
          if (t1 == Tid || t1 == Tnot || t1 == Tnumber || t1 == Ttrue ||
              t1 == Tfalse || t1 == Tos) {
            mag[z] = A21;
            mag[++z] = A3;
          } else
            printerror("операнд выражения", lex);
          break;
        case A21:
          if (t1 == Tand) {
            mag[z] = A21;
            //
            mag[++z] = DeltA11;
            par1[++p1] = Tand;
            //
            mag[++z] = A3;
            mag[++z] = Tand;
          } else if (t1 == Tmult) {
            mag[z] = A21;
            //
            mag[++z] = DeltA11;
            par1[++p1] = Tmult;
            //
            mag[++z] = A3;
            mag[++z] = Tmult;
          } else if (t1 == Tdiv) {
            mag[z] = A21;
            //
            mag[++z] = DeltA11;
            par1[++p1] = Tdiv;
            //
            mag[++z] = A3;
            mag[++z] = Tdiv;
          } else if (t1 == Tmod) {
            mag[z] = A21;
            //
            mag[++z] = DeltA11;
            par1[++p1] = Tmod;
            //
            mag[++z] = A3;
            mag[++z] = Tmod;
          } else
            eps();
          break;
        case A3:
          if (t1 == Tid || t1 == Tnumber || t1 == Ttrue || t1 == Tfalse ||
              t1 == Tos) {
            mag[z] = A4;
          } else if (t1 == Tnot) {
            par1[++p1] = Tnot;
            //
            mag[z] = DeltPM;
            //
            mag[++z] = A4;
            mag[++z] = Tnot;
          } else
            printerror("операнд выражения", lex);
          break;
        case A4:
          if (t1 == Tid) {
            int uk;
            uk = getuk();
            int t1;
            Tlex l1;
            t1 = scaner(l1);
            if (t1 == Tos) {  //вызов функции
              putuk(uk);
              //
              mag[z] = DeltCall;
              //
              mag[++z] = Tzs;
              mag[++z] = A5;
              //
              mag[++z] = DeltaCHR0;  //число парам=0
              //
              mag[++z] = Tos;

              mag[++z] = Tid;
              mag[++z] = DeltaGetID;  //получит идент.
            } else {
              putuk(uk);
              mag[z] = Tid;
              //
              strcpy(r[++rr].ident, lex);
              r[rr].tip = Norma;
              bool nnn;
              T[++tt] = Sem1(lex, &nnn);
              //
            }
          } else if (t1 == Tnumber) {
            mag[z] = Tnumber;
            //
            strcpy(r[++rr].ident, lex);
            r[rr].tip = Norma;
            T[++tt] = typeint;
            //
          } else if (t1 == Ttrue) {
            mag[z] = Ttrue;
            //
            strcpy(r[++rr].ident, lex);
            r[rr].tip = Norma;
            T[++tt] = typebool;
            //
          } else if (t1 == Tfalse) {
            mag[z] = Tfalse;
            //
            strcpy(r[++rr].ident, lex);
            r[rr].tip = Norma;
            T[++tt] = typebool;
            //
          } else if (t1 == Tos) {
            mag[z] = Tzs;
            mag[++z] = A0;
            mag[++z] = Tos;
            //???
          } else
            printerror("операнд выражения", lex);
          break;
        case A5:
          if (t1 == Tid || t1 == Tnumber || t1 == Ttrue || t1 == Tfalse ||
              t1 == Tos || t1 == Tplus || t1 == Tminus) {
            //
            //                 mag[z]=DeltPush ;
            //
            mag[z] = A51;
            mag[++z] = DeltPush;
            mag[++z] = A0;
          } else
            printerror("операнд выражения", lex);
          break;
        case A51:
          if (t1 == Tpause) {
            //
            // mag[z]=DeltPush ;
            //
            mag[++z] = A51;
            mag[++z] = DeltPush;
            mag[++z] = A0;
            mag[++z] = Tpause;
          } else
            eps();
          break;
        case Con:
          if (t1 == Tnumber) {
            mag[z] = Tnumber;
            //
            mag[++z] = DeltaGetID;
            //
          } else if (t1 == Ttrue) {
            mag[z] = Ttrue;
            //
            mag[++z] = DeltaGetID;
            //
          } else if (t1 == Tfalse) {
            mag[z] = Tfalse;
            //
            mag[++z] = DeltaGetID;
            //
          } else if (t1 == Tplus) {
            flag_minus = false;
            int uk = getuk();
            Tlex l1;
            int t1 = scaner(l1);
            putuk(uk);
            if (t1 != Tnumber) printerror(Tnumber, l1);
            mag[z] = Tnumber;
            //
            mag[++z] = DeltaGetID;
            //
            mag[++z] = Tplus;
          } else if (t1 == Tminus) {
            flag_minus = true;
            int uk = getuk();
            Tlex l1;
            mag[z] = Tplus;
            int t1 = scaner(l1);
            putuk(uk);
            if (t1 != Tnumber) printerror(Tnumber, l1);
            mag[z] = Tnumber;
            //
            mag[++z] = DeltaGetID;
            //
            mag[++z] = Tminus;
          } else
            printerror("константа", lex);
          break;
        case DeltPM:
          //достать знак операции
          {
            int znak = par1[p1];
            p1--;
            int t1 = T[tt--];
            //      int t2=T[tt--] ;
            T[++tt] = Sem31(t1, znak);
            if (znak == Tplus) {
              triads[K].op[0] = '+';
              triads[K].op[1] = 0;
              sprintf(triads[K].op2, "");
              if (r[rr].tip == Link)
                sprintf(triads[K].op1, "(%d)", r[rr--].value);
              else
                sprintf(triads[K].op1, "%s", r[rr--].ident);
            } else if (znak == Tminus) {
              triads[K].op[0] = '-';
              triads[K].op[1] = 0;
              sprintf(triads[K].op2, "");
              if (r[rr].tip == Link)
                sprintf(triads[K].op1, "(%d)", r[rr--].value);
              else
                sprintf(triads[K].op1, "%s", r[rr--].ident);
            } else {
              strcpy(triads[K].op, "not");
              sprintf(triads[K].op2, "");
              if (r[rr].tip == Link)
                sprintf(triads[K].op1, "(%d)", r[rr--].value);
              else
                sprintf(triads[K].op1, "%s", r[rr--].ident);
            };
            r[++rr].tip = Link;
            r[rr].value = K;
            K++;
            eps();
          }
          break;

        case DeltV1: {
          int znak = par1[p1--];

          int t2 = T[tt--];
          int t1 = T[tt--];
          T[++tt] = Sem3(t1, t2, znak);

          char st[30];
          strcpy(st, t2lex(znak));
          strcpy(triads[K].op, st);

          if (r[rr].tip == Link)
            sprintf(triads[K].op2, "(%d)", r[rr--].value);
          else
            sprintf(triads[K].op2, "%s", r[rr--].ident);

          if (r[rr].tip == Link)
            sprintf(triads[K].op1, "(%d)", r[rr--].value);
          else
            sprintf(triads[K].op1, "%s", r[rr--].ident);
          r[++rr].tip = Link;
          r[rr].value = K;
          K++;

          eps();
        } break;

        case DeltA11: {
          int znak = par1[p1--];
          char st[30];
          int t2 = T[tt--];
          int t1 = T[tt--];
          T[++tt] = Sem3(t1, t2, znak);

          strcpy(st, t2lex(znak));
          strcpy(triads[K].op, st);

          if (r[rr].tip == Link)
            sprintf(triads[K].op2, "(%d)", r[rr--].value);
          else
            sprintf(triads[K].op2, "%s", r[rr--].ident);

          if (r[rr].tip == Link)
            sprintf(triads[K].op1, "(%d)", r[rr--].value);
          else
            sprintf(triads[K].op1, "%s", r[rr--].ident);
          r[++rr].tip = Link;
          r[rr].value = K;
          K++;

          eps();
        } break;
        case DeltPri: {
          char le[MAXLEX];
          int t2 = T[tt--];  //тип выражения
          int t1 = T[tt--];  //тип чему присваивают
          Sem3(t1, t2, Tassign);
          strcpy(le, par2[p2--]);  //название ид
          strcpy(triads[K].op, ":=");
          if (r[rr].tip == Link)
            sprintf(triads[K].op2, "(%d)", r[rr--].value);
          else
            sprintf(triads[K].op2, "%s", r[rr--].ident);

          strcpy(triads[K].op1, le);
          K++;
          eps();
        } break;
        case DeltaNUllP:
          cp = 0;
          eps();
          break;
        case DeltaSaveP:
          par1[++p1] = cp;
          eps();
          break;
        case DeltaGetID:
          strcpy(par2[++p2], lex);
          eps();
          break;
        case DeltaFunc: {
          strcpy(triads[K].op, "proc");
          strcpy(triads[K].op1, par2[p2--]);         //идент
          sprintf(triads[K].op2, "%d", par1[p1--]);  //ч.п.
        }
          par1[++p1] = K;
          K++;
          eps();
          break;
        case DeltaEndFunc: {
          strcpy(triads[K].op, "endp");
          int k1 = par1[p1--];
          strcpy(triads[K].op1, triads[k1].op1);  //идент
          strcpy(triads[K].op2, triads[k1].op2);  //ч.п.
        }
          K++;
          eps();
          break;
        case DeltaPpp:
          cp++;
          Sem14p(par2[p2--], cp == 1, is_var);
          if (cp == 1) tek = tek->p;
          Sem17(par3[p3]);
          eps();
          break;
        case DeltaWhile1:
          strcpy(triads[K].op, "metka");
          strcpy(triads[K].op1, par2[p2--]);
          strcpy(triads[K].op2, "");
          par1[++p1] = K;  //зап. адрес метки
          K++;
          eps();
          break;
        case DeltaWhile2:
          Sem8(T[tt--]);

          strcpy(triads[K].op, "cmp");
          if (r[rr].tip == Link)
            sprintf(triads[K].op1, "(%d)", r[rr--].value);
          else
            sprintf(triads[K].op1, "%s", r[rr--].ident);
          strcpy(triads[K].op2, "");
          K++;
          strcpy(triads[K].op, "jfalse");
          strcpy(triads[K].op1, "");
          strcpy(triads[K].op2, "");
          par1[++p1] = K;  //запомнить. его надо доформировать.
          K++;
          eps();
          break;
        case DeltaWhile3: {
          int kk = par1[p1--];
          strcpy(triads[K].op, "go");
          strcpy(triads[K].op1, triads[par1[p1--]].op1);
          strcpy(triads[K].op2, "");
          K++;
          strcpy(triads[K].op, "metka");
          sprintf(triads[K].op1, "_m%d", nmet);
          strcpy(triads[K].op2, "");
          sprintf(triads[kk].op1, "_m%d", nmet++);  //доф. jz
          K++;
        }
          eps();
          break;
        case DeltaCHR0: {
          par1[++p1] = Sem5(par2[p2], &par3[++p3]);
        }
          par1[++p1] = cpv;
          cpv = 0;

          eps();
          break;

        case DeltCall:
          //проверить число параметров
          Sem7(par3[p3--], cpv);
          //вернуть параметры
          cpv = par1[p1--];
          //вернуть тип
          T[++tt] = par1[p1--];

          strcpy(triads[K].op, "call");
          strcpy(triads[K].op1, par2[p2--]);
          strcpy(triads[K].op2, "");
          r[++rr].tip = Link;
          r[rr].value = K;

          K++;
          eps();
          break;
        case DeltPush: {
          cpv++;
          int t1 = T[tt--];  //тип
          Sem6(t1, par3[p3], cpv, r[rr].tip != Link);
          strcpy(triads[K].op, "push");
          if (r[rr].tip == Link)
            sprintf(triads[K].op1, "(%d)", r[rr--].value);
          else
            sprintf(triads[K].op1, "%s", r[rr--].ident);
          strcpy(triads[K].op2, "");
          K++;
        }

          eps();
          break;
        case DeltGetCurrTree:
          tek = Sem13();
          eps();
          break;
        case DeltaZaID: {
          char st[MAXLEX];
          strcpy(st, par2[p2--]);  //получить ид
          Sem14(st);               //занести
        }
          eps();
          break;
        case DeltaSetTip:  //изменить тип у переменных
        {
          int tt = Sem10(par1[p1--]);
          Sem15(tek, tt);
        }
          eps();
          break;
        case DeltaSetConst1:
          Sem14c(par2[p2--]);
          eps();
          break;
        case DeltaSetConst2:
          Sem15c(par2[p2--], flag_minus);
          eps();
          break;
        case DeltCreateTreeFun:
          par3[++p3] = Sem16(par2[p2]);
          eps();
          break;
        case DeltaChangeFun: {
          int tt = Sem10(par1[p1--]);
          Sem15f(par3[p3], tt);
        }
          eps();
          break;
        case DeltaExitFun:
          Sem18(par3[p3--]);
          eps();
          break;
        case DeltaChangeParam: {
          int tt = Sem10(par1[p1--]);
          Sem15(tek, tt);
        }
          eps();
          break;
        case DeltaGetTipID:
          T[++tt] = Sem9(par2[p2]);
          eps();
          break;

        default:
          printerror("нетерминал", "что-то не то");

      };  // switch mag
    };
  };

  delete[] mag;

  //вывести триады в файл
  FILE *f = fopen("triada.ёклмн", "w");

  for (int i = 1; i < K; i++)
    fprintf(f, "%d) %s %s %s\n", i, triads[i].op, triads[i].op1, triads[i].op2);
  fclose(f);

  return flag_err_comp;
};
