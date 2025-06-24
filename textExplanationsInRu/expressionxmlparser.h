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

    static QDomDocument readXML(const QString& filePath, QList<TEException>& errors );
    static QTemporaryFile* createTempCopy(const QString &sourceFilePath, QList<TEException>& errors);

    //////////////////////////////////////////////////
    /// Методы для исправления XML формата
    /////////////////////////////////////////////////

    static QString escapeXmlText(const QString& text);
    static QString fixXmlFlags(const QString& xmlString);
    static QString fixXmlExpression(const QString& xmlString);
    static QString fixXmlDescriptions(const QString& xmlString);
    static QString fixXmlCaseTags(const QString &xmlString);

    //////////////////////////////////////////////////
    /// Методы для обработки XML
    /////////////////////////////////////////////////

    static void parseQDomDocument(const QDomDocument& doc, Expression &expression, QList<TEException>& errors);
    static QString parseExpression(const QDomElement& _expression, QList<TEException>& errors);

    // ₋	от 0 до 20 переменных;
    static QHash<QString, Variable> parseVariables(const QDomElement& _variables, QList<TEException>& errors);
    static Variable parseVariable(const QDomElement& _variable, QList<TEException>& errors);

    // ₋	от 0 до 20 различных функций;
    static QHash<QString, Function> parseFunctions(const QDomElement& _functions, QList<TEException>& errors);
    static Function parseFunction(const QDomElement& _function, QList<TEException>& errors);

    // ₋	от 0 до 20 пользовательских типов;
    static QHash<QString, Union> parseUnions(const QDomElement& _unions, QList<TEException>& errors);
    // ₋	от 0 до 20 параметров у каждого пользовательского типа;
    static Union parseUnion(const QDomElement& _union, QList<TEException>& errors);

    // ₋	от 0 до 20 пользовательских типов;
    static QHash<QString, Structure> parseStructures(const QDomElement& _structures, QList<TEException>& errors);
    // ₋	от 0 до 20 параметров у каждого пользовательского типа;
    static Structure parseStructure(const QDomElement& _structure, QList<TEException>& errors);

    // ₋	от 0 до 20 пользовательских типов;
    static QHash<QString, Class> parseClasses(const QDomElement& _classes, QList<TEException>& errors);
    // ₋	от 0 до 20 параметров у каждого пользовательского типа;
    static Class parseClass(const QDomElement& _class, QList<TEException>& errors);

    // ₋	от 0 до 20 пользовательских типов;
    static QHash<QString, Enum> parseEnums(const QDomElement& _enums, QList<TEException>& errors);
    // ₋	от 0 до 20 параметров у каждого пользовательского типа;
    static Enum parseEnum(const QDomElement& _enum, QList<TEException>& errors);
    static QHash<QString, QHash<Case, QString> > parseEnumValues(const QDomElement& _values, QList<TEException>& errors);

    // ₋	от 0 до 256 символов для переменных;
    // ₋	от 0 до 256 символов для функции;
    // ₋	от 0 до 256 символов для параметров пользовательских типов;
    static QString parseDescription(const QDomElement& _description, QList<TEException>& errors);
    static QHash<Case, QString> parseCases(const QDomElement &parentElement, QList<TEException>& errors);

    // ₋	от 1 до 32 символов для имени переменной;
    // ₋	от 1 до 32 символов для имени функции;
    // ₋	от 1 до 32 символов для имени параметра функции;
    // ₋	от 1 до 32 символов для имени пользовательских типов;
    // ₋	от 1 до 32 символов для имени параметров пользовательских типов;
    //Названия всех переменных, функций и пользовательских типов должны быть уникальны. Названия должны начинаться с латинской буквы или спец символ ‘_’ (нижнее подчеркивание),
    //содержать только цифры от 0 до 9 включительно, все буквы латинского алфавита и спец символ ‘_’ (нижнее подчеркивание).
    static QString parseName(const QDomElement& element, QList<TEException>& errors);

    // Простые типы данных
    //Целое число           int
    //Вещественное число	float
    //Вещественное число	double
    //Символ                char
    //Логический тип        bool
    //Строковый тип         String
    //Массив                {a}[]
    //Указатели             {a}*

    static QString parseType(const QDomElement& element, QList<TEException>& errors);

    // ₋	от 0 до 5 параметров функции;
    static int parseParamsCount(const QDomElement& element, QList<TEException>& errors);

    //////////////////////////////////////////////////
    /// Методы для валидации XML элементов и атрибутов
    /////////////////////////////////////////////////

    static void validateElement(const QDomElement& curElement, const QList<QString>& allowedAttributes, const QHash<QString, int>& allowedElements, QList<TEException>& errors, bool checkRequired = true, bool textIsError = true);
    static void validateAttributes(const QDomElement& curElement, const QList<QString>& attributes, QList<TEException>& errors);
    static void validateChildElements(const QDomElement& curElement, const QHash<QString, int>& allowedElements, QList<TEException>& errors);
    static void validateRequiredAttributes(const QDomElement& curElement, const QList<QString>& attributes, QList<TEException>& errors);
    static void validateRequiredChildElements(const QDomElement& curElement, const QList<QString>& elements, QList<TEException>& errors);
    static void validateCases(const QDomElement& curDescription, QList<TEException>& errors);
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
