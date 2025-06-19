#ifndef CODEENTITY_H
#define CODEENTITY_H

#include <QList>
#include <QHash>
#include <QSet>
#include <QString>

enum class Case{
    Nominative,     // Именительный
    Genitive,       // Родительный
    Dative,         // Дательный
    Accusative,     // Винительный
    Instrumental,   // Творительный
    Prepositional   // Предложный
};

enum class EntityType {
    Operation,
    Const,
    Variable,
    Function,
    CustomTypeWithFields,
    Enum,
    Undefined
};

enum class OperationArity {
    Unary,
    Binary
};

enum class OperationType {
    PrefixIncrement,          // Префиксный инкремент
    PrefixDecrement,          // Префиксный декремент
    ArrayAccess,              // Обращение к элементу под индексом
    FieldAccess,              // Обращение к полю элемента
    PointerFieldAccess,       // Обращение к полю по указателю
    Dereference,              // Обращение к значению по адресу
    AddressOf,                // Обращение к адресу элемента
    UnaryMinus,               // Унарный минус
    Not,                      // Логическое «не»
    And,                      // Логическое «и»
    Or,                       // Логическое «или»
    Multiplication,           // Умножение
    Division,                 // Деление
    Modulus,                  // Остаток от деления
    Addition,                 // Сложение
    Concatenation,            // Конкатенация
    Subtraction,              // Вычитание
    LessThan,                 // Оператор сравнения «меньше»
    GreaterThan,              // Оператор сравнения «больше»
    LessThanOrEqual,          // Оператор сравнения «меньше либо равно»
    GreaterThanOrEqual,       // Оператор сравнения «больше либо равно»
    Equal,                    // Оператор сравнения «равно»
    NotEqual,                 // Оператор сравнения «не равно»
    ModulusAssignment,        // Взятие остатка от деления с присваиванием
    DivisionAssignment,       // Деление с присваиванием
    MultiplicationAssignment, // Умножение с присваиванием
    SubtractionAssignment,    // Вычитание с присваиванием
    AdditionAssignment,       // Сложение с присваиванием
    Assignment,               // Присваивание
    PostfixDecrement,         // Постфиксный декремент
    PostfixIncrement,         // Постфиксный инкремент
    StaticMemberAccess,       // Обращение к статическому элементу
    NotLessThan,              // Логическое "не меньше"
    NotLessThanOrEqual,       // Логическое "не меньше или равно"
    NotGreaterThan,           // Логическое "не больше"
    NotGreaterThanOrEqual,    // Логическое "не больше или равно"
    PointerIndexAccess,       // Получение элементу по индексу указателя
    SubtractionSequence,      // Операции вычитания подряд
    DivisionSequence,         // Операции деления подряд
    SingleIncrement,          // Один инкремент
    SingleDecrement,          // Один декремент
    FunctionCall,
    None
};

struct OperatorInfo {
    OperationArity arity;
    OperationType type;
};

extern const QSet<QString> DataTypes;

extern const QHash<OperationType, OperationType> InverseComparisonOperationsMap;

extern const QHash<QString, OperatorInfo> OperationMap;

// Переменная
struct Variable {

    QString name;           // имя
    QString type;           // тип
    QHash<Case, QString> description;    // описание

    Variable(const QString& name = "", const QString& type = "", const QHash<Case, QString>& description = {});

    QString toQString(const QString& startLine = "") const;
};

// Функция
struct Function {

    QString name;           // Имя
    QString type;           // Тип
    int paramsCount;        // Количество входных параметров
    QHash<Case, QString> description;    // Описание

    Function(const QString& name = "", const QString& type = "", int paramsCount = 0, const QHash<Case, QString>& description = {});

    QString toQString(const QString& startLine = "") const;

};

// Пользовательский тип данных
struct CustomTypeWithFields {
    QString name;               // Имя
    QHash<QString, Variable> variables; // Переменные
    QHash<QString, Function> functions; // Функции

    CustomTypeWithFields(const QString& name = "", const QHash<QString, Variable>& variables = {}, const QHash<QString, Function>& functions = {});
    QString toQString(const QString& startLine = "") const;
};


// Объединение
struct Union : public CustomTypeWithFields {
    Union(const QString& name = "", const QHash<QString, Variable>& variables = {}, const QHash<QString, Function>& functions = {});

    QString toQString(const QString& startLine = "") const;
};

// Структура
struct Structure : public CustomTypeWithFields {
    Structure(const QString& name = "", const QHash<QString, Variable>& variables = {}, const QHash<QString, Function>& functions = {});

    QString toQString(const QString& startLine = "") const;
};

// Класс
struct Class : public CustomTypeWithFields {
    Class(const QString& name = "", const QHash<QString, Variable>& variables = {}, const QHash<QString, Function>& functions = {});

    QString toQString(const QString& startLine = "") const;
};

// Перечисление
struct Enum {

    QString name;                   // Имя
    QHash<QString, QHash<Case, QString>> values;  // Значения

    Enum(const QString& name = "", const QHash<QString, QHash<Case, QString>>& values = {});

    QString toQString(const QString& startLine = "") const;
};

#endif // CODEENTITY_H
