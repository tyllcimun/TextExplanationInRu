#ifndef CODEENTITY_H
#define CODEENTITY_H

#include <QList>
#include <QHash>
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
    Union,
    Structure,
    Class,
    Enum,
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
    None
};

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

// Объединение
struct Union {

    QString name;                        // Имя
    QHash<QString, Variable> variables;  // Поля
    QHash<QString, Function> functions;  // Методы

    Union(const QString& name = "", const QHash<QString, Variable>& variables = {}, const QHash<QString, Function>& functions = {});

    QString toQString(const QString& startLine = "") const;
};

// Структура
struct Structure {

    QString name;                        // Имя
    QHash<QString, Variable> variables;  // Поля
    QHash<QString, Function> functions;  // Методы

    Structure(const QString& name = "", const QHash<QString, Variable>& variables = {}, const QHash<QString, Function>& functions = {});

    QString toQString(const QString& startLine = "") const;
};

// Класс
struct Class {

    QString name;                        // Имя
    QHash<QString, Variable> variables;  // Поля
    QHash<QString, Function> functions;  // Методы

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
