#include "expressionnode.h"

// Конструктор по умолчанию
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

QString ExpressionNode::toString() const {
    QString result;

    // Добавляем информацию об узле
    result += value.isEmpty() ? "Unknown" : value;

    // Если это функция, добавляем аргументы
    if (nodeType == EntityType::Function && FunctionArgs) {
        result += "(";
        QStringList args;
        for (auto* arg : *FunctionArgs) {
            args << arg->toString(); // Рекурсивно вызываем для аргументов
        }
        result += args.join(", "); // Соединяем аргументы через запятую
        result += ")";
    }

    // Обрабатываем левый и правый узлы
    if (left || right) {
        result += " (";
        if (left) {
            result += left->toString();
        }
        result += "; ";
        if (right) {
            result += right->toString();
        }
        result += ")";
    }

    return result;
}

bool ExpressionNode::isReducibleUnarySelfInverse() const
{
    if(this->getLeftNode() != nullptr && this->getRightNode() == nullptr){
        if((this->getOperType() == this->getLeftNode()->getOperType() && (this->getOperType() == OperationType::UnaryMinus ||this->getOperType() == OperationType::Not)) ||
            (this->getOperType() == OperationType::Dereference && this->getLeftNode()->getOperType() == OperationType::AddressOf) ||
            (this->getOperType() == OperationType::AddressOf && this->getLeftNode()->getOperType() == OperationType::Dereference))
        {
            return true;
        }
    }
    return false;
}

bool ExpressionNode::isComparisonOperation() const {
    if (this != nullptr && this->getNodeType() == EntityType::Operation) {
        OperationType operType = this->getOperType();
        return operType == OperationType::LessThan ||
               operType == OperationType::LessThanOrEqual ||
               operType == OperationType::GreaterThan ||
               operType == OperationType::GreaterThanOrEqual ||
               operType == OperationType::Equal ||
               operType == OperationType::NotEqual ||
               operType == OperationType::NotLessThan ||
               operType == OperationType::NotLessThanOrEqual ||
               operType == OperationType::NotGreaterThan ||
               operType == OperationType::NotGreaterThanOrEqual;
    }
    return false;
}

bool ExpressionNode::isIncrementOrDecrement() const {
    if (this != nullptr && this->getNodeType() == EntityType::Operation) {
        OperationType operType = this->getOperType();
        return operType == OperationType::PostfixIncrement ||
               operType == OperationType::PrefixIncrement ||
               operType == OperationType::PostfixDecrement ||
               operType == OperationType::PrefixDecrement;
    }
    return false;
}

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

bool ExpressionNode::operator!=(const ExpressionNode& other) const {
    return !(*this == other);
}

bool ExpressionNode::operator==(const ExpressionNode& other) const {
    // Сравниваем основные поля узла
    bool areBasicFieldsEqual =
        value == other.value &&
        nodeType == other.nodeType &&
        operType == other.operType &&
        dataType == other.dataType;

    // Сравниваем дочерние узлы (рекурсивно)
    bool areLeftNodesEqual = (left == nullptr && other.left == nullptr) ||
                             (left != nullptr && other.left != nullptr && *left == *other.left);

    bool areRightNodesEqual = (right == nullptr && other.right == nullptr) ||
                              (right != nullptr && other.right != nullptr && *right == *other.right);

    // Сравниваем аргументы функции
    bool areFunctionArgsEqual = (FunctionArgs == nullptr && other.FunctionArgs == nullptr) ||
                                (FunctionArgs != nullptr && other.FunctionArgs != nullptr && compareFunctionArgs(*FunctionArgs, *other.FunctionArgs));

    // Итоговое сравнение
    return areBasicFieldsEqual && areLeftNodesEqual && areRightNodesEqual && areFunctionArgsEqual;
}

// Вспомогательная функция для сравнения списков аргументов функции
bool ExpressionNode::compareFunctionArgs(const QList<ExpressionNode*>& args1, const QList<ExpressionNode*>& args2) const {
    if (args1.size() != args2.size()) {
        return false; // Разные размеры списков
    }

    // Сравниваем содержимое списков
    for (int i = 0; i < args1.size(); ++i) {
        if (*args1[i] != *args2[i]) {
            return false; // Если хотя бы один узел отличается
        }
    }
    return true; // Все узлы совпадают
}
