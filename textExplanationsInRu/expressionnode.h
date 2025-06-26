/*!
 * \file
 * \brief Заголовочный файл, содержащий описание класса ExpressionNode, представляющего узел дерева выражения.
 */

#ifndef EXPRESSIONNODE_H
#define EXPRESSIONNODE_H
#include <QString>
#include "codeentity.h"

/*!
 * \brief Класс, представляющий узел дерева математического или логического выражения.
 */
class ExpressionNode
{
public:
    /*!
     * \brief Конструктор по умолчанию.
     */
    explicit ExpressionNode();

    /*!
     * \brief Конструктор с параметрами.
     * \param[in] nodeType Тип сущности (переменная, функция, оператор и т.д.).
     * \param[in] value Значение узла (например, имя переменной или символ оператора).
     * \param[in] left Указатель на левый дочерний узел.
     * \param[in] right Указатель на правый дочерний узел.
     * \param[in] dataType Тип данных.
     * \param[in] operType Тип операции.
     * \param[in] functionArgs Аргументы функции (если применимо).
     */
    explicit ExpressionNode(EntityType nodeType, const QString& value,
                            ExpressionNode* left = nullptr,
                            ExpressionNode* right = nullptr,
                            const QString& dataType = "",
                            OperationType operType = OperationType::None,
                            QList<ExpressionNode*>* functionArgs = {});

    /*!
     * \brief Преобразует дерево выражения в строку.
     * \return Строковое представление выражения.
     */
    QString toString() const;

    /*!
     * \brief Проверяет, является ли унарная операция обратимой к самой себе (например, `!`).
     * \return true, если операция обратима.
     */
    bool isReducibleUnarySelfInverse() const;

    /*!
     * \brief Получение типа операции.
     * \return Тип операции (OperationType).
     */
    OperationType getOperType() const;

    /*!
     * \brief Получение типа сущности.
     * \return Тип сущности (EntityType).
     */
    EntityType getNodeType() const;

    /*!
     * \brief Получение значения узла.
     * \return Значение (строка).
     */
    QString getValue() const;

    /*!
     * \brief Получение типа данных.
     * \return Тип данных в строковом виде.
     */
    QString getDataType() const;

    /*!
     * \brief Получение аргументов функции.
     * \return Указатель на список аргументов функции.
     */
    QList<ExpressionNode*>* getFunctionArgs() const;

    /*!
     * \brief Получение правого дочернего узла.
     * \return Указатель на правый узел.
     */
    ExpressionNode* getRightNode() const;

    /*!
     * \brief Получение левого дочернего узла.
     * \return Указатель на левый узел.
     */
    ExpressionNode* getLeftNode() const;

    /*!
     * \brief Установка типа операции.
     * \param[in] newOperType Новый тип операции.
     */
    void setOperType(OperationType newOperType);

    /*!
     * \brief Установка типа операции.
     * \param[in] newOperType Новый тип операции.
     */
    /*!
     * \brief Установка типа сущности.
     * \param[in] newNodeType Новый тип сущности.
     */
    void setNodeType(EntityType newNodeType);

    /*!
     * \brief Установка значения узла.
     * \param[in] newValue Новое значение.
     */
    void setValue(QString newValue);

    /*!
     * \brief Установка типа данных.
     * \param[in] newDataType Новый тип данных.
     */
    void setDataType(QString newDataType);

    /*!
     * \brief Установка аргументов функции.
     * \param[in] newFunctionArgs Новый список аргументов.
     */
    void setFunctionArgs(QList<ExpressionNode*>* newFunctionArgs);

    /*!
     * \brief Установка правого дочернего узла.
     * \param[in] newRightNode Новый правый узел.
     */
    void setRightNode(ExpressionNode* newRightNode);

    /*!
     * \brief Установка левого дочернего узла.
     * \param[in] newLeftNode Новый левый узел.
     */
    void setLeftNode(ExpressionNode* newLeftNode);

    /*!
     * \brief Сравнение узлов на равенство.
     * \param[in] other Узел для сравнения.
     * \return true, если узлы равны.
     */
    bool operator==(const ExpressionNode& other) const;

    /*!
     * \brief Сравнение узлов на неравенство.
     * \param[in] other Узел для сравнения.
     * \return true, если узлы различны.
     */
    bool operator!=(const ExpressionNode& other) const;

    /*!
     * \brief Сравнение двух списков аргументов функции.
     * \param[in] args1 Первый список.
     * \param[in] args2 Второй список.
     * \return true, если аргументы равны.
     */
    bool compareFunctionArgs(const QList<ExpressionNode*>& args1, const QList<ExpressionNode*>& args2) const;

    /*!
     * \brief Проверка, является ли операция сравнением (==, !=, < и т.д.).
     * \return true, если операция — сравнение.
     */
    bool isComparisonOperation() const;

    /*!
     * \brief Проверка, является ли операция инкрементом или декрементом (++/--).
     * \return true, если операция — инкремент или декремент.
     */
    bool isIncrementOrDecrement() const;
private:
    QString value;                          /*!< Значение узла */
    ExpressionNode* right;                  /*!< Правый дочерний узел */
    ExpressionNode* left;                   /*!< Левый дочерний узел */
    EntityType nodeType;                    /*!< Тип сущности */
    OperationType operType;                 /*!< Тип операции */
    QString dataType;                       /*!< Тип данных */
    QList<ExpressionNode*>* FunctionArgs;   /*!< Аргументы функции */
};

#endif // EXPRESSIONNODE_H
