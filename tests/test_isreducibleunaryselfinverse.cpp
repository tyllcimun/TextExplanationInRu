#include "test_isreducibleunaryselfinverse.h"
#include <QtTest/QTest>
#include <expression.h>


test_isReducibleUnarySelfInverse::test_isReducibleUnarySelfInverse(QObject *parent)
    : QObject{parent}
{}

void test_isReducibleUnarySelfInverse::isReducibleUnarySelfInverse()
{
    QFETCH(OperationType, thisOperType);
    QFETCH(OperationType, leftOperType);
    QFETCH(bool, result);

    ExpressionNode leftNode(EntityType::Operation, "", nullptr, nullptr, "", leftOperType);
    ExpressionNode thisNode(EntityType::Operation, "", &leftNode, nullptr, "", thisOperType);

    QCOMPARE(thisNode.isReducibleUnarySelfInverse(), result);
}

void test_isReducibleUnarySelfInverse::isReducibleUnarySelfInverse_data()
{
    QTest::addColumn<OperationType>("thisOperType");
    QTest::addColumn<OperationType>("leftOperType");
    QTest::addColumn<bool>("result");

    // Тест 1: this - Dereference, left - AddressOf
    // Dereference и AddressOf инверсны.
    QTest::newRow("Dereference-AddressOf")
        << OperationType::Dereference
        << OperationType::AddressOf
        << true;

    // Тест 2: this - AddressOf, left - Dereference
    // AddressOf и Dereference инверсны.
    QTest::newRow("AddressOf-Dereference")
        << OperationType::AddressOf
        << OperationType::Dereference
        << true;

    // Тест 3: this - UnaryMinus, left - UnaryMinus
    // UnaryMinus самообратен.
    QTest::newRow("UnaryMinus-UnaryMinus")
        << OperationType::UnaryMinus
        << OperationType::UnaryMinus
        << true;

    // Тест 4: this - Not, left - Not
    // Not самообратен.
    QTest::newRow("Not-Not")
        << OperationType::Not
        << OperationType::Not
        << true;

    // Некорректные конфигурации
    // Тест 5: this - Dereference, left - UnaryMinus
    // Dereference и UnaryMinus не инверсны.
    QTest::newRow("Dereference-UnaryMinus")
        << OperationType::Dereference
        << OperationType::UnaryMinus
        << false;

    // Тест 6: this - UnaryMinus, left - AddressOf
    // UnaryMinus и AddressOf не инверсны.
    QTest::newRow("UnaryMinus-AddressOf")
        << OperationType::UnaryMinus
        << OperationType::AddressOf
        << false;

    // Тест 7: this - Not, left - Dereference
    // Not и Dereference не инверсны.
    QTest::newRow("Not-Dereference")
        << OperationType::Not
        << OperationType::Dereference
        << false;

    // Тест 8: this - AddressOf, left - Not
    // AddressOf и Not не инверсны.
    QTest::newRow("AddressOf-Not")
        << OperationType::AddressOf
        << OperationType::Not
        << false;
}

