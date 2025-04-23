#include "expressionxmlparser.h"
#include "teexception.h"
#include <QCoreApplication>
#include <QDir>

void ExpressionXmlParser::readDataFromXML(const QString& inputFilePath, Expression &expression) {

    QDomDocument doc = readXML(inputFilePath);

    // TODO: парсинг

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
    result = fixXmlDescriptions(result);

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

QString ExpressionXmlParser::fixXmlDescriptions(const QString& xmlString) {
    QString result = xmlString;

    // Обработка всех тегов <description> (с конца)
    int descriptionEnd = result.length();
    while ((descriptionEnd = result.lastIndexOf("</description>", descriptionEnd)) != -1) {
        int descriptionStart = result.lastIndexOf("<description>", descriptionEnd);
        if (descriptionStart == -1) break;

        // Вычисляем позиции содержимого
        int contentStart = descriptionStart + QString("<description>").length();
        int contentLength = descriptionEnd - contentStart;

        // Извлекаем содержимое
        QString content = result.mid(contentStart, contentLength);

        // Заменяем специальные символы
        QString escapedContent = escapeXmlText(content);

        // Заменяем оригинальное содержимое на обработанное
        result.replace(contentStart, contentLength, escapedContent);

        // Продолжаем поиск с предыдущей позиции
        descriptionEnd = descriptionStart;
    }

    return result;
}
