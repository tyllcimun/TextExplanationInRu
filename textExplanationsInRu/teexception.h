/*!
 * \file
 * \brief Заголовочный файл, содержащий описание класса TEException и перечисления ErrorType для обработки ошибок при парсинге XML.
 */

#ifndef TEEXCEPTION_H
#define TEEXCEPTION_H

#include <QList>
#include <QString>
#include <QHash>

/*!
 * \brief Перечисление типов ошибок, возникающих при обработке XML и файлов.
 */
enum class ErrorType {
    // Ошибки файлов
    InputFileNotFound,               /*!< Входной файл не найден или недоступен */
    InputCopyFileCannotBeCreated,   /*!< Невозможно создать копию входного файла */
    OutputFileCannotBeCreated,      /*!< Невозможно создать выходной файл */

    // Общие ошибки XML
    Parsing,                         /*!< Ошибка разбора XML */
    MissingRootElemnt,              /*!< Отсутствует корневой элемент <root> */
    UnexpectedElement,              /*!< Неожиданный элемент в текущей структуре */
    UnexpectedAttribute,            /*!< Неожиданный атрибут в текущем элементе */
    MissingRequiredChildElement,    /*!< Отсутствует необходимый дочерний элемент */
    MissingRequiredAttribute,       /*!< Отсутствует необходимый атрибут */
    DuplicateElement,               /*!< Повторяющийся элемент */
    DuplicateAttribute,             /*!< Повторяющийся атрибут */
    EmptyElementValue,              /*!< Значение элемента не указано */
    EmptyAttributeName,             /*!< Имя атрибута не указано */
    ParamsCountFunctionMissmatch,   /*!< Несоответствие количества параметров и описания */
    InputSizeExceeded,              /*!< Превышен допустимый размер входных данных */
    InputElementsExceeded,          /*!< Превышено количество допустимых элементов */

    // Ошибки элемента <expression>
    UndefinedId,                    /*!< Неопределённый идентификатор */
    InvalidSymbol,                  /*!< Недопустимый символ в выражении */
    InputDataExprSizeExceeded,      /*!< Превышен размер выражения */
    MissingOperand,                 /*!< Отсутствует операнд */
    MissingOperations,              /*!< Отсутствует операция */
    MultipleIncrementDecrement,     /*!< Повторное использование ++ или -- */
    NeverUsedElement,               /*!< Элемент не был использован */

    // Ошибки элемента <function>
    ParamsCountDescriptionDifference, /*!< Несоответствие количества замен в <description> и параметров */

    // Ошибки атрибута "name"
    NonUniqueName,                  /*!< Имя не уникально */
    InvalidName,                    /*!< Имя содержит недопустимые символы */

    // Ошибки атрибута "type"
    UnidentifedType,                /*!< Неизвестный тип данных */
    InvalidType,                    /*!< Неверное значение типа */

    // Ошибки атрибута "paramsCount"
    InvalidParamsCount,             /*!< Неверный формат значения атрибута paramsCount */

    // Ошибки описаний и падежей
    MissingCases,                   /*!< Отсутствует обязательный падеж */
    MissingReplacementArguments,    /*!< Отсутствуют аргументы для замены */
    UnexpectedCaseType,             /*!< Отсутствует атрибут type в элементе падежа */
    IncorrectCaseInPlaceHolder,     /*!< Неправильно указан падеж в плейсхолдере */
    VariableWithVoidType            /*!< Переменная с типом войд  */
};

/*!
 * \brief Класс, представляющий исключение при обработке XML.
 */
class TEException
{
public:

    /*!
     * \brief Конструктор исключения с указанием всех параметров.
     * \param[in] errorType Тип ошибки.
     * \param[in] filename Имя файла, в котором произошла ошибка.
     * \param[in] line Номер строки.
     * \param[in] args Аргументы для шаблона ошибки.
     */
    TEException(const ErrorType errorType, const QString &filename, const int line, const QList<QString> args = QList<QString>());

    /*!
     * \brief Конструктор исключения без имени файла.
     * \param[in] errorType Тип ошибки.
     * \param[in] line Номер строки.
     * \param[in] args Аргументы для шаблона ошибки.
     */
    TEException(const ErrorType errorType, const int line, const QList<QString> args = QList<QString>());

    /*!
     * \brief Конструктор исключения без строки.
     * \param[in] errorType Тип ошибки.
     * \param[in] filename Имя файла.
     * \param[in] args Аргументы для шаблона ошибки.
     */
    TEException(const ErrorType errorType, const QString &filename, const QList<QString> args = QList<QString>());

    /*!
     * \brief Конструктор исключения только с типом ошибки.
     * \param[in] errorType Тип ошибки.
     * \param[in] args Аргументы для шаблона ошибки.
     */
    TEException(const ErrorType errorType, const QList<QString> args = QList<QString>());

    /*!
     * \brief Получение текстового описания ошибки.
     * \return Строка с описанием.
     */
    QString what() const;

    /*!
     * \brief Получение типа ошибки.
     * \return Тип ошибки.
     */
    ErrorType getErrorType() const;

    /*!
     * \brief Получение строки, на которой произошла ошибка.
     * \return Номер строки.
     */
    int getLine() const;

    /*!
     * \brief Получение списка аргументов ошибки.
     * \return Список строк-аргументов.
     */
    QList<QString> getArgs() const;

    /*!
     * \brief Подстановка аргументов в шаблон сообщения.
     * \param[in] pattern Шаблон с плейсхолдерами.
     * \param[in] args Аргументы для подстановки.
     * \return Строка с подставленными значениями.
     */
    QString replacePlaceholders(QString pattern, const QList<QString> args) const;

    /*!
     * \brief Отображение имён ошибок по типу.
     */
    static const QHash<ErrorType, QString> ErrorTypeNames;

private:
    ErrorType errorType; /*!< Тип ошибки */
    QString filename;    /*!< Имя файла */
    int line;            /*!< Номер строки */
    QList<QString> args; /*!< Аргументы ошибки */
};

#endif // TEEXCEPTION_H
