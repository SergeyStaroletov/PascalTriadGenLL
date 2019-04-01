#include "testtriade.h"

#include "defs.h"
#include "ll.h"
#include "scaner.h"
#include "semantic.h"

void TestTriade::testTrivial() {
  // QSKIP("hi");
  QString src =
      "var a:integer;"
      " begin  "
      "  a:=13; "
      " end.    ";
  QString expected = "1) := a 13";

  QVERIFY(runTest(src, expected));
}

void TestTriade::testExpression() {
  QString src =
      "var a, b, c:integer; "
      "  begin   "
      "   a:= 1; "
      "   b:= 2; "
      "   c:= ((a + b) div 2) * a * b; "
      "  end.";
  QString expected =
      "1) := a 1"
      "2) := b 2"
      "3) + a b"
      "4) div (3) 2"
      "5) * (4) a"
      "6) * (5) b"
      "7) := c (6)";

  QVERIFY(runTest(src, expected));
}

void TestTriade::testFunction() {
  QString src =
      "function f(i: integer):integer;"
      "begin           "
      "  f:= i + 1;    "
      "end;"
      " var a:integer; "
      "  begin         "
      "   a:= f(12356 div 123);"
      "  end.";
  QString expected =
      "1) proc f 1"  // one local var
      "2) + i 1"
      "3) := f (2)"
      "4) endp f 1"
      "5) div 12356 123"
      "6) push (5) "
      "7) call f "
      "8) := a (7)";

  QVERIFY(runTest(src, expected));
}

void TestTriade::testWhile() {
  QString src =
      "var a:integer; b: integer;\
          begin\
              a:= 0;\
              while a < 10 do\
              begin\
              b:= 5;\
              a:= a - 1;\
              end;\
          end.";
  QString expected =
      "1) := a 0"
      "2) metka _m0 "
      "3) < a 10"
      "4) cmp (3) "
      "5) jfalse _m1 "
      "6) := b 5"
      "7) - a 1"
      "8) := a (7)"
      "9) go _m0 "
      "10) metka _m1 ";

  QVERIFY(runTest(src, expected));
}

// magic
bool TestTriade::runTest(QString &src, QString &expected) {
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

  return testResult.remove('\n') == expected.remove('\n');
}

QTEST_MAIN(TestTriade)
