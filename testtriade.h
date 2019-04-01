#ifndef TESTP_H
#define TESTP_H
#include <QCoreApplication>
#include <QtTest/QtTest>

class TestTriade : public QObject {
  Q_OBJECT
 private slots:
  void testTrivial();
  void testExpression();
  void testFunction();
  void testWhile();

 private:
  bool runTest(QString &src, QString &expected);
};
#endif
