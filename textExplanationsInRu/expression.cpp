#include "expression.h"
#include "expressionxmlparser.h"
#include "expressiontranslator.h"

void Expression::setExpression(const QString &newExpression)
{
    expression = newExpression;
}

const QString* Expression::getExpression() const
{
    return &expression;
}

const QHash<QString, Variable>* Expression::getVariables() const
{
    return &variables;
}

void Expression::setVariables(const QHash<QString, Variable> &newVariables)
{
    variables = newVariables;
}

const Variable Expression::getVarByName(const QString & name) const
{
    return getVariables()->value(name);
}

const QHash<QString, Function>* Expression::getFunctions() const
{
    return &functions;
}

void Expression::setFunctions(const QHash<QString, Function> &newFunctions)
{
    functions = newFunctions;
}

const Function Expression::getFuncByName(const QString & name) const
{
    return getFunctions()->value(name);
}

const QHash<QString, Union>* Expression::getUnions() const
{
    return &unions;
}

void Expression::setUnions(const QHash<QString, Union> &newUnions)
{
    unions = newUnions;
}

const Union Expression::getUnionByName(const QString & name) const
{
    return getUnions()->value(name);
}

const QHash<QString, Structure>* Expression::getStructures() const
{
    return &structures;
}

void Expression::setStructures(const QHash<QString, Structure> &newStructures)
{
    structures = newStructures;
}

const Structure Expression::getStructByName(const QString & name) const
{
    return getStructures()->value(name);
}

const QHash<QString, Class>* Expression::getClasses() const
{
    return &classes;
}

void Expression::setClasses(const QHash<QString, Class> &newClasses)
{
    classes = newClasses;
}

const Class Expression::getClassByName(const QString & name) const
{
    return getClasses()->value(name);
}

const QHash<QString, Enum>* Expression::getEnums() const
{
    return &enums;
}

void Expression::setEnums(const QHash<QString, Enum> &newEnums)
{
    enums = newEnums;
}

const Enum Expression::getEnumByName(const QString & name) const
{
    return getEnums()->value(name);
}

const Variable Expression::getVariableByNameFromCustomData(QString varName, QString dataName) const
{
    // Получить пользовательский тип данных по его имени
    CustomTypeWithFields customType = getCustomTypeByName(dataName);
    return customType.variables.value(varName);
}

const Function Expression::getFunctionByNameFromCustomData(QString funcName, QString dataName) const
{
    // Получить пользовательский тип данных по его имени
    CustomTypeWithFields customType = getCustomTypeByName(dataName);
    return customType.functions.value(funcName);
}

bool Expression::isEnumValue(const QString &value, const QString &enumName) const
{
    bool ok = false;
    if(getEnumByName(enumName).values.contains(value)) ok = true;
    return ok;
}

const CustomTypeWithFields Expression::getCustomTypeByName(const QString &typeName) const
{
    CustomTypeWithFields type;
    if (classes.contains(typeName)) {
        type = classes.value(typeName);
    }
    else if (structures.contains(typeName)) {
        type = structures.value(typeName);
    }
    else if (unions.contains(typeName)) {
        type = unions.value(typeName);
    }
    return type;
}

Expression Expression::fromFile(const QString &path)
{
    Expression expr;
    ExpressionXmlParser::readDataFromXML(path, expr);
    return expr;
}

QSet<QString> Expression::getCustomDataTypes() const
{
    QSet<QString> customDataTypes;

    for (auto it = unions.cbegin(); it != unions.cend(); ++it) {
        customDataTypes.insert(it.value().name);
    }
    for (auto it = structures.cbegin(); it != structures.cend(); ++it) {
        customDataTypes.insert(it.value().name);
    }
    for (auto it = classes.cbegin(); it != classes.cend(); ++it) {
        customDataTypes.insert(it.value().name);
    }
    for (auto it = enums.cbegin(); it != enums.cend(); ++it) {
        customDataTypes.insert(it.value().name);
    }

    return customDataTypes;
}


QString Expression::ToQstring()
{
    QString result = "Expression:   "  + this->expression;

    result += "\nVariables:";
    QHashIterator itV(this->variables);
    while(itV.hasNext()) {
        Variable val = itV.next().value();
        result += "\n" + val.toQString("    ");
    }

    result += "\nFunctions:";
    QHashIterator itF(this->functions);
    while(itF.hasNext()) {
        Function func = itF.next().value();
        result += "\n" + func.toQString("    ");
    }

    result += "\nUnions:";
    QHashIterator itU(this->unions);
    while(itU.hasNext()) {
        Union _union = itU.next().value();
        result += "\n" + _union.toQString("    ");
    }

    result += "\nStructures:";
    QHashIterator itS(this->structures);
    while(itS.hasNext()) {
        Structure structure = itS.next().value();
        result += "\n" + structure.toQString("    ");
    }

    result += "\nClasses:";
    QHashIterator itC(this->classes);
    while(itC.hasNext()) {
        Class _class = itC.next().value();
        result += "\n" + _class.toQString("    ");
    }

    result += "\nEnums:";
    QHashIterator itE(this->enums);
    while(itE.hasNext()) {
        Enum _enum = itE.next().value();
        result += "\n" + _enum.toQString("    ");
    }

    return result;
}

QHash<Case, QString> Expression::toExplanation(const ExpressionNode *node, QHash<Case, QString> &intermediateDescription, const QString& className, OperationType parentOperType) const
{
    QHash<Case, QString> description = {};
    QHash<Case, QString> descOfRightNode = {};
    QHash<Case, QString> descOfLeftNode = {};

    if(node->getNodeType() == EntityType::Operation) {
        description = handleOperationNode(node, intermediateDescription, className, parentOperType, descOfLeftNode, descOfRightNode);
    }
    else if(node->getNodeType() == EntityType::Const) {
        description = handleConstNode(node);
    }
    else if(node->getNodeType() == EntityType::Function) {
        description = handleFunctionNode(node, intermediateDescription, className);
    }
    else if(node->getNodeType() == EntityType::Variable) {
        description = handleVariableNode(node, className, parentOperType);
    }
    else if(node->getNodeType() == EntityType::Enum) {
        description = handleEnumNode();
    }
    else {
        throw TEException(ErrorType::UnidentifedType, QList<QString>{node->getDataType()});
    }

    if(!intermediateDescription.isEmpty() && parentOperType == OperationType::None) {
        description = ExpressionTranslator::getExplanation(intermediateDescription, QList<QHash<Case, QString>>{{}, description});
    }

    return description;
}

QHash<Case, QString> Expression::handleOperationNode(const ExpressionNode *node, QHash<Case, QString> &intermediateDescription, const QString& className, OperationType parentOperType, QHash<Case, QString> &descOfLeftNode, QHash<Case, QString> &descOfRightNode) const
{
    QHash<Case, QString> description = {};

    if(node->isReducibleUnarySelfInverse())
    {
        description = toExplanation(node->getLeftNode()->getLeftNode(), intermediateDescription, "", node->getOperType());
    }
    else if(node->getOperType() == OperationType::Not && node->getLeftNode()->isComparisonOperation())
    {
        description = toExplanation(node->getLeftNode(), intermediateDescription, "", node->getOperType());
    }
    else if(node->isIncrementOrDecrement())
    {
        QHash<Case, QString> secondValueDescription;
        for (Case c : {Case::Nominative, Case::Genitive, Case::Dative, Case::Accusative, Case::Instrumental, Case::Prepositional}) {
            secondValueDescription[c] = "{2 (в)}";
        }
        description = toExplanation(node->getLeftNode(), intermediateDescription, "", node->getOperType());

        if(intermediateDescription.isEmpty())
        {
            if (parentOperType != OperationType::None){
                intermediateDescription = ExpressionTranslator::getExplanation(ExpressionTranslator::Templates.value(node->getOperType()), QList<QHash<Case, QString>>{description, secondValueDescription});
            }
            else {
                if(node->getOperType() == OperationType::PostfixIncrement || node->getOperType() == OperationType::PrefixIncrement)
                    intermediateDescription = ExpressionTranslator::getExplanation(ExpressionTranslator::Templates.value(OperationType::SingleIncrement), QList<QHash<Case, QString>>{description});
                else if(node->getOperType() == OperationType::PostfixDecrement || node->getOperType() == OperationType::PrefixDecrement)
                    intermediateDescription = ExpressionTranslator::getExplanation(ExpressionTranslator::Templates.value(OperationType::SingleDecrement), QList<QHash<Case, QString>>{description});
            }
        }
        else {
            QHash<Case, QString> nestedDescription = ExpressionTranslator::getExplanation(ExpressionTranslator::Templates.value(node->getOperType()), QList<QHash<Case, QString>>{description, secondValueDescription});
            intermediateDescription = ExpressionTranslator::getExplanation(intermediateDescription, QList<QHash<Case, QString>>{{}, nestedDescription});
        }
    }
    else
    {
        descOfLeftNode = toExplanation(node->getLeftNode(), intermediateDescription, "" , node->getOperType());

        if(node->getOperType() == OperationType::FieldAccess)
            descOfRightNode = toExplanation(node->getRightNode(), intermediateDescription, node->getLeftNode()->getDataType(), node->getOperType());
        else if(node->getOperType() == OperationType::StaticMemberAccess)
            descOfRightNode = toExplanation(node->getRightNode(), intermediateDescription, node->getLeftNode()->getValue(), node->getOperType());
        else if(node->getRightNode() != nullptr)
            descOfRightNode = toExplanation(node->getRightNode(), intermediateDescription, "", node->getOperType());

        if(description.isEmpty()){
            if(parentOperType == node->getOperType()){
                if(node->getOperType() == OperationType::Subtraction && node->getLeftNode()->getOperType() != OperationType::Subtraction && node->getRightNode()->getOperType() != OperationType::Subtraction)
                    description = ExpressionTranslator::getExplanation(ExpressionTranslator::Templates.value(OperationType::SubtractionSequence), QList<QHash<Case, QString>>{descOfLeftNode, descOfRightNode});
                else if(node->getOperType() == OperationType::Division && node->getLeftNode()->getOperType() != OperationType::Division && node->getRightNode()->getOperType() != OperationType::Division)
                    description = ExpressionTranslator::getExplanation(ExpressionTranslator::Templates.value(OperationType::DivisionSequence), QList<QHash<Case, QString>>{descOfLeftNode, descOfRightNode});
                else {
                    for (Case c : {Case::Nominative, Case::Genitive, Case::Dative,
                                   Case::Accusative, Case::Instrumental, Case::Prepositional}) {
                        description[c] = descOfLeftNode[c] + ", " + descOfRightNode[c];
                    }
                }
            }
            else if((node->getOperType() == OperationType::Subtraction && node->getLeftNode()->getOperType() == OperationType::Subtraction) ||
                     (node->getOperType() == OperationType::Division && node->getLeftNode()->getOperType() == OperationType::Division))
            {
                for (Case c : {Case::Nominative, Case::Genitive, Case::Dative,
                               Case::Accusative, Case::Instrumental, Case::Prepositional}) {
                    description[c] = descOfLeftNode[c] + ", " + descOfRightNode[c];
                }
            }
            else if(node->getOperType() == OperationType::Dereference && node->getLeftNode()->getNodeType() == EntityType::Operation)
            {
                description = ExpressionTranslator::getExplanation(ExpressionTranslator::Templates.value(OperationType::PointerIndexAccess), QList<QHash<Case, QString>>{descOfLeftNode, descOfRightNode});
            }
            else if(node->isComparisonOperation() && parentOperType == OperationType::Not)
            {
                description = ExpressionTranslator::getExplanation(ExpressionTranslator::Templates.value(InverseComparisonOperationsMap.value(node->getOperType())), QList<QHash<Case, QString>>{descOfLeftNode, descOfRightNode});
            }
            else
            {
                if(node->getLeftNode()->getDataType() == "string" && node->getLeftNode()->getDataType() == node->getRightNode()->getDataType() && node->getOperType() == OperationType::Addition)
                    description = ExpressionTranslator::getExplanation(ExpressionTranslator::Templates.value(OperationType::Concatenation), QList<QHash<Case, QString>>{descOfLeftNode, descOfRightNode});
                else
                    description = ExpressionTranslator::getExplanation(ExpressionTranslator::Templates.value(node->getOperType()), QList<QHash<Case, QString>>{descOfLeftNode, descOfRightNode});
            }
        }
    }

    return description;
}

QHash<Case, QString> Expression::handleConstNode(const ExpressionNode *node) const
{
    QHash<Case, QString> description;
    for (Case c : {Case::Nominative, Case::Genitive, Case::Dative,
                   Case::Accusative, Case::Instrumental, Case::Prepositional}) {
        description[c] = node->getValue();
    }
    return description;
}

QHash<Case, QString> Expression::handleFunctionNode(const ExpressionNode *node, QHash<Case, QString> &intermediateDescription, const QString& className) const
{
    QHash<Case, QString> description;
    if(!className.isEmpty()){
        description = this->getFunctionByNameFromCustomData(node->getValue(), className).description;
    }
    else{
        description = this->getFuncByName(node->getValue()).description;
    }
    if(node->getFunctionArgs()->count()){
        description = ExpressionTranslator::getExplanation(description, argsToDescr(node->getFunctionArgs(), intermediateDescription, "", OperationType::FunctionCall));
    }
    return description;
}

QHash<Case, QString> Expression::handleVariableNode(const ExpressionNode *node, const QString& className, OperationType parentOperType) const
{
    QHash<Case, QString> description;
    if(className != ""){
        if(parentOperType == OperationType::FieldAccess){
            description = this->getVariableByNameFromCustomData(node->getValue(), className).description;
        }
        else if(parentOperType == OperationType::StaticMemberAccess){
            description = this->getEnumByName(className).values.value(node->getValue());
        }
    }
    else{
        description = this->getVarByName(node->getValue()).description;
    }
    return description;
}

QHash<Case, QString> Expression::handleEnumNode() const
{
    return {};
}

QString Expression::getExplanationInRu()
{
    //...Считать что объяснение пустое
    QString explanation = "";
    if(!this->getExpression()->isEmpty() || !this->getAllNames().isEmpty()){
        // Преобразовать выражение в дерево
        ExpressionNode* explanationTree = this->expressionToNodes();
        // Получить объяснение выражения
        QHash<Case, QString> intermediateDescription = {};
        explanation = this->toExplanation(explanationTree, intermediateDescription).value(Case::Nominative);
    }
    // Удалить дубликаты слов в полученном выражении
    explanation = removeConsecutiveDuplicates(explanation);
    return explanation;
}

QStringList Expression::splitExpression(const QString &str) {
    QStringList tokens;
    QString currentToken;
    bool insideQuotes = false;

    for (int i = 0; i < str.size(); ++i) {
        QChar currentChar = str[i];
        // Если обнаружили кавычку, переключаем режим обработки
        if (currentChar == '"') {
            insideQuotes = !insideQuotes;
            currentToken += currentChar;
        }
        // Если находимся внутри кавычек, добавляем символ в текущий токен
        else if (insideQuotes) {
            currentToken += currentChar;
        }
        // Если пробел и не внутри кавычек, завершаем текущий токен
        else if (currentChar.isSpace()) {
            if (!currentToken.isEmpty()) {
                tokens.append(currentToken);
                currentToken.clear();
            }
        }
        // Иначе добавляем символ в текущий токен
        else {
            currentToken += currentChar;
        }
    }
    // Добавляем последний токен, если он не пустой
    if (!currentToken.isEmpty()) {
        tokens.append(currentToken);
    }
    return tokens;
}

QString Expression::sanitizeDataType(const QString& dataType) {
    if (dataType.contains('[')) {
        return dataType.left(dataType.indexOf('['));
    } else if (dataType.contains('*')) {
        return dataType.left(dataType.indexOf('*'));
    }
    return dataType;
}

ExpressionNode* Expression::expressionToNodes() {
    QSet<QString> customDataTypes = getCustomDataTypes();
    // Разделяем выражение на лексемы
    QStringList tokens = splitExpression(*this->getExpression());
    //...Считаем, что стек узлов пустой
    QStack<ExpressionNode*> nodeStack;
    //...Считаем что количество операций = 0
    int operationCounter = 0;
    //...Считаем что ни один элемент не использован
    QSet<QString> usedElements;

    // Иначе если выражение было пустым, то дерева нет
    if(expression.isEmpty()) return new ExpressionNode();

    QStringList::const_iterator i;
    // Для каждой лексемы и пока количество операций не превышает 20
    for (i = tokens.constBegin(); i != tokens.constEnd() && operationCounter <= 20; i++) {
        // Получить тип лексемы
        EntityType nodeType = getEntityTypeByStr(*i);

        if (nodeType == EntityType::Operation) {
            processOperation(*i, nodeStack, operationCounter, tokens, i);
        }
        else if (nodeType == EntityType::Const) {
            processConst(*i, nodeStack);
        }
        else if (nodeType == EntityType::Variable) {
            processVariable(*i, nodeStack, usedElements, customDataTypes, tokens, i);
        }
        else if (nodeType == EntityType::Enum) {
            processEnum(*i, nodeStack, usedElements);
        }
        else if (nodeType == EntityType::Function) {
            processFunction(*i, nodeStack, customDataTypes, usedElements, tokens, i);
        }
        else if (nodeType == EntityType::Undefined || nodeType == EntityType::CustomTypeWithFields) {
            throw TEException(ErrorType::UndefinedId, QList<QString>{*i});
        }
    }

    finalizeNodeProcessing(nodeStack, *this->getExpression(), operationCounter, usedElements);


    return nodeStack.pop();
}

void Expression::processOperation(const QString& token, QStack<ExpressionNode*>& nodeStack, int& operationCounter, const QStringList& tokens, QStringList::const_iterator i) {
    // Увеличить счетчик операций
    operationCounter++;
    OperationType operType = getOperationTypeByStr(token);
    ExpressionNode* right = nullptr;
    ExpressionNode* left = nullptr;

    // Если операция – инкремент или декремент и следующая операция такого же типа
    if (!nodeStack.empty() &&
        (operType == OperationType::PostfixIncrement || operType == OperationType::PrefixIncrement ||
         operType == OperationType::PostfixDecrement || operType == OperationType::PrefixDecrement) &&
        (i + 1) != tokens.end())
    {
        OperationType newOperType = getOperationTypeByStr(*(i + 1));
        if ((newOperType == OperationType::PostfixIncrement || newOperType == OperationType::PrefixIncrement ||
             newOperType == OperationType::PostfixDecrement || newOperType == OperationType::PrefixDecrement))
            throw TEException(ErrorType::MultipleIncrementDecrement, QList<QString>{nodeStack.top()->getValue()});
    }

    if (nodeStack.size() >= 2 && OperationMap.value(token).arity == OperationArity::Binary) {
        right = nodeStack.pop();
        left = nodeStack.pop();
    }
    else if ((nodeStack.size() == 1 && operType == OperationType::Subtraction) ||
             (nodeStack.size() >= 1 && OperationMap.value(token).arity == OperationArity::Unary)) {
        left = nodeStack.pop();
        if (operType == OperationType::Subtraction) operType = OperationType::UnaryMinus;
    }
    else if (nodeStack.size() < 2) {
        throw TEException(ErrorType::MissingOperand, QList<QString>{token});
    }
    else if (nodeStack.size() > 2) {
        throw TEException(ErrorType::MissingOperations, QList<QString>{nodeStack.pop()->getValue()});
    }
    nodeStack.push(new ExpressionNode(EntityType::Operation, token, left, right, "", operType));
}

void Expression::processConst(const QString& token, QStack<ExpressionNode*>& nodeStack) {
    if (token.startsWith("\"") && token.endsWith("\""))
        nodeStack.push(new ExpressionNode(EntityType::Const, token, nullptr, nullptr, "string"));
    else
        nodeStack.push(new ExpressionNode(EntityType::Const, token, nullptr, nullptr));
}

void Expression::processVariable(const QString& token, QStack<ExpressionNode*>& nodeStack, QSet<QString>& usedElements, const QSet<QString>& customDataTypes, const QStringList& tokens, QStringList::const_iterator i) {
    QString className;
    QString dataType = getVariables()->value(token).type;
    // если тип данных не определен
    if (dataType == "") {
        dataType = handleVariableTypeInference(token, nodeStack, tokens, i, className);
    }
    if (dataType != "") {
        dataType = sanitizeDataType(dataType);
        if (customDataTypes.contains(dataType) || DataTypes.contains(dataType)) {
            if (customDataTypes.contains(dataType)) usedElements.insert(dataType);
            nodeStack.push(new ExpressionNode(EntityType::Variable, token, nullptr, nullptr, dataType));
            if (!className.isEmpty()) {
                usedElements.insert(className + "." + token);
                usedElements.insert(className);
            }
            else usedElements.insert(token);
        }
        else if (dataType == "void") throw TEException(ErrorType::VariableWithVoidType, QList<QString>{token});
        else throw TEException(ErrorType::UnidentifedType, QList<QString>{dataType});
    }
    else throw TEException(ErrorType::UndefinedId, QList<QString>{token});
}

void Expression::processEnum(const QString& token, QStack<ExpressionNode*>& nodeStack, QSet<QString>& usedElements) {
    nodeStack.push(new ExpressionNode(EntityType::Enum, token, nullptr, nullptr, ""));
    usedElements.insert(token);
}

void Expression::processFunction(const QString& token, QStack<ExpressionNode*>& nodeStack, const QSet<QString>& customDataTypes, QSet<QString>& usedElements, const QStringList& tokens, QStringList::const_iterator i) {
    int argCountStart = token.indexOf('(');
    int argCountEnd = token.indexOf(')');
    int argCount = token.mid(argCountStart + 1, argCountEnd - argCountStart - 1).toInt();
    QString funcName = token.left(argCountStart);
    QString className;
    QString funcDataType = sanitizeDataType(getFunctions()->value(funcName).type);

    if (funcDataType == "") {
        if (!nodeStack.empty()) {
            ExpressionNode* rightSibling = nodeStack.top();
            if (i + 1 != tokens.end()) {
                QString nextToken = *(i + 1);
                if (nextToken == "." || nextToken == "->") {
                    funcDataType = sanitizeDataType(getFunctionByNameFromCustomData(funcName, rightSibling->getDataType()).type);
                    className = sanitizeDataType(rightSibling->getDataType());
                }
            }
        }
    }

    if (funcDataType != "") {
        funcDataType = sanitizeDataType(funcDataType);
        if (argCount != getFunctions()->value(funcName).paramsCount)
            throw TEException(ErrorType::ParamsCountFunctionMissmatch, QList<QString>{token});
        QList<ExpressionNode*>* functionArgs = new QList<ExpressionNode*>();
        if (nodeStack.size() < argCount)
            throw TEException(ErrorType::MissingOperand, QList<QString>{token});
        else {
            for (int j = 0; j < argCount; j++) {
                functionArgs->prepend(nodeStack.pop());
            }
        }
        if (customDataTypes.contains(funcDataType) || DataTypes.contains(funcDataType) || funcDataType == "void") {
            if (customDataTypes.contains(funcDataType)) usedElements.insert(funcDataType);
            ExpressionNode* functionNode = new ExpressionNode(EntityType::Function, funcName, nullptr, nullptr, funcDataType, OperationType::None, functionArgs);
            nodeStack.push(functionNode);
            if (!className.isEmpty()) {
                usedElements.insert(className + "." + funcName);
                usedElements.insert(className);
            }
            else usedElements.insert(funcName);
        }
        else throw TEException(ErrorType::UnidentifedType, QList<QString>{funcDataType});
    }
    else throw TEException(ErrorType::UndefinedId, QList<QString>{funcName});
}

QString Expression::handleVariableTypeInference(const QString& token, QStack<ExpressionNode*>& nodeStack, const QStringList& tokens, QStringList::const_iterator i, QString& className) {
    QString dataType;
    if (!nodeStack.empty()) {
        ExpressionNode* rightSibling = nodeStack.top();
        if (i + 1 != tokens.end()) {
            QString nextToken = *(i + 1);
            if (nextToken == "." || nextToken == "->") {
                className = sanitizeDataType(rightSibling->getDataType());
                dataType = getVariableByNameFromCustomData(token, rightSibling->getDataType()).type;
            }
            else if (nextToken == "::") {
                dataType = isEnumValue(token, rightSibling->getValue()) ? rightSibling->getValue() : "";
                className = sanitizeDataType(dataType);
            }
        }
    }
    return dataType;
}

void Expression::finalizeNodeProcessing(QStack<ExpressionNode*>& nodeStack, const QString& expression, int operationCounter, const QSet<QString>& usedElements) {
    if (nodeStack.size() > 1) throw TEException(ErrorType::MissingOperations, QList<QString>{nodeStack.pop()->getValue()});
    else if (expression.isEmpty()) return; // Возвращаем nullptr или new ExpressionNode() - по твоей логике

    else if (operationCounter > 20) throw TEException(ErrorType::InputDataExprSizeExceeded, QList<QString>{QString::number(operationCounter)});

    QSet<QString> allElements = this->getAllNames();
    QSet<QString> unusedElements = allElements - usedElements;

    if (!unusedElements.isEmpty())
        throw TEException(ErrorType::NeverUsedElement, QList<QString>{unusedElements.values().join(", ")});
}

void Expression::getCustomTypeFields(QSet<QString>& names, const CustomTypeWithFields& customType) {
    names.insert(customType.name);
    for (auto i = customType.variables.cbegin(); i != customType.variables.cend(); i++) {
        names.insert(customType.name + "." + i.value().name);
    }
    for (auto i = customType.functions.cbegin(); i != customType.functions.cend(); i++) {
        names.insert(customType.name + "." + i.value().name);
    }
}


QSet<QString> Expression::getAllNames() {
    QSet<QString> names;
    // переменные
    for (auto i = variables.cbegin(); i != variables.cend(); i++) {
        names.insert(i.value().name);
    }
    // функции
    for (auto i = functions.cbegin(); i != functions.cend(); i++) {
        names.insert(i.value().name);
    }
    for (auto i = unions.cbegin(); i != unions.cend(); i++) {
        getCustomTypeFields(names, i.value());
    }
    for (auto i = structures.cbegin(); i != structures.cend(); i++) {
        getCustomTypeFields(names, i.value());
    }
    for (auto i = classes.cbegin(); i != classes.cend(); i++) {
        getCustomTypeFields(names, i.value());
    }
    // перечисления
    for (auto i = enums.cbegin(); i != enums.cend(); i++) {
        names.insert(i.value().name);
        for (auto enumI = i.value().values.cbegin(); enumI != i.value().values.cend(); enumI++) {
            names.insert(i.value().name + "." + enumI.key());
        }
    }
    return names;
}

EntityType Expression::getEntityTypeByStr(const QString &str)
{
    //...Считаем, что тип неопределен
    EntityType type = EntityType::Undefined;
    if(isConst(str)) type = EntityType::Const;
    else if(isFunction(str)) type = EntityType::Function;
    else if(isCustomTypeWithFields(str)) type = EntityType::CustomTypeWithFields;
    else if(isEnum(str)) type = EntityType::Enum;
    else if(getOperationTypeByStr(str) != OperationType::None) type = EntityType::Operation;
    else if(isVariable(str)) type = EntityType::Variable;
    return type;
}

bool Expression::isConst(const QString &str)
{
    //...Считаем что строка не является константой
    bool ok = false;
    // Если строку можно перевести в число или строка константа типа bool то
    if(str.toInt() || str.toFloat() || str.toDouble() || str == "true" || str == "false" ||
        (str.startsWith("\"") && str.endsWith("\""))){
        // строка является константой
        ok = true;
    }
    return ok;
}

bool Expression::isVariable(const QString &str)
{
    bool ok = false;
    if(isIdentifier(str)){
        ok = true;
    }
    else throw TEException(ErrorType::InvalidSymbol, QList<QString>{str});
    return ok;
}

bool Expression::isFunction(const QString &str)
{
    bool ok = false;
    if(str.contains('(') && str.endsWith(')')){
        QString identifier = str.left(str.indexOf('('));
        QString contentInParentheses = str.mid(str.indexOf('(') + 1, str.length() - str.indexOf('(') - 2).trimmed();
        bool isNumber = false;
        contentInParentheses.toDouble(&isNumber);
        if(isIdentifier(identifier) && isNumber) ok = true;
    }
    return ok;
}

bool Expression::isCustomTypeWithFields(const QString &str)
{
    bool ok = false;
    if(getCustomTypeByName(str).name != ""){
        ok = true;
    }
    return ok;
}

bool Expression::isEnum(const QString &str)
{
    bool ok = false;
    if(getEnumByName(str).name != ""){
        ok = true;
    }
    return ok;
}

bool Expression::isIdentifier(const QString &str)
{
    bool isInd = true;
    // Первый символ - латинская буква или _
    if (str.isEmpty()) isInd = false;
    else{
        if (!(isLatinLetter(str[0]) || str[0] == '_')) {
            throw TEException(ErrorType::InvalidSymbol, QList<QString>{str[0]});
        }
        // Остальные символы - латинские буквы, цифры или _
        for(int i = 0; i < str.length() && isInd == true; i++) {
            if (!(isLatinLetter(str[i]) || str[i].isDigit() || str[i] == '_')) {
                throw TEException(ErrorType::InvalidSymbol, QList<QString>{str[i]});
            }
        }
    }
    return isInd;
}

bool Expression::isLatinLetter(const QChar c)
{
    // Явная проверка латинских букв
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

OperationType Expression::getOperationTypeByStr(const QString &str)
{
    OperationType type = OperationType::None;
    if(OperationMap.contains(str)){
        type = OperationMap.value(str).type;
    }
    return type;
}

QString Expression::removeConsecutiveDuplicates(const QString &str)
{
    // Разделить строку на отдельные слова
    QStringList words = str.split(' ', Qt::SkipEmptyParts);
    QStringList copyOfWords = str.toLower().split(' ', Qt::SkipEmptyParts);


    QStringList seenWords;
    QStringList result;

    // Для каждого слова в строке
    for (int i = 0; i < words.count(); i++) {
        // Если предыдущее слово в строке не совпадает с текущим
        if (!seenWords.endsWith(copyOfWords[i]) || (copyOfWords[i].endsWith(","))) {
            // Запоминаем его и добавляем в результирующую строку
            seenWords.append(copyOfWords[i]);
            result.append(words[i]);
        }
    }
    return result.join(" ");
}

QList<QHash<Case, QString>> Expression::argsToDescr(const QList<ExpressionNode *> *functionArgs, QHash<Case, QString>& intermediateDescription, QString customDataType, OperationType parentOperType) const
{
    QList<QHash<Case, QString>> descriptions;
    QList<ExpressionNode *>::const_iterator i;
    for(i = functionArgs->constBegin(); i != functionArgs->constEnd(); i++){
        descriptions.append(toExplanation(*i, intermediateDescription, customDataType, parentOperType));
    }
    return descriptions;
}
