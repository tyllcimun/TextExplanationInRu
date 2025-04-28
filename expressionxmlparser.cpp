#include "expressionxmlparser.h"
#include "teexception.h"
#include <QCoreApplication>
#include <QDir>

void ExpressionXmlParser::readDataFromXML(const QString& inputFilePath, Expression &expression) {

    QDomDocument doc = readXML(inputFilePath);

    parseQDomDocument(doc, expression);

    try {

    }
    catch(TEException exception) {
        throw TEException(exception.getErrorType(), inputFilePath, exception.getLine(), exception.getArgs());
    }
}

QDomDocument ExpressionXmlParser::readXML(const QString& inputFilePath) {

    if(inputFilePath.isEmpty())
        throw TEException(ErrorType::InputFileNotFound, inputFilePath);

    QTemporaryFile* tmpFilePath = createTempCopy(inputFilePath);

    tmpFilePath->open();
    QString xmlContent = tmpFilePath->readAll();
    xmlContent = fixXmlFlags(xmlContent);

    QDomDocument doc;
    QString errorMsg;
    int errorLine, errorColumn;

    if (!doc.setContent(xmlContent, &errorMsg, &errorLine, &errorColumn)) {
        delete tmpFilePath;
        throw TEException(ErrorType::Parssing, inputFilePath, errorLine);
    }

    delete tmpFilePath;
    return doc;
}

QTemporaryFile *ExpressionXmlParser::createTempCopy(const QString &sourceFilePath) {

    QTemporaryFile* tempFile = new QTemporaryFile(QDir(QCoreApplication::applicationDirPath()).filePath("temp_XXXXXX"));

    if (!tempFile->open()) {
        delete tempFile;
        throw TEException(ErrorType::InputCopyFileCannotBeCreated);
    }

    // Открываем исходный файл для чтения
    QFile sourceFile(sourceFilePath);
    if (!sourceFile.open(QIODevice::ReadOnly)){
        throw TEException(ErrorType::InputFileNotFound);
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

void ExpressionXmlParser::parseQDomDocument(const QDomDocument& doc, Expression &expression) {

    QDomElement root = doc.documentElement();
    if (root.isNull() || root.tagName() != "root") {
        throw TEException(ErrorType::MissingRootElemnt);
    }

    validateElement(root, QList<QString>{}, QHash<QString, int>{{"expression", 1}, {"variables", 1}, {"functions", 1}, {"unions", 1}, {"structures", 1}, {"classes", 1}, {"enums", 1}});

    expression.setExpression(parseExpression(root.firstChildElement("expression")));
    expression.setVariables(parseVariables(root.firstChildElement("variables")));
    //TODO реализовать парсинг элементов
}

QString ExpressionXmlParser::parseExpression(const QDomElement &_expression)
{
    QString res = _expression.text();
    if(res.isEmpty() || res.length() < 1)
        throw TEException(ErrorType::EmptyElementValue, _expression.lineNumber());

    if(res.length() > expressionMaxLength) throw TEException(ErrorType::InputSizeExceeded, _expression.lineNumber(), QList<QString>{"description", QString::number(res.length()), QString::number(expressionMaxLength)});

    return res;
}

QHash<QString, Variable> ExpressionXmlParser::parseVariables(const QDomElement &_variables)
{
    validateElement(_variables, QList<QString>{}, QHash<QString, int>{{"variable", childElementsMaxCount}}, false);

    QHash<QString, Variable> result;
    if(_variables.childNodes().isEmpty()) return result;

    QDomNode childNode = _variables.firstChild();
    while (!childNode.isNull()) {

        Variable child = parseVariable(childNode.toElement());
        result.insert(child.name, child);
        childNode = childNode.nextSibling();
    }

    return result;
}

Variable ExpressionXmlParser::parseVariable(const QDomElement &_variable)
{
    validateElement(_variable, QList<QString>{"name", "type"}, QHash<QString, int>{{"description", 1}}, true);

    QString name = parseName(_variable);
    QString type = _variable.attribute("type");
    QHash<Case, QString> desc = parseCases(_variable.firstChildElement("description"));

    return Variable(name, type, desc);
}

QString ExpressionXmlParser::parseName(const QDomElement &element) {
    QString res = element.attribute("name");
    return res;
}
QHash<Case, QString> ExpressionXmlParser::parseCases(const QDomElement &parentElement)
{
    QHash<Case, QString> cases;
    return cases;
}

void ExpressionXmlParser::validateElement(const QDomElement& curElement, const QList<QString>& allowedAttributes, const QHash<QString, int>& allowedElements, bool checkRequired, bool textIsError) {
    validateAttributes(curElement, allowedAttributes);
    validateChildElements(curElement, allowedElements);

    if (checkRequired) {
        validateRequiredAttributes(curElement, allowedAttributes);
        validateRequiredChildElements(curElement, allowedElements.keys());
    }

}

void ExpressionXmlParser::validateAttributes(const QDomElement& curElement, const QList<QString>& attributes) {

    QDomNamedNodeMap getAttributes = curElement.attributes();
    for (int i = 0; i < getAttributes.length(); i++) {
        QDomAttr attribute = getAttributes.item(i).toAttr();

        // Проверяем, есть ли этот атрибут в списке elements
        if (!attributes.contains(attribute.name()) || attributes.count() == 0) {
            throw TEException(ErrorType::UnexpectedAttribute, attribute.lineNumber(), QList<QString>{attribute.name(), attributes.join("; ")});
        }
    }
}

void ExpressionXmlParser::validateChildElements(const QDomElement& curElement, const QHash<QString, int>& elements) {

    QDomNode childNode = curElement.firstChild();
    while (!childNode.isNull()) {
        if (childNode.isElement()) {
            QDomElement childElement = childNode.toElement();
            QString childName = childElement.tagName();

            if (!elements.contains(childName) || elements.count() == 0) {
                throw TEException(ErrorType::UnexpectedElement, childElement.lineNumber(), QList<QString>{childName, elements.keys().join("; ")});
            }
            else {
                int count = countDirectChildren(curElement, childName);
                int value = elements.value(childName);
                if(count > value)
                    throw TEException(ErrorType::DuplicateElement, childElement.lineNumber(), QList<QString>{childName});
            }
        }

        childNode = childNode.nextSibling();
    }
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

void ExpressionXmlParser::validateRequiredAttributes(const QDomElement& curElement, const QList<QString>& attributes) {

    QListIterator<QString> iter(attributes);

    while(iter.hasNext()){
        QString it = iter.next();
        if (!curElement.hasAttribute(it)) {
            throw TEException(ErrorType::MissingRequiredAttribute, curElement.lineNumber(), QList<QString>{it});
        }
    }
}

void ExpressionXmlParser::validateRequiredChildElements(const QDomElement& curElement, const QList<QString>& elements) {
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
            throw TEException(ErrorType::MissingRequiredChildElement, curElement.lineNumber(), QList<QString>{it});
        }
    }
}

