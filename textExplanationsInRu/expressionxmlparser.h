/*!
 * \file
 * \brief Заголовочный файл, содержащий объявления класса ExpressionXmlParser для разбора XML-файлов в структуру Expression.
 */

#ifndef EXPRESSIONXMLPARSER_H
#define EXPRESSIONXMLPARSER_H

#include "expression.h"
#include <QDomDocument>
#include <QString>
#include <QTemporaryFile>

/*!
 * \brief Класс для разбора XML-файлов и преобразования их в структуру Expression.
 */
class ExpressionXmlParser
{
public:
    /*!
     * \brief Чтение данных из XML-файла и формирование структуры Expression.
     * \param[in] inputFilePath Путь к входному XML-файлу.
     * \param[out] expression Заполняемая структура Expression.
     */
    static void readDataFromXML(const QString& inputFilePath, Expression& expression);

private:

    //////////////////////////////////////////////////
    /// Методы для работы с файлами
    /////////////////////////////////////////////////

    /*!
     * \brief Считывание XML-документа из файла.
     * \param[in] filePath Путь к XML-файлу.
     * \param[out] errors Список ошибок.
     * \return Объект QDomDocument.
     */
    static QDomDocument readXML(const QString& filePath, QList<TEException>& errors);

    /*!
     * \brief Создание временной копии XML-файла.
     * \param[in] sourceFilePath Путь к исходному файлу.
     * \param[out] errors Список ошибок.
     * \return Указатель на временный файл.
     */
    static QTemporaryFile* createTempCopy(const QString &sourceFilePath, QList<TEException>& errors);

    //////////////////////////////////////////////////
    /// Методы для исправления XML формата
    /////////////////////////////////////////////////

    /*!
     * \brief Экранирование специальных символов XML.
     * \param[in] text Текст для экранирования.
     * \return Экранированный текст.
     */
    static QString escapeXmlText(const QString& text);

    /*!
     * \brief Исправление флагов в XML.
     * \param[in] xmlString XML-строка.
     * \return Исправленная строка.
     */
    static QString fixXmlFlags(const QString& xmlString);

    /*!
     * \brief Исправление выражения в XML.
     * \param[in] xmlString XML-строка.
     * \return Исправленное выражение.
     */
    static QString fixXmlExpression(const QString& xmlString);

    /*!
     * \brief Исправление тегов падежей в XML.
     * \param[in] xmlString XML-строка.
     * \return Обновлённая строка.
     */
    static QString fixXmlCaseTags(const QString &xmlString);

    //////////////////////////////////////////////////
    /// Методы для обработки XML
    /////////////////////////////////////////////////

    /*!
     * \brief Парсинг документа XML в Expression.
     * \param[in] doc XML-документ.
     * \param[out] expression Заполняемая структура.
     * \param[out] errors Список ошибок.
     */
    static void parseQDomDocument(const QDomDocument& doc, Expression &expression, QList<TEException>& errors);

    /*!
     * \brief Извлечение выражения.
     * \param[in] _expression Элемент <expression>.
     * \param[out] errors Список ошибок.
     * \return Строка выражения.
     */
    static QString parseExpression(const QDomElement& _expression, QList<TEException>& errors);

    /*!
     * \brief Извлечение переменных.
     */
    static QHash<QString, Variable> parseVariables(const QDomElement& _variables, QList<TEException>& errors);

    /*!
     * \brief Парсинг одной переменной.
     */
    static Variable parseVariable(const QDomElement& _variable, QList<TEException>& errors);

    /*!
     * \brief Извлечение функций.
     */
    static QHash<QString, Function> parseFunctions(const QDomElement& _functions, QList<TEException>& errors);

    /*!
     * \brief Парсинг одной функции.
     */
    static Function parseFunction(const QDomElement& _function, QList<TEException>& errors);

    /*!
     * \brief Извлечение объединений.
     */
    static QHash<QString, Union> parseUnions(const QDomElement& _unions, QList<TEException>& errors);

    /*!
     * \brief Парсинг одного объединения.
     */
    static Union parseUnion(const QDomElement& _union, QList<TEException>& errors);

    /*!
     * \brief Извлечение структур.
     */
    static QHash<QString, Structure> parseStructures(const QDomElement& _structures, QList<TEException>& errors);

    /*!
     * \brief Парсинг одной структуры.
     */
    static Structure parseStructure(const QDomElement& _structure, QList<TEException>& errors);

    /*!
     * \brief Извлечение классов.
     */
    static QHash<QString, Class> parseClasses(const QDomElement& _classes, QList<TEException>& errors);

    /*!
     * \brief Парсинг одного класса.
     */
    static Class parseClass(const QDomElement& _class, QList<TEException>& errors);

    /*!
     * \brief Извлечение перечислений.
     */
    static QHash<QString, Enum> parseEnums(const QDomElement& _enums, QList<TEException>& errors);

    /*!
     * \brief Парсинг одного перечисления.
     */
    static Enum parseEnum(const QDomElement& _enum, QList<TEException>& errors);

    /*!
     * \brief Извлечение значений перечисления.
     */
    static QHash<QString, QHash<Case, QString>> parseEnumValues(const QDomElement& _values, QList<TEException>& errors);

    /*!
     * \brief Извлечение падежей.
     */
    static QHash<Case, QString> parseCases(const QDomElement &parentElement, QList<TEException>& errors);

    /*!
     * \brief Извлечение имени.
     */
    static QString parseName(const QDomElement& element, QList<TEException>& errors);
    /*!
     * \brief Извлечение типа данных.
     */
    static QString parseType(const QDomElement& element, QList<TEException>& errors);

    /*!
     * \brief Извлечение количества параметров функции.
     */
    static int parseParamsCount(const QDomElement& element, QList<TEException>& errors);

    //////////////////////////////////////////////////
    /// Методы для валидации XML элементов и атрибутов
    /////////////////////////////////////////////////

    /*!
     * \brief Проверка соответствия элемента требованиям.
     */
    static void validateElement(const QDomElement& curElement, const QList<QString>& allowedAttributes, const QHash<QString, int>& allowedElements, QList<TEException>& errors, bool checkRequired = true, bool textIsError = true);

    /*!
     * \brief Проверка атрибутов элемента.
     */
    static void validateAttributes(const QDomElement& curElement, const QList<QString>& attributes, QList<TEException>& errors);

    /*!
     * \brief Проверка допустимых дочерних элементов.
     */
    static void validateChildElements(const QDomElement& curElement, const QHash<QString, int>& allowedElements, QList<TEException>& errors);

    /*!
     * \brief Проверка обязательных атрибутов.
     */
    static void validateRequiredAttributes(const QDomElement& curElement, const QList<QString>& attributes, QList<TEException>& errors);

    /*!
     * \brief Проверка обязательных дочерних элементов.
     */
    static void validateRequiredChildElements(const QDomElement& curElement, const QList<QString>& elements, QList<TEException>& errors);

    /*!
     * \brief Проверка корректности блоков падежей.
     */
    static void validateCases(const QDomElement& curDescription, QList<TEException>& errors);

    /*!
     * \brief Подсчет количества прямых дочерних элементов.
     */
    static int countDirectChildren(const QDomElement& element, const QString& childName);

    /*!
     * \brief Проверка, является ли символ латинской буквой.
     */
    static bool isLatinLetter(const QChar c);

    //////////////////////////////////////////////////
    /// Константы
    /////////////////////////////////////////////////

    /*! \brief Максимальная длина имени. */
    static constexpr int nameMaxLength = 32;

    /*! \brief Максимальная длина описания. */
    static constexpr int descMaxLength = 256;

    /*! \brief Максимальная длина выражения. */
    static constexpr int expressionMaxLength = 1024;

    /*! \brief Максимальное количество дочерних элементов. */
    static constexpr int childElementsMaxCount = 20;

    /*! \brief Максимальное количество параметров функции. */
    static constexpr int functionParamsMaxCount = 5;

    /*! \brief Список поддерживаемых типов данных для переменных. */
    static const QList<QString> supportedDataTypesForVar;

    /*! \brief Отображение строковых значений в падежах. */
    static const QHash<QString, Case> caseMapping;
    };

#endif // EXPRESSIONXMLPARSER_H
