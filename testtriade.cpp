#include "testtriade.h"

#include "defs.h"
#include "ll.h"
#include "scaner.h"
#include "semantic.h"

bool TestTriade::runTest(QString &src, QString expected) {
  QFile testFile("test.pas");

  if (testFile.open(QIODevice::WriteOnly)) {
    QTextStream out(&testFile);
    out << src;
    testFile.close();
  }

  getdata("test.pas");
  if (syntax() != 0) return false;

  QFile resultFile("triada.ёклмн");
  QString testResult;

  if (resultFile.open(QIODevice::ReadOnly)) {
    QTextStream in(&resultFile);
    testResult = in.readAll();
    testFile.close();
  }

  qDebug() << testResult;
  qDebug() << expected;

  return testResult.trimmed() == expected;
}

void TestTriade::testTrivial() {
  // QSKIP("hi");
  QString src =
      "var a:integer; \
        begin  \
         a:=13; \
        end.";
  QString expected = "1) := a 13";

  QVERIFY(runTest(src, expected));
}

void TestTriade::testExpression() {
  QString src =
      "var a, b, c:integer; \
        begin  \
         a:= 1; \
         b:= 2; \
         c:= ((a + b) div 2) * a * b; \
        end.";
  QString expected =
      "1) := a 1\n\
2) := b 2\n\
3) + a b\n\
4) div (3) 2\n\
5) * (4) a\n\
6) * (5) b\n\
7) := c (6)";

  QVERIFY(runTest(src, expected));
}

void TestTriade::testFunction() {
  QString src =
      "function f(i: integer):integer;\
      begin \
        f:= i + 1;\
      end;\
       var a:integer; \
        begin  \
         a:= f(12356 div 123);\
        end.";
  QString expected =
      "1) proc f 1\n\
2) + i 1\n\
3) := f (2)\n\
4) endp f 1\n\
5) div 12356 123\n\
6) push (5) \n\
7) call f \n\
8) := a (7)";

  QVERIFY(runTest(src, expected));
}

QTEST_MAIN(TestTriade)
