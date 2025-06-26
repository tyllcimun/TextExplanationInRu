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

QHash<Case, QString> Expression::toExplanation(const ExpressionNode *node, QHash<Case, QString> &intermediateDescription, const QString& className, OperationType parentOperType) const {
    switch (node->getNodeType()) {
    case EntityType::Operation:
        return processOperationExplanation(node, intermediateDescription, className, parentOperType);
    case EntityType::Const:
        return processConstExplanation(node);
    case EntityType::Function:
        return processFunctionExplanation(node, intermediateDescription, className);
    case EntityType::Variable:
        return processVariableExplanation(node, className, parentOperType);
    case EntityType::Enum:
        return {};
    default:
        throw TEException(ErrorType::UnidentifedType, QList<QString>{node->getDataType()});
    }
}

QHash<Case, QString> Expression::processOperationExplanation(const ExpressionNode *node, QHash<Case, QString> &intermediateDescription, const QString& className, OperationType parentOperType) const {
    QHash<Case, QString> descOfLeftNode;
    QHash<Case, QString> descOfRightNode;

    if (node->isReducibleUnarySelfInverse()) {
        return toExplanation(node->getLeftNode()->getLeftNode(), intermediateDescription, "", node->getOperType());
    }
    else if (node->getOperType() == OperationType::Not && node->getLeftNode()->isComparisonOperation()) {
        return toExplanation(node->getLeftNode(), intermediateDescription, "", node->getOperType());
    }
    else if (node->isIncrementOrDecrement()) {
        return handleIncrementOrDecrement(node, intermediateDescription, parentOperType);
    } else {
        descOfLeftNode = toExplanation(node->getLeftNode(), intermediateDescription, "", node->getOperType());

        if (node->getOperType() == OperationType::FieldAccess) {
            descOfRightNode = toExplanation(node->getRightNode(), intermediateDescription, node->getLeftNode()->getDataType(), node->getOperType());
        } else if (node->getOperType() == OperationType::StaticMemberAccess) {
            descOfRightNode = toExplanation(node->getRightNode(), intermediateDescription, node->getLeftNode()->getValue(), node->getOperType());
        } else if (node->getRightNode() != nullptr) {
            descOfRightNode = toExplanation(node->getRightNode(), intermediateDescription, "", node->getOperType());
        }
    }

    return buildOperationExplanation(node, descOfLeftNode, descOfRightNode, intermediateDescription, parentOperType);
}

QHash<Case, QString> Expression::processConstExplanation(const ExpressionNode *node) const {
    QHash<Case, QString> description;
    for (Case c : {Case::Nominative, Case::Genitive, Case::Dative, Case::Accusative, Case::Instrumental, Case::Prepositional}) {
        description[c] = node->getValue();
    }
    return description;
}

QHash<Case, QString> Expression::processFunctionExplanation(const ExpressionNode *node, QHash<Case, QString> &intermediateDescription, const QString& className) const {
    QHash<Case, QString> description;

    if (!className.isEmpty()) {
        description = this->getFunctionByNameFromCustomData(node->getValue(), className).description;
    } else {
        description = this->getFuncByName(node->getValue()).description;
    }

    if (node->getFunctionArgs()->count()) {
        description = ExpressionTranslator::getExplanation(description, argsToDescr(node->getFunctionArgs(), intermediateDescription, "", OperationType::FunctionCall));
    }

    return description;
}

QHash<Case, QString> Expression::processVariableExplanation(const ExpressionNode *node, const QString& className, OperationType parentOperType) const {
    QHash<Case, QString> description;

    if (!className.isEmpty()) {
        if (parentOperType == OperationType::FieldAccess) {
            description = this->getVariableByNameFromCustomData(node->getValue(), className).description;
        } else if (parentOperType == OperationType::StaticMemberAccess) {
            description = this->getEnumByName(className).values.value(node->getValue());
        }
    } else {
        description = this->getVarByName(node->getValue()).description;
    }

    return description;
}

QHash<Case, QString> Expression::buildOperationExplanation(const ExpressionNode *node, const QHash<Case, QString>& descOfLeftNode, const QHash<Case, QString>& descOfRightNode, QHash<Case, QString> &intermediateDescription, OperationType parentOperType) const {
    QHash<Case, QString> description;

    if (parentOperType == node->getOperType()) {
        if (node->getOperType() == OperationType::Subtraction && node->getLeftNode()->getOperType() != OperationType::Subtraction && node->getRightNode()->getOperType() != OperationType::Subtraction) {
            description = ExpressionTranslator::getExplanation(ExpressionTranslator::Templates.value(OperationType::SubtractionSequence), QList<QHash<Case, QString>>{descOfLeftNode, descOfRightNode});
        } else if (node->getOperType() == OperationType::Division && node->getLeftNode()->getOperType() != OperationType::Division && node->getRightNode()->getOperType() != OperationType::Division) {
            description = ExpressionTranslator::getExplanation(ExpressionTranslator::Templates.value(OperationType::DivisionSequence), QList<QHash<Case, QString>>{descOfLeftNode, descOfRightNode});
        } else {
            for (Case c : {Case::Nominative, Case::Genitive, Case::Dative, Case::Accusative, Case::Instrumental, Case::Prepositional}) {
                description[c] = descOfLeftNode[c] + ", " + descOfRightNode[c];
            }
        }
    } else {
        description = ExpressionTranslator::getExplanation(ExpressionTranslator::Templates.value(node->getOperType()), QList<QHash<Case, QString>>{descOfLeftNode, descOfRightNode});
    }

    return description;
}

QHash<Case, QString> Expression::handleIncrementOrDecrement(const ExpressionNode *node, QHash<Case, QString> &intermediateDescription, OperationType parentOperType) const {
    QHash<Case, QString> description = toExplanation(node->getLeftNode(), intermediateDescription, "", node->getOperType());

    if (intermediateDescription.isEmpty()) {
        if (parentOperType != OperationType::None) {
            QHash<Case, QString> secondValueDescription;
            for (Case c : {Case::Nominative, Case::Genitive, Case::Dative, Case::Accusative, Case::Instrumental, Case::Prepositional}) {
                secondValueDescription[c] = "{2 (в)}";
            }
            intermediateDescription = ExpressionTranslator::getExplanation(ExpressionTranslator::Templates.value(node->getOperType()), QList<QHash<Case, QString>>{description, secondValueDescription});
        } else {
            if (node->getOperType() == OperationType::PostfixIncrement || node->getOperType() == OperationType::PrefixIncrement) {
                intermediateDescription = ExpressionTranslator::getExplanation(ExpressionTranslator::Templates.value(OperationType::SingleIncrement), QList<QHash<Case, QString>>{description});
            } else if (node->getOperType() == OperationType::PostfixDecrement || node->getOperType() == OperationType::PrefixDecrement) {
                intermediateDescription = ExpressionTranslator::getExplanation(ExpressionTranslator::Templates.value(OperationType::SingleDecrement), QList<QHash<Case, QString>>{description});
            }
        }
    } else {
        QHash<Case, QString> nestedDescription = ExpressionTranslator::getExplanation(ExpressionTranslator::Templates.value(node->getOperType()), QList<QHash<Case, QString>>{description});
        intermediateDescription = ExpressionTranslator::getExplanation(intermediateDescription, QList<QHash<Case, QString>>{{}, nestedDescription});
    }

    return description;
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
    QStringList tokens = splitExpression(*this->getExpression());
    QStack<ExpressionNode*> nodeStack;
    int operationCounter = 0;
    QSet<QString> usedElements;

    for (const QString& token : tokens) {
        EntityType nodeType = getEntityTypeByStr(token);

        switch (nodeType) {
        case EntityType::Operation:
            processOperationNode(token, nodeStack, operationCounter);
            break;
        case EntityType::Const:
            processConstNode(token, nodeStack);
            break;
        case EntityType::Variable:
            processVariableNode(token, nodeStack, usedElements, customDataTypes);
            break;
        case EntityType::Function:
            processFunctionNode(token, nodeStack);
            break;
        default:
            throw TEException(ErrorType::UndefinedId, QList<QString>{token});
        }

        if (operationCounter > 20) {
            throw TEException(ErrorType::InputDataExprSizeExceeded);
        }
    }

    if (nodeStack.size() != 1) {
        throw TEException(ErrorType::MissingOperations);
    }

    return nodeStack.pop();
}

void Expression::processOperationNode(const QString& token, QStack<ExpressionNode*>& stack, int& operationCounter) {
    OperationType operType = getOperationTypeByStr(token);

    if (!stack.empty() && (operType == OperationType::PostfixIncrement || operType == OperationType::PrefixIncrement ||
                           operType == OperationType::PostfixDecrement || operType == OperationType::PrefixDecrement)) {
        throw TEException(ErrorType::MultipleIncrementDecrement, QList<QString>{stack.top()->getValue()});
    }

    ExpressionNode* right = nullptr;
    ExpressionNode* left = nullptr;

    if (stack.size() >= 2 && OperationMap.value(token).arity == OperationArity::Binary) {
        right = stack.pop();
        left = stack.pop();
    } else if ((stack.size() == 1 && operType == OperationType::Subtraction) ||
               (stack.size() >= 1 && OperationMap.value(token).arity == OperationArity::Unary)) {
        left = stack.pop();
        if (operType == OperationType::Subtraction) operType = OperationType::UnaryMinus;
    } else {
        throw TEException(ErrorType::MissingOperand, QList<QString>{token});
    }

    stack.push(new ExpressionNode(EntityType::Operation, token, left, right, "", operType));
    operationCounter++;
}

void Expression::processConstNode(const QString& token, QStack<ExpressionNode*>& stack) {
    if (token.startsWith("\"") && token.endsWith("\"")) {
        stack.push(new ExpressionNode(EntityType::Const, token, nullptr, nullptr, "string"));
    } else {
        stack.push(new ExpressionNode(EntityType::Const, token, nullptr, nullptr));
    }
}

void Expression::processVariableNode(const QString& token, QStack<ExpressionNode*>& stack, QSet<QString>& usedElements, const QSet<QString>& customDataTypes) {
    QString dataType = sanitizeDataType(getVariables()->value(token).type);

    if (customDataTypes.contains(dataType) || DataTypes.contains(dataType)) {
        usedElements.insert(dataType);
        stack.push(new ExpressionNode(EntityType::Variable, token, nullptr, nullptr, dataType));
    } else {
        throw TEException(ErrorType::UndefinedId, QList<QString>{token});
    }
}

void Expression::processFunctionNode(const QString& token, QStack<ExpressionNode*>& stack) {
    int argCount = token.mid(token.indexOf('(') + 1, token.indexOf(')') - token.indexOf('(') - 1).toInt();
    QList<ExpressionNode*>* functionArgs = new QList<ExpressionNode*>();

    if (stack.size() < argCount) {
        throw TEException(ErrorType::MissingOperand, QList<QString>{token});
    }

    for (int j = 0; j < argCount; j++) {
        functionArgs->prepend(stack.pop());
    }

    stack.push(new ExpressionNode(EntityType::Function, token, nullptr, nullptr, "", OperationType::None, functionArgs));
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
            isInd = false;
            throw TEException(ErrorType::InvalidSymbol, QList<QString>{str[0]});
        }
        // Остальные символы - латинские буквы, цифры или _
        for(int i = 0; i < str.length() && isInd == true; i++) {
            if (!(isLatinLetter(str[i]) || str[i].isDigit() || str[i] == '_')) {
                isInd = false;
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
