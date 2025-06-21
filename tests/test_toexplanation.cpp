#include "qobject.h"
#include "test_toexplanation.h"
#include "expression.h"
#include "expressionnode.h"
#include "qtestcase.h"
#include "codeentity.h"
#include "teexception.h"
#include <QString>
#include <QHash>
#include <QTest>

Q_DECLARE_METATYPE(Case)
using CaseExplanationHash = QHash<Case, QString>;
Q_DECLARE_METATYPE(CaseExplanationHash)

test_toExplanation::test_toExplanation(QObject *parent)
    : QObject{parent}
{}

void test_toExplanation::toExplanation()
{
    QFETCH(Expression, expression);
    QFETCH(ExpressionNode*, node);
    QFETCH(QString, className);
    QFETCH(OperationType, parentOperType);
    QFETCH(CaseExplanationHash, expectedExplanation);

    try {
        QHash<Case, QString> empty = {};
        QHash<Case, QString> result = expression.toExplanation(node, empty, className, parentOperType);
        qDebug() << "Actual Explanation: " << result[Case::Nominative];
        qDebug() << "Expected Explanation: " << expectedExplanation[Case::Nominative];

        if (result != expectedExplanation) {
            QFAIL("Test failed.");
        }
    } catch (const TEException &error) {
        QFETCH(ErrorType, expectedError);
        qDebug() << "Caught Exception:" << TEException::ErrorTypeNames.value(error.getErrorType());
        qDebug() << "Expected Error: " << TEException::ErrorTypeNames.value(expectedError);

        if (error.getErrorType() != expectedError) {
            QFAIL("Test failed.");
        }
    }
}


void test_toExplanation::toExplanation_data()
{
    QTest::addColumn<Expression>("expression");
    QTest::addColumn<ExpressionNode*>("node");
    QTest::addColumn<QString>("className");
    QTest::addColumn<OperationType>("parentOperType");
    QTest::addColumn<QHash<Case, QString>>("expectedExplanation");
    QTest::addColumn<ErrorType>("expectedError");

    // Тест 1: Одна операция
    {
        Expression expression("1 1 +", {});
        ExpressionNode* node = new ExpressionNode(EntityType::Operation, "+",
                                                  new ExpressionNode(EntityType::Const, "1"),
                                                  new ExpressionNode(EntityType::Const, "1"), "", OperationType::Addition);
        QHash<Case, QString> explanation;
        explanation[Case::Nominative] = "сумма 1 и 1";
        explanation[Case::Genitive] = "суммы 1 и 1";
        explanation[Case::Dative] = "сумме 1 и 1";
        explanation[Case::Accusative] = "сумму 1 и 1";
        explanation[Case::Instrumental] = "суммой 1 и 1";
        explanation[Case::Prepositional] = "сумме 1 и 1";

        QTest::newRow("single-operation")
            << expression
            << node
            << QString()
            << OperationType::None
            << explanation;
    }

    // Тест 2: Операция повторяется несколько раз
    {
        Expression expression("1 1 + 1 +", {});
        ExpressionNode* node = new ExpressionNode(EntityType::Operation, "+",
                                                  new ExpressionNode(EntityType::Operation, "+",
                                                                     new ExpressionNode(EntityType::Const, "1"),
                                                                     new ExpressionNode(EntityType::Const, "1"), "", OperationType::Addition),
                                                  new ExpressionNode(EntityType::Const, "1"), "", OperationType::Addition);
        QHash<Case, QString> explanation;
        explanation[Case::Nominative] = "сумма 1, 1 и 1";
        explanation[Case::Genitive] = "суммы 1, 1 и 1";
        explanation[Case::Dative] = "сумме 1, 1 и 1";
        explanation[Case::Accusative] = "сумму 1, 1 и 1";
        explanation[Case::Instrumental] = "суммой 1, 1 и 1";
        explanation[Case::Prepositional] = "сумме 1, 1 и 1";

        QTest::newRow("repeated-operation")
            << expression
            << node
            << QString()
            << OperationType::None
            << explanation;
    }

    // Тест 3: Один операнд
    {
        Expression expression("1", {});
        ExpressionNode* node = new ExpressionNode(EntityType::Const, "1");

        QHash<Case, QString> explanation;
        explanation[Case::Nominative] = "1";
        explanation[Case::Genitive] = "1";
        explanation[Case::Dative] = "1";
        explanation[Case::Accusative] = "1";
        explanation[Case::Instrumental] = "1";
        explanation[Case::Prepositional] = "1";

        QTest::newRow("single-operand")
            << expression
            << node
            << QString()
            << OperationType::None
            << explanation;
    }

    // Тест 4: Двойная логическая операция, которая должна сократиться
    {
        Expression expression("1 ! !", {});
        ExpressionNode* node = new ExpressionNode(EntityType::Operation, "!",
                                                  new ExpressionNode(EntityType::Operation, "!",
                                                                     new ExpressionNode(EntityType::Const, "1"), nullptr, "", OperationType::Not),
                                                  nullptr, "", OperationType::Not);

        QHash<Case, QString> explanation;
        explanation[Case::Nominative] = "1";
        explanation[Case::Genitive] = "1";
        explanation[Case::Dative] = "1";
        explanation[Case::Accusative] = "1";
        explanation[Case::Instrumental] = "1";
        explanation[Case::Prepositional] = "1";

        QTest::newRow("double-logical-operation")
            << expression
            << node
            << QString()
            << OperationType::None
            << explanation;
    }

    // Тест 5: Префиксный инкремент
    {
        Expression expression(
            "a ++_",
            {{"a", Variable("a", "int",
                            {{Case::Nominative, "количество дней"},
                             {Case::Genitive, "количества дней"},
                             {Case::Dative, "количеству дней"},
                             {Case::Accusative, "количество дней"},
                             {Case::Instrumental, "количеством дней"},
                             {Case::Prepositional, "о количестве дней"}})}},
            {},
            {},
            {},
            {},
            {}
            );

        ExpressionNode* node = new ExpressionNode(EntityType::Operation, "++_",
                                                  new ExpressionNode(EntityType::Variable, "a"),
                                                  nullptr, "", OperationType::PrefixIncrement);

        QHash<Case, QString> explanation;
        explanation[Case::Nominative] = "инкрементировать количество дней";
        explanation[Case::Genitive] = "инкрементирования количества дней";
        explanation[Case::Dative] = "инкрементированию количества дней";
        explanation[Case::Accusative] = "инкрементирование количества дней";
        explanation[Case::Instrumental] = "инкрементированием количества дней";
        explanation[Case::Prepositional] = "инкрементировании количества дней";

        QTest::newRow("prefix-increment")
            << expression
            << node
            << QString()
            << OperationType::None
            << explanation;
    }

    // Тест 6: Постфиксный инкремент
    {
        Expression expression(
            "a _++",
            {{"a", Variable("a", "int",
                            {{Case::Nominative, "количество дней"},
                             {Case::Genitive, "количества дней"},
                             {Case::Dative, "количеству дней"},
                             {Case::Accusative, "количество дней"},
                             {Case::Instrumental, "количеством дней"},
                             {Case::Prepositional, "количестве дней"}})}},
            {},
            {},
            {},
            {},
            {}
            );

        ExpressionNode* node = new ExpressionNode(EntityType::Operation, "_++",
                                                  new ExpressionNode(EntityType::Variable, "a"),
                                                  nullptr, "", OperationType::PostfixIncrement);

        QHash<Case, QString> explanation;
        explanation[Case::Nominative] = "инкрементировать количество дней";
        explanation[Case::Genitive] = "инкрементирования количества дней";
        explanation[Case::Dative] = "инкрементированию количества дней";
        explanation[Case::Accusative] = "инкрементирование количества дней";
        explanation[Case::Instrumental] = "инкрементированием количества дней";
        explanation[Case::Prepositional] = "инкрементировании количества дней";

        QTest::newRow("postfix-increment")
            << expression
            << node
            << QString() // className пустой
            << OperationType::None
            << explanation;
    }

    // Тест 7: Операция операнда с константой
    {
        Expression expression(
            "1 a +",
            {{"a", Variable("a", "int",
                            {{Case::Nominative, "количество дней"},
                             {Case::Genitive, "количества дней"},
                             {Case::Dative, "количеству дней"},
                             {Case::Accusative, "количество дней"},
                             {Case::Instrumental, "количеством дней"},
                             {Case::Prepositional, "количестве дней"}})}},
            {},
            {},
            {},
            {},
            {}
            );

        ExpressionNode* node = new ExpressionNode(EntityType::Operation, "+",
                                                  new ExpressionNode(EntityType::Const, "1"),
                                                  new ExpressionNode(EntityType::Variable, "a"), "", OperationType::Addition);

        QHash<Case, QString> explanation;
        explanation[Case::Nominative] = "сумма 1 и количества дней";
        explanation[Case::Genitive] = "суммы 1 и количества дней";
        explanation[Case::Dative] = "сумме 1 и количества дней";
        explanation[Case::Accusative] = "сумму 1 и количества дней";
        explanation[Case::Instrumental] = "суммой 1 и количества дней";
        explanation[Case::Prepositional] = "сумме 1 и количества дней";

        QTest::newRow("operand-operation-constant")
            << expression
            << node
            << QString()
            << OperationType::None
            << explanation;
    }

    // Тест 8: Операция поля класса с переменной
    {
        Expression expression(
            "chel age . a +",
            {{"a", Variable("a", "int",
                            {{Case::Nominative, "количество дней"},
                             {Case::Genitive, "количества дней"},
                             {Case::Dative, "количеству дней"},
                             {Case::Accusative, "количество дней"},
                             {Case::Instrumental, "количеством дней"},
                             {Case::Prepositional, "количестве дней"}})},
             {"chel", Variable("chel", "Human",
                               {{Case::Nominative, "человек"},
                                {Case::Genitive, "человека"},
                                {Case::Dative, "человеку"},
                                {Case::Accusative, "человека"},
                                {Case::Instrumental, "человеком"},
                                {Case::Prepositional, "человеке"}})}},
            {}, {}, {},
            {{"Human", Class("Human",
                             {{"age", Variable("age", "int",
                                               {{Case::Nominative, "возраст"},
                                                {Case::Genitive, "возраста"},
                                                {Case::Dative, "возрасту"},
                                                {Case::Accusative, "возраст"},
                                                {Case::Instrumental, "возрастом"},
                                                {Case::Prepositional, "возрасте"}})}})}},
            {}
            );

        ExpressionNode* node = new ExpressionNode(EntityType::Operation, "+",
                                                  new ExpressionNode(EntityType::Operation, ".",
                                                                     new ExpressionNode(EntityType::Variable, "chel", nullptr, nullptr, "Human"),
                                                                     new ExpressionNode(EntityType::Variable, "age", nullptr, nullptr, "int"), "", OperationType::FieldAccess),
                                                  new ExpressionNode(EntityType::Variable, "a"), "", OperationType::Addition);

        QHash<Case, QString> explanation;
        explanation[Case::Nominative] = "сумма возраста человека и количества дней";
        explanation[Case::Genitive] = "суммы возраста человека и количества дней";
        explanation[Case::Dative] = "сумме возраста человека и количества дней";
        explanation[Case::Accusative] = "сумму возраста человека и количества дней";
        explanation[Case::Instrumental] = "суммой возраста человека и количества дней";
        explanation[Case::Prepositional] = "сумме возраста человека и количества дней";

        QTest::newRow("class-field-operation-variable")
            << expression
            << node
            << QString()
            << OperationType::None
            << explanation;
    }

    // Тест 9: Операция с полем структуры и переменной
    {
        Expression expression(
            "chel age . a +",
            {{"a", Variable("a", "int",
                            {{Case::Nominative, "количество дней"},
                             {Case::Genitive, "количества дней"},
                             {Case::Dative, "количеству дней"},
                             {Case::Accusative, "количество дней"},
                             {Case::Instrumental, "количеством дней"},
                             {Case::Prepositional, "количестве дней"}})},
             {"chel", Variable("chel", "Human",
                               {{Case::Nominative, "человек"},
                                {Case::Genitive, "человека"},
                                {Case::Dative, "человеку"},
                                {Case::Accusative, "человека"},
                                {Case::Instrumental, "человеком"},
                                {Case::Prepositional, "человеке"}})}},
            {}, {},
            {{"Human", Structure("Human",
                                 {{"age", Variable("age", "int",
                                                   {{Case::Nominative, "возраст"},
                                                    {Case::Genitive, "возраста"},
                                                    {Case::Dative, "возрасту"},
                                                    {Case::Accusative, "возраст"},
                                                    {Case::Instrumental, "возрастом"},
                                                    {Case::Prepositional, "возрасте"}})}})}}
            );

        ExpressionNode* node = new ExpressionNode(EntityType::Operation, "+",
                                                  new ExpressionNode(EntityType::Operation, ".",
                                                                     new ExpressionNode(EntityType::Variable, "chel", nullptr, nullptr, "Human"),
                                                                     new ExpressionNode(EntityType::Variable, "age", nullptr, nullptr, "int"), "", OperationType::FieldAccess),
                                                  new ExpressionNode(EntityType::Variable, "a"), "", OperationType::Addition);

        QHash<Case, QString> explanation;
        explanation[Case::Nominative] = "сумма возраста человека и количества дней";
        explanation[Case::Genitive] = "суммы возраста человека и количества дней";
        explanation[Case::Dative] = "сумме возраста человека и количества дней";
        explanation[Case::Accusative] = "сумму возраста человека и количества дней";
        explanation[Case::Instrumental] = "суммой возраста человека и количества дней";
        explanation[Case::Prepositional] = "сумме возраста человека и количества дней";

        QTest::newRow("structure-field-operation-variable")
            << expression
            << node
            << QString()
            << OperationType::None
            << explanation;
    }

    // Тест 10: Операция с полем объединения и переменной
    {
        Expression expression(
            "chel age . a +",
            {{"a", Variable("a", "int",
                            {{Case::Nominative, "количество дней"},
                             {Case::Genitive, "количества дней"},
                             {Case::Dative, "количеству дней"},
                             {Case::Accusative, "количество дней"},
                             {Case::Instrumental, "количеством дней"},
                             {Case::Prepositional, "количестве дней"}})},
             {"chel", Variable("chel", "Human",
                               {{Case::Nominative, "человек"},
                                {Case::Genitive, "человека"},
                                {Case::Dative, "человеку"},
                                {Case::Accusative, "человека"},
                                {Case::Instrumental, "человеком"},
                                {Case::Prepositional, "человеке"}})}},
            {},
            {{"Human", Union("Human",
                             {{"age", Variable("age", "int",
                                               {{Case::Nominative, "возраст"},
                                                {Case::Genitive, "возраста"},
                                                {Case::Dative, "возрасту"},
                                                {Case::Accusative, "возраст"},
                                                {Case::Instrumental, "возрастом"},
                                                {Case::Prepositional, "возрасте"}})}})}},
            {},
            {},
            {}
            );

        ExpressionNode* node = new ExpressionNode(EntityType::Operation, "+",
                                                  new ExpressionNode(EntityType::Operation, ".",
                                                                     new ExpressionNode(EntityType::Variable, "chel", nullptr, nullptr, "Human"),
                                                                     new ExpressionNode(EntityType::Variable, "age", nullptr, nullptr, "int"), "", OperationType::FieldAccess),
                                                  new ExpressionNode(EntityType::Variable, "a"), "", OperationType::Addition);

        QHash<Case, QString> explanation;
        explanation[Case::Nominative] = "сумма возраста человека и количества дней";
        explanation[Case::Genitive] = "суммы возраста человека и количества дней";
        explanation[Case::Dative] = "сумме возраста человека и количества дней";
        explanation[Case::Accusative] = "сумму возраста человека и количества дней";
        explanation[Case::Instrumental] = "суммой возраста человека и количества дней";
        explanation[Case::Prepositional] = "сумме возраста человека и количества дней";

        QTest::newRow("union-field-operation-variable")
            << expression
            << node
            << QString() // className пустой
            << OperationType::None
            << explanation;
    }

    // Тест 11: Вызов функции
    {
        Expression expression(
            "rnd(0)",
            {},
            {{"rnd", Function("rnd", "int", 0,
                              {{Case::Nominative, "получение случайного числа"},
                               {Case::Genitive, "получения случайного числа"},
                               {Case::Dative, "получению случайного числа"},
                               {Case::Accusative, "получение случайного числа"},
                               {Case::Instrumental, "получением случайного числа"},
                               {Case::Prepositional, "получении случайного числа"}})}},
            {},
            {},
            {},
            {}
            );

        ExpressionNode* node = new ExpressionNode(EntityType::Function, "rnd", nullptr, nullptr, "int", OperationType::None, new QList<ExpressionNode*>{});

        QHash<Case, QString> explanation;
        explanation[Case::Nominative] = "получение случайного числа";
        explanation[Case::Genitive] = "получения случайного числа";
        explanation[Case::Dative] = "получению случайного числа";
        explanation[Case::Accusative] = "получение случайного числа";
        explanation[Case::Instrumental] = "получением случайного числа";
        explanation[Case::Prepositional] = "получении случайного числа";

        QTest::newRow("function-call")
            << expression
            << node
            << QString()
            << OperationType::None
            << explanation;
    }

    // Тест 12: Вызов функции класса
    {
        Expression expression(
            "chel kill(0) .",
            {{"chel", Variable("chel", "Human",
                               {{Case::Nominative, "человек"},
                                {Case::Genitive, "человека"},
                                {Case::Dative, "человеку"},
                                {Case::Accusative, "человека"},
                                {Case::Instrumental, "человеком"},
                                {Case::Prepositional, "человеке"}})}},
            {},
            {},
            {},
            {{"Human", Class("Human", {},
                             {{"kill", Function("kill", "void", 0,
                                                {{Case::Nominative, "убийство"},
                                                 {Case::Genitive, "убийства"},
                                                 {Case::Dative, "убийству"},
                                                 {Case::Accusative, "убийство"},
                                                 {Case::Instrumental, "убийством"},
                                                 {Case::Prepositional, "убийстве"}})}})}},
            {}
            );

        ExpressionNode* node = new ExpressionNode(EntityType::Operation, ".",
                                                  new ExpressionNode(EntityType::Variable, "chel", nullptr, nullptr, "Human"),
                                                  new ExpressionNode(EntityType::Function, "kill", nullptr, nullptr, "void", OperationType::None, new QList<ExpressionNode*>{}), "", OperationType::FieldAccess, new QList<ExpressionNode*>{});

        QHash<Case, QString> explanation;
        explanation[Case::Nominative] = "убийство человека";
        explanation[Case::Genitive] = "убийства человека";
        explanation[Case::Dative] = "убийству человека";
        explanation[Case::Accusative] = "убийство человека";
        explanation[Case::Instrumental] = "убийством человека";
        explanation[Case::Prepositional] = "убийстве человека";

        QTest::newRow("class-method-call")
            << expression
            << node
            << QString()
            << OperationType::None
            << explanation;
    }

    // Тест 13: Вызов функции структуры
    {
        Expression expression(
            "chel kill(0) .",
            {{"chel", Variable("chel", "Human",
                               {{Case::Nominative, "человек"},
                                {Case::Genitive, "человека"},
                                {Case::Dative, "человеку"},
                                {Case::Accusative, "человека"},
                                {Case::Instrumental, "человеком"},
                                {Case::Prepositional, "человеке"}})}},
            {},
            {},
            {{"Human", Structure("Human", {},
                                 {{"kill", Function("kill", "void", 0,
                                                    {{Case::Nominative, "убийство"},
                                                     {Case::Genitive, "убийства"},
                                                     {Case::Dative, "убийству"},
                                                     {Case::Accusative, "убийство"},
                                                     {Case::Instrumental, "убийством"},
                                                     {Case::Prepositional, "убийстве"}})}})}},
            {},
            {}
            );

        ExpressionNode* node = new ExpressionNode(EntityType::Operation, ".",
                                                  new ExpressionNode(EntityType::Variable, "chel", nullptr, nullptr, "Human"),
                                                  new ExpressionNode(EntityType::Function, "kill", nullptr, nullptr, "void", OperationType::None, new QList<ExpressionNode*>{}), "", OperationType::FieldAccess);

        QHash<Case, QString> explanation;
        explanation[Case::Nominative] = "убийство человека";
        explanation[Case::Genitive] = "убийства человека";
        explanation[Case::Dative] = "убийству человека";
        explanation[Case::Accusative] = "убийство человека";
        explanation[Case::Instrumental] = "убийством человека";
        explanation[Case::Prepositional] = "убийстве человека";

        QTest::newRow("structure-method-call")
            << expression
            << node
            << QString()
            << OperationType::None
            << explanation;
    }

    // Тест 14: Вызов функции объединения
    {
        Expression expression(
            "chel kill(0) .",
            {{"chel", Variable("chel", "Human",
                               {{Case::Nominative, "человек"},
                                {Case::Genitive, "человека"},
                                {Case::Dative, "человеку"},
                                {Case::Accusative, "человека"},
                                {Case::Instrumental, "человеком"},
                                {Case::Prepositional, "человеке"}})}},
            {},
            {{"Human", Union("Human", {},
                             {{"kill", Function("kill", "void", 0,
                                                {{Case::Nominative, "убийство"},
                                                 {Case::Genitive, "убийства"},
                                                 {Case::Dative, "убийству"},
                                                 {Case::Accusative, "убийство"},
                                                 {Case::Instrumental, "убийством"},
                                                 {Case::Prepositional, "убийстве"}})}})}},
            {},
            {},
            {}
            );

        ExpressionNode* node = new ExpressionNode(EntityType::Operation, ".",
                                                  new ExpressionNode(EntityType::Variable, "chel", nullptr, nullptr, "Human"),
                                                  new ExpressionNode(EntityType::Function, "kill", nullptr, nullptr, "void", OperationType::None, new QList<ExpressionNode*>{}), "", OperationType::FieldAccess);

        QHash<Case, QString> explanation;
        explanation[Case::Nominative] = "убийство человека";
        explanation[Case::Genitive] = "убийства человека";
        explanation[Case::Dative] = "убийству человека";
        explanation[Case::Accusative] = "убийство человека";
        explanation[Case::Instrumental] = "убийством человека";
        explanation[Case::Prepositional] = "убийстве человека";

        QTest::newRow("union-method-call")
            << expression
            << node
            << QString() // className пустой
            << OperationType::None
            << explanation;
    }

    // Тест 15: Получение значения перечисления
    {
        Expression expression(
            "Status Alive ::",
            {},
            {},
            {},
            {},
            {},
            {{"Status", Enum("Status",
                             {{"Alive", {{Case::Nominative, "живой"},
                                         {Case::Genitive, "живого"},
                                         {Case::Dative, "живому"},
                                         {Case::Accusative, "живого"},
                                         {Case::Instrumental, "живым"},
                                         {Case::Prepositional, "живом"}}}})}}
            );

        ExpressionNode* node = new ExpressionNode(EntityType::Operation, "::",
                                                  new ExpressionNode(EntityType::Enum, "Status", nullptr, nullptr, ""),
                                                  new ExpressionNode(EntityType::Variable, "Alive", nullptr, nullptr, ""), "", OperationType::StaticMemberAccess);

        QHash<Case, QString> explanation;
        explanation[Case::Nominative] = "живой";
        explanation[Case::Genitive] = "живого";
        explanation[Case::Dative] = "живому";
        explanation[Case::Accusative] = "живого";
        explanation[Case::Instrumental] = "живым";
        explanation[Case::Prepositional] = "живом";

        QTest::newRow("enum-value-retrieval")
            << expression
            << node
            << QString()
            << OperationType::None
            << explanation;
    }

    // Тест 16: Получение значения перечисления при нескольких значениях
    {
        Expression expression(
            "Status Alive ::",
            {},
            {},
            {},
            {},
            {},
            {{"Status", Enum("Status",
                             {{"Alive", {{Case::Nominative, "живой"},
                                         {Case::Genitive, "живого"},
                                         {Case::Dative, "живому"},
                                         {Case::Accusative, "живого"},
                                         {Case::Instrumental, "живым"},
                                         {Case::Prepositional, "живом"}}},
                              {"Dead", {{Case::Nominative, "мертвый"},
                                        {Case::Genitive, "мертвого"},
                                        {Case::Dative, "мертвому"},
                                        {Case::Accusative, "мертвого"},
                                        {Case::Instrumental, "мертвым"},
                                        {Case::Prepositional, "мертвом"}}}})}}
            );

        ExpressionNode* node = new ExpressionNode(EntityType::Operation, "::",
                                                  new ExpressionNode(EntityType::Enum, "Status", nullptr, nullptr, ""),
                                                  new ExpressionNode(EntityType::Variable, "Alive", nullptr, nullptr, ""), "", OperationType::StaticMemberAccess);

        QHash<Case, QString> explanation;
        explanation[Case::Nominative] = "живой";
        explanation[Case::Genitive] = "живого";
        explanation[Case::Dative] = "живому";
        explanation[Case::Accusative] = "живого";
        explanation[Case::Instrumental] = "живым";
        explanation[Case::Prepositional] = "живом";

        QTest::newRow("enum-value-with-multiple-values")
            << expression
            << node
            << QString()
            << OperationType::None
            << explanation;
    }

    // Тест 17: Сложение двух строк
    {
        Expression expression(
            "oleg cool +",
            {{"oleg", Variable("oleg", "string",
                               {{Case::Nominative, "олег"},
                                {Case::Genitive, "олега"},
                                {Case::Dative, "олегу"},
                                {Case::Accusative, "олега"},
                                {Case::Instrumental, "олегом"},
                                {Case::Prepositional, "олеге"}})},
             {"cool", Variable("cool", "string",
                               {{Case::Nominative, "крутой"},
                                {Case::Genitive, "крутого"},
                                {Case::Dative, "крутому"},
                                {Case::Accusative, "крутого"},
                                {Case::Instrumental, "крутым"},
                                {Case::Prepositional, "крутом"}})}},
            {},
            {},
            {},
            {},
            {}
            );

        ExpressionNode* node = new ExpressionNode(EntityType::Operation, "+",
                                                  new ExpressionNode(EntityType::Variable, "oleg", nullptr, nullptr, "string"),
                                                  new ExpressionNode(EntityType::Variable, "cool", nullptr, nullptr, "string"), "", OperationType::Addition);

        QHash<Case, QString> explanation;
        explanation[Case::Nominative] = "конкатенация олега и крутого";
        explanation[Case::Genitive] = "конкатенации олега и крутого";
        explanation[Case::Dative] = "конкатенации олега и крутого";
        explanation[Case::Accusative] = "конкатенацию олега и крутого";
        explanation[Case::Instrumental] = "конкатенацией олега и крутого";
        explanation[Case::Prepositional] = "конкатенации олега и крутого";

        QTest::newRow("string-concatenation")
            << expression
            << node
            << QString()
            << OperationType::None
            << explanation;
    }

    // Тест 18: Сложение двух одинаковых переменных
    {
        Expression expression(
            "oleg oleg +",
            {{"oleg", Variable("oleg", "int",
                               {{Case::Nominative, "олег"},
                                {Case::Genitive, "олега"},
                                {Case::Dative, "олегу"},
                                {Case::Accusative, "олега"},
                                {Case::Instrumental, "олегом"},
                                {Case::Prepositional, "олеге"}})}},
            {},
            {},
            {},
            {},
            {}
            );

        ExpressionNode* node = new ExpressionNode(EntityType::Operation, "+",
                                                  new ExpressionNode(EntityType::Variable, "oleg"),
                                                  new ExpressionNode(EntityType::Variable, "oleg"), "", OperationType::Addition);

        QHash<Case, QString> explanation;
        explanation[Case::Nominative] = "сумма олега и олега";
        explanation[Case::Genitive] = "суммы олега и олега";
        explanation[Case::Dative] = "сумме олега и олега";
        explanation[Case::Accusative] = "сумму олега и олега";
        explanation[Case::Instrumental] = "суммой олега и олега";
        explanation[Case::Prepositional] = "сумме олега и олега";

        QTest::newRow("addition-of-identical-variables")
            << expression
            << node
            << QString()
            << OperationType::None
            << explanation;
    }
}
