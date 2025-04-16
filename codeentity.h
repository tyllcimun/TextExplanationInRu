#ifndef CODEENTITY_H
#define CODEENTITY_H

#include <QList>
#include <QHash>
#include <QString>

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

enum class OperationType{
    Add,
    Sub,
    UnaryMinus,
    Mul,
    Div,
    Mod,
    PrefixInc,
    PostfixInc,
    PrefixDec,
    PostfixDec,
    Not,
    And,
    Or,
    Less,
    LessEq,
    Greater,
    GreaterEq,
    Eq,
    NotEq,
    Assign,
    AddAssign,
    SubAssign,
    MulAssign,
    DivAssign,
    ModAssign,
    ArrayAccess,
    Dereference,
    AddressOf,
    PointerField,
    FieldAccess,
    EnumGetValue,
    None,
};

// Переменная
struct Variable {

    QString name;           // имя
    QString type;           // тип
    QString description;    // описание
};

// Функция
struct Function {

    QString name;           // Имя
    QString type;           // Тип
    int paramsCount;        // Количество входных параметров
    QString description;    // Описание
};

// Объединение
struct Union {

    QString name;                        // Имя
    QHash<QString, Variable> variables;  // Поля
    QHash<QString, Function> functions;  // Методы
};

// Структура
struct Structure {

    QString name;                        // Имя
    QHash<QString, Variable> variables;  // Поля
    QHash<QString, Function> functions;  // Методы
};

// Класс
struct Class {

    QString name;                        // Имя
    QHash<QString, Variable> variables;  // Поля
    QHash<QString, Function> functions;  // Методы
};

// Перечисление
struct Enum {

    QString name;                    // Имя
    QHash<QString, QString> values;  // Значения
};

#endif // CODEENTITY_H
