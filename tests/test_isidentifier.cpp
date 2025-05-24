#include "test_isidentifier.h"
#include <QtTest/QTest>
#include <expression.h>


test_isIdentifier::test_isIdentifier(QObject *parent)
    : QObject{parent}
{}

void test_isIdentifier::isIdentifier() {
    QFETCH(QString, string);
    QFETCH(bool, result);
    QFETCH(bool, shouldThrow);

    if (shouldThrow) {
        try {
            // Выполняем функцию и ожидаем выброса исключения
            bool actualResult = Expression::isIdentifier(string);
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
            bool actualResult = Expression::isIdentifier(string);
            QCOMPARE(actualResult, result);
        } catch (const TEException& e) {
            qDebug() << "Unexpected exception occurred.";
            qDebug() << "Exception type: " << TEException::ErrorTypeNames.value(e.getErrorType());
            QFAIL("Unexpected exception thrown.");
        }
    }
}

void test_isIdentifier::isIdentifier_data() {
    QTest::addColumn<QString>("string");
    QTest::addColumn<bool>("result");
    QTest::addColumn<bool>("shouldThrow");
    QTest::addColumn<ErrorType>("expectedError");

    QTest::newRow("first-latin-character") << "a" << true << false;
    QTest::newRow("last-latin-character") << "z" << true << false;
    QTest::newRow("single-uppercase-latin-character") << "V" << true << false;
    QTest::newRow("contains-special-character-underscore") << "_" << true << false;
    QTest::newRow("contains-single-digit") << "1" << false << true << ErrorType::InvalidSymbol;
    QTest::newRow("contains-minimum-range-digit") << "a0" << true << false;
    QTest::newRow("contains-maximum-range-digit") << "a9" << true << false;
    QTest::newRow("starts-with-underscore-and-ends-with-latin") << "_z" << true << false;
    QTest::newRow("empty-string") << "" << false << false;
    QTest::newRow("only-invalid-character") << "$" << false << true << ErrorType::InvalidSymbol;
    QTest::newRow("ends-with-invalid-character") << "a$" << false << true << ErrorType::InvalidSymbol;
    QTest::newRow("contains-invalid-character-in-middle") << "a$a" << false << true << ErrorType::InvalidSymbol;
    QTest::newRow("contains-cyrillic-letter") << "Ъ" << false << true << ErrorType::InvalidSymbol;
}
