#include "scaner.h"
#include "defs.h"
extern int uk = 0;
extern int save_pos = 0;
extern int save_col = 0;
extern bool orfo;

Ttext t;
int getuk(void) {
  save_pos = pos;
  save_col = col;
  return uk;
}
void putuk(int p) {
  uk = p;
  pos = save_pos;
  col = save_col;
  return;
}

void getdata(const char *name) {
  uk = 0;
  save_pos = 0;
  save_col = 0;
  for (int j = 0; j < MAXTEXT; j++) t[j] = 0;
  // strcpy(t,"") ;

  FILE *f = fopen(name, "rb");
  if (!f) return;
  fread(t, sizeof(char), MAXTEXT - 2, f);
  orfo = true;
  if (strstr(t, "set orfo to off") != NULL) orfo = false;
  // fgets(t,MAXTEXT,f) ;
  fclose(f);
};

void printerror(const char *lex){
    // printf("[error!] in %s\n",lex);
};

void pr_commenterror(int i) {
  printf("Non-ending comment! in line %d\n", pos);
  exit(1);
};

//\/\/\/\\/\/\/\/\/\\/\/\/\/\/\\/\/\/\/\/\/\/\/\\/\/\/\/\\/\/\\/\\/\/\/\/\\/\/\/\/\/

int scaner(Tlex l) {
  // 14
  const char *sign[] = {"const", "begin",   "end",      "var",   "or",
                        "not",   "xor",     "function", "div",   "mod",
                        "and",   "integer", "true",     "false", "boolean",
                        "while", "do"};
  int tip[] = {Tconst, Tbegin, Tend, Tvar,  Tor,    Tnot,  Txor,   Tfun, Tdiv,
               Tmod,   Tand,   Tint, Ttrue, Tfalse, Tbool, Twhile, Tdo};

  bool iscomment = false;
  int i = 0;
  // memset(l,0,sizeof(l)) ;
  for (int j = 0; j < MAXLEX; j++) l[j] = 0;

start:
  iscomment = false;
  //пропуск незначащих символов
  while (t[uk] == ' ' || t[uk] == '\n' || t[uk] == '\r' || t[uk] == 9) {
    if (t[uk] == 9)
      col = col + 8;
    else if (t[uk] == '\r')
      col = 0;
    else if (t[uk] == '\n')
      pos++;
    else
      col++;
    uk++;
    if (t[uk] == '\0') {
      pos1 = 1;
      return -1;
    }
  };

  //пропуск комментариев

k0:
  if (t[uk] == '{') {
    uk++;
    col++;
    goto k1;
  } else if (t[uk] == '(') {
    uk++;
    col++;
    goto k2;
  } else {
    goto k_fin;
  };
k1:
  if (t[uk] == '\0') {
    pr_commenterror(uk);
    pos1 = 1;
    return -1;
  } else if (t[uk] != '}') {
    if (t[uk] == '\n') {
      pos++;
    } else if (t[uk] == '\r')
      col = 0;
    else
      col++;
    uk++;
    goto k1;
  } else {
    uk++;
    col++;
    iscomment = true;
    goto k_fin;
  };
k2:
  if (t[uk] == '\0') {
    pr_commenterror(uk);
    pos1 = 1;
    return -1;
  } else if (t[uk] == '*') {
    uk++;
    col++;
    goto k3;
  } else {
    //символ ( , а потом нет * - это не коментарий
    uk--;
    col--;
    goto k_fin;
  };
k3:
  if (t[uk] == '\0') {
    pr_commenterror(uk);
    pos1 = 1;
    return -1;
  } else if (t[uk] != '*') {
    if (t[uk] == '\n') {
      pos++;
    } else if (t[uk] == '\r')
      col = 0;
    else
      col++;
    uk++;

    goto k3;
  } else {
    uk++;
    col++;
    goto k4;
  };
k4:
  if (t[uk] == '\0') {
    pr_commenterror(uk);
    pos1 = 1;
    return -1;
  } else if (t[uk] == ')') {
    uk++;
    col++;
    iscomment = true;
    goto k_fin;
  } else {
    if (t[uk] == '\n') {
      pos++;
    } else if (t[uk] == '\r')
      col = 0;
    else
      col++;
    uk++;
    goto k3;
  };
k_fin:
  if (iscomment) goto start;
  //разбор

  //сначала эти как их  идентицфикаторы

id_0:
  if (t[uk] == 0) {
    pos1 = 1;
    return -1;
  };

  if (isalpha(t[uk]) || t[uk] == '_') {
    l[i++] = t[uk++];
    col++;
    pos1 = col;
  id_1:
    if (isalnum(t[uk]) || t[uk] == '_') {
      l[i++] = t[uk++];
      col++;
      if (i < MAXLEX - 1)
        goto id_1;
      else {
        //слишком длинный
        // printerror(l) ; return Tnone ;
        while (isalnum(t[uk]) || t[uk] == '_') {
          uk++;
          col++;
        };
        return Tid;
      };
    } else {
      //закончить идент

      //проверить на ключевое слово
      for (int kl = 0; kl < 17; kl++)
        if (!strcmp((sign[kl]), l)) {
          ;
          return tip[kl];
        };

      return Tid;
    };
  };
//цифра
num_0:
  if ((isalnum(t[uk])) && (!isalpha(t[uk]))) {
    l[i++] = t[uk++];
    col++;
    pos1 = col;
  num_1:
    if ((isalnum(t[uk])) && (!isalpha(t[uk]))) {
      l[i++] = t[uk++];
      col++;
      if (i < MAXLEX - 1)
        goto num_1;
      else {  //длинная цифра
        while ((isalnum(t[uk])) && (!isalpha(t[uk]))) {
          uk++;
          col++;
        };
        return Tnumber;
      };
    } else {
      //вернуть цифру

      return Tnumber;
    };
  };
  //присвоить и двоеточие
  if (t[uk] == ':') {
    l[i++] = t[uk++];
    col++;
    pos1 = col;
    if (t[uk] == '=') {
      l[i++] = t[uk++];
      col++;
      pos1 = col;
      //   col-- ;
      return Tassign;
    } else {
      //   col-- ;
      return Tdv;
    };
  };
  //точка с запятой
  if (t[uk] == ';') {
    l[i++] = t[uk++];
    col++;
    pos1 = col;
    // col-- ;
    return Ttz;
  };
  //двоеточие
  if (t[uk] == ':') {
    l[i++] = t[uk++];
    col++;
    pos1 = col;
    // col-- ;
    return Tdv;
  };
  //  (
  if (t[uk] == '(') {
    l[i++] = t[uk++];
    col++;
    pos1 = col;
    // col-- ;
    return Tos;
  };
  //  )
  if (t[uk] == ')') {
    l[i++] = t[uk++];
    col++;
    pos1 = col;
    // col-- ;
    return Tzs;
  };
  //+
  if (t[uk] == '+') {
    l[i++] = t[uk++];
    col++;
    pos1 = col;
    // col-- ;
    return Tplus;
  };
  //-
  if (t[uk] == '-') {
    l[i++] = t[uk++];
    col++;
    pos1 = col;
    // col-- ;
    return Tminus;
  };
  //умножить
  if (t[uk] == '*') {
    l[i++] = t[uk++];
    col++;
    pos1 = col;
    // col-- ;
    return Tmult;
  };

  //точка
  if (t[uk] == '.') {
    l[i++] = t[uk++];
    col++;
    pos1 = col;
    // col-- ;
    return Tpoint;
  };
  //равно
  if (t[uk] == '=') {
    l[i++] = t[uk++];
    col++;
    pos1 = col;
    // col-- ;
    return Tequ;
  };

  //запятая
  if (t[uk] == ',') {
    l[i++] = t[uk++];
    col++;
    pos1 = col;
    // col-- ;
    return Tpause;
  };

  //Меньше и меньше или равно или неравно
  if (t[uk] == '<') {
    l[i++] = t[uk++];
    col++;
    pos1 = col;
    if (t[uk] == '=') {
      l[i++] = t[uk++];
      col++;
      pos1 = col;
      //    col-- ;
      return Tmenequ;
    } else if (t[uk] == '>') {
      l[i++] = t[uk++];
      col++;
      pos1 = col;
      //    col-- ;
      return Tnonequ;
    } else

      return Tmenee;
  };
  //Больше и больше или равно
  if (t[uk] == '>') {
    l[i++] = t[uk++];
    col++;
    pos1 = col;
    if (t[uk] == '=') {
      l[i++] = t[uk++];
      col++;
      pos1 = col;
      //    col-- ;
      return Tbolequ;
    } else {  // col-- ;
      return Tbolee;
    };
  };
  //конец файла
  if (t[uk] == 0) {
    pos1 = 1;
    return -1;
  };
  //ничто
  l[i++] = t[uk++];
  col++;
  pos1 = col;
  printerror(l);
  return 0;
};
