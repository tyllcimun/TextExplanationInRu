/*!
 * \file
 * \brief Заголовочный файл, содержащий перечисления, структуры и глобальные переменные, описывающие сущности выражений (переменные, функции, типы и т.д.).
 */

#ifndef CODEENTITY_H
#define CODEENTITY_H

#include <QList>
#include <QHash>
#include <QSet>
#include <QString>

/*!
 * \brief Перечисление падежей для описания сущностей.
 */
enum class Case{
    Nominative,     /*!< Именительный */
    Genitive,       /*!< Родительный */
    Dative,         /*!< Дательный */
    Accusative,     /*!< Винительный */
    Instrumental,   /*!< Творительный */
    Prepositional   /*!< Предложный */
};

/*!
 * \brief Перечисление типов сущностей.
 */
enum class EntityType {
    Operation,              /*!< Операция */
    Const,                  /*!< Константа */
    Variable,               /*!< Переменная */
    Function,               /*!< Функция */
    CustomTypeWithFields,   /*!< Пользовательский тип с полями */
    Enum,                   /*!< Перечисление */
    Undefined               /*!< Неопределённый тип */
};

/*!
 * \brief Арность операции (унарная или бинарная).
 */
enum class OperationArity {
    Unary,      /*!< Унарная операция */
    Binary      /*!< Бинарная операция */
};

/*!
 * \brief Перечисление типов операций.
 */
enum class OperationType {
    PrefixIncrement,
    PrefixDecrement,
    ArrayAccess,
    FieldAccess,
    PointerFieldAccess,
    Dereference,
    AddressOf,
    UnaryMinus,
    Not,
    And,
    Or,
    Multiplication,
    Division,
    Modulus,
    Addition,
    Concatenation,
    Subtraction,
    LessThan,
    GreaterThan,
    LessThanOrEqual,
    GreaterThanOrEqual,
    Equal,
    NotEqual,
    ModulusAssignment,
    DivisionAssignment,
    MultiplicationAssignment,
    SubtractionAssignment,
    AdditionAssignment,
    Assignment,
    PostfixDecrement,
    PostfixIncrement,
    StaticMemberAccess,
    NotLessThan,
    NotLessThanOrEqual,
    NotGreaterThan,
    NotGreaterThanOrEqual,
    PointerIndexAccess,
    SubtractionSequence,
    DivisionSequence,
    SingleIncrement,
    SingleDecrement,
    FunctionCall,
    None
};

/*!
 * \brief Структура, описывающая информацию об операторе.
 */
struct OperatorInfo {
    OperationArity arity;      /*!< Арность операции */
    OperationType type;        /*!< Тип операции */
};

/*!
 * \brief Глобальное множество поддерживаемых стандартных типов данных.
 */
extern const QSet<QString> DataTypes;

/*!
 * \brief Глобальная таблица соответствий операций и их логических инверсий.
 */
extern const QHash<OperationType, OperationType> InverseComparisonOperationsMap;

/*!
 * \brief Глобальная таблица операций: строка → информация об операторе.
 */
extern const QHash<QString, OperatorInfo> OperationMap;

/*!
 * \brief Глобальная таблица строковых имён типов сущностей.
 */
extern const QHash<EntityType, QString> EntityTypeNames;

/*!
 * \brief Глобальная таблица строковых имён типов операций.
 */
extern const QHash<OperationType, QString> OperationTypeNames;

/*!
 * \brief Структура, описывающая переменную.
 */
struct Variable {

    QString name;                       /*!< Имя переменной */
    QString type;                       /*!< Тип переменной */
    QHash<Case, QString> description;   /*!< Описание в разных падежах */

    /*!
     * \brief Конструктор переменной.
     */
    Variable(const QString& name = "", const QString& type = "", const QHash<Case, QString>& description = {});

    /*!
     * \brief Преобразует переменную в строку.
     * \param[in] startLine Префикс каждой строки (например, отступ).
     * \return Строка, представляющая переменную.
     */
    QString toQString(const QString& startLine = "") const;
};

/*!
 * \brief Структура, описывающая функцию.
 */
struct Function {
    QString name;                       /*!< Имя функции */
    QString type;                       /*!< Тип возвращаемого значения */
    int paramsCount;                    /*!< Количество параметров */
    QHash<Case, QString> description;   /*!< Описание функции в разных падежах */

    /*!
     * \brief Конструктор функции.
     */
    Function(const QString& name = "", const QString& type = "", int paramsCount = 0, const QHash<Case, QString>& description = {});

    /*!
     * \brief Преобразует функцию в строку.
     * \param[in] startLine Префикс каждой строки.
     * \return Строка, представляющая функцию.
     */
    QString toQString(const QString& startLine = "") const;
};

/*!
 * \brief Базовая структура пользовательского типа с полями.
 */
struct CustomTypeWithFields {
    QString name;                               /*!< Имя типа */
    QHash<QString, Variable> variables;         /*!< Поля-переменные */
    QHash<QString, Function> functions;         /*!< Методы */

    /*!
     * \brief Конструктор пользовательского типа.
     */
    CustomTypeWithFields(const QString& name = "", const QHash<QString, Variable>& variables = {}, const QHash<QString, Function>& functions = {});

    /*!
     * \brief Преобразует тип в строку.
     * \param[in] startLine Префикс.
     * \return Строка с описанием.
     */
    QString toQString(const QString& startLine = "") const;
};

/*!
 * \brief Объединение (union), наследует CustomTypeWithFields.
 */
struct Union : public CustomTypeWithFields {
    Union(const QString& name = "", const QHash<QString, Variable>& variables = {}, const QHash<QString, Function>& functions = {});
    QString toQString(const QString& startLine = "") const;
};

/*!
 * \brief Структура (struct), наследует CustomTypeWithFields.
 */
struct Structure : public CustomTypeWithFields {
    Structure(const QString& name = "", const QHash<QString, Variable>& variables = {}, const QHash<QString, Function>& functions = {});
    QString toQString(const QString& startLine = "") const;
};

/*!
 * \brief Класс (class), наследует CustomTypeWithFields.
 */
struct Class : public CustomTypeWithFields {
    Class(const QString& name = "", const QHash<QString, Variable>& variables = {}, const QHash<QString, Function>& functions = {});
    QString toQString(const QString& startLine = "") const;
};

/*!
 * \brief Перечисление (enum), содержит список значений с описанием.
 */
struct Enum {
    QString name;                                           /*!< Имя перечисления */
    QHash<QString, QHash<Case, QString>> values;            /*!< Значения и их описания по падежам */

    /*!
     * \brief Конструктор перечисления.
     */
    Enum(const QString& name = "", const QHash<QString, QHash<Case, QString>>& values = {});

    /*!
     * \brief Преобразует перечисление в строку.
     * \param[in] startLine Префикс строки.
     * \return Строковое описание перечисления.
     */
    QString toQString(const QString& startLine = "") const;
};

#endif // CODEENTITY_H
