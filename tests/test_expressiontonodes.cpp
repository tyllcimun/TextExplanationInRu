#include "test_expressiontonodes.h"
#include <QtTest/QTest>
#include <expression.h>
#include <expressionnode.h>
#include <teexception.h>

test_expressionToNodes::test_expressionToNodes(QObject *parent)
    : QObject{parent}
{}

void test_expressionToNodes::expressionToNodes()
{
    QFETCH(QString, expressionString);
    QFETCH(Expression, expressionObject);
    QFETCH(bool, shouldThrow);
    QFETCH(ExpressionNode*, expectedTree);

    if (shouldThrow) {
        try {
            // Ожидаем выброс исключения
            ExpressionNode* actualTree = expressionObject.expressionToNodes();
            delete actualTree; // Если дерево создано, но этого не ожидалось
            qDebug() << "Expected exception but got tree.";
            QFAIL("Expected an exception, but none was thrown.");
        } catch (const TEException& e) {
            QFETCH(ErrorType, expectedError);
            QString actualError = TEException::ErrorTypeNames.value(e.getErrorType());
            QString expectedErrorStr = TEException::ErrorTypeNames.value(expectedError);

            if (actualError != expectedErrorStr) {
                qDebug() << "Actual error:  " << actualError;
                qDebug() << "Expected error:" << expectedErrorStr;
                QFAIL("Error types do not match.");
            } else {
                qDebug() << "Correct exception thrown: " << actualError;
            }
        }
    } else {
        try {
            // Создаем дерево без ошибок
            ExpressionNode* actualTree = expressionObject.expressionToNodes();

            if (!(*actualTree == *expectedTree)) {
                QString actualTreeStr = actualTree->toString();
                QString expectedTreeStr = expectedTree->toString();

                qDebug().noquote() << "Actual tree:\n" << actualTreeStr;
                qDebug().noquote() << "Expected tree:\n" << expectedTreeStr;
                QFAIL("Trees do not match.");
            }

            // Удаляем деревья
            delete actualTree;
            delete expectedTree;
        } catch (const TEException& e) {
            qDebug() << "Unexpected exception occurred.";
            qDebug() << "Exception type: " << TEException::ErrorTypeNames.value(e.getErrorType());
            QFAIL("Unexpected exception thrown.");
        }
    }
}

void test_expressionToNodes::expressionToNodes_data()
{
    QTest::addColumn<QString>("expressionString");
    QTest::addColumn<Expression>("expressionObject");
    QTest::addColumn<bool>("shouldThrow");
    QTest::addColumn<ExpressionNode*>("expectedTree");
    QTest::addColumn<ErrorType>("expectedError");

    // Тест 1: Строка содержит одну операцию "1 1 +"
    {
        QString exprString = "1 1 +";
        Expression expression(exprString, {});

        // Ожидаемое дерево:
        //        +
        //       / \
        //      1   1
        ExpressionNode* left = new ExpressionNode(EntityType::Const, "1", nullptr, nullptr, "");
        ExpressionNode* right = new ExpressionNode(EntityType::Const, "1", nullptr, nullptr, "");
        ExpressionNode* root = new ExpressionNode(EntityType::Operation, "+", left, right, "", OperationType::Addition);

        QTest::newRow("single-operation")
            << exprString
            << expression
            << false
            << root;
    }

    // Тест 2: Строка содержит несколько операций "1 1 1 + +"
    {
        QString exprString = "1 1 + 1 +";
        Expression expression(exprString, {});

        // Ожидаемое дерево:
        //          +
        //         / \
        //        +   1
        //       / \
        //      1   1
        ExpressionNode* leftChild = new ExpressionNode(EntityType::Const, "1", nullptr, nullptr);
        ExpressionNode* rightChild = new ExpressionNode(EntityType::Const, "1", nullptr, nullptr);
        ExpressionNode* left = new ExpressionNode(EntityType::Operation, "+", leftChild, rightChild, "", OperationType::Addition);
        ExpressionNode* root = new ExpressionNode(EntityType::Operation, "+", left, new ExpressionNode(EntityType::Const, "1", nullptr, nullptr), "", OperationType::Addition);

        QTest::newRow("multiple-operations")
            << exprString
            << expression
            << false
            << root;
    }

    // Тест 3: Строка содержит только операцию "+"
    {
        QString exprString = "+";
        Expression expression(exprString, {});
        ExpressionNode* root = new ExpressionNode(EntityType::Operation, "+", nullptr, nullptr, "", OperationType::Addition);


        QTest::newRow("only-operation")
            << exprString
            << expression
            << true
            << root
            << ErrorType::MissingOperand;
    }

    // Тест 4: Строка содержит унарную операцию "1 !"
    {
        QString exprString = "1 !";
        Expression expression(exprString, {});

        // Ожидаемое дерево:
        //        !
        //       /
        //      1
        ExpressionNode* left = new ExpressionNode(EntityType::Const, "1", nullptr, nullptr);
        ExpressionNode* root = new ExpressionNode(EntityType::Operation, "!", left, nullptr, "", OperationType::Not);

        QTest::newRow("unary-operation")
            << exprString
            << expression
            << false
            << root;
    }

    // Тест 5: Строка содержит четное количество унарных операций "1 ! ! ! !"
    {
        QString exprString = "1 ! ! ! !";
        Expression expression(exprString, {});

        // Ожидаемое дерево:
        //        !
        //       /
        //        !
        //       /
        //        !
        //       /
        //        !
        //       /
        //      1
        ExpressionNode* node1 = new ExpressionNode(EntityType::Const, "1", nullptr, nullptr);
        ExpressionNode* node2 = new ExpressionNode(EntityType::Operation, "!", node1, nullptr, "", OperationType::Not);
        ExpressionNode* node3 = new ExpressionNode(EntityType::Operation, "!", node2, nullptr, "", OperationType::Not);
        ExpressionNode* node4 = new ExpressionNode(EntityType::Operation, "!", node3, nullptr, "", OperationType::Not);
        ExpressionNode* root = new ExpressionNode(EntityType::Operation, "!", node4, nullptr, "", OperationType::Not);

        QTest::newRow("even-unary-operations")
            << exprString
            << expression
            << false
            << root;
    }

    // Тест 6: Строка пустая ""
    {
        QString exprString = "";
        Expression expression(exprString, {});
        ExpressionNode* emptyNode = new ExpressionNode();

        QTest::newRow("empty-string")
            << exprString
            << expression
            << false
            << emptyNode;
    }

    // Тест 7: Строка содержит унарную операцию и простую операцию сложения "1 ++_ 2 +"
    {
        QString exprString = "1 ++_ 2 +";
        Expression expression(exprString, {});

        // Ожидаемое дерево:
        //         +
        //       /   \
        //    ++_     2
        //     |
        //     1
        ExpressionNode* unaryNode = new ExpressionNode(EntityType::Operation, "++_", new ExpressionNode(EntityType::Const, "1", nullptr, nullptr), nullptr, "", OperationType::PrefixIncrement);
        ExpressionNode* root = new ExpressionNode(EntityType::Operation, "+", unaryNode, new ExpressionNode(EntityType::Const, "2", nullptr, nullptr), "", OperationType::Addition);

        QTest::newRow("unary-and-addition-operation")
            << exprString
            << expression
            << false
            << root;
    }


    // Тест 8: Строка содержит функцию без аргументов "helloWorld(0)"
    {
        QString exprString = "helloWorld(0)";
        Expression expression(exprString, {}, {{"helloWorld", Function("helloWorld", "void")}});
        auto* args = new QList<ExpressionNode*>;

        // Ожидаемое дерево:
        //    helloWorld
        ExpressionNode* root = new ExpressionNode(EntityType::Function, "helloWorld", nullptr, nullptr, "void", OperationType::None, args);

        QTest::newRow("function-no-arguments")
            << exprString
            << expression
            << false
            << root;
    }

    // Тест 9: Строка содержит функцию с одной переменной "1 factorial(1)"
    {
        QString exprString = "1 factorial(1)";
        Expression expression(exprString, {}, {{"factorial", Function("factorial", "int", 1)}});

        // Ожидаемое дерево:
        //    factorial
        //        |
        //        1
        ExpressionNode* left = new ExpressionNode(EntityType::Const, "1", nullptr, nullptr);
        auto* args = new QList<ExpressionNode*>();
        args->append(left);

        ExpressionNode* root = new ExpressionNode(EntityType::Function, "factorial", nullptr, nullptr, "int", OperationType::None, args);

        QTest::newRow("function-one-variable")
            << exprString
            << expression
            << false
            << root;
    }

    // Тест 10: Строка содержит функцию с несколькими переменными "3 4 5 6 7 max(5)"
    {
        QString exprString = "3 4 5 6 7 max(5)";
        Expression expression(exprString, {}, {{"max", Function("max", "int", 5)}});

        // Ожидаемое дерево:
        //            max
        //          / / | | \
        //         3  4  5 6  7
        QList<ExpressionNode*>* functionArgs = new QList<ExpressionNode*>{
            new ExpressionNode(EntityType::Const, "3", nullptr, nullptr),
            new ExpressionNode(EntityType::Const, "4", nullptr, nullptr),
            new ExpressionNode(EntityType::Const, "5", nullptr, nullptr),
            new ExpressionNode(EntityType::Const, "6", nullptr, nullptr),
            new ExpressionNode(EntityType::Const, "7", nullptr, nullptr)
        };
        ExpressionNode* root = new ExpressionNode(EntityType::Function, "max", nullptr, nullptr, "int", OperationType::None, functionArgs);

        QTest::newRow("function-with-multiple-variables")
            << exprString
            << expression
            << false
            << root;
    }

    // Тест 11: Функция имеет больше фактических аргументов, чем указано "1 2 3 sum(2)"
    {
        QString exprString = "1 2 3 sum(2)";
        Expression expression(exprString, {}, {{"sum", Function("sum", "int", 2)}});

        QTest::newRow("function-excessive-arguments")
            << exprString
            << expression
            << true
            << static_cast<ExpressionNode*>(nullptr)
            << ErrorType::MissingOperations;
    }

    // Тест 12: Функция имеет меньше фактических аргументов, чем указано "1 2 sum(3)"
    {
        QString exprString = "1 2 sum(3)";
        Expression expression(exprString, {}, {{"sum", Function("sum", "int", 3)}});


        QTest::newRow("function-insufficient-arguments")
            << exprString
            << expression
            << true
            << static_cast<ExpressionNode*>(nullptr)
            << ErrorType::MissingOperand;
    }

    // Тест 13: Строка содержит массив, индекс которого получается простой операцией "1 1 + array[]"
    {
        QString exprString = "array 1 1 + []";
        Expression expression(exprString, {{"array", Variable("array", "int[]")}});

        // Ожидаемое дерево:
        //          []
        //         /  \
        //     array   +
        //            / \
        //           1   1
        ExpressionNode* addition = new ExpressionNode(EntityType::Operation, "+",
                                                      new ExpressionNode(EntityType::Const, "1", nullptr, nullptr),
                                                      new ExpressionNode(EntityType::Const, "1", nullptr, nullptr), "", OperationType::Addition);
        ExpressionNode* root = new ExpressionNode(EntityType::Operation, "[]",
                                                  new ExpressionNode(EntityType::Variable, "array", nullptr, nullptr, "int"),
                                                  addition, "", OperationType::ArrayAccess);

        QTest::newRow("array-with-calculated-index")
            << exprString
            << expression
            << false
            << root;
    }

    // Тест 14: Строка содержит пользовательский тип перечисление "TestEnum ValueEnum ::"
    {
        QString exprString = "TestEnum ValueEnum ::";
        Expression expression(exprString, {}, {}, {}, {}, {}, {{"TestEnum", Enum({"TestEnum"}, {{"ValueEnum", {}}})}});

        // Ожидаемое дерево:
        //          ::
        //         /  \
        //   TestEnum ValueEnum
        ExpressionNode* root = new ExpressionNode(
            EntityType::Operation, "::",
            new ExpressionNode(EntityType::Enum, "TestEnum", nullptr, nullptr),
            new ExpressionNode(EntityType::Variable, "ValueEnum", nullptr, nullptr, "TestEnum"), "", OperationType::StaticMemberAccess
            );

        QTest::newRow("enum-access")
            << exprString
            << expression
            << false
            << root;
    }

    // Тест 15: Строка содержит только операнды "1 1 1"
    {
        QString exprString = "1 1 1";
        Expression expression(exprString, {});

        QTest::newRow("only-operands")
            << exprString
            << expression
            << true
            << static_cast<ExpressionNode*>(nullptr)
            << ErrorType::MissingOperations;
    }

    // Тест 16: Строка содержит строку с польской записью "3 2 sum(2)"
    {
        QString exprString = "\"3 2 sum(2)\"";
        Expression expression(exprString, {}, {});

        // Ожидаемое дерево:
        //         "3 2 sum(2)"
        ExpressionNode* root = new ExpressionNode(EntityType::Const, "\"3 2 sum(2)\"", nullptr, nullptr, "string");

        QTest::newRow("polish-notation-sum")
            << exprString
            << expression
            << false
            << root;
    }

    // Тест 17: Строка содержит выражение в обычной записи "1 + 1"
    {
        QString exprString = "1 + 1";
        Expression expression(exprString, {});

        // Ожидаемое дерево:
        //       +
        //      / \
        //     1   1
        ExpressionNode* left = new ExpressionNode(EntityType::Const, "1", nullptr, nullptr);
        ExpressionNode* right = new ExpressionNode(EntityType::Const, "1", nullptr, nullptr);
        ExpressionNode* root = new ExpressionNode(EntityType::Operation, "+", left, right, "", OperationType::Addition);

        QTest::newRow("infix-expression")
            << exprString
            << expression
            << true
            << static_cast<ExpressionNode*>(nullptr)
            << ErrorType::MissingOperand;
    }

    // Тест 18: Строка содержит функцию с аргументами в простой записи "sum(3, 4)"
    {
        QString exprString = "sum(3, 4)";
        Expression expression(exprString, {}, {{"sum", Function("sum", "int", 2)}});

        QTest::newRow("function-simple-notation-params-count-mismatch")
            << exprString
            << expression
            << true
            << static_cast<ExpressionNode*>(nullptr)
            << ErrorType::InvalidSymbol;
    }

    // Тест 19: Строка содержит булеву константу и логическую операцию "true !"
    {
        QString exprString = "true !";
        Expression expression(exprString, {});

        // Ожидаемое дерево:
        //       !
        //      /
        //   true
        ExpressionNode* left = new ExpressionNode(EntityType::Const, "true", nullptr, nullptr);
        ExpressionNode* root = new ExpressionNode(EntityType::Operation, "!", left, nullptr, "", OperationType::Not);

        QTest::newRow("bool-const-unary-operation")
            << exprString
            << expression
            << false
            << root;
    }

    // Тест 20: Строка содержит количество операций, превышающее лимит (20)
    {
        QString exprString = "1 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 +";

        Expression expression(exprString, {});

        QTest::newRow("exceed-operations-limit")
            << exprString
            << expression
            << true
            << static_cast<ExpressionNode*>(nullptr)
            << ErrorType::InputDataExprSizeExceeded;
    }

    // Тест 21: Строка содержит максимальное количество операций (20)
    {
        QString exprString = "1 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 +";

        Expression expression(exprString, {});

        ExpressionNode* node = new ExpressionNode(EntityType::Const, "1", nullptr, nullptr);
        for (int i = 0; i < 20; ++i) {
            node = new ExpressionNode(EntityType::Operation, "+", node, new ExpressionNode(EntityType::Const, "1", nullptr, nullptr), "", OperationType::Addition);
        }
        ExpressionNode* root = node;

        QTest::newRow("max-operations-count")
            << exprString
            << expression
            << false
            << root;
    }

    // Тест 22: Строка содержит функцию, которой нет в списке функций "myFunction()"
    {
        QString exprString = "myFunction(0)";
        Expression expression(exprString, {}, {}); // пустой список функций

        QTest::newRow("undefined-function")
            << exprString
            << expression
            << true
            << static_cast<ExpressionNode*>(nullptr)
            << ErrorType::UndefinedId;
    }

    // Тест 23: Строка содержит переменную, которой нет в списке переменных "myVariable 1 ="
    {
        QString exprString = "myVariable 1 =";
        Expression expression(exprString, {}); // пустой список переменных

        QTest::newRow("undefined-variable")
            << exprString
            << expression
            << true
            << static_cast<ExpressionNode*>(nullptr)
            << ErrorType::UndefinedId;
    }

    // Тест 24: Строка содержит поле пользовательского типа данных, которого нет в списке полей пользовательских типов данных "Pi Math getPi(0) . ="
    {
        QString exprString = "Pi Math getPi(0) . =";
        Expression expression(exprString, {{"Pi", Variable("Pi", "double")}}, {}, {}, {}, {{"Math", Class()}}, {});

        QTest::newRow("undefined-field-in-class")
            << exprString
            << expression
            << true
            << static_cast<ExpressionNode*>(nullptr)
            << ErrorType::UndefinedId;
    }

    // Тест 25: Строка содержит пользовательский тип данных, которого нет в списке пользовательских типов данных "Pi Math getPi(0) . ="
    {
        QString exprString = "Pi Math getPi(0) . =";
        Expression expression(exprString, {{"Pi", Variable("Pi", "double")}});

        QTest::newRow("undefined-user-type")
            << exprString
            << expression
            << true
            << static_cast<ExpressionNode*>(nullptr)
            << ErrorType::UndefinedId;
    }

    // Тест 26: Строка содержит пользовательский тип перечисления, которого нет в списке пользовательских типов "TestEnum ValueEnum ::"
    {
        QString exprString = "TestEnum ValueEnum ::";
        Expression expression(exprString, {});

        QTest::newRow("undefined-enum-type")
            << exprString
            << expression
            << true
            << static_cast<ExpressionNode*>(nullptr)
            << ErrorType::UndefinedId;
    }

    // Тест 27: Строка содержит пользовательский тип значения перечисления, которого нет в списке пользовательских типов значений "TestEnum ValueEnum ::"
    {
        QString exprString = "TestEnum ValueEnum ::";
        Expression expression(exprString, {}, {}, {}, {}, {}, {{"TestEnum", Enum("TestEnum", {})}});

        QTest::newRow("undefined-enum-value")
            << exprString
            << expression
            << true
            << static_cast<ExpressionNode*>(nullptr)
            << ErrorType::UndefinedId;
    }

    // Тест 28: Строка содержит символ, не являющийся операцией, операндом, переменной, функцией или пользовательским типом "#"
    {
        QString exprString = "#";
        Expression expression(exprString, {});

        QTest::newRow("invalid-symbol")
            << exprString
            << expression
            << true
            << static_cast<ExpressionNode*>(nullptr)
            << ErrorType::InvalidSymbol;
    }

    // Тест 29: Строка содержит обращение к полю класса, которого нет, однако, это название используется в названии переменной "table size ."
    {
        QString exprString = "table size .";
        Expression expression(exprString, {{"size", Variable("size", "int")}}, {}, {}, {}, {{"table", Class()}});

        QTest::newRow("undefined-class-field")
            << exprString
            << expression
            << true
            << static_cast<ExpressionNode*>(nullptr)
            << ErrorType::UndefinedId;
    }

    // Тест 30: Строка содержит элемент перечисления при наличии второго элемента на последнем месте "TestEnum ValueEnum1 ::"
    {
        QString exprString = "TestEnum ValueEnum1 ::";
        Expression expression(exprString, {}, {}, {}, {}, {}, {{"TestEnum", Enum("TestEnum", {{"ValueEnum1", {}}, {"ValueEnum2", {}}})}});

        // Ожидаемое дерево:
        //          ::
        //         /  \
        //   TestEnum ValueEnum1
        ExpressionNode* left = new ExpressionNode(EntityType::Enum, "TestEnum", nullptr, nullptr);
        ExpressionNode* right = new ExpressionNode(EntityType::Variable, "ValueEnum1", nullptr, nullptr, "TestEnum");
        ExpressionNode* root = new ExpressionNode(EntityType::Operation, "::", left, right, "", OperationType::StaticMemberAccess);

        QTest::newRow("enum-element-last")
            << exprString
            << expression
            << true
            << static_cast<ExpressionNode*>(nullptr)
            << ErrorType::NeverUsedElement;
    }

    // Тест 31: Строка содержит элемент перечисления при наличии второго элемента на первом месте "TestEnum ValueEnum2 ::"
    {
        QString exprString = "TestEnum ValueEnum2 ::";
        Expression expression(exprString, {}, {}, {}, {}, {}, {{"TestEnum", Enum("TestEnum", {{"ValueEnum1", {}}, {"ValueEnum2", {}}})}});

        // Ожидаемое дерево:
        //          ::
        //         /  \
        //   TestEnum ValueEnum2
        ExpressionNode* left = new ExpressionNode(EntityType::Enum, "TestEnum", nullptr, nullptr);
        ExpressionNode* right = new ExpressionNode(EntityType::Variable, "ValueEnum2", nullptr, nullptr, "TestEnum");
        ExpressionNode* root = new ExpressionNode(EntityType::Operation, "::", left, right, "", OperationType::StaticMemberAccess);

        QTest::newRow("enum-element-first")
            << exprString
            << expression
            << true
            << static_cast<ExpressionNode*>(nullptr)
            << ErrorType::NeverUsedElement;
    }

    // Тест 32: Строка не использует все переменные "myVar1"
    {
        QString exprString = "myVar1";
        Expression expression(exprString, {{"myVar1", Variable("myVar1", "int")}, {"myVar2", Variable("myVar2", "int")}});

        QTest::newRow("not-all-variables-used")
            << exprString
            << expression
            << true
            << static_cast<ExpressionNode*>(nullptr)
            << ErrorType::NeverUsedElement;
    }

    // Тест 33: Строка не использует переменную "1"
    {
        QString exprString = "1";
        Expression expression(exprString, {{"myVar", Variable("myVar", "int")}});

        QTest::newRow("variable-not-used")
            << exprString
            << expression
            << true
            << static_cast<ExpressionNode*>(nullptr)
            << ErrorType::NeverUsedElement;
    }

    // Тест 34: Строка не использует все функции "hello(0)"
    {
        QString exprString = "hello(0)";
        Expression expression(exprString, {}, {{"hello", Function("hello", "void", 0)}, {"bye", Function("bye", "void", 0)}});

        QTest::newRow("not-all-functions-used")
            << exprString
            << expression
            << true
            << static_cast<ExpressionNode*>(nullptr)
            << ErrorType::NeverUsedElement;
    }

    // Тест 35: Строка не использует функцию "1"
    {
        QString exprString = "1";
        Expression expression(exprString, {}, {{"hello", Function("hello", "void", 0)}});

        QTest::newRow("function-not-used")
            << exprString
            << expression
            << true
            << static_cast<ExpressionNode*>(nullptr)
            << ErrorType::NeverUsedElement;
    }

    // Тест 36: Строка использует поле класса, которое схоже с названием другого поля класса "chel age ."
    {
        QString exprString = "chel age .";
        Expression expression(exprString, {{"chel", Variable("chel", "Human")}}, {}, {}, {}, {{"Human", Class("Human", {{"ages", Variable("ages", "int", {})}, {"age", Variable("age", "int", {})}})}});

        QTest::newRow("similar-class-fields")
            << exprString
            << expression
            << true
            << static_cast<ExpressionNode*>(nullptr)
            << ErrorType::NeverUsedElement;
    }

    // Тест 37: Строка использует поле объединения, которое схоже с названием другого поля класса "chel age ."
    {
        QString exprString = "chel age .";
        Expression expression(exprString, {{"chel", Variable("chel", "Human")}}, {}, {{"Human", Union("Human", {{"ages", Variable("ages", "int")}, {"age", Variable("age", "int")}}, {})}});

        QTest::newRow("union-similar-fields")
            << exprString
            << expression
            << true
            << static_cast<ExpressionNode*>(nullptr)
            << ErrorType::NeverUsedElement;
    }

    // Тест 38: Строка использует поле структуры, которое схоже с названием другого поля класса "chel age ."
    {
        QString exprString = "chel age .";
        Expression expression(exprString, {{"chel", Variable("chel", "Human")}}, {}, {}, {{"Human", Structure("Human", {{"ages", Variable("ages", "int")}, {"age", Variable("age", "int")}}, {})}});

        QTest::newRow("structure-similar-fields")
            << exprString
            << expression
            << true
            << static_cast<ExpressionNode*>(nullptr)
            << ErrorType::NeverUsedElement;
    }

    // Тест 39: Строка использует функцию класса, которая схожа с названием другой функции класса "chel revive(0) ."
    {
        QString exprString = "chel revive(0) .";
        Expression expression(exprString, {{"chel", Variable("chel", "Human")}}, {}, {}, {}, {{"Human", Class("Human", {}, {{"revive", Function("revive", "void", 0)}, {"revives", Function("revives", "void", 0)}})}});

        QTest::newRow("class-similar-functions")
            << exprString
            << expression
            << true
            << static_cast<ExpressionNode*>(nullptr)
            << ErrorType::NeverUsedElement;
    }

    // Тест 40: Строка использует функцию объединения, которая схожа с названием другой функции объединения "chel revive(0) ."
    {
        QString exprString = "chel revive(0) .";
        Expression expression(exprString, {{"chel", Variable("chel", "Human")}}, {}, {{"Human", Union("Human", {}, {{"revive", Function("revive", "void", 0)}, {"revives", Function("revives", "void", 0)}})}});

        QTest::newRow("union-similar-functions")
            << exprString
            << expression
            << true
            << static_cast<ExpressionNode*>(nullptr)
            << ErrorType::NeverUsedElement;
    }

    // Тест 41: Строка использует функцию структуры, которая схожа с названием другой функции структуры "chel revive(0) ."
    {
        QString exprString = "chel revive(0) .";
        Expression expression(exprString, {{"chel", Variable("chel", "Human")}}, {}, {}, {{"Human", Structure("Human", {}, {{"revive", Function("revive", "void", 0)}, {"revives", Function("revives", "void", 0)}})}});

        QTest::newRow("structure-similar-functions")
            << exprString
            << expression
            << true
            << static_cast<ExpressionNode*>(nullptr)
            << ErrorType::NeverUsedElement;
    }

    // Тест 42: Строка использует поле структуры, при этом присутствует такое же название в другом регистре "chel age ."
    {
        QString exprString = "chel age .";
        Expression expression(exprString, {{"chel", Variable("chel", "Human")}}, {}, {}, {{"Human", Structure("Human", {{"Age", Variable("Age", "int")}, {"age", Variable("age", "int")}}, {})}});

        QTest::newRow("structure-case-sensitive-fields-lowercase")
            << exprString
            << expression
            << true
            << static_cast<ExpressionNode*>(nullptr)
            << ErrorType::NeverUsedElement;
    }

    // Тест 43: Строка использует поле класса, при этом присутствует такое же название в другом регистре "chel age ."
    {
        QString exprString = "chel age .";
        Expression expression(exprString, {{"chel", Variable("chel", "Human")}}, {}, {}, {}, {{"Human", Class("Human", {{"Age", Variable("Age", "int")}, {"age", Variable("age", "int")}}, {})}});

        QTest::newRow("class-case-sensitive-fields")
            << exprString
            << expression
            << true
            << static_cast<ExpressionNode*>(nullptr)
            << ErrorType::NeverUsedElement;
    }

    // Тест 44: Строка использует поле объединения, при этом присутствует такое же название в другом регистре "chel age ."
    {
        QString exprString = "chel age .";
        Expression expression(exprString, {{"chel", Variable("chel", "Human")}}, {}, {{"Human", Union("Human", {{"Age", Variable("Age", "int")}, {"age", Variable("age", "int")}}, {})}});

        QTest::newRow("union-case-sensitive-fields")
            << exprString
            << expression
            << true
            << static_cast<ExpressionNode*>(nullptr)
            << ErrorType::NeverUsedElement;
    }

    // Тест 45: Строка использует поле структуры, при этом присутствует такое же название в другом регистре "chel Age ."
    {
        QString exprString = "chel Age .";
        Expression expression(exprString, {{"chel", Variable("chel", "Human")}}, {}, {}, {{"Human", Structure("Human", {{"Age", Variable("Age", "int")}, {"age", Variable("age", "int")}}, {})}});

        QTest::newRow("structure-case-sensitive-fields-uppercase")
            << exprString
            << expression
            << true
            << static_cast<ExpressionNode*>(nullptr)
            << ErrorType::NeverUsedElement;
    }

    // Тест 46: Строка использует поле объединения, при этом присутствует такое же название в другом регистре "chel Age ."
    {
        QString exprString = "chel Age .";
        Expression expression(exprString, {{"chel", Variable("chel", "Human")}}, {}, {{"Human", Union("Human", {{"Age", Variable("Age", "int")}, {"age", Variable("age", "int")}}, {})}});

        QTest::newRow("union-case-sensitive-fields-age")
            << exprString
            << expression
            << true
            << static_cast<ExpressionNode*>(nullptr)
            << ErrorType::NeverUsedElement;
    }

    // Тест 47: Строка использует поле объединения, при этом присутствует такое же название в другом регистре "chel Age ."
    {
        QString exprString = "chel Age .";
        Expression expression(exprString, {{"chel", Variable("chel", "Human")}}, {}, {{"Human", Union("Human", {{"Age", Variable("Age", "int")}, {"age", Variable("age", "int")}}, {})}});

        QTest::newRow("union-case-sensitive-fields-duplication")
            << exprString
            << expression
            << true
            << static_cast<ExpressionNode*>(nullptr)
            << ErrorType::NeverUsedElement;
    }

    // Тест 48: Строка использует метод класса, при этом присутствует такое же название в другом регистре "chel revive(0) ."
    {
        QString exprString = "chel revive(0) .";
        Expression expression(exprString, {{"chel", Variable("chel", "Human")}}, {}, {}, {}, {{"Human", Class("Human", {}, {{"revive", Function("revive", "void", 0)}, {"Revive", Function("Revive", "void", 0)}})}});

        QTest::newRow("class-case-sensitive-methods")
            << exprString
            << expression
            << true
            << static_cast<ExpressionNode*>(nullptr)
            << ErrorType::NeverUsedElement;
    }

    // Тест 49: Строка использует метод объединения, при этом присутствует такое же название в другом регистре "chel revive(0) ."
    {
        QString exprString = "chel revive(0) .";
        Expression expression(exprString, {{"chel", Variable("chel", "Human")}}, {}, {{"Human", Union("Human", {}, {{"revive", Function("revive", "void", 0)}, {"Revive", Function("Revive", "void", 0)}})}});

        QTest::newRow("union-case-sensitive-methods-revive")
            << exprString
            << expression
            << true
            << static_cast<ExpressionNode*>(nullptr)
            << ErrorType::NeverUsedElement;
    }

    // Тест 50: Строка использует метод структуры, при этом присутствует такое же название в другом регистре "chel revive(0) ."
    {
        QString exprString = "chel revive(0) .";
        Expression expression(exprString, {{"chel", Variable("chel", "Human")}}, {}, {}, {{"Human", Structure("Human", {}, {{"revive", Function("revive", "void", 0)}, {"Revive", Function("Revive", "void", 0)}})}});

        QTest::newRow("structure-case-sensitive-methods-revive")
            << exprString
            << expression
            << true
            << static_cast<ExpressionNode*>(nullptr)
            << ErrorType::NeverUsedElement;
    }

    // Тест 51: Строка использует метод класса, при этом присутствует такое же название в другом регистре "chel Revive(0) ."
    {
        QString exprString = "chel Revive(0) .";
        Expression expression(exprString, {{"chel", Variable("chel", "Human")}}, {}, {}, {}, {{"Human", Class("Human", {}, {{"revive", Function("revive", "void", 0)}, {"Revive", Function("Revive", "void", 0)}})}});

        QTest::newRow("class-case-sensitive-methods-Revive")
            << exprString
            << expression
            << true
            << static_cast<ExpressionNode*>(nullptr)
            << ErrorType::NeverUsedElement;
    }

    // Тест 52: Строка использует метод объединения, при этом присутствует такое же название в другом регистре "chel Revive(0) ."
    {
        QString exprString = "chel Revive(0) .";
        Expression expression(exprString, {{"chel", Variable("chel", "Human")}}, {}, {{"Human", Union("Human", {}, {{"revive", Function("revive", "void", 0)}, {"Revive", Function("Revive", "void", 0)}})}});

        QTest::newRow("union-case-sensitive-methods-Revive")
            << exprString
            << expression
            << true
            << static_cast<ExpressionNode*>(nullptr)
            << ErrorType::NeverUsedElement;
    }

    // Тест 53: Строка использует метод структуры, при этом присутствует такое же название в другом регистре "chel Revive(0) ."
    {
        QString exprString = "chel Revive(0) .";
        Expression expression(exprString, {{"chel", Variable("chel", "Human")}}, {}, {}, {{"Human", Structure("Human", {}, {{"revive", Function("revive", "void", 0)}, {"Revive", Function("Revive", "void", 0)}})}});

        QTest::newRow("structure-case-sensitive-methods-Revive")
            << exprString
            << expression
            << true
            << static_cast<ExpressionNode*>(nullptr)
            << ErrorType::NeverUsedElement;
    }

    // Тест 54: Строка не использует пользовательский тип данных "1 1 +"
    {
        QString exprString = "1 1 +";
        Expression expression(exprString, {}, {}, {}, {{"Human", Structure("Human", {{"age", Variable("age", "int")}}, {})}});

        QTest::newRow("unused-custom-type")
            << exprString
            << expression
            << true
            << static_cast<ExpressionNode*>(nullptr)
            << ErrorType::NeverUsedElement;
    }

    // Тест 55: Строка два раза инкрементирует одну переменную "a _++ ++_"
    {
        QString exprString = "a _++ ++_";
        Expression expression(exprString, {{"a", Variable("a", "int")}});

        QTest::newRow("multiple-increment-decrement")
            << exprString
            << expression
            << true
            << static_cast<ExpressionNode*>(nullptr)
            << ErrorType::MultipleIncrementDecrement;
    }

    // Тест 56: Один из аргументов функции — это операция "1 + 1 sum(3)"
    {
        QString exprString = "1 + 1 sum(3)";
        Expression expression(exprString, {}, {{"sum", Function("sum", "int", 1)}});

        QTest::newRow("function-argument-is-operation")
            << exprString
            << expression
            << true
            << static_cast<ExpressionNode*>(nullptr)
            << ErrorType::MissingOperand;
    }

    // Тест 57: Один из аргументов функции — вызов функции "1 2 max(2) 3 sum(2)"
    {
        QString exprString = "1 2 max(2) 3 sum(2)";
        Expression expression(exprString, {}, {
                                                  {"max", Function("max", "int", 2)},
                                                  {"sum", Function("sum", "int", 2)}
                                              });

        // Ожидаемое дерево:
        //           sum
        //          /   \
        //         max  3
        //       /   \
        //       1   2
        // Создаем узлы для аргументов функции max
        ExpressionNode* arg1 = new ExpressionNode(EntityType::Const, "1", nullptr, nullptr);
        ExpressionNode* arg2 = new ExpressionNode(EntityType::Const, "2", nullptr, nullptr);

        // Создаем узел для функции max с аргументами
        ExpressionNode* maxNode = new ExpressionNode(EntityType::Function, "max",
                                                     nullptr, nullptr,
                                                     "int", OperationType::None,
                                                     new QList<ExpressionNode*>{arg1, arg2});

        // Создаем узел для функции sum, использующей maxNode как левый аргумент
        ExpressionNode* root = new ExpressionNode(EntityType::Function, "sum",
                                                  nullptr, nullptr,
                                                  "int", OperationType::None,
                                                  new QList<ExpressionNode*>{maxNode, new ExpressionNode(EntityType::Const, "3", nullptr, nullptr)});

        QTest::newRow("function-argument-is-function")
            << exprString
            << expression
            << false
            << root;
    }

    // Тест 58: Внутри функции происходит операция "1 1 + toString(1)"
    {
        QString exprString = "1 1 + toString(1)";
        Expression expression(exprString, {}, {
                                                  {"toString", Function("toString", "string", 1)}
                                              });

        // Ожидаемое дерево:
        //          toString
        //             |
        //            +
        //           / \
        //          1   1
        ExpressionNode* addition = new ExpressionNode(EntityType::Operation, "+",
                                                      new ExpressionNode(EntityType::Const, "1", nullptr, nullptr),
                                                      new ExpressionNode(EntityType::Const, "1", nullptr, nullptr),
                                                      "", OperationType::Addition);
        ExpressionNode* root = new ExpressionNode(EntityType::Function, "toString",
                                                  nullptr,
                                                  nullptr,
                                                  "string", OperationType::None, new QList<ExpressionNode*>({addition}));

        QTest::newRow("operation-inside-function")
            << exprString
            << expression
            << false
            << root;
    }
}
