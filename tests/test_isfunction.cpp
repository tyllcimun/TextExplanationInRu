#include "test_isfunction.h"
#include <QtTest/QTest>
#include <expression.h>

test_isFunction::test_isFunction(QObject *parent)
    : QObject{parent}
{}

void test_isFunction::isFunction() {
    QFETCH(QString, string);
    QFETCH(bool, result);
    QFETCH(bool, shouldThrow);

    if (shouldThrow) {
        try {
            // Выполняем функцию и ожидаем выброса исключения
            bool actualResult = Expression::isFunction(string);
            qDebug() << "Expected exception but got result:" << actualResult;
            QFAIL("Expected an exception, but none was thrown.");
        } catch (const TEException& e) {
            QFETCH(ErrorType, expectedError);
            // Проверяем, что тип ошибки совпадает с ожидаемым
            ErrorType actualError = e.getErrorType();
            if (actualError != expectedError) {
                QString actualErrorStr = TEException::ErrorTypeNames.value(actualError);
                QString expectedErrorStr = TEException::ErrorTypeNames.value(expectedError);
                qDebug() << "Actual error:  " << actualErrorStr;
                qDebug() << "Expected error:" << expectedErrorStr;
                QFAIL("Error types do not match.");
            } else {
                qDebug() << "Correct exception thrown: " << TEException::ErrorTypeNames.value(actualError);
            }
        }
    } else {
        try {
            // Выполняем функцию и сравниваем результат
            bool actualResult = Expression::isFunction(string);
            QCOMPARE(actualResult, result);
        } catch (const TEException& e) {
            qDebug() << "Unexpected exception occurred.";
            qDebug() << "Exception type: " << TEException::ErrorTypeNames.value(e.getErrorType());
            QFAIL("Unexpected exception thrown.");
        }
    }
}

void test_isFunction::isFunction_data()
{
    QTest::addColumn<QString>("string");
    QTest::addColumn<bool>("result");
    QTest::addColumn<bool>("shouldThrow");
    QTest::addColumn<ErrorType>("expectedError");

    QTest::newRow("function-has-1-argument") << "factorial(1)" << true << false;
    QTest::newRow("function-has-no-arguments") << "helloWorld(0)" << true << false;
    QTest::newRow("function-with-multiple-arguments") << "sum(3,4)" << false << false;
    QTest::newRow("function-has-string-argument") << "printf(oleg)" << false << false;
    QTest::newRow("space-between-function-and-arguments") << "helloWorld (0)" << false  << true << ErrorType::InvalidSymbol;
    QTest::newRow("operand-in-parentheses") << "(3)" << false << false;
    QTest::newRow("regular-string-without-arguments") << "helloWorld" << false << false;
    QTest::newRow("empty-string") << "" << false << false;
    QTest::newRow("function-name-in-cyrillic") << "факториал(1)" << false << true << ErrorType::InvalidSymbol;
}
