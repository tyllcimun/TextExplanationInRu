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

    // ₋	от 0 до 20 переменных;
    static QHash<QString, Variable> parseVariables(const QDomElement& _variables);
    static Variable parseVariable(const QDomElement& _variable);

    // ₋	от 0 до 20 различных функций;
    static QHash<QString, Function> parseFunctions(const QDomElement& _functions);
    static Function parseFunction(const QDomElement& _function);

    // ₋	от 0 до 20 пользовательских типов;
    static QHash<QString, Structure> parseStructures(const QDomElement& _structures);
    // ₋	от 0 до 20 параметров у каждого пользовательского типа;
    static Structure parseStructure(const QDomElement& _structure);

    // ₋	от 0 до 20 пользовательских типов;
    static QHash<QString, Class> parseClasses(const QDomElement& _classes);
    // ₋	от 0 до 20 параметров у каждого пользовательского типа;
    static Class parseClass(const QDomElement& _class);

    // ₋	от 0 до 20 пользовательских типов;
    static QHash<QString, Enum> parseEnums(const QDomElement& _enums);
    // ₋	от 0 до 20 параметров у каждого пользовательского типа;
    static Enum parseEnum(const QDomElement& _enum);
    static QHash<QString, QHash<Case, QString> > parseEnumValues(const QDomElement& _values);

    // ₋	от 0 до 256 символов для переменных;
    // ₋	от 0 до 256 символов для функции;
    // ₋	от 0 до 256 символов для параметров пользовательских типов;
    static QHash<Case, QString> parseCases(const QDomElement &parentElement);

    // ₋	от 1 до 32 символов для имени переменной;
    // ₋	от 1 до 32 символов для имени функции;
    // ₋	от 1 до 32 символов для имени параметра функции;
    // ₋	от 1 до 32 символов для имени пользовательских типов;
    // ₋	от 1 до 32 символов для имени параметров пользовательских типов;
    //Названия всех переменных, функций и пользовательских типов должны быть уникальны. Названия должны начинаться с латинской буквы или спец символ ‘_’ (нижнее подчеркивание),
    //содержать только цифры от 0 до 9 включительно, все буквы латинского алфавита и спец символ ‘_’ (нижнее подчеркивание).
    static QString parseName(const QDomElement& element);

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
    static const QHash<QString, Case> caseMapping;
};

#endif // EXPRESSIONXMLPARSER_H
