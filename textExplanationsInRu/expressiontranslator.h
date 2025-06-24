/*!
 * \file
 * \brief Заголовочный файл, содержащий описание класса ExpressionTranslator, предназначенного для генерации текстовых представлений выражений.
 */

#ifndef EXPRESSIONTRANSLATOR_H
#define EXPRESSIONTRANSLATOR_H

#include "codeentity.h"
#include <QString>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QRegularExpressionMatchIterator>

#include <QHash>
#include <QString>

/*!
 * \brief Класс для генерации описаний выражений на основе шаблонов.
 */
class ExpressionTranslator
{
public:
    /*!
     * \brief Конструктор класса ExpressionTranslator.
     */
    ExpressionTranslator();

    /*!
     * \brief Словарь шаблонов операций с падежными формами.
     *
     * Ключ — тип операции (OperationType), значение — набор строк в разных падежах.
     */
    static const QHash<OperationType, QHash<Case, QString>> Templates;

    /*!
     * \brief Генерация пояснения (описания) выражения на основе шаблона и аргументов.
     * \param[in] description Шаблон описания операции с подстановочными элементами.
     * \param[in] arguments Список аргументов в разных падежах.
     * \return Результат с подставленными аргументами.
     */
    static QHash<Case, QString> getExplanation(const QHash<Case, QString> &description, const QList<QHash<Case, QString>> &arguments);

    /*!
     * \brief Разбор строкового значения падежа.
     * \param[in] caseChar Строковое представление падежа (например, "n" для именительного).
     * \return Значение перечисления падежей.
     */
    static Case parseCase(const QString &caseChar);

    /*!
     * \brief Замена плейсхолдеров вида {0:n}, {1:a} и т.д. в шаблоне.
     * \param[in] pattern Шаблон строки с плейсхолдерами.
     * \param[in] args Аргументы, содержащие формы в разных падежах.
     * \param[in] placeholderRegex Регулярное выражение для поиска плейсхолдеров.
     * \return Строка с подставленными значениями.
     */
    static QString replacePlaceholders(const QString &pattern, const QList<QHash<Case, QString>> &args, QRegularExpression& placeholderRegex);
};

#endif // EXPRESSIONTRANSLATOR_H
