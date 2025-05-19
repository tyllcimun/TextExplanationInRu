#include "expressionnode.h"

ExpressionNode::ExpressionNode()
    : value(""),
    right(nullptr),
    left(nullptr),

    nodeType(EntityType::Undefined),
    operType(OperationType::None),
    dataType(""),
    FunctionArgs(nullptr) {}

ExpressionNode::ExpressionNode(EntityType nodeType, const QString &value, ExpressionNode *left, ExpressionNode *right, const QString &dataType, OperationType operType, QList<ExpressionNode *> *functionArgs)
    : value(value),
    right(right),
    left(left),
    nodeType(nodeType),
    operType(operType),
    dataType(dataType),
    FunctionArgs(functionArgs) {}

OperationType ExpressionNode::getOperType() const {
    return operType;
}

EntityType ExpressionNode::getNodeType() const {
    return nodeType;
}

QString ExpressionNode::getValue() const {
    return value;
}

QString ExpressionNode::getDataType() const {
    return dataType;
}

QList<ExpressionNode*>* ExpressionNode::getFunctionArgs() const {
    return FunctionArgs;
}

ExpressionNode* ExpressionNode::getRightNode() const {
    return right;
}

ExpressionNode* ExpressionNode::getLeftNode() const {
    return left;
}

void ExpressionNode::setOperType(OperationType newOperType) {
    operType = newOperType;
}

void ExpressionNode::setNodeType(EntityType newNodeType) {
    nodeType = newNodeType;
}

void ExpressionNode::setValue(QString newValue) {
    value = newValue;
}

void ExpressionNode::setDataType(QString newDataType) {
    dataType = newDataType;
}

void ExpressionNode::setFunctionArgs(QList<ExpressionNode*>* newFunctionArgs) {
    FunctionArgs = newFunctionArgs;
}

void ExpressionNode::setRightNode(ExpressionNode* newRightNode) {
    right = newRightNode;
}

void ExpressionNode::setLeftNode(ExpressionNode* newLeftNode) {
    left = newLeftNode;
}
