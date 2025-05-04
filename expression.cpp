#include "expression.h"
#include "expressionxmlparser.h"
#include "expressiontranslator.h"

Expression::Expression(const QString inputXMLFile) {

    ExpressionXmlParser::readDataFromXML(inputXMLFile, *this);

}

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

const Variable Expression::getVarByName(QString name) const
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

const Function Expression::getFuncByName(QString name) const
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

const Union Expression::getUnionByName(QString name) const
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

const Structure Expression::getStructByName(QString name) const
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

const Class Expression::getClassByName(QString name) const
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

const Enum Expression::getEnumByName(QString name) const
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

QHash<Case, QString> Expression::toExplanation(const ExpressionNode *node, const QString &className, OperationType parentOperType) const
{
    //...Считать что описание пустое
    QHash<Case, QString> description = {};

    QHash<Case, QString> descOfRightNode = {};
    QHash<Case, QString> descOfLeftNode = {};

    //Если тип текущей ноды является операцией, то
    if(node->getNodeType() == EntityType::Operation) {
        descOfLeftNode = toExplanation(node->getLeftNode(), "" , node->getOperType()); //##Получить описание первой ноды сиблинга## (Рекурсия)

        //Если тип второй ноды сиблинга является полем, то
        if(node->getOperType() == OperationType::FieldAccess)
            descOfRightNode = toExplanation(node->getRightNode(), node->getLeftNode()->getDataType(), node->getOperType()); //##Получить описание второго сиблинга в соответствии с типом первого сиблинга## (Рекурсия)
        //Если тип второй ноды сиблинга является вариантом перечисления, то
        else if(node->getOperType() == OperationType::StaticMemberAccess)
            descOfRightNode = toExplanation(node->getRightNode(), node->getLeftNode()->getValue(), node->getOperType()); //##Получить описание второго сиблинга в соответствии с типом первого сиблинга## (Рекурсия)

        // Иначе если операция бинарная
        else if(node->getRightNode() != nullptr)
            descOfRightNode = toExplanation(node->getRightNode(), "", node->getOperType()); //##Получить описание второго сиблинга## (Рекурсия)

        //Если родительская нода является операцией такого же типа что и текущая, то
        if(parentOperType == node->getOperType()){
            //считать что описание это перечисление сиблингов через запятую
            for (Case c : {Case::Nominative, Case::Genitive, Case::Dative,
                           Case::Accusative, Case::Instrumental, Case::Prepositional}) {
                description[c] = descOfLeftNode[c] + ", " + descOfRightNode[c];
            }
        }
        //Иначе
        else{
            //Считать что описание это ##сформированная в соответствии с типом операции и операндами строка##
            description = ExpressionTranslator::getExplanation(ExpressionTranslator::Templates.value(node->getOperType()), QList<QHash<Case, QString>>{descOfLeftNode, descOfRightNode});
        }
    }
    //Иначе если тип текущей ноды является  константой, то
    else if(node->getNodeType() == EntityType::Const){
        //считать что описание это константа
        for (Case c : {Case::Nominative, Case::Genitive, Case::Dative,
                       Case::Accusative, Case::Instrumental, Case::Prepositional}) {
            description[c] = node->getValue();
        }
    }
    //Иначе если тип текущей ноды является функцией, то
    else if(node->getNodeType() == EntityType::Function){
        //Если поле принадлежит классу, то
        if(!className.isEmpty()){
            //Считать что описание это ##полученное описание функции## класса
            description = this->getFunctionByNameFromCustomData(node->getValue(), className).description;
        }
        //Иначе
        else{
            //Считать что описание это ##полученное описание функции##.
            description = this->getFuncByName(node->getValue()).description;
        }

        //Если у функции есть хотя бы один аргумент, то
        if(node->getFunctionArgs()->count()){
            //##Заполнить описание данными из аргументов функции##.
            description = ExpressionTranslator::getExplanation(description, argsToDescr(node->getFunctionArgs()));
        }
    }
    //Иначе если тип текущей ноды является переменной, то
    else if(node->getNodeType() == EntityType::Variable){
        //Если переменная является полем пользовательского типа
        if(className != ""){
            if(parentOperType == OperationType::FieldAccess){
                //считать что описание это ##полученное описание переменной класса##
                description = this->getVariableByNameFromCustomData(node->getValue(), className).description;
            }
            else if(parentOperType == OperationType::StaticMemberAccess){
                //считать что описание это ##полученное описание варианта перечисления##
                description = this->getEnumByName(className).values.value(node->getValue());
            }
        }
        //Иначе
        else{
            //считать что описание это ##полученное описание переменной##.
            description = this->getVarByName(node->getValue()).description;
        }
    }
    //Иначе если тип текущей ноды является перечисление, то
    else if(node->getNodeType() == EntityType::Enum){
        description = {};
    }

    //Иначе
    else{
        //Вызвать ошибку о несогласованности типов.
        throw TEException(ErrorType::UnidentifedType, QList<QString>{node->getDataType()});
    }
    //Вернуть описание
    return description;
}

QString Expression::getExplanationInRu()
{
    //...Считать что объяснение пустое
    QString explanation = "";
    // Преобразовать выражение в дерево
    ExpressionNode* explanationTree = this->expressionToNodes();
    // Получить объяснение выражения
    explanation = this->toExplanation(explanationTree).value(Case::Nominative);

    // Удалить дубликаты слов в полученном выражении
    explanation = removeDuplicates(explanation);
    return explanation;
}

ExpressionNode *Expression::expressionToNodes()
{

}

bool Expression::isConst(const QString &str)
{
    //...Считаем что строка не является константой
    bool ok = false;
    // Если строку можно перевести в число, то
    if(str.toInt() || str.toFloat() || str.toDouble()){
        // строка является константой
        ok = true;
    }
    return ok;
}

bool Expression::isVariable(const QString &str)
{
    bool ok = false;
    if(!isIdentifier(str)){
        ok = true;
    }
    else throw TEException(ErrorType::InvalidSymbol, QList<QString>{str});
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
        }
        // Остальные символы - латинские буквы, цифры или _
        for(int i = 0; i < str.length(); i++) {
            if (!(isLatinLetter(str[i]) || str[i].isDigit() || str[i] == '_')) {
                isInd = false;
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

QString Expression::removeDuplicates(const QString &str)
{
    return str;
}

QList<QHash<Case, QString>> Expression::argsToDescr(const QList<ExpressionNode *> *functionArgs, QString customDataType) const
{
    QList<QHash<Case, QString>> descriptions;
    QList<ExpressionNode *>::const_iterator i;
    for(i = functionArgs->constBegin(); i != functionArgs->constEnd(); i++){
        descriptions.append(toExplanation(*i));
    }
    return descriptions;
}
