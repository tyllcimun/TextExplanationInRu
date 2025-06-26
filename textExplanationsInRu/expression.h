/*!
 * \file
 * \brief Заголовочный файл, содержащий описание класса Expression, представляющего выражение и связанные с ним сущности.
 */

#ifndef EXPRESSION_H
#define EXPRESSION_H
#include "expressionnode.h"
#include "teexception.h"

#include <QHash>
#include <QString>
#include <QStack>

/*!
 * \brief Класс, представляющий выражение и связанные с ним переменные, функции и пользовательские типы.
 */
class Expression
{
public:
    /*!
     * \brief Конструктор класса Expression.
     * \param[in] expr Строка выражения.
     * \param[in] vars Переменные.
     * \param[in] funcs Функции.
     * \param[in] unns Объединения (union).
     * \param[in] strucs Структуры.
     * \param[in] cls Классы.
     * \param[in] enms Перечисления (enum).
     */
    explicit Expression(
        const QString& expr = "",
        const QHash<QString, Variable>& vars = {},
        const QHash<QString, Function>& funcs = {},
        const QHash<QString, Union>& unns = {},
        const QHash<QString, Structure>& strucs = {},
        const QHash<QString, Class>& cls = {},
        const QHash<QString, Enum>& enms = {}
        )
        : expression(expr)
        , variables(vars)
        , functions(funcs)
        , unions(unns)
        , structures(strucs)
        , classes(cls)
        , enums(enms)
    {}

    /*!
     * \brief Создание объекта Expression из XML-файла.
     * \param[in] path Путь к файлу.
     * \return Объект Expression.
     */
    static Expression fromFile(const QString& path);

    /*!
     * \brief Получает множество пользовательских типов данных, определённых в выражении.
     *
     * \return Множество строк с именами пользовательских типов данных.
     */
    QSet<QString> getCustomDataTypes() const;

    /*!
     * \brief Получение выражения в виде строки.
     * \return Строковое представление выражения.
     */
    QString ToQstring();

    /*!
     * \brief Генерация пояснения выражения на всех падежах.
     * \param[in] node Корневой узел выражения.
     * \param[in|out] intermediateDescription Промежуточное описание.
     * \param[in] className Имя класса (если есть).
     * \param[in] parentOperType Тип родительской операции.
     * \return Хэш падежей и строк описания.
     */
    QHash<Case, QString> toExplanation(const ExpressionNode *node, QHash<Case, QString> &intermediateDescription, const QString& className = "", OperationType parentOperType = OperationType::None) const;

    /*!
     * \brief Генерация пояснения выражения на русском языке.
     * \return Строка пояснения.
     */
    QString getExplanationInRu();

    /*!
     * \brief Преобразование выражения в дерево ExpressionNode.
     * \return Указатель на корневой узел дерева.
     */
    ExpressionNode* expressionToNodes();

    /*!
     * \brief Получение всех имён, используемых в выражении.
     * \return Множество имён.
     */
    QSet<QString> getAllNames();

    /*!
     * \brief Определение типа сущности по строке.
     * \param[in] str Строка.
     * \return Тип сущности.
     */
    EntityType getEntityTypeByStr(const QString& str);

    /*!
     * \brief Проверка, является ли идентификатор константой.
     * \param[in] str Идентификатор.
     * \return true, если это константа.
     */
    static bool isConst(const QString& str);

    /*!
     * \brief Проверка, является ли идентификатор переменной.
     * \param[in] str Идентификатор.
     * \return true, если это переменная.
     */
    bool isVariable(const QString& str);

    /*!
     * \brief Проверка, является ли идентификатор функцией.
     * \param[in] str Идентификатор.
     * \return true, если это функция.
     */
    static bool isFunction(const QString& str);

    /*!
     * \brief Проверка, является ли тип пользовательским типом с полями.
     */
    bool isCustomTypeWithFields(const QString& str);

    /*!
     * \brief Проверка, является ли тип перечислением.
     */
    bool isEnum(const QString& str);

    /*!
     * \brief Проверка, является ли строка допустимым идентификатором.
     */
    static bool isIdentifier(const QString& str);

    /*!
     * \brief Проверка, является ли символ латинской буквой.
     */
    static bool isLatinLetter(const QChar c);

    /*!
     * \brief Преобразование аргументов функции в падежные описания.
     */
    QList<QHash<Case, QString>> argsToDescr(const QList<ExpressionNode *> *functionArgs, QHash<Case, QString> &intermediateDescription, QString customDataType = "", OperationType parentOperType = OperationType::None) const;

    /*!
     * \brief Получение типа операции по строке.
     */
    OperationType getOperationTypeByStr(const QString& str);

    /*!
     * \brief Удаление идущих подряд дубликатов.
     */
    static QString removeConsecutiveDuplicates(const QString &str);

    /*!
     * \brief Установка нового выражения.
     */
    void setExpression(const QString &newExpression);

    /*!
     * \brief Получение текущего выражения.
     */
    const QString* getExpression() const;

    /*!
     * \brief Получение списка переменных.
     */
    const QHash<QString, Variable>* getVariables() const;

    /*!
     * \brief Установка новых переменных.
     */
    void setVariables(const QHash<QString, Variable> &newVariables);

    /*!
     * \brief Получение переменной по имени.
     */
    const Variable getVarByName(const QString &name) const;

    /*!
     * \brief Получение списка функций.
     */
    const QHash<QString, Function>* getFunctions() const;

    /*!
     * \brief Установка новых функций.
     */
    void setFunctions(const QHash<QString, Function> &newFunctions);

    /*!
     * \brief Получение функции по имени.
     */
    const Function getFuncByName(const QString &name) const;

    /*!
     * \brief Получение списка объединений.
     */
    const QHash<QString, Union>* getUnions() const;

    /*!
     * \brief Установка новых объединений.
     */
    void setUnions(const QHash<QString, Union> &newUnions);

    /*!
     * \brief Получение объединения по имени.
     */
    const Union getUnionByName(const QString &name) const;

    /*!
     * \brief Получение списка структур.
     */
    const QHash<QString, Structure>* getStructures() const;

    /*!
     * \brief Установка новых структур.
     */
    void setStructures(const QHash<QString, Structure> &newStructures);

    /*!
     * \brief Получение структуры по имени.
     */
    const Structure getStructByName(const QString &name) const;

    /*!
     * \brief Получение списка классов.
     */
    const QHash<QString, Class>* getClasses() const;

    /*!
     * \brief Установка новых классов.
     */
    void setClasses(const QHash<QString, Class> &newClasses);

    /*!
     * \brief Получение класса по имени.
     */
    const Class getClassByName(const QString &name) const;

    /*!
     * \brief Получение списка перечислений.
     */
    const QHash<QString, Enum>* getEnums() const;

    /*!
     * \brief Установка новых перечислений.
     */
    void setEnums(const QHash<QString, Enum> &newEnums);

    /*!
     * \brief Получение перечисления по имени.
     */
    const Enum getEnumByName(const QString &name) const;

    /*!
     * \brief Получение переменной из пользовательского типа по имени.
     */
    const Variable getVariableByNameFromCustomData(QString varName, QString dataName) const;

    /*!
     * \brief Получение функции из пользовательского типа по имени.
     */
    const Function getFunctionByNameFromCustomData(QString funcName, QString dataName) const;

    /*!
     * \brief Проверка, является ли значение элементом перечисления.
     */
    bool isEnumValue(const QString& value, const QString& enumName) const;

    /*!
     * \brief Получение пользовательского типа по имени.
     */
    const CustomTypeWithFields getCustomTypeByName(const QString &typeName) const;

    /*!
     * \brief Получение всех полей пользовательского типа.
     * \param[out] names Список, куда  добавляются имена полей.
     * \param[in] customType Пользовательский тип.
     */
    void getCustomTypeFields(QSet<QString> &names, const CustomTypeWithFields &customType);

    /*!
     * \brief Разделение выражения на составляющие.
     * \param[in] str Выражение.
     * \return Список токенов.
     */
    static QStringList splitExpression(const QString &str);

    /*!
     * \brief Преобразует строковое представление типа данных в стандартизированный формат.
     * \param[in] dataType Строковое представление типа данных.
     * \return Стандартизированное строковое представление базового типа данных.
     */
    QString sanitizeDataType(const QString &dataType);

    /*!
     * \brief Обрабатывает операцию и добавляет соответствующий узел в стек.
     * \param[in] token Токен, представляющий операцию.
     * \param[in,out] nodeStack Стек узлов выражения.
     * \param[in,out] operationCounter Счётчик операций в выражении.
     * \param[in] tokens Полный список токенов выражения.
     * \param[in] i Итератор текущей позиции в списке токенов.
     */
    void processOperation(const QString &token, QStack<ExpressionNode *> &nodeStack, int &operationCounter, const QStringList &tokens, QStringList::const_iterator i);

    /*!
     * \brief Обрабатывает константу и добавляет соответствующий узел в стек.
     * \param[in] token Токен, представляющий константу.
     * \param[in,out] nodeStack Стек узлов выражения.
     */
    void processConst(const QString &token, QStack<ExpressionNode *> &nodeStack);

    /*!
     * \brief Обрабатывает переменную и добавляет соответствующий узел в стек.
     * \param[in] token Токен, представляющий переменную.
     * \param[in,out] nodeStack Стек узлов выражения.
     * \param[in,out] usedElements Набор используемых переменных.
     * \param[in] customDataTypes Набор пользовательских типов данных.
     * \param[in] tokens Полный список токенов выражения.
     * \param[in] i Итератор текущей позиции в списке токенов.
     */
    void processVariable(const QString &token, QStack<ExpressionNode *> &nodeStack, QSet<QString> &usedElements, const QSet<QString> &customDataTypes, const QStringList &tokens, QStringList::const_iterator i);

    /*!
     * \brief Обрабатывает перечисление (enum) и добавляет соответствующий узел в стек.
     * \param[in] token Токен, представляющий элемент перечисления.
     * \param[in,out] nodeStack Стек узлов выражения.
     * \param[in,out] usedElements Набор используемых элементов перечисления.
     */
    void processEnum(const QString &token, QStack<ExpressionNode *> &nodeStack, QSet<QString> &usedElements);

    /*!
     * \brief Обрабатывает функцию и добавляет соответствующий узел в стек.
     * \param[in] token Токен, представляющий функцию.
     * \param[in,out] nodeStack Стек узлов выражения.
     * \param[in] customDataTypes Набор пользовательских типов данных.
     * \param[in,out] usedElements Набор используемых элементов.
     * \param[in] tokens Полный список токенов выражения.
     * \param[in] i Итератор текущей позиции в списке токенов.
     */
    void processFunction(const QString &token, QStack<ExpressionNode *> &nodeStack, const QSet<QString> &customDataTypes, QSet<QString> &usedElements, const QStringList &tokens, QStringList::const_iterator i);

    /*!
     * \brief Определяет тип переменной на основе контекста.
     * \param[in] token Токен, представляющий переменную.
     * \param[in,out] nodeStack Стек узлов выражения.
     * \param[in] tokens Полный список токенов выражения.
     * \param[in] i Итератор текущей позиции в списке токенов.
     * \param[out] className Название класса, к которому принадлежит переменная.
     * \return Тип переменной.
     */
    QString handleVariableTypeInference(const QString &token, QStack<ExpressionNode *> &nodeStack, const QStringList &tokens, QStringList::const_iterator i, QString &className);

    /*!
     * \brief Завершает обработку узлов и формирует результирующее выражение.
     * \param[in,out] nodeStack Стек узлов выражения.
     * \param[in] expression Исходное строковое выражение.
     * \param[in] operationCounter Счётчик операций в выражении.
     * \param[in] usedElements Набор используемых элементов.
     */
    void finalizeNodeProcessing(QStack<ExpressionNode *> &nodeStack, const QString &expression, int operationCounter, const QSet<QString> &usedElements);

    /*!
     * \brief Обрабатывает узел типа перечисления (enum).
     * \return Описание узла в формате QHash<Case, QString>.
     */
    QHash<Case, QString> handleEnumNode() const;

    /*!
     * \brief Обрабатывает узел типа переменной.
     * \param[in] node Узел выражения, представляющий переменную.
     * \param[in] className Название класса, если переменная принадлежит классу.
     * \param[in] parentOperType Тип родительской операции.
     * \return Описание узла в формате QHash<Case, QString>.
     */
    QHash<Case, QString> handleVariableNode(const ExpressionNode *node, const QString &className, OperationType parentOperType) const;

    /*!
     * \brief Обрабатывает узел типа функции.
     * \param[in] node Узел выражения, представляющий функцию.
     * \param[in,out] intermediateDescription Промежуточное описание функции.
     * \param[in] className Название класса, если функция принадлежит классу.
     * \return Описание узла в формате QHash<Case, QString>.
     */
    QHash<Case, QString> handleFunctionNode(const ExpressionNode *node, QHash<Case, QString> &intermediateDescription, const QString &className) const;

    /*!
     * \brief Обрабатывает узел типа константы.
     * \param[in] node Узел выражения, представляющий константу.
     * \return Описание узла в формате QHash<Case, QString>.
     */
    QHash<Case, QString> handleConstNode(const ExpressionNode *node) const;

    /*!
     * \brief Обрабатывает узел типа операции.
     * \param[in] node Узел выражения, представляющий операцию.
     * \param[in,out] intermediateDescription Промежуточное описание операции.
     * \param[in] className Название класса, если операция связана с классом.
     * \param[in] parentOperType Тип родительской операции.
     * \param[in,out] descOfLeftNode Описание левого поддерева.
     * \param[in,out] descOfRightNode Описание правого поддерева.
     * \return Описание узла в формате QHash<Case, QString>.
     */
    QHash<Case, QString> handleOperationNode(const ExpressionNode *node, QHash<Case, QString> &intermediateDescription, const QString &className, OperationType parentOperType, QHash<Case, QString> &descOfLeftNode, QHash<Case, QString> &descOfRightNode) const;
private:
    QString expression;                          /*!< Строка выражения */
    QHash<QString, Variable> variables;          /*!< Список переменных */
    QHash<QString, Function> functions;          /*!< Список функций */
    QHash<QString, Union> unions;                /*!< Список объединений */
    QHash<QString, Structure> structures;        /*!< Список структур */
    QHash<QString, Class> classes;               /*!< Список классов */
    QHash<QString, Enum> enums;                  /*!< Список перечислений */
};

#endif // EXPRESSION_H
