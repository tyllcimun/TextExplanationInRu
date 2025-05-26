#include "test_iscustomtypewithfileds.h"
#include <QtTest/QTest>
#include <expression.h>

test_isCustomTypeWithFileds::test_isCustomTypeWithFileds(QObject *parent)
    : QObject{parent}
{}

void test_isCustomTypeWithFileds::isCustomTypeWithFileds()
{
    QFETCH(QString, string);
    QFETCH(bool, result);
    QFETCH(Expression, expression);

    QCOMPARE(expression.isCustomTypeWithFields(string), result);
}

void test_isCustomTypeWithFileds::isCustomTypeWithFileds_data()
{
    QTest::addColumn<QString>("string");
    QTest::addColumn<Expression>("expression");
    QTest::addColumn<bool>("result");

    QTest::newRow("empty-data")
        << "a"
        << Expression("", {}, {}, {}, {}, {}, {})
        << false;

    QTest::newRow("empty-name-search-in-filled-data")
        << ""
        << Expression("", {}, {}, {{"c", Union("c")}}, {{"b", Structure("b")}}, {{"a", Class("a")}}, {{"d", Enum("d")}})
        << false;

    QTest::newRow("name-not-in-data-types")
        << "e"
        << Expression("", {}, {}, {{"c", Union("c")}}, {{"b", Structure("b")}}, {{"a", Class("a")}}, {{"d", Enum("d")}})
        << false;

    QTest::newRow("correct-class-name")
        << "a"
        << Expression("", {}, {}, {}, {}, {{"a", Class("a")}}, {})
        << true;

    QTest::newRow("correct-structure-name")
        << "a"
        << Expression("", {}, {}, {}, {{"a", Structure("a")}}, {}, {})
        << true;

    QTest::newRow("correct-union-name")
        << "a"
        << Expression("", {}, {}, {{"a", Union("a")}}, {}, {}, {})
        << true;

    QTest::newRow("correct-enum-name")
        << "a"
        << Expression("", {}, {}, {}, {}, {}, {{"a", Enum("a")}})
        << false;

    QTest::newRow("name-contains-search-string")
        << "a"
        << Expression("", {}, {}, {}, {}, {{"abv", Class("abv")}}, {})
        << false;

    QTest::newRow("case-mismatch-lower-string-upper-type")
        << "a"
        << Expression("", {}, {}, {}, {}, {{"A", Class("A")}}, {})
        << false;

    QTest::newRow("case-mismatch-upper-string-lower-type")
        << "A"
        << Expression("", {}, {}, {}, {}, {{"a", Class("a")}}, {})
        << false;
}
