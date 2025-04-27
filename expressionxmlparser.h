#ifndef EXPRESSIONXMLPARSER_H
#define EXPRESSIONXMLPARSER_H

#include "expression.h"
#include <QDomDocument>
#include <QString>
#include <QTemporaryFile>


class ExpressionXmlParser
{
public:

    static void readDataFromXML(const QString& inputFilePath, Expression& expression);

private:

    //////////////////////////////////////////////////
    /// Методы для работы с файлами
    /////////////////////////////////////////////////

    static QDomDocument readXML(const QString& filePath);
    static bool checkFileReadAccess(const QString& filePath);
    static QTemporaryFile* createTempCopy(const QString &sourceFilePath);

    //////////////////////////////////////////////////
    /// Методы для исправления XML формата
    /////////////////////////////////////////////////

    static QString escapeXmlText(const QString& text);
    static QString fixXmlFlags(const QString& xmlString);
    static QString fixXmlExpression(const QString& xmlString);
    static QString fixXmlCaseTags(const QString& xmlString);

    //////////////////////////////////////////////////
    /// Методы для обработки XML
    /////////////////////////////////////////////////

    static void parseQDomDocument(const QDomDocument& doc, Expression &expression);
    static QString parseExpression(const QDomElement& _expression);


    //////////////////////////////////////////////////
    /// Методы для валидации XML элементов и атрибутов
    /////////////////////////////////////////////////

    static void validateElement(const QDomElement& curElement, const QList<QString>& allowedAttributes, const QHash<QString, int>& allowedElements, bool checkRequired = true, bool textIsError = true);
    static void validateAttributes(const QDomElement& curElement, const QList<QString>& attributes);
    static void validateChildElements(const QDomElement& curElement, const QHash<QString, int>& allowedElements);
    static void validateRequiredAttributes(const QDomElement& curElement, const QList<QString>& attributes);
    static void validateRequiredChildElements(const QDomElement& curElement, const QList<QString>& elements);
    static int countDirectChildren(const QDomElement& element, const QString& childName);
    static bool isLatinLetter(const QChar c);

    //////////////////////////////////////////////////
    /// Константы
    /////////////////////////////////////////////////

    static constexpr int nameMaxLength = 32;
    static constexpr int descMaxLength = 256;
    static constexpr int expressionMaxLength = 1024;
    static constexpr int childElementsMaxCount = 20;
    static constexpr int functionParamsMaxCount = 5;
};

#endif // EXPRESSIONXMLPARSER_H
