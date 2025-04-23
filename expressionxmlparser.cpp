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
    // TODO: экранировать символы XML

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
