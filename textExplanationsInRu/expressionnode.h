#ifndef EXPRESSIONNODE_H
#define EXPRESSIONNODE_H
#include <QString>
#include "codeentity.h"

class ExpressionNode
{
public:
    ExpressionNode();
    ExpressionNode(EntityType nodeType, const QString& value,  ExpressionNode* left = nullptr, ExpressionNode* right = nullptr, const QString& dataType = "", OperationType operType = OperationType::None, QList<ExpressionNode*>* functionArgs = {});
    QString toString() const;
    bool isReducibleUnarySelfInverse() const;

    OperationType getOperType() const;
    EntityType getNodeType() const;
    QString getValue() const;
    QString getDataType() const;
    QList<ExpressionNode*>* getFunctionArgs() const;
    ExpressionNode* getRightNode() const;
    ExpressionNode* getLeftNode() const;

    void setOperType(OperationType newOperType);
    void setNodeType(EntityType newNodeType);
    void setValue(QString newValue);
    void setDataType(QString newDataType);
    void setFunctionArgs(QList<ExpressionNode*>* newFunctionArgs);
    void setRightNode(ExpressionNode* newRightNode);
    void setLeftNode(ExpressionNode* newLeftNode);

    bool operator ==(const ExpressionNode& other) const;
    bool operator !=(const ExpressionNode &other) const;

    bool compareFunctionArgs(const QList<ExpressionNode *> &args1, const QList<ExpressionNode *> &args2) const;
    bool isComparisonOperation() const;
    bool isIncrementOrDecrement() const;
private:
    QString value; // содержание ноды
    ExpressionNode* right;
    ExpressionNode* left;
    EntityType nodeType;

    OperationType operType;
    QString dataType; // тип данных
    QList<ExpressionNode*>* FunctionArgs; // аргументы функции
};

#endif // EXPRESSIONNODE_H
