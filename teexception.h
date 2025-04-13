#ifndef TEEXCEPTION_H
#define TEEXCEPTION_H

#include <QList>
#include <QString>


enum class ErrorType {

    // Ошибки файлов
    InputFileNotFound,               // Указанный входной файл не существует или нет доступа
    InputCopyFileCannotBeCreated,   // Невозможно создать копию входного файла
    OutputFileCannotBeCreated,      // Невозможно создать указанный выходной файл

    // Ошибки формата XML (общие)
    Parssing,                       // Ошибка разбора XML
    MissingRootElemnt,              // Отсутствие корневого элемента <root>
    UnexpectedElement,              // Присутствует элемент, который не должен быть в текущей вложенности
    UnexpectedAttribute,            // Присутствует атрибут, который не должен быть в текущем элементе
    MissingRequiredChildElement,    // У элемента отсутствует необходимый атрибут
    MissingRequiredAttribute,       // У элемента отсутствует необходимый дочерний элемент
    DuplicateElement,               // Необходимый элемент встречается более допустимого количества раз
    DuplicateAttribute,             // Необходимый атрибут встречается более допустимого количества раз
    EmptyElementValue,              // Значение элемента не заполнено
    EmptyAttributeName,             // Значение атрибута не заполнено
    ParamsCountFunctionMissmatch,   // Количество параметров в "expression" не соответствует "paramsCount"
    InputSizeExceeded,              // Превышен допустимый размер входных данных

    // Ошибки формата XML (элемент <expression>)
    UndefinedId,                // Используется переменная, функция или пользовательский тип, которые отсутствуют в соответствующих элементах
    InvalidSymbol,              // В выражении присутствует недопустимый символ
    MissingOperand,             // Для операции отсутствует необходимое количество операндов
    MissingOperations,          // Для операнда отсутствует необходимое количество операций
    MultipleIncrementDecrement, // Используется более одного инкремента или декремента для одной и той же переменной
    NewerUsedElement,           // Переменная, функция или пользовательский тип объявленные в элементах, не были использованы ни разу.

    // Ошибки формата XML (элемент <function>)
    ParamsCountDescriptionDifference, // Количество замен участка в <description> больше чем принимает функция

    // Ошибки формата XML (атрибут "name")
    NonUniqueName,          // У атрибута "name" не уникальное название
    InvalidName,    // В значении атрибута "name" используются недопустимые символы

    // Ошибки формата XML (атрибут "type")
    UnidentifedType,    // В значении атрибута "type" указан неидентифицированный тип данных

    // Ошибки формата XML (атрибут "paramsCount")
    InvalidParamsCount, // У атрибута "paramsCount" указан неправильный формат данных

};

class TEException
{
public:

private:

    ErrorType errorType; // Тип ошибки
    QString filename;    // Файл в котором произошла ошибка
    int line;            // Строка, на которой произошла ошибка
    QList<QString> args; // Аргументы, которые будут подставлены в шаблон ошибки
};

#endif // TEEXCEPTION_H
