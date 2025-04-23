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
    static QString fixXmlDescriptions(const QString& xmlString);

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
