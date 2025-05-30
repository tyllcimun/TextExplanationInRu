#include "test_getexplanationinru.h"
#include <QtTest/QTest>
#include <expression.h>

test_getExplanationInRu::test_getExplanationInRu(QObject *parent)
    : QObject{parent}
{}

void test_getExplanationInRu::getExplanationInRu()
{
    QFETCH(Expression, expression);
    QFETCH(QVariant, result);

    // Тип, который хранит QVariant
    int userType = result.userType();

    if (userType == qMetaTypeId<QString>()) {
        // Если ожидается строковый результат
        QString actualResult;
        try {
            actualResult = expression.getExplanationInRu();
        } catch (const TEException& e) {
            qDebug() << "Expected result:" << result.toString();
            qDebug() << "Actual error:" << TEException::ErrorTypeNames.value(e.getErrorType());
            QFAIL("Unexpected exception thrown when a string result was expected.");
        }

        QString expectedResult = result.toString();
        qDebug() << "Actual result:" << actualResult;
        qDebug() << "Expected result:" << expectedResult;

        if (actualResult != expectedResult) {
            QFAIL("String results mismatch");
        }
    }
    else if (userType == qMetaTypeId<ErrorType>()) {
        // Если ожидается ошибка
        try {
            QString actualResult = expression.getExplanationInRu();
            qDebug() << "Actual result:" << actualResult;
            qDebug() << "Expected error:" << TEException::ErrorTypeNames.value(result.value<ErrorType>());
            QFAIL("Expected an exception, but got a string result.");
        } catch (const TEException& e) {
            ErrorType actualError = e.getErrorType();
            ErrorType expectedError = result.value<ErrorType>();

            qDebug() << "Actual error:" << TEException::ErrorTypeNames.value(actualError);
            qDebug() << "Expected error:" << TEException::ErrorTypeNames.value(expectedError);

            if (actualError != expectedError) {
                QFAIL("Error types mismatch");
            }
        }
    }
    else {
        QFAIL("Unexpected type in 'result' column");
    }
}

void test_getExplanationInRu::getExplanationInRu_data()
{
    QTest::addColumn<Expression>("expression");
    QTest::addColumn<QVariant>("result");

    // Тест 1: Сложение двух констант
    QTest::newRow("addition-of-two-constants")
        << Expression("1 1 +", {}, {}, {}, {}, {}, {})
        << QVariant("сумма 1 и 1");

    // Тест 2: Сложение двух одинаковых переменных
    QTest::newRow("addition-of-two-identical-variables")
        << Expression(
               "appleCount appleCount +",
               {{"appleCount", Variable("appleCount", "int",
                                        {{Case::Nominative, "количество яблок"},
                                         {Case::Genitive, "количества яблок"},
                                         {Case::Dative, "количеству яблок"},
                                         {Case::Accusative, "количество яблок"},
                                         {Case::Instrumental, "количеством яблок"},
                                         {Case::Prepositional, "о количестве яблок"}})}},
               {},
               {},
               {},
               {},
               {})
        << QVariant("сумма количества яблок и количества яблок");

    // Тест 3: Сложение двух разных переменных
    QTest::newRow("addition-of-two-different-variables")
        << Expression(
               "warnings errors +",
               {{"warnings", Variable("warnings", "int",
                                      {{Case::Nominative, "предупреждения программы"},
                                       {Case::Genitive, "предупреждений программы"},
                                       {Case::Dative, "предупреждениям программы"},
                                       {Case::Accusative, "предупреждения программы"},
                                       {Case::Instrumental, "предупреждениями программы"},
                                       {Case::Prepositional, "о предупреждениях программы"}})},
                {"errors", Variable("errors", "int",
                                    {{Case::Nominative, "ошибки"},
                                     {Case::Genitive, "ошибок"},
                                     {Case::Dative, "ошибкам"},
                                     {Case::Accusative, "ошибки"},
                                     {Case::Instrumental, "ошибками"},
                                     {Case::Prepositional, "об ошибках"}})}},
               {},
               {},
               {},
               {},
               {})
        << QVariant("сумма предупреждений программы и ошибок");

    // Тест 4: Вывод одной переменной
    QTest::newRow("output-single-variable")
        << Expression(
               "appleCount",
               {{"appleCount", Variable("appleCount", "int",
                                        {{Case::Nominative, "количество яблок"},
                                         {Case::Genitive, "количества яблок"},
                                         {Case::Dative, "количеству яблок"},
                                         {Case::Accusative, "количество яблок"},
                                         {Case::Instrumental, "количеством яблок"},
                                         {Case::Prepositional, "о количестве яблок"}})}},
               {},
               {},
               {},
               {},
               {})
        << QVariant("количество яблок");

    // Тест 5: Ввод переменной с унарной операцией
    QTest::newRow("unary-operation-variable")
        << Expression(
               "isApple !",
               {{"isApple", Variable("isApple", "bool",
                                     {{Case::Nominative, "яблоко"},
                                      {Case::Genitive, "яблока"},
                                      {Case::Dative, "яблоку"},
                                      {Case::Accusative, "яблоко"},
                                      {Case::Instrumental, "яблоком"},
                                      {Case::Prepositional, "о яблоке"}})}},
               {},
               {},
               {},
               {},
               {})
        << QVariant("не яблоко");

    // Тест 6: Ввод переменной с двойным отрицанием
    QTest::newRow("double-negation-variable")
        << Expression(
               "isApple ! !",
               {{"isApple", Variable("isApple", "bool",
                                     {{Case::Nominative, "яблоко"},
                                      {Case::Genitive, "яблока"},
                                      {Case::Dative, "яблоку"},
                                      {Case::Accusative, "яблоко"},
                                      {Case::Instrumental, "яблоком"},
                                      {Case::Prepositional, "о яблоке"}})}},
               {},
               {},
               {},
               {},
               {})
        << QVariant("яблоко");

    // Тест 7: Ввод неиспользуемой переменной
    QTest::newRow("unused-variable")
        << Expression(
               "1",
               {{"isApple", Variable("isApple", "bool",
                                     {{Case::Nominative, "яблоко"},
                                      {Case::Genitive, "яблока"},
                                      {Case::Dative, "яблоку"},
                                      {Case::Accusative, "яблоко"},
                                      {Case::Instrumental, "яблоком"},
                                      {Case::Prepositional, "о яблоке"}})}},
               {},
               {},
               {},
               {},
               {})
        << QVariant::fromValue<ErrorType>(ErrorType::NeverUsedElement);

    // Тест 8: Ввод неиспользуемой функции
    QTest::newRow("unused-function")
        << Expression(
               "1",
               {},
               {{"getApple", Function("getApple", "int", 0,
                                      {{Case::Nominative, "получение яблок"},
                                       {Case::Genitive, "получения яблок"},
                                       {Case::Dative, "получению яблок"},
                                       {Case::Accusative, "получение яблок"},
                                       {Case::Instrumental, "получением яблок"},
                                       {Case::Prepositional, "о получении яблок"}})}},
               {},
               {},
               {},
               {})
        << QVariant::fromValue<ErrorType>(ErrorType::NeverUsedElement);

    // Тест 9: Ввод неиспользуемого класса
    QTest::newRow("unused-class")
        << Expression(
               "1",
               {},
               {},
               {},
               {},
               {{"Apple", Class("Apple", {}, {})}},
               {})
        << QVariant::fromValue<ErrorType>(ErrorType::NeverUsedElement);

    // Тест 10: Ввод неиспользуемого поля класса
    QTest::newRow("unused-class-field")
        << Expression(
               "newApple age .",
               {{"newApple", Variable("newApple", "Apple")}},
               {},
               {},
               {},
               {{"Apple", Class("Apple",
                                {{"age", Variable("age", "int",
                                                  {{Case::Nominative, "возраст"},
                                                   {Case::Genitive, "возраста"},
                                                   {Case::Dative, "возрасту"},
                                                   {Case::Accusative, "возраст"},
                                                   {Case::Instrumental, "возрастом"},
                                                   {Case::Prepositional, "о возрасте"}})},
                                 {"sort", Variable("sort", "string",
                                                   {{Case::Nominative, "сорт"},
                                                    {Case::Genitive, "сорта"},
                                                    {Case::Dative, "сорту"},
                                                    {Case::Accusative, "сорт"},
                                                    {Case::Instrumental, "сортом"},
                                                    {Case::Prepositional, "сорте"}})},
                                 },
                                {})}},
               {})
        << QVariant::fromValue<ErrorType>(ErrorType::NeverUsedElement);

    // Тест 11: Ввод неиспользуемой структуры
    QTest::newRow("unused-structure")
        << Expression(
               "1",
               {},
               {},
               {},
               {{"Apple", Structure("Apple",
                                    {{"count", Variable("count", "int",
                                                        {{Case::Nominative, "количество"},
                                                         {Case::Genitive, "количества"},
                                                         {Case::Dative, "количеству"},
                                                         {Case::Accusative, "количество"},
                                                         {Case::Instrumental, "количеством"},
                                                         {Case::Prepositional, "о количестве"}})}},
                                    {})}},
               {},
               {})
        << QVariant::fromValue<ErrorType>(ErrorType::NeverUsedElement);

    // Тест 12: Ввод неиспользуемого поля структуры
    QTest::newRow("unused-structure-field")
        << Expression(
               "newApple age .",
               {{"newApple", Variable("newApple", "Apple")}},
               {},
               {},
               {{"Apple", Structure("Apple",
                                    {{"age", Variable("age", "int",
                                                      {{Case::Nominative, "возраст"},
                                                       {Case::Genitive, "возраста"},
                                                       {Case::Dative, "возрасту"},
                                                       {Case::Accusative, "возраст"},
                                                       {Case::Instrumental, "возрастом"},
                                                       {Case::Prepositional, "о возрасте"}})},
                                     {"sort", Variable("sort", "string",
                                                       {{Case::Nominative, "сорт"},
                                                        {Case::Genitive, "сорта"},
                                                        {Case::Dative, "сорту"},
                                                        {Case::Accusative, "сорт"},
                                                        {Case::Instrumental, "сортом"},
                                                        {Case::Prepositional, "сорте"}})},
                                     }, {})}},
               {},
               {})
        << QVariant::fromValue<ErrorType>(ErrorType::NeverUsedElement);

    // Тест 13: Ввод неиспользуемого объединения
    QTest::newRow("unused-union")
        << Expression(
               "1",
               {},
               {},
               {{"Apple", Union("Apple", {}, {})}},
               {},
               {},
               {})
        << QVariant::fromValue<ErrorType>(ErrorType::NeverUsedElement);

    // Тест 14: Ввод неиспользуемого поля объединения
    QTest::newRow("unused-union-field")
        << Expression(
               "newApple age .",
               {{"newApple", Variable("newApple", "Apple")}},
               {},
               {{"Apple", Union("Apple",
                                {{"age", Variable("age", "int",
                                                  {{Case::Nominative, "возраст"},
                                                   {Case::Genitive, "возраста"},
                                                   {Case::Dative, "возрасту"},
                                                   {Case::Accusative, "возраст"},
                                                   {Case::Instrumental, "возрастом"},
                                                   {Case::Prepositional, "о возрасте"}})},
                                 {"sort", Variable("sort", "string",
                                                   {{Case::Nominative, "сорт"},
                                                    {Case::Genitive, "сорта"},
                                                    {Case::Dative, "сорту"},
                                                    {Case::Accusative, "сорт"},
                                                    {Case::Instrumental, "сортом"},
                                                    {Case::Prepositional, "сорте"}})},
                                 }, {})}},
               {},
               {})
        << QVariant::fromValue<ErrorType>(ErrorType::NeverUsedElement);

    // Тест 15: Ввод неиспользуемого перечисления
    QTest::newRow("unused-enum")
        << Expression(
               "1",
               {},
               {},
               {},
               {},
               {},
               {{"Fruits", Enum("Fruits", {})}})
        << QVariant::fromValue<ErrorType>(ErrorType::NeverUsedElement);

    // Тест 16: Ввод неиспользуемого значения перечисления
    QTest::newRow("unused-enum-value")
        << Expression(
               "curFruit Fruits Apple :: == ",
               {{"curFruit", Variable("curFruit", "Fruits")}},
               {},
               {},
               {},
               {},
               {{"Fruits", Enum("Fruits",
                                {{"Apple", {{Case::Nominative, "яблоко"},
                                            {Case::Genitive, "яблока"},
                                            {Case::Dative, "яблоку"},
                                            {Case::Accusative, "яблоко"},
                                            {Case::Instrumental, "яблоком"},
                                            {Case::Prepositional, "о яблоке"}}},
                                 {"NotApple", {{Case::Nominative, "не яблоко"},
                                               {Case::Genitive, "не яблока"},
                                               {Case::Dative, "не яблоку"},
                                               {Case::Accusative, "не яблоко"},
                                               {Case::Instrumental, "не яблоком"},
                                               {Case::Prepositional, "о не яблоке"}}}})}}
               )
        << QVariant::fromValue<ErrorType>(ErrorType::NeverUsedElement);

    // Тест 17: Ввод двух одинаковых полей у разных классов
    QTest::newRow("duplicate-fields-different-classes")
        << Expression(
               "newApple sort . newPinApple sort . == ",
               {{"newApple", Variable("newApple", "Apple",
                                      {{Case::Nominative, "яблоко"},
                                       {Case::Genitive, "яблока"},
                                       {Case::Dative, "яблоку"},
                                       {Case::Accusative, "яблоко"},
                                       {Case::Instrumental, "яблоком"},
                                       {Case::Prepositional, "о яблоке"}})},
                {"newPinApple", Variable("newPinApple", "PinApple",
                                         {{Case::Nominative, "ананас"},
                                          {Case::Genitive, "ананаса"},
                                          {Case::Dative, "ананасу"},
                                          {Case::Accusative, "ананас"},
                                          {Case::Instrumental, "ананасом"},
                                          {Case::Prepositional, "о ананасе"}})}},
               {},
               {},
               {},
               {{"Apple", Class("Apple",
                                {{"sort", Variable("sort", "string",
                                                   {{Case::Nominative, "сорт"},
                                                    {Case::Genitive, "сорта"},
                                                    {Case::Dative, "сорту"},
                                                    {Case::Accusative, "сорт"},
                                                    {Case::Instrumental, "сортом"},
                                                    {Case::Prepositional, "о сорте"}})}})},
                {"PinApple", Class("PinApple",
                                   {{"sort", Variable("sort", "string",
                                                      {{Case::Nominative, "сорт"},
                                                       {Case::Genitive, "сорта"},
                                                       {Case::Dative, "сорту"},
                                                       {Case::Accusative, "сорт"},
                                                       {Case::Instrumental, "сортом"},
                                                       {Case::Prepositional, "о сорте"}})}})}},
               {})
        << QVariant("сорт яблока равно сорту ананаса");

    // Тест 18: Ввод двух одинаковых полей у разных структур
    QTest::newRow("duplicate-fields-different-structures")
        << Expression(
               "newApple sort . newPinApple sort . == ",
               {{"newApple", Variable("newApple", "Apple",
                                      {{Case::Nominative, "яблоко"},
                                       {Case::Genitive, "яблока"},
                                       {Case::Dative, "яблоку"},
                                       {Case::Accusative, "яблоко"},
                                       {Case::Instrumental, "яблоком"},
                                       {Case::Prepositional, "о яблоке"}})},
                {"newPinApple", Variable("newPinApple", "PinApple",
                                         {{Case::Nominative, "ананас"},
                                          {Case::Genitive, "ананаса"},
                                          {Case::Dative, "ананасу"},
                                          {Case::Accusative, "ананас"},
                                          {Case::Instrumental, "ананасом"},
                                          {Case::Prepositional, "о ананасе"}})}},
               {}, {},
               {{"Apple", Structure("Apple",
                                    {{"sort", Variable("sort", "string",
                                                       {{Case::Nominative, "сорт"},
                                                        {Case::Genitive, "сорта"},
                                                        {Case::Dative, "сорту"},
                                                        {Case::Accusative, "сорт"},
                                                        {Case::Instrumental, "сортом"},
                                                        {Case::Prepositional, "о сорте"}})}})},
                {"PinApple", Structure("PinApple",
                                       {{"sort", Variable("sort", "string",
                                                          {{Case::Nominative, "сорт"},
                                                           {Case::Genitive, "сорта"},
                                                           {Case::Dative, "сорту"},
                                                           {Case::Accusative, "сорт"},
                                                           {Case::Instrumental, "сортом"},
                                                           {Case::Prepositional, "о сорте"}})}})}},
               {},
               {})
        << QVariant("сорт яблока равно сорту ананаса");

    // Тест 19: Ввод двух одинаковых полей у разных объединений
    QTest::newRow("duplicate-fields-different-unions")
        << Expression(
               "newApple sort . newPinApple sort . == ",
               {{"newApple", Variable("newApple", "Apple",
                                      {{Case::Nominative, "яблоко"},
                                       {Case::Genitive, "яблока"},
                                       {Case::Dative, "яблоку"},
                                       {Case::Accusative, "яблоко"},
                                       {Case::Instrumental, "яблоком"},
                                       {Case::Prepositional, "о яблоке"}})},
                {"newPinApple", Variable("newPinApple", "PinApple",
                                         {{Case::Nominative, "ананас"},
                                          {Case::Genitive, "ананаса"},
                                          {Case::Dative, "ананасу"},
                                          {Case::Accusative, "ананас"},
                                          {Case::Instrumental, "ананасом"},
                                          {Case::Prepositional, "о ананасе"}})}},
               {},
               {{"Apple", Union("Apple",
                                {{"sort", Variable("sort", "string",
                                                   {{Case::Nominative, "сорт"},
                                                    {Case::Genitive, "сорта"},
                                                    {Case::Dative, "сорту"},
                                                    {Case::Accusative, "сорт"},
                                                    {Case::Instrumental, "сортом"},
                                                    {Case::Prepositional, "о сорте"}})}})},
                {"PinApple", Union("PinApple",
                                   {{"sort", Variable("sort", "string",
                                                      {{Case::Nominative, "сорт"},
                                                       {Case::Genitive, "сорта"},
                                                       {Case::Dative, "сорту"},
                                                       {Case::Accusative, "сорт"},
                                                       {Case::Instrumental, "сортом"},
                                                       {Case::Prepositional, "о сорте"}})}})}},
               {})
        << QVariant("сорт яблока равно сорту ананаса");

    // Тест 20: Ввод двух одинаковых значений у разных перечислений
    QTest::newRow("duplicate-values-different-enums")
        << Expression(
               "notFruitSort Fruit Sort :: != notFruitSort Vegetable Sort :: == &&",
               {{"notFruitSort", Variable("notFruitSort", "Vegetable",
                                          {{Case::Nominative, "не фруктовый сорт"},
                                           {Case::Genitive, "не фруктового сорта"},
                                           {Case::Dative, "не фруктовому сорту"},
                                           {Case::Accusative, "не фруктовый сорт"},
                                           {Case::Instrumental, "не фруктовым сортом"},
                                           {Case::Prepositional, "о не фруктовом сорте"}})}},
               {},
               {},
               {},
               {},
               {{"Fruit", Enum("Fruit",
                               {{"Sort", {{Case::Nominative, "сорт"},
                                          {Case::Genitive, "сорта"},
                                          {Case::Dative, "сорту"},
                                          {Case::Accusative, "сорт"},
                                          {Case::Instrumental, "сортом"},
                                          {Case::Prepositional, "о сорте"}}}})},
                {"Vegetable", Enum("Vegetable",
                                   {{"Sort", {{Case::Nominative, "сорт"},
                                              {Case::Genitive, "сорта"},
                                              {Case::Dative, "сорту"},
                                              {Case::Accusative, "сорт"},
                                              {Case::Instrumental, "сортом"},
                                              {Case::Prepositional, "о сорте"}}}})}}
               )
        << QVariant("не фруктовый сорт не равно сорту и не фруктовый сорт равно сорту");

    // Тест 21: Одинаковое описание у разных переменных
    QTest::newRow("duplicate-descriptions-different-variables")
        << Expression(
               "oleg victor +",
               {{"oleg", Variable("oleg", "string",
                                  {{Case::Nominative, "имя"},
                                   {Case::Genitive, "имени"},
                                   {Case::Dative, "имени"},
                                   {Case::Accusative, "имя"},
                                   {Case::Instrumental, "именем"},
                                   {Case::Prepositional, "об имени"}})},
                {"victor", Variable("victor", "string",
                                    {{Case::Nominative, "имя"},
                                     {Case::Genitive, "имени"},
                                     {Case::Dative, "имени"},
                                     {Case::Accusative, "имя"},
                                     {Case::Instrumental, "именем"},
                                     {Case::Prepositional, "об имени"}})}},
               {},
               {},
               {},
               {},
               {})
        << QVariant("конкатенация имени и имени");

    // Тест 22: Использование класса в выражении
    QTest::newRow("undefined-class-in-expression")
        << Expression(
               "Apple",
               {},
               {},
               {},
               {},
               {{"Apple", Class("Apple")}},
               {})
        << QVariant::fromValue<ErrorType>(ErrorType::UndefinedId);

    // Тест 23: Использование структуры в выражении
    QTest::newRow("undefined-structure-in-expression")
        << Expression(
               "Apple",
               {},
               {},
               {},
               {{"Apple", Structure("Apple")}},
               {},
               {})
        << QVariant::fromValue<ErrorType>(ErrorType::UndefinedId);

    // Тест 24: Использование объединения в выражении
    QTest::newRow("undefined-union-in-expression")
        << Expression(
               "Apple",
               {},
               {},
               {{"Apple", Union("Apple")}},
               {},
               {},
               {})
        << QVariant::fromValue<ErrorType>(ErrorType::UndefinedId);

    // Тест 25: Максимальное количество операций
    QTest::newRow("maximum-operations")
        << Expression(
               "1 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 +",
               {},
               {},
               {},
               {},
               {},
               {})
        << QVariant("сумма 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 и 1");

    // Тест 26: Пустые данные
    QTest::newRow("empty-data")
        << Expression(
               "",
               {},
               {},
               {},
               {},
               {},
               {})
        << QVariant("");

    // Тест 27: Максимальное количество операндов в функции
    QTest::newRow("maximum-operands-in-function")
        << Expression(
               "1 2 3 4 5 sum(5)",
               {},
               {{"sum", Function("sum", "int", 5,
                                 {{Case::Nominative, "сложение {1(р)}, {2(р)}, {3(р)}, {4(р)}, {5(р)}"},
                                  {Case::Genitive, "сложения {1(р)}, {2(р)}, {3(р)}, {4(р)}, {5(р)}"},
                                  {Case::Dative, "сложению {1(р)}, {2(р)}, {3(р)}, {4(р)}, {5(р)}"},
                                  {Case::Accusative, "сложение {1(р)}, {2(р)}, {3(р)}, {4(р)}, {5(р)}"},
                                  {Case::Instrumental, "сложением {1(р)}, {2(р)}, {3(р)}, {4(р)}, {5(р)}"},
                                  {Case::Prepositional, "о сложении {1(р)}, {2(р)}, {3(р)}, {4(р)}, {5(р)}"}})}},
               {},
               {},
               {},
               {})
        << QVariant("сложение 1, 2, 3, 4, 5");


    // Тест 28: Отсутствие операндов в функции
    QTest::newRow("function-without-operands")
        << Expression(
               "helloworld(0)",
               {},
               {{"helloworld", Function("helloworld", "void", 0,
                                        {{Case::Nominative, "напечатать привет мир"},
                                         {Case::Genitive, "напечатания привет мира"},
                                         {Case::Dative, "напечатанию привет мира"},
                                         {Case::Accusative, "напечатать привет мир"},
                                         {Case::Instrumental, "напечатанием привет мира"},
                                         {Case::Prepositional, "о напечатании привет мира"}})}},
               {},
               {},
               {},
               {})
        << QVariant("напечатать привет мир");


    // Тест 29: Вызов функции в параметрах функции
    QTest::newRow("function-in-function-parameters")
        << Expression(
               "1 4 max(2) 3 sum(2)",
               {},
               {{"sum", Function("sum", "int", 2,
                                 {{Case::Nominative, "сложение {1(р)} и {2(р)}"},
                                  {Case::Genitive, "сложения {1(р)} и {2(р)}"},
                                  {Case::Dative, "сложению {1(р)} и {2(р)}"},
                                  {Case::Accusative, "сложение {1(р)} и {2(р)}"},
                                  {Case::Instrumental, "сложением {1(р)} и {2(р)}"},
                                  {Case::Prepositional, "о сложении {1(р)} и {2(р)}"}})},
                {"max", Function("max", "int", 2,
                                 {{Case::Nominative, "максимум из {1(р)} и {2(р)}"},
                                  {Case::Genitive, "максимума из {1(р)} и {2(р)}"},
                                  {Case::Dative, "максимуму из {1(р)} и {2(р)}"},
                                  {Case::Accusative, "максимум из {1(р)} и {2(р)}"},
                                  {Case::Instrumental, "максимумом из {1(р)} и {2(р)}"},
                                  {Case::Prepositional, "о максимуме из {1(р)} и {2(р)}"}})}},
               {},
               {},
               {},
               {})
        << QVariant("сложение максимума из 1 и 4 и 3");

    // Тест 30: Операция в параметре функции
    QTest::newRow("operation-in-function-parameter")
        << Expression(
               "1 1 + tostring(1)",
               {},
               {{"tostring", Function("tostring", "string", 1,
                                      {{Case::Nominative, "напечатать {1(в)}"},
                                       {Case::Genitive, "напечатания {1(в)}"},
                                       {Case::Dative, "напечатанию {1(в)}"},
                                       {Case::Accusative, "напечатать {1(в)}"},
                                       {Case::Instrumental, "напечатанием {1(в)}"},
                                       {Case::Prepositional, "о напечатании {1(в)}"}})}},
               {},
               {},
               {},
               {})
        << QVariant("напечатать сумму 1 и 1");

    // Тест 31: Вызов функции класса
    QTest::newRow("class-function-call")
        << Expression(
               "oleg getAge(0) .",
               {{"oleg", Variable("oleg", "Human",
                                  {{Case::Nominative, "олег"},
                                   {Case::Genitive, "олега"},
                                   {Case::Dative, "олегу"},
                                   {Case::Accusative, "олега"},
                                   {Case::Instrumental, "олегом"},
                                   {Case::Prepositional, "о олеге"}})}},
               {},
               {},
               {},
               {{"Human", Class("Human", {},
                                {{"getAge", Function("getAge", "int", 0,
                                                     {{Case::Nominative, "возраст"},
                                                      {Case::Genitive, "возраста"},
                                                      {Case::Dative, "возрасту"},
                                                      {Case::Accusative, "возраст"},
                                                      {Case::Instrumental, "возрастом"},
                                                      {Case::Prepositional, "о возрасте"}})}})}},
               {})
        << QVariant("возраст олега");

    // Тест 32: Вызов функции объединения
    QTest::newRow("union-function-call")
        << Expression(
               "oleg getAge(0) .",
               {{"oleg", Variable("oleg", "Human",
                                  {{Case::Nominative, "олег"},
                                   {Case::Genitive, "олега"},
                                   {Case::Dative, "олегу"},
                                   {Case::Accusative, "олега"},
                                   {Case::Instrumental, "олегом"},
                                   {Case::Prepositional, "о олеге"}})}},
               {},
               {{"Human", Union("Human", {},
                                {{"getAge", Function("getAge", "int", 0,
                                                     {{Case::Nominative, "возраст"},
                                                      {Case::Genitive, "возраста"},
                                                      {Case::Dative, "возрасту"},
                                                      {Case::Accusative, "возраст"},
                                                      {Case::Instrumental, "возрастом"},
                                                      {Case::Prepositional, "о возрасте"}})}})}},
               {},
               {},
               {})
        << QVariant("возраст олега");


    // Тест 33: Вызов функции структуры
    QTest::newRow("structure-function-call")
        << Expression(
               "oleg getAge(0) .",
               {{"oleg", Variable("oleg", "Human",
                                  {{Case::Nominative, "олег"},
                                   {Case::Genitive, "олега"},
                                   {Case::Dative, "олегу"},
                                   {Case::Accusative, "олега"},
                                   {Case::Instrumental, "олегом"},
                                   {Case::Prepositional, "о олеге"}})}},
               {},
               {},
               {{"Human", Structure("Human", {},
                                    {{"getAge", Function("getAge", "int", 0,
                                                         {{Case::Nominative, "возраст"},
                                                          {Case::Genitive, "возраста"},
                                                          {Case::Dative, "возрасту"},
                                                          {Case::Accusative, "возраст"},
                                                          {Case::Instrumental, "возрастом"},
                                                          {Case::Prepositional, "о возрасте"}})}})}},
               {},
               {})
        << QVariant("возраст олега");

    // Тест 34: Количество аргументов функции выражения не совпадает с количеством, указанным в функции
    QTest::newRow("function-argument-mismatch")
        << Expression(
               "tostring(1)",
               {},
               {{"tostring", Function("tostring", "string", 1,
                                      {{Case::Nominative, "напечатать {1(в)} и {2(в)}"},
                                       {Case::Genitive, "печати {1(в)} и {2(в)}"},
                                       {Case::Dative, "печати {1(в)} и {2(в)}"},
                                       {Case::Accusative, "печать {1(в)} и {2(в)}"},
                                       {Case::Instrumental, "печатью {1(в)} и {2(в)}"},
                                       {Case::Prepositional, "о печати {1(в)} и {2(в)}"}})}},
               {},
               {},
               {},
               {})
        << QVariant::fromValue<ErrorType>(ErrorType::MissingOperand);


    // Тест 35: В выражение передается не обрабатываемая операция
    QTest::newRow("invalid-operation-symbol")
        << Expression(
               "1 1 $",
               {},
               {},
               {},
               {},
               {},
               {})
        << QVariant::fromValue<ErrorType>(ErrorType::InvalidSymbol);

    // Тест 36: Функция содержит в описании дублирующее слово в переменной
    QTest::newRow("function-with-redundant-word-in-description")
        << Expression(
               "s v t getDistance(2) =",
               {{"v", Variable("v", "int",
                               {{Case::Nominative, "скорость машины"},
                                {Case::Genitive, "скорости машины"},
                                {Case::Dative, "скорости машине"},
                                {Case::Accusative, "скорость машины"},
                                {Case::Instrumental, "скоростью машины"},
                                {Case::Prepositional, "о скорости машины"}})},
                {"s", Variable("s", "int",
                               {{Case::Nominative, "расстояние"},
                                {Case::Genitive, "расстояния"},
                                {Case::Dative, "расстоянию"},
                                {Case::Accusative, "расстояние"},
                                {Case::Instrumental, "расстоянием"},
                                {Case::Prepositional, "о расстоянии"}})},
                {"t", Variable("t", "int",
                               {{Case::Nominative, "время"},
                                {Case::Genitive, "времени"},
                                {Case::Dative, "времени"},
                                {Case::Accusative, "время"},
                                {Case::Instrumental, "временем"},
                                {Case::Prepositional, "о времени"}})}},
               {{"getDistance", Function("getDistance", "int", 2,
                                         {{Case::Nominative, "расстояние по скорости {1(р)} и времени {2(р)}"},
                                          {Case::Genitive, "расстояния по скорости {1(р)} и времени {2(р)}"},
                                          {Case::Dative, "расстоянию по скорости {1(р)} и времени {2(р)}"},
                                          {Case::Accusative, "расстояние по скорости {1(р)} и времени {2(р)}"},
                                          {Case::Instrumental, "расстоянием по скорости {1(р)} и времени {2(р)}"},
                                          {Case::Prepositional, "о расстоянии по скорости {1(р)} и времени {2(р)}"}})}},
               {},
               {},
               {})
        << QVariant("присваивание расстоянию значения расстояния по скорости машины и времени");

    // Тест 37: Префиксная инкрементация
    QTest::newRow("prefix-increment")
        << Expression(
               "a ++_ 1 +",
               {{"a", Variable("a", "int",
                               {{Case::Nominative, "крутое значение"},
                                {Case::Genitive, "крутого значения"},
                                {Case::Dative, "крутому значению"},
                                {Case::Accusative, "крутое значение"},
                                {Case::Instrumental, "крутым значением"},
                                {Case::Prepositional, "о крутом значении"}})}},
               {},
               {},
               {},
               {},
               {})
        << QVariant("инкрементировать крутое значение, а затем получить сумму крутого значения и 1");

    // Тест 38: Постфиксная инкрементация
    QTest::newRow("postfix-increment")
        << Expression(
               "a _++ 1 +",
               {{"a", Variable("a", "int",
                               {{Case::Nominative, "крутое значение"},
                                {Case::Genitive, "крутого значения"},
                                {Case::Dative, "крутому значению"},
                                {Case::Accusative, "крутое значение"},
                                {Case::Instrumental, "крутым значением"},
                                {Case::Prepositional, "о крутом значении"}})}},
               {},
               {},
               {},
               {},
               {})
        << QVariant("получить сумму крутого значения и 1, а затем инкрементировать крутое значение");

    // Тест 39: Постфиксная и префиксная инкрементация разных переменных
    QTest::newRow("postfix-and-prefix-increment-different-vars")
        << Expression(
               "a _++ b ++_ +",
               {{"a", Variable("a", "int",
                               {{Case::Nominative, "крутое значение"},
                                {Case::Genitive, "крутого значения"},
                                {Case::Dative, "крутому значению"},
                                {Case::Accusative, "крутое значение"},
                                {Case::Instrumental, "крутым значением"},
                                {Case::Prepositional, "о крутом значении"}})},
                {"b", Variable("b", "int",
                               {{Case::Nominative, "не крутое значение"},
                                {Case::Genitive, "не крутого значения"},
                                {Case::Dative, "не крутому значению"},
                                {Case::Accusative, "не крутое значение"},
                                {Case::Instrumental, "не крутым значением"},
                                {Case::Prepositional, "о не крутом значении"}})}},
               {},
               {},
               {},
               {},
               {})
        << QVariant("получить инкрементирование не крутого значения, а затем получить сумму крутого значения и не крутого значения, а затем инкрементировать крутое значение");

    // Тест 40: Постфиксная и префиксная инкрементация одной переменной
    QTest::newRow("postfix-and-prefix-increment-same-var")
        << Expression(
               "a _++ ++_",
               {{"a", Variable("a", "int",
                               {{Case::Nominative, "крутое значение"},
                                {Case::Genitive, "крутого значения"},
                                {Case::Dative, "крутому значению"},
                                {Case::Accusative, "крутое значение"},
                                {Case::Instrumental, "крутым значением"},
                                {Case::Prepositional, "о крутом значении"}})}},
               {},
               {},
               {},
               {},
               {})
        << QVariant::fromValue<ErrorType>(ErrorType::MultipleIncrementDecrement);

    // Тест 41: Отрицание операции сравнения
    QTest::newRow("negation-of-comparison-operation")
        << Expression(
               "a b < !",
               {{"a", Variable("a", "int",
                               {{Case::Nominative, "крутое значение"},
                                {Case::Genitive, "крутого значения"},
                                {Case::Dative, "крутому значению"},
                                {Case::Accusative, "крутое значение"},
                                {Case::Instrumental, "крутым значением"},
                                {Case::Prepositional, "о крутом значении"}})},
                {"b", Variable("b", "int",
                               {{Case::Nominative, "не крутое значение"},
                                {Case::Genitive, "не крутого значения"},
                                {Case::Dative, "не крутому значению"},
                                {Case::Accusative, "не крутое значение"},
                                {Case::Instrumental, "не крутым значением"},
                                {Case::Prepositional, "о не крутом значении"}})}},
               {},
               {},
               {},
               {},
               {})
        << QVariant("крутое значение не меньше не крутого значения");

    // Тест 42: Несколько операций разности подряд
    QTest::newRow("multiple-subtraction-operations")
        << Expression(
               "1 1 - 1 -",
               {},
               {},
               {},
               {},
               {},
               {})
        << QVariant("разность 1 и суммы 1, 1");

    // Тест 43: Несколько операций деления подряд
    QTest::newRow("multiple-division-operations")
        << Expression(
               "1 1 / 1 /",
               {},
               {},
               {},
               {},
               {},
               {})
        << QVariant("частное 1 и произведения 1, 1");

    // Тест 44: Указатель на адрес
    QTest::newRow("pointer-to-address")
        << Expression(
               "a & *_",
               {{"a", Variable("a", "int",
                               {{Case::Nominative, "крутое значение"},
                                {Case::Genitive, "крутого значения"},
                                {Case::Dative, "крутому значению"},
                                {Case::Accusative, "крутое значение"},
                                {Case::Instrumental, "крутым значением"},
                                {Case::Prepositional, "о крутом значении"}})}},
               {},
               {},
               {},
               {},
               {})
        << QVariant("крутое значение");

    // Тест 45: Разыменование суммы
    QTest::newRow("dereferencing-sum")
        << Expression(
               "1 2 + 3 + *_",
               {},
               {},
               {},
               {},
               {},
               {})
        << QVariant("получение элемента по индексу, равному указателю суммы 1, 2 и 3");
}
