#include "test_getexplanation.h"
#include <QtTest/QTest>
#include <expression.h>
#include <expressiontranslator.h>
#include <QString>
#include <QHash>
#include <QTest>

Q_DECLARE_METATYPE(Case)
using CaseExplanationHash = QHash<Case, QString>;
Q_DECLARE_METATYPE(CaseExplanationHash)

test_getExplanation::test_getExplanation(QObject *parent)
    : QObject{parent}
{}

void test_getExplanation::getExplanation()
{
    QFETCH(CaseExplanationHash, description);
    QFETCH(QList<CaseExplanationHash>, arguments);
    QFETCH(CaseExplanationHash, expectedResult);

    try {
        // Получаем результат вызова функции
        QHash<Case, QString> actualResult = ExpressionTranslator::getExplanation(description, arguments);

        // Выводим результаты для отладки
        qDebug() << "Description:" << description[Case::Nominative];
        qDebug() << "Arguments:";
        for (int i = 0; i < arguments.size(); ++i) {
            qDebug() << QString("  Argument %1: %2").arg(i + 1).arg(arguments[i].value(Case::Nominative, "N/A"));
        }
        qDebug() << "Actual result:" << actualResult[Case::Nominative];
        qDebug() << "Expected result:" << expectedResult[Case::Nominative];

        // Сравниваем результаты
        if (actualResult != expectedResult) {
            QString errorMessage = QString("Test failed");
            QFAIL(qPrintable(errorMessage));
        }
    } catch (const TEException &error) {
        // Если было выброшено исключение, проверяем тип ошибки
        QFETCH(ErrorType, expectedError);

        qDebug() << "Caught TEException with error type:" << TEException::ErrorTypeNames.value(error.getErrorType(), "UnknownError");
        if (error.getErrorType() != expectedError) {
            QString errorMessage = QString("Test failed");
            QFAIL(qPrintable(errorMessage));
        }
    }
}

void test_getExplanation::getExplanation_data()
{
    QTest::addColumn<QHash<Case, QString>>("description");
    QTest::addColumn<QList<QHash<Case, QString>>>("arguments");
    QTest::addColumn<QHash<Case, QString>>("expectedResult");
    QTest::addColumn<ErrorType>("expectedError");

    // Тест 1: Шаблон описания во всех падежах содержит только одно место для замены в соответствующем падеже
    {
        QHash<Case, QString> description = {
            {Case::Nominative, "{1(и)}"},
            {Case::Genitive, "{1(р)}"},
            {Case::Dative, "{1(д)}"},
            {Case::Accusative, "{1(в)}"},
            {Case::Instrumental, "{1(т)}"},
            {Case::Prepositional, "{1(п)}"}
        };
        QHash<Case, QString> argument = {
            {Case::Nominative, "привет"},
            {Case::Genitive, "привета"},
            {Case::Dative, "привету"},
            {Case::Accusative, "привет"},
            {Case::Instrumental, "приветом"},
            {Case::Prepositional, "привете"}
        };
        QList<QHash<Case, QString>> arguments = {argument};
        QHash<Case, QString> expectedResult = argument;
        QTest::newRow("single-argument-template")
            << description
            << arguments
            << expectedResult;
    }

    // Тест 2: Шаблон описания во всех падежах содержит только одно место для замены, аргумент пустой
    {
        QHash<Case, QString> description = {
            {Case::Nominative, "привет"},
            {Case::Genitive, "привета"},
            {Case::Dative, "привету"},
            {Case::Accusative, "привет"},
            {Case::Instrumental, "приветом"},
            {Case::Prepositional, "привете"}
        };
        QList<QHash<Case, QString>> arguments = {};
        QHash<Case, QString> expectedResult = description;
        QTest::newRow("empty-arguments")
            << description
            << arguments
            << expectedResult;
    }

    // Тест 3: В шаблоне нет мест для замены
    {
        QHash<Case, QString> description = {
            {Case::Nominative, "привет"},
            {Case::Genitive, "привета"},
            {Case::Dative, "привету"},
            {Case::Accusative, "привет"},
            {Case::Instrumental, "приветом"},
            {Case::Prepositional, "привете"}
        };
        QHash<Case, QString> argument = {
            {Case::Nominative, "привет"},
            {Case::Genitive, "привета"},
            {Case::Dative, "привету"},
            {Case::Accusative, "привет"},
            {Case::Instrumental, "приветом"},
            {Case::Prepositional, "привете"}
        };
        QList<QHash<Case, QString>> arguments = {argument};
        QHash<Case, QString> expectedResult = description;
        QTest::newRow("no-placeholders")
            << description
            << arguments
            << expectedResult;
    }

    // Тест 4: В шаблоне место для замены с неподходящим форматом
    {
        QHash<Case, QString> description = {
            {Case::Nominative, "{привет}"},
            {Case::Genitive, "{привет}"},
            {Case::Dative, "{привет}"},
            {Case::Accusative, "{привет}"},
            {Case::Instrumental, "{привет}"},
            {Case::Prepositional, "{привет}"}
        };
        QHash<Case, QString> argument = {
            {Case::Nominative, "привет"},
            {Case::Genitive, "привета"},
            {Case::Dative, "привету"},
            {Case::Accusative, "привет"},
            {Case::Instrumental, "приветом"},
            {Case::Prepositional, "привете"}
        };
        QList<QHash<Case, QString>> arguments = {argument};
        QHash<Case, QString> expectedResult = description;
        QTest::newRow("invalid-placeholder-format")
            << description
            << arguments
            << expectedResult;
    }

    // Тест 5: В шаблоне одно место для замены в соответствующем падеже и подстрока
    {
        QHash<Case, QString> description = {
            {Case::Nominative, "{1(и)} мир!"},
            {Case::Genitive, "{1(р)} миру!"},
            {Case::Dative, "{1(д)} мира!"},
            {Case::Accusative, "{1(в)} миру!"},
            {Case::Instrumental, "{1(т)} миру!"},
            {Case::Prepositional, "{1(п)} миру!"}
        };
        QHash<Case, QString> argument = {
            {Case::Nominative, "привет"},
            {Case::Genitive, "привета"},
            {Case::Dative, "привету"},
            {Case::Accusative, "привет"},
            {Case::Instrumental, "приветом"},
            {Case::Prepositional, "привете"}
        };
        QList<QHash<Case, QString>> arguments = {argument};
        QHash<Case, QString> expectedResult = {
            {Case::Nominative, "привет мир!"},
            {Case::Genitive, "привета миру!"},
            {Case::Dative, "привету мира!"},
            {Case::Accusative, "привет миру!"},
            {Case::Instrumental, "приветом миру!"},
            {Case::Prepositional, "привете миру!"}
        };
        QTest::newRow("placeholder-with-substring")
            << description
            << arguments
            << expectedResult;
    }

    // Тест 6: Шаблон пустой, аргументов нет
    {
        QHash<Case, QString> description = {
            {Case::Nominative, ""},
            {Case::Genitive, ""},
            {Case::Dative, ""},
            {Case::Accusative, ""},
            {Case::Instrumental, ""},
            {Case::Prepositional, ""}
        };
        QList<QHash<Case, QString>> arguments = {{
            {Case::Nominative, ""},
            {Case::Genitive, ""},
            {Case::Dative, ""},
            {Case::Accusative, ""},
            {Case::Instrumental, ""},
            {Case::Prepositional, ""}
        }};
        QHash<Case, QString> expectedResult = {
            {Case::Nominative, ""},
            {Case::Genitive, ""},
            {Case::Dative, ""},
            {Case::Accusative, ""},
            {Case::Instrumental, ""},
            {Case::Prepositional, ""}
        };
        QTest::newRow("empty-template-and-arguments")
            << description
            << arguments
            << expectedResult;
    }

    // Тест 7: В шаблоне два места для замены в соответствующих падежах, не хватает аргументов
    {
        QHash<Case, QString> description = {
            {Case::Nominative, "{1(и)} {3(и)}"},
            {Case::Genitive, "{1(р)} {3(р)}"},
            {Case::Dative, "{1(д)} {3(д)}"},
            {Case::Accusative, "{1(в)} {3(в)}"},
            {Case::Instrumental, "{1(т)} {3(т)}"},
            {Case::Prepositional, "{1(п)} {3(п)}"}
        };
        QHash<Case, QString> argument1 = {
            {Case::Nominative, "{2(и)}"},
            {Case::Genitive, "{2(р)}"},
            {Case::Dative, "{2(д)}"},
            {Case::Accusative, "{2(в)}"},
            {Case::Instrumental, "{2(т)}"},
            {Case::Prepositional, "{2(п)}"}
        };
        QHash<Case, QString> argument2 = {
            {Case::Nominative, "{4(и)}"},
            {Case::Genitive, "{4(р)}"},
            {Case::Dative, "{4(д)}"},
            {Case::Accusative, "{4(в)}"},
            {Case::Instrumental, "{4(т)}"},
            {Case::Prepositional, "{4(п)}"}
        };
        QList<QHash<Case, QString>> arguments = {argument1, argument2};
        QHash<Case, QString> expectedResult = {};
        QTest::newRow("missing-replacement-arguments")
            << description
            << arguments
            << expectedResult
            << ErrorType::MissingReplacementArguments;
    }

    // Тест 8: В шаблоне два места для замены, аргументы содержат строки с местом и обычной строкой
    {
        QHash<Case, QString> description = {
            {Case::Nominative, "{1(и)} {2(и)}"},
            {Case::Genitive, "{1(р)} {2(р)}"},
            {Case::Dative, "{1(д)} {2(д)}"},
            {Case::Accusative, "{1(в)} {2(в)}"},
            {Case::Instrumental, "{1(т)} {2(т)}"},
            {Case::Prepositional, "{1(п)} {2(п)}"}
        };
        QHash<Case, QString> argument1 = {
            {Case::Nominative, "{2(и)}"},
            {Case::Genitive, "{2(р)}"},
            {Case::Dative, "{2(д)}"},
            {Case::Accusative, "{2(в)}"},
            {Case::Instrumental, "{2(т)}"},
            {Case::Prepositional, "{2(п)}"}
        };
        QHash<Case, QString> argument2 = {
            {Case::Nominative, "привет"},
            {Case::Genitive, "привета"},
            {Case::Dative, "привету"},
            {Case::Accusative, "привет"},
            {Case::Instrumental, "приветом"},
            {Case::Prepositional, "привете"}
        };
        QList<QHash<Case, QString>> arguments = {argument1, argument2};
        QHash<Case, QString> expectedResult = {
            {Case::Nominative, "привет привет"},
            {Case::Genitive, "привета привета"},
            {Case::Dative, "привету привету"},
            {Case::Accusative, "привет привет"},
            {Case::Instrumental, "приветом приветом"},
            {Case::Prepositional, "привете привете"}
        };
        QTest::newRow("multiple-placeholders")
            << description
            << arguments
            << expectedResult;
    }

    // Тест 9: Шаблон содержит только место для замены второго аргумента
    {
        QHash<Case, QString> description = {
            {Case::Nominative, "{2(и)}"},
            {Case::Genitive, "{2(р)}"},
            {Case::Dative, "{2(д)}"},
            {Case::Accusative, "{2(в)}"},
            {Case::Instrumental, "{2(т)}"},
            {Case::Prepositional, "{2(п)}"}
        };
        QHash<Case, QString> argument1 = {
            {Case::Nominative, "привет"},
            {Case::Genitive, "привета"},
            {Case::Dative, "привету"},
            {Case::Accusative, "привет"},
            {Case::Instrumental, "приветом"},
            {Case::Prepositional, "привете"}
        };
        QHash<Case, QString> argument2 = {
            {Case::Nominative, "мир"},
            {Case::Genitive, "мира"},
            {Case::Dative, "миру"},
            {Case::Accusative, "мир"},
            {Case::Instrumental, "миром"},
            {Case::Prepositional, "мире"}
        };
        QList<QHash<Case, QString>> arguments = {argument1, argument2};
        QHash<Case, QString> expectedResult = {
            {Case::Nominative, "мир"},
            {Case::Genitive, "мира"},
            {Case::Dative, "миру"},
            {Case::Accusative, "мир"},
            {Case::Instrumental, "миром"},
            {Case::Prepositional, "мире"}
        };
        QTest::newRow("single-placeholder-second-argument")
            << description
            << arguments
            << expectedResult;
    }

    // Тест 10: Шаблон содержит подстроку и место для замены первого аргумента в конце строки
    {
        QHash<Case, QString> description = {
            {Case::Nominative, "привет{1(и)}"},
            {Case::Genitive, "привета{1(р)}"},
            {Case::Dative, "привету{1(д)}"},
            {Case::Accusative, "привет{1(в)}"},
            {Case::Instrumental, "приветом{1(т)}"},
            {Case::Prepositional, "привете{1(п)}"}
        };
        QHash<Case, QString> argument = {
            {Case::Nominative, "мир"},
            {Case::Genitive, "мира"},
            {Case::Dative, "миру"},
            {Case::Accusative, "мир"},
            {Case::Instrumental, "миром"},
            {Case::Prepositional, "мире"}
        };
        QList<QHash<Case, QString>> arguments = {argument};
        QHash<Case, QString> expectedResult = {
            {Case::Nominative, "приветмир"},
            {Case::Genitive, "приветамира"},
            {Case::Dative, "приветумиру"},
            {Case::Accusative, "приветмир"},
            {Case::Instrumental, "приветоммиром"},
            {Case::Prepositional, "приветемире"}
        };
        QTest::newRow("substring-and-placeholder-end")
            << description
            << arguments
            << expectedResult;
    }

    // Тест 11: Шаблон содержит подстроку и место для замены первого аргумента в начале строки
    {
        QHash<Case, QString> description = {
            {Case::Nominative, "{1(и)}мир!"},
            {Case::Genitive, "{1(р)}миру!"},
            {Case::Dative, "{1(д)}мира!"},
            {Case::Accusative, "{1(в)}миру!"},
            {Case::Instrumental, "{1(т)}миру!"},
            {Case::Prepositional, "{1(п)}миру!"}
        };
        QHash<Case, QString> argument = {
            {Case::Nominative, "привет"},
            {Case::Genitive, "привета"},
            {Case::Dative, "привету"},
            {Case::Accusative, "привет"},
            {Case::Instrumental, "приветом"},
            {Case::Prepositional, "привете"}
        };
        QList<QHash<Case, QString>> arguments = {argument};
        QHash<Case, QString> expectedResult = {
            {Case::Nominative, "приветмир!"},
            {Case::Genitive, "приветамиру!"},
            {Case::Dative, "приветумира!"},
            {Case::Accusative, "приветмиру!"},
            {Case::Instrumental, "приветоммиру!"},
            {Case::Prepositional, "приветемиру!"}
        };
        QTest::newRow("substring-and-placeholder-start")
            << description
            << arguments
            << expectedResult;
    }

    // Тест 12: Шаблон содержит вложенные места для замены, два аргумента пустые
    {
        QHash<Case, QString> description = {
            {Case::Nominative, "{3(и){2(и){1(и)}}}"},
            {Case::Genitive, "{3(р){2(р){1(р)}}}"},
            {Case::Dative, "{3(д){2(д){1(д)}}}"},
            {Case::Accusative, "{3(в){2(в){1(в)}}}"},
            {Case::Instrumental, "{3(т){2(т){1(т)}}}"},
            {Case::Prepositional, "{3(п){2(п){1(п)}}}"}
        };
        QHash<Case, QString> argument1 = {
            {Case::Nominative, ""},
            {Case::Genitive, ""},
            {Case::Dative, ""},
            {Case::Accusative, ""},
            {Case::Instrumental, ""},
            {Case::Prepositional, ""}
        };
        QHash<Case, QString> argument2 = {
            {Case::Nominative, "привет"},
            {Case::Genitive, "привета"},
            {Case::Dative, "привету"},
            {Case::Accusative, "привет"},
            {Case::Instrumental, "приветом"},
            {Case::Prepositional, "привете"}
        };
        QList<QHash<Case, QString>> arguments = {argument1, argument1, argument2};
        QHash<Case, QString> expectedResult = {
            {Case::Nominative, "{3(и){2(и)}}"},
            {Case::Genitive, "{3(р){2(р)}}"},
            {Case::Dative, "{3(д){2(д)}}"},
            {Case::Accusative, "{3(в){2(в)}}"},
            {Case::Instrumental, "{3(т){2(т)}}"},
            {Case::Prepositional, "{3(п){2(п)}}"}
        };
        QTest::newRow("nested-placeholders-two-empty-args")
            << description
            << arguments
            << expectedResult;
    }

    // Тест 13: Шаблон содержит места для замены с подстановкой аргументов в обратном порядке
    {
        QHash<Case, QString> description = {
            {Case::Nominative, "{2(и)}{1(и)}"},
            {Case::Genitive, "{2(р)}{1(р)}"},
            {Case::Dative, "{2(д)}{1(д)}"},
            {Case::Accusative, "{2(в)}{1(в)}"},
            {Case::Instrumental, "{2(т)}{1(т)}"},
            {Case::Prepositional, "{2(п)}{1(п)}"}
        };
        QHash<Case, QString> argument1 = {
            {Case::Nominative, "привет"},
            {Case::Genitive, "привета"},
            {Case::Dative, "привету"},
            {Case::Accusative, "привет"},
            {Case::Instrumental, "приветом"},
            {Case::Prepositional, "привете"}
        };
        QHash<Case, QString> argument2 = {
            {Case::Nominative, "мир!"},
            {Case::Genitive, "мира!"},
            {Case::Dative, "миру!"},
            {Case::Accusative, "мир!"},
            {Case::Instrumental, "миром!"},
            {Case::Prepositional, "мире!"}
        };
        QList<QHash<Case, QString>> arguments = {argument1, argument2};
        QHash<Case, QString> expectedResult = {
            {Case::Nominative, "мир!привет"},
            {Case::Genitive, "мира!привета"},
            {Case::Dative, "миру!привету"},
            {Case::Accusative, "мир!привет"},
            {Case::Instrumental, "миром!приветом"},
            {Case::Prepositional, "мире!привете"}
        };
        QTest::newRow("placeholders-reversed-args")
            << description
            << arguments
            << expectedResult;
    }

    // Тест 14: Шаблон содержит два места для замены первого аргумента
    {
        QHash<Case, QString> description = {
            {Case::Nominative, "{1(и)}{1(и)}"},
            {Case::Genitive, "{1(р)}{1(р)}"},
            {Case::Dative, "{1(д)}{1(д)}"},
            {Case::Accusative, "{1(в)}{1(в)}"},
            {Case::Instrumental, "{1(т)}{1(т)}"},
            {Case::Prepositional, "{1(п)}{1(п)}"}
        };
        QHash<Case, QString> argument1 = {
            {Case::Nominative, "привет"},
            {Case::Genitive, "привета"},
            {Case::Dative, "привету"},
            {Case::Accusative, "привет"},
            {Case::Instrumental, "приветом"},
            {Case::Prepositional, "привете"}
        };
        QList<QHash<Case, QString>> arguments = {argument1};
        QHash<Case, QString> expectedResult = {
            {Case::Nominative, "приветпривет"},
            {Case::Genitive, "приветапривета"},
            {Case::Dative, "приветупривету"},
            {Case::Accusative, "приветпривет"},
            {Case::Instrumental, "приветомприветом"},
            {Case::Prepositional, "приветепривете"}
        };
        QTest::newRow("two-placeholders-for-one-arg")
            << description
            << arguments
            << expectedResult;
    }

    // Тест 15: Шаблон и аргумент содержат место для замены
    {
        QHash<Case, QString> description = {
            {Case::Nominative, "{1(и)}"},
            {Case::Genitive, "{1(р)}"},
            {Case::Dative, "{1(д)}"},
            {Case::Accusative, "{1(в)}"},
            {Case::Instrumental, "{1(т)}"},
            {Case::Prepositional, "{1(п)}"}
        };
        QHash<Case, QString> argument1 = {
            {Case::Nominative, "{1(и)}"},
            {Case::Genitive, "{1(р)}"},
            {Case::Dative, "{1(д)}"},
            {Case::Accusative, "{1(в)}"},
            {Case::Instrumental, "{1(т)}"},
            {Case::Prepositional, "{1(п)}"}
        };
        QList<QHash<Case, QString>> arguments = {argument1};
        QHash<Case, QString> expectedResult = argument1;
        QTest::newRow("placeholder-in-placeholder")
            << description
            << arguments
            << expectedResult;
    }

    // Тест 16: Шаблон содержит место для замены, аргумент содержит два места для замены
    {
        QHash<Case, QString> description = {
            {Case::Nominative, "{1(и)}"},
            {Case::Genitive, "{1(р)}"},
            {Case::Dative, "{1(д)}"},
            {Case::Accusative, "{1(в)}"},
            {Case::Instrumental, "{1(т)}"},
            {Case::Prepositional, "{1(п)}"}
        };
        QHash<Case, QString> argument1 = {
            {Case::Nominative, "{1(и)} {1(и)}"},
            {Case::Genitive, "{1(р)} {1(р)}"},
            {Case::Dative, "{1(д)} {1(д)}"},
            {Case::Accusative, "{1(в)} {1(в)}"},
            {Case::Instrumental, "{1(т)} {1(т)}"},
            {Case::Prepositional, "{1(п)} {1(п)}"}
        };
        QList<QHash<Case, QString>> arguments = {argument1};
        QHash<Case, QString> expectedResult = argument1;
        QTest::newRow("arg-with-multiple-placeholders")
            << description
            << arguments
            << expectedResult;
    }

    // Тест 17: Некорректное указание падежа в шаблоне
    {
        QHash<Case, QString> description = {
            {Case::Nominative, "{1(ь)}"},
            {Case::Genitive, ""},
            {Case::Dative, ""},
            {Case::Accusative, ""},
            {Case::Instrumental, ""},
            {Case::Prepositional, ""}
        };
        QHash<Case, QString> argument1 = {
            {Case::Nominative, "привет"},
            {Case::Genitive, "привета"},
            {Case::Dative, "привету"},
            {Case::Accusative, "привет"},
            {Case::Instrumental, "приветом"},
            {Case::Prepositional, "привете"}
        };
        QList<QHash<Case, QString>> arguments = {argument1};
        QHash<Case, QString> expectedResult;

        QTest::newRow("incorrect-case-in-placeholder")
            << description
            << arguments
            << expectedResult
            << ErrorType::IncorrectCaseInPlaceHolder;
    }
}

