#include "expressionxmlparser.h"
#include "teexception.h"
#include <QCoreApplication>
#include <QDir>

const QHash<QString, Case> ExpressionXmlParser::caseMapping = {
    {"именительный", Case::Nominative},
    {"родительный", Case::Genitive},
    {"дательный", Case::Dative},
    {"винительный", Case::Accusative},
    {"творительный", Case::Instrumental},
    {"предложный", Case::Prepositional}
};

void ExpressionXmlParser::readDataFromXML(const QString& inputFilePath, Expression &expression) {

    QList<TEException> errors;

    try {

        QDomDocument doc = readXML(inputFilePath, errors);
        parseQDomDocument(doc, expression, errors);
    }
    catch(...) {}

    if(errors.count() > 0) throw errors;
}

QDomDocument ExpressionXmlParser::readXML(const QString& inputFilePath, QList<TEException>& errors) {

    if(inputFilePath.isEmpty())
        errors.append(TEException(ErrorType::InputFileNotFound, inputFilePath));

    QTemporaryFile* tmpFilePath = createTempCopy(inputFilePath, errors);

    tmpFilePath->open();
    QString xmlContent = tmpFilePath->readAll();
    xmlContent = fixXmlFlags(xmlContent);

    QDomDocument doc;
    QString errorMsg;
    int errorLine, errorColumn;

    //std::cout << xmlContent.toStdString();
    if (!doc.setContent(xmlContent, &errorMsg, &errorLine, &errorColumn)) {
        delete tmpFilePath;
        errors.append(TEException(ErrorType::Parsing, inputFilePath, errorLine));
        throw NULL;
    }

    delete tmpFilePath;
    return doc;
}

QTemporaryFile *ExpressionXmlParser::createTempCopy(const QString &sourceFilePath, QList<TEException>& errors) {

    QTemporaryFile* tempFile = new QTemporaryFile(QDir(QCoreApplication::applicationDirPath()).filePath("temp_XXXXXX"));
    //tempFile->setAutoRemove(true);

    if (!tempFile->open()) {
        delete tempFile;
        errors.append(TEException(ErrorType::InputCopyFileCannotBeCreated, QList<QString>{sourceFilePath, QCoreApplication::applicationDirPath()}));
        throw NULL;
    }

    // Открываем исходный файл для чтения
    QFile sourceFile(sourceFilePath);
    if (!sourceFile.open(QIODevice::ReadOnly)){
        errors.append(TEException(ErrorType::InputFileNotFound, sourceFilePath));
        throw NULL;
    }

    tempFile->write(sourceFile.readAll());
    tempFile->close();
    sourceFile.close();

    return tempFile;
}

QString ExpressionXmlParser::escapeXmlText(const QString& text) {

    QString output = text;
    output.replace("&", "&amp;")
        .replace("<", "&lt;")
        .replace(">", "&gt;")
        .replace("\"", "&quot;")
        .replace("'", "&apos;");
    return output;

}

QString ExpressionXmlParser::fixXmlFlags(const QString& xmlString) {

    QString result = fixXmlExpression(xmlString);
    result = fixXmlCaseTags(result);

    return result;
}

QString ExpressionXmlParser::fixXmlExpression(const QString& xmlString) {
    QString result = xmlString;

    // Находим начало и конец тега <expression>
    int expressionStart = result.indexOf("<expression>");
    int expressionEnd = result.indexOf("</expression>");

    // Если тег <expression> найден
    if (expressionStart != -1 && expressionEnd != -1) {
        // Вычисляем позиции содержимого
        int contentStart = expressionStart + QString("<expression>").length();
        int contentLength = expressionEnd - contentStart;

        // Извлекаем содержимое
        QString content = result.mid(contentStart, contentLength);

        // Заменяем специальные символы
        QString escapedContent = escapeXmlText(content);

        // Заменяем оригинальное содержимое на обработанное
        result.replace(contentStart, contentLength, escapedContent);
    }

    return result;
}

QString ExpressionXmlParser::fixXmlCaseTags(const QString& xmlString) {
    QString result = xmlString;

    // Обработка всех тегов <case> (с конца)
    int caseEnd = result.length();
    while ((caseEnd = result.lastIndexOf("</case>", caseEnd)) != -1) {
        int caseStart = result.lastIndexOf("<case", caseEnd);
        if (caseStart == -1) break;

        // Найдём начало содержимого, игнорируя атрибуты внутри <case>
        int contentStart = result.indexOf('>', caseStart) + 1;
        int contentLength = caseEnd - contentStart;

        // Извлекаем содержимое
        QString content = result.mid(contentStart, contentLength);

        // Заменяем специальные символы
        QString escapedContent = escapeXmlText(content);

        // Заменяем оригинальное содержимое на обработанное
        result.replace(contentStart, contentLength, escapedContent);

        // Продолжаем поиск с предыдущей позиции
        caseEnd = caseStart;
    }

    return result;
}

void ExpressionXmlParser::parseQDomDocument(const QDomDocument& doc, Expression &expression, QList<TEException>& errors) {

    QDomElement root = doc.documentElement();
    if (root.isNull() || root.tagName() != "root") {
        errors.append(TEException(ErrorType::MissingRootElemnt));
        throw NULL;
    }

    validateElement(root, QList<QString>{}, QHash<QString, int>{{"expression", 1}, {"variables", 1}, {"functions", 1}, {"unions", 1}, {"structures", 1}, {"classes", 1}, {"enums", 1}}, errors);

    expression.setExpression(parseExpression(root.firstChildElement("expression"), errors));
    expression.setVariables(parseVariables(root.firstChildElement("variables"), errors));
    expression.setFunctions(parseFunctions(root.firstChildElement("functions"), errors));
    expression.setUnions(parseUnions(root.firstChildElement("unions"), errors));
    expression.setStructures(parseStructures(root.firstChildElement("structures"), errors));
    expression.setClasses(parseClasses(root.firstChildElement("classes"), errors));
    expression.setEnums(parseEnums(root.firstChildElement("enums"), errors));
}

QString ExpressionXmlParser::parseExpression(const QDomElement &_expression, QList<TEException>& errors)
{
    QString res = _expression.text();
    if(res.isEmpty() || res.length() < 1)
        errors.append(TEException(ErrorType::EmptyElementValue, _expression.lineNumber(), QList<QString>{"expression"}));


    if(res.length() > expressionMaxLength) errors.append(TEException(ErrorType::InputSizeExceeded, _expression.lineNumber(), QList<QString>{"expression", QString::number(res.length()), QString::number(expressionMaxLength)}));


    return res;
}

QHash<QString, Variable> ExpressionXmlParser::parseVariables(const QDomElement &_variables, QList<TEException>& errors)
{
    validateElement(_variables, QList<QString>{}, QHash<QString, int>{{"variable", childElementsMaxCount}}, errors, false);

    QHash<QString, Variable> result;
    if(_variables.childNodes().isEmpty()) return result;

    QDomNode childNode = _variables.firstChild();
    while (!childNode.isNull()) {

        Variable child = parseVariable(childNode.toElement(), errors);
        result.insert(child.name, child);
        childNode = childNode.nextSibling();
    }
    return result;
}

Variable ExpressionXmlParser::parseVariable(const QDomElement &_variable, QList<TEException>& errors)
{

    validateElement(_variable, QList<QString>{"name", "type"}, QHash<QString, int>{{"description", 1}}, errors, true);

    QString name = parseName(_variable, errors);
    QString type = _variable.attribute("type");
    QDomElement descr = _variable.firstChildElement("description");

    QHash<Case, QString> desc = parseCases(_variable.firstChildElement("description"), errors);
    return Variable(name, type, desc);
}

QHash<QString, Function> ExpressionXmlParser::parseFunctions(const QDomElement &_functions, QList<TEException>& errors)
{
    validateElement(_functions, QList<QString>{}, QHash<QString, int>{{"function", childElementsMaxCount}}, errors, false);

    QHash<QString, Function> result;
    if(_functions.childNodes().isEmpty()) return result;

    QDomNode childNode = _functions.firstChild();
    while (!childNode.isNull()) {

        Function child = parseFunction(childNode.toElement(), errors);
        result.insert(child.name, child);

        childNode = childNode.nextSibling();
    }
    return result;
}

Function ExpressionXmlParser::parseFunction(const QDomElement &_function, QList<TEException>& errors)
{
    validateElement(_function, QList<QString>{"name", "type", "paramsCount"}, QHash<QString, int>{{"description", 1}}, errors, true);

    QString name = parseName(_function, errors);
    QString type = parseType(_function, errors);
    int paramsCount = parseParamsCount(_function, errors);
    QHash<Case, QString> desc = parseCases(_function.firstChildElement("description"), errors);

    return Function(name, type, paramsCount, desc);
}

QHash<QString, Union> ExpressionXmlParser::parseUnions(const QDomElement &_unions, QList<TEException>& errors)
{
    validateElement(_unions, QList<QString>{}, QHash<QString, int>{{"union", childElementsMaxCount}}, errors, false);

    QHash<QString, Union> result;
    if(_unions.childNodes().isEmpty()) return result;

    QDomNode childNode = _unions.firstChild();
    while (!childNode.isNull()) {
        Union child = parseUnion(childNode.toElement(), errors);
        result.insert(child.name, child);

        childNode = childNode.nextSibling();
    }

    return result;
}

Union ExpressionXmlParser::parseUnion(const QDomElement &_union, QList<TEException>& errors)
{
    validateElement(_union, QList<QString>{"name"}, QHash<QString, int>{{"variables", 1}, {"functions", 1}}, errors, true);

    QString name = parseName(_union, errors);
    QHash<QString, Variable> variables = parseVariables(_union.firstChildElement("variables"), errors);
    QHash<QString, Function> functions = parseFunctions(_union.firstChildElement("functions"), errors);

    int elementsCount = variables.count() + functions.count();
    if(elementsCount > childElementsMaxCount)
        errors.append(TEException(ErrorType::InputElementsExceeded, _union.lineNumber(), QList<QString>{"union", QString::number(elementsCount), QString::number(childElementsMaxCount)}));

    return Union(name, variables, functions);
}

QHash<QString, Structure> ExpressionXmlParser::parseStructures(const QDomElement &_structures, QList<TEException>& errors)
{

    validateElement(_structures, QList<QString>{}, QHash<QString, int>{{"structure", childElementsMaxCount}}, errors, false);

    QHash<QString, Structure> result;
    if(_structures.childNodes().isEmpty()) return result;

    QDomNode childNode = _structures.firstChild();
    while (!childNode.isNull()) {

        Structure child = parseStructure(childNode.toElement(), errors);
        result.insert(child.name, child);

        childNode = childNode.nextSibling();
    }
    return result;
}

Structure ExpressionXmlParser::parseStructure(const QDomElement &_structure, QList<TEException>& errors)
{
    validateElement(_structure, QList<QString>{"name"}, QHash<QString, int>{{"variables", 1}, {"functions", 1}}, errors, true);

    QString name = parseName(_structure, errors);
    QHash<QString, Variable> variables = parseVariables(_structure.firstChildElement("variables"), errors);
    QHash<QString, Function> functions = parseFunctions(_structure.firstChildElement("functions"), errors);

    int elementsCount = variables.count() + functions.count();
    if(elementsCount > childElementsMaxCount)
        errors.append(TEException(ErrorType::InputElementsExceeded, _structure.lineNumber(), QList<QString>{"structure", QString::number(elementsCount), QString::number(childElementsMaxCount)}));

    return Structure(name, variables, functions);
}

QHash<QString, Class> ExpressionXmlParser::parseClasses(const QDomElement &_classes, QList<TEException>& errors)
{

    validateElement(_classes, QList<QString>{}, QHash<QString, int>{{"class", childElementsMaxCount}}, errors, false);

    QHash<QString, Class> result;
    if(_classes.childNodes().isEmpty()) return result;

    QDomNode childNode = _classes.firstChild();
    while (!childNode.isNull()) {

        Class child = parseClass(childNode.toElement(), errors);
        result.insert(child.name, child);

        childNode = childNode.nextSibling();
    }
    return result;
}

Class ExpressionXmlParser::parseClass(const QDomElement &_class, QList<TEException>& errors)
{
    validateElement(_class, QList<QString>{"name"}, QHash<QString, int>{{"variables", 1}, {"functions", 1}}, errors, true);

    QString name = parseName(_class, errors);
    QHash<QString, Variable> variables = parseVariables(_class.firstChildElement("variables"), errors);
    QHash<QString, Function> functions = parseFunctions(_class.firstChildElement("functions"), errors);

    int elementsCount = variables.count() + functions.count();
    if(elementsCount > childElementsMaxCount)
        errors.append(TEException(ErrorType::InputElementsExceeded, _class.lineNumber(), QList<QString>{"class", QString::number(elementsCount), QString::number(childElementsMaxCount)}));

    return Class(name, variables, functions);
}

QHash<QString, Enum> ExpressionXmlParser::parseEnums(const QDomElement &_enums, QList<TEException>& errors)
{

    validateElement(_enums, QList<QString>{}, QHash<QString, int>{{"enum", 20}}, errors, false);

    QHash<QString, Enum> result;
    if(_enums.childNodes().isEmpty()) return result;

    QDomNode childNode = _enums.firstChild();
    while (!childNode.isNull()) {

        Enum child = parseEnum(childNode.toElement(), errors);
        result.insert(child.name, child);

        childNode = childNode.nextSibling();
    }
    return result;
}

Enum ExpressionXmlParser::parseEnum(const QDomElement &_enum, QList<TEException>& errors)
{
    validateElement(_enum, QList<QString>{"name"}, QHash<QString, int>{{"value", 20}}, errors, true);

    QString name = parseName(_enum, errors);
    QHash<QString, QHash<Case, QString>> values = parseEnumValues(_enum, errors);

    return Enum(name, values);
}

QHash<QString, QHash<Case, QString>> ExpressionXmlParser::parseEnumValues(const QDomElement &_values, QList<TEException>& errors)
{
    QHash<QString, QHash<Case, QString>> result;
    // Перебираем все элементы <value> внутри <enum>
    QDomNodeList valueNodes = _values.elementsByTagName("value");
    for (int i = 0; i < valueNodes.size(); ++i) {
        QDomElement valueElement = valueNodes.at(i).toElement();

        validateElement(valueElement, QList<QString>{"name"}, QHash<QString, int>{{"description", 1}}, errors, true);

        QString valueName = valueElement.attribute("name");
        QHash<Case, QString> description = parseCases(valueElement.firstChildElement("description"), errors);

        result.insert(valueName, description);
    }

    return result;
}

QString ExpressionXmlParser::parseName(const QDomElement &element, QList<TEException>& errors) {

    QString res = element.attribute("name");
    if(res.isEmpty() || res.length() < 1)
    {
        errors.append(TEException(ErrorType::EmptyAttributeName, element.lineNumber(), {"name"}));
        return "";
    }
    if(res.length() > nameMaxLength) errors.append(TEException(ErrorType::InputSizeExceeded, element.lineNumber(), QList<QString>{res, QString::number(res.length()), QString::number(nameMaxLength)}));
    // Первый символ - латинская буква или _
    const QChar first = res[0];
    if (!(isLatinLetter(first) || first == '_')) {
        errors.append(TEException(ErrorType::InvalidName, element.lineNumber(), QList<QString>{res}));
    }

    // Остальные символы - латинские буквы, цифры или _
    for(int i = 0; i < res.length(); i++) {
        if (!(isLatinLetter(res[i]) || res[i].isDigit() || res[i] == '_')) {
            errors.append(TEException(ErrorType::InvalidName, element.lineNumber(), QList<QString>{res}));
        }
    }

    return res;

}

QString ExpressionXmlParser::parseType(const QDomElement& element, QList<TEException> &errors)
{
    QString res = element.attribute("type");
    if(res.isEmpty() || res.length() < 1) {
        errors.append(TEException(ErrorType::EmptyAttributeName, element.lineNumber(), QList<QString>{"type"}));
        return "";
    }
    if(res.length() > nameMaxLength) errors.append(TEException(ErrorType::InputSizeExceeded, element.lineNumber(), QList<QString>{res, QString::number(res.length()), QString::number(nameMaxLength)}));

    // Первый символ - латинская буква или _
    const QChar first = res[0];
    if (!(isLatinLetter(first) || first == '_')) {
        errors.append(TEException(ErrorType::InvalidType, element.lineNumber(), QList<QString>{res}));
    }

    return res;
}

int ExpressionXmlParser::parseParamsCount(const QDomElement& element, QList<TEException> &errors)
{
    QString res = element.attribute("paramsCount");
    if(res.isEmpty() || res.length() < 1) {
        errors.append(TEException(ErrorType::EmptyAttributeName, element.lineNumber(), QList<QString>{"paramsCount"}));
        return 0;
    }

    bool parseSuccess = false;
    int count = res.toInt(&parseSuccess);

    if(!parseSuccess) {
        errors.append(TEException(ErrorType::InvalidParamsCount, element.lineNumber(), {"res"}));
        count = 0;
    }

    if(count < 0 || count > functionParamsMaxCount) errors.append(TEException(ErrorType::InvalidParamsCount, element.lineNumber(), QList<QString>{QString::number(count)}));

    return count;
}

QHash<Case, QString> ExpressionXmlParser::parseCases(const QDomElement &parentElement, QList<TEException>& errors)
{
    QHash<Case, QString> cases;
    QDomNodeList caseNodes = parentElement.elementsByTagName("case");

    for (int i = 0; i < caseNodes.size(); i++) {
        QDomElement caseElement = caseNodes.at(i).toElement();
        QString caseType = caseElement.attribute("type").trimmed().toLower();

        Case currentCase = caseMapping[caseType];
        QString text = caseElement.text().trimmed();

        if(text.isEmpty()) errors.append(TEException(ErrorType::EmptyElementValue, caseElement.lineNumber(), QList<QString>{"case"}));
        if(text.length() > descMaxLength) errors.append(TEException(ErrorType::InputSizeExceeded, caseElement.lineNumber(), QList<QString>{text, QString::number(text.length()), QString::number(descMaxLength)}));

        cases.insert(currentCase, text);
    }

    return cases;
}

bool ExpressionXmlParser::isLatinLetter(const QChar c) {
    // Явная проверка латинских букв
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

void ExpressionXmlParser::validateElement(const QDomElement& curElement, const QList<QString>& allowedAttributes, const QHash<QString, int>& allowedElements, QList<TEException>& errors, bool checkRequired, bool textIsError) {
    validateAttributes(curElement, allowedAttributes, errors);
    validateChildElements(curElement, allowedElements, errors);

    if (checkRequired) {
        validateRequiredAttributes(curElement, allowedAttributes, errors);
        validateRequiredChildElements(curElement, allowedElements.keys(), errors);
    }

}

void ExpressionXmlParser::validateAttributes(const QDomElement& curElement, const QList<QString>& attributes, QList<TEException>& errors) {

    QDomNamedNodeMap getAttributes = curElement.attributes();
    for (int i = 0; i < getAttributes.length(); i++) {
        QDomAttr attribute = getAttributes.item(i).toAttr();

        // Проверяем, есть ли этот атрибут в списке elements
        if (!attributes.contains(attribute.name()) || attributes.count() == 0) {
            errors.append(TEException(ErrorType::UnexpectedAttribute, attribute.lineNumber(), QList<QString>{attribute.name(), attributes.join("; ")}));
        }
    }
}

void ExpressionXmlParser::validateChildElements(const QDomElement& curElement, const QHash<QString, int>& elements, QList<TEException>& errors) {

    QDomNode childNode = curElement.firstChild();
    while (!childNode.isNull()) {
        if (childNode.isElement()) {
            QDomElement childElement = childNode.toElement();
            QString childName = childElement.tagName();

            if (!elements.contains(childName) || elements.count() == 0) {
                errors.append(TEException(ErrorType::UnexpectedElement, childElement.lineNumber(), QList<QString>{childName, elements.keys().join("; ")}));
            }
            else {
                int count = countDirectChildren(curElement, childName);
                int value = elements.value(childName);
                if(count > value)
                    errors.append(TEException(ErrorType::DuplicateElement, childElement.lineNumber(), QList<QString>{childName}));
                // Специальная проверка для case-элементов в description
                if (childName == "description") {
                    validateCases(childElement, errors);
                }
            }
        }
        childNode = childNode.nextSibling();
    }
}

void ExpressionXmlParser::validateRequiredAttributes(const QDomElement& curElement, const QList<QString>& attributes, QList<TEException>& errors) {

    QListIterator<QString> iter(attributes);

    while(iter.hasNext()){
        QString it = iter.next();
        if (!curElement.hasAttribute(it)) {
            errors.append(TEException(ErrorType::MissingRequiredAttribute, curElement.lineNumber(), QList<QString>{it}));
        }
    }
}

void ExpressionXmlParser::validateRequiredChildElements(const QDomElement& curElement, const QList<QString>& elements, QList<TEException>& errors) {

    QListIterator<QString> iter(elements);
    while(iter.hasNext()){
        QString it = iter.next();

        bool foundCurElement = false;
        QDomNode childNode = curElement.firstChild();
        while (!childNode.isNull()) {
            if (childNode.isElement() && childNode.toElement().tagName() == it) {
                foundCurElement = true;
                break;
            }
            childNode = childNode.nextSibling();
        }

        if (!foundCurElement) {
            errors.append(TEException(ErrorType::MissingRequiredChildElement, curElement.lineNumber(), QList<QString>{it}));
        }

    }

}

void ExpressionXmlParser::validateCases(const QDomElement &curDescription, QList<TEException>& errors)
{
    // Список обязательных падежей
    const QSet<QString> requiredCases = {
        "именительный", "родительный", "дательный",
        "винительный", "творительный", "предложный"
    };

    QDomNodeList caseNodes = curDescription.elementsByTagName("case");
    if (caseNodes.size() > 0) {
        QSet<QString> foundCases;
        QSet<QString> duplicateCases;

        // Собираем все найденные падежи
        for (int i = 0; i < caseNodes.size(); ++i) {
            QDomElement caseElem = caseNodes.at(i).toElement();
            QString caseType = caseElem.attribute("type").trimmed().toLower();

            // Проверяем наличие атрибута "type"
            if (!caseElem.hasAttribute("type")) {
                throw TEException(ErrorType::MissingRequiredAttribute, caseElem.lineNumber(),
                                  {"type"});
            }
            // Проверяем на неожиданные значения атрибута "type"
            if (!requiredCases.contains(caseType)) {
                throw TEException(ErrorType::UnexpectedAttribute, caseElem.lineNumber(),
                                  {caseType, requiredCases.values().join(", ")});
            }
            // Проверяем на дублирующиеся значения
            if (foundCases.contains(caseType)) {
                duplicateCases.insert(caseType);
            } else {
                foundCases.insert(caseType);
            }
        }
        // Если найдены дубликаты, выбрасываем исключение
        if (!duplicateCases.isEmpty()) {
            errors.append(TEException(ErrorType::DuplicateElement, curDescription.lineNumber(),
                                      {QString("case type=\"%1\"").arg(duplicateCases.values().join(", "))}));
        }

        // Проверяем, что все обязательные падежи присутствуют
        QSet<QString> missingCases = requiredCases - foundCases;
        if (!missingCases.isEmpty()) {
            errors.append(TEException(ErrorType::MissingCases, curDescription.lineNumber(),
                                      QList<QString>{missingCases.values().join(", ")}));
        }
    }
    else errors.append(TEException(ErrorType::MissingCases, curDescription.lineNumber(),
                                  QList<QString>{requiredCases.values().join(", ")}));

}

int ExpressionXmlParser::countDirectChildren(const QDomElement& element, const QString& childName) {
    int count = 0;

    QDomNode childNode = element.firstChild();
    while (!childNode.isNull()) {
        if (childNode.isElement() && childNode.toElement().tagName() == childName) {
            count++;
        }
        childNode = childNode.nextSibling();
    }

    return count;
}

