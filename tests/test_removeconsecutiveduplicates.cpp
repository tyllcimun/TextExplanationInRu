#include "test_removeconsecutiveduplicates.h"
#include <QtTest/QTest>
#include <expression.h>


test_removeConsecutiveDuplicates::test_removeConsecutiveDuplicates(QObject *parent)
    : QObject{parent}
{}

void test_removeConsecutiveDuplicates::removeConsecutiveDuplicates()
{
    QFETCH(QString, string);
    QFETCH(QString, result);

    // Выполняем удаление дубликатов
    QString actualResult = Expression::removeConsecutiveDuplicates(string);

    // Сравниваем результаты
    if (actualResult != result) {
        qDebug() << "Actual result: " << actualResult;
        qDebug() << "Expected result:" << result;
        QFAIL("Test failed.");
    }
}

void test_removeConsecutiveDuplicates::removeConsecutiveDuplicates_data()
{
    QTest::addColumn<QString>("string");
    QTest::addColumn<QString>("result");

    // Test 1: Строка содержит одно слово
    QTest::newRow("single-word")
        << "слово"
        << "слово";

    // Test 2: Пустая строка
    QTest::newRow("empty-string")
        << ""
        << "";

    // Test 3: Два одинаковых слова подряд
    QTest::newRow("two-identical-words-consecutive")
        << "слово слово"
        << "слово";

    // Test 4: Два одинаковых слова подряд с разным регистром
    QTest::newRow("two-identical-words-different-case")
        << "Слово слово"
        << "Слово";

    // Test 5: Три одинаковых слова подряд с разным регистром
    QTest::newRow("three-identical-words-different-case")
        << "Слово слово слово"
        << "Слово";

    // Test 6: Два одинаковых слова, не стоящих рядом
    QTest::newRow("two-identical-words-non-consecutive")
        << "Мое слово это не твое слово"
        << "Мое слово это не твое слово";

    // Test 7: Два одинаковых слова, не стоящих рядом (короткая строка)
    QTest::newRow("two-identical-words-separated")
        << "слово в слово"
        << "слово в слово";

    // Test 8: Второе слово является подстрокой первого
    QTest::newRow("second-word-is-substring-of-first")
        << "словарные слова"
        << "словарные слова";

    // Test 9: Первое слово является подстрокой второго
    QTest::newRow("first-word-is-substring-of-second")
        << "слова словарные"
        << "слова словарные";

    // Test 10: Строка не содержит одинаковых слов
    QTest::newRow("no-duplicate-words")
        << "Привет мир"
        << "Привет мир";

    // Test 11: Два одинаковых слова подряд, разделенных запятой
    QTest::newRow("two-identical-words-separated-by-comma")
        << "слово, слово"
        << "слово, слово";

    // Test 12: Два не разделенных одинаковых слова
    QTest::newRow("two-identical-words-not-separated")
        << "словослово"
        << "словослово";
}
