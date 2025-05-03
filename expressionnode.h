#ifndef EXPRESSIONNODE_H
#define EXPRESSIONNODE_H
#include <QString>
#include "codeentity.h"

class ExpressionNode
{
public:
    ExpressionNode();

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
