#include "teexception.h"

TEException::TEException(const ErrorType errorType, const QString &filename, const int line, const QList<QString> args)
    : errorType(errorType), filename(filename), line(line), args(args)
{
}

TEException::TEException(const ErrorType errorType,  const QString &filename, const QList<QString> args)
    : TEException(errorType, filename, 0, args)
{
}

TEException::TEException(const ErrorType errorType,  const int line, const QList<QString> args)
    : TEException(errorType, "", line, args)
{
}

TEException::TEException(const ErrorType errorType, const QList<QString> args)
    : TEException(errorType, "", 0, args)
{
}

QString TEException::what() const {

    QString message = "Error: ";

    if(this->filename != "") message += "in \"" + this->filename + "\" ";

    if(this->line > 0) message += "in line " + QString::number(this->line) + ": ";

    switch(this->errorType) {

    case ErrorType::InputFileNotFound:
        message += "Неверно указан путь к входному файлу. Возможно, файл не существует или к нему нет доступа.";
        break;
    case ErrorType::InputCopyFileCannotBeCreated:
        message += "Не удалось создать копию файла {1} в \"{2}\". Возможно, нет прав на запись";
        break;
    case ErrorType::OutputFileCannotBeCreated:
        message += "Неверно указан путь к выходному файлу. Возможно, указанного расположения не существует или нет прав на запись.";
        break;
    case ErrorType::Parsing:
        message += "синтаксическая ошибка обнаружена в процессе обработки XML файла";
        break;
    case ErrorType::MissingRootElemnt:
        message += "не найден корневой тег элемента <root>";
        break;
    case ErrorType::UnexpectedElement:
        message += "найден элемент <{1}>, ожидается: {2}";
        break;
    case ErrorType::UnexpectedAttribute:
        message += "получен атрибут \"{1}\", ожидается: {2}";
        break;
    case ErrorType::MissingRequiredChildElement:
        message += "отсутствует элемент <{1}>";
        break;
    case ErrorType::MissingRequiredAttribute:
        message += "отсутствует атрибут \"{1}\"";
        break;
    case ErrorType::DuplicateElement:
        message += "элемент <{1}> встречается более допустимого количества раз.";
        break;
    case ErrorType::DuplicateAttribute:
        message += "атрибут \"{1}\" встречается более допустимого количества раз.";
        break;
    case ErrorType::EmptyElementValue:
        message += "значение элемента <{1}> не заполнено.";
        break;
    case ErrorType::EmptyAttributeName:
        message += "значение атрибута \"{1}\" не заполнено.";
        break;
    case ErrorType::ParamsCountFunctionMissmatch:
        message += "количество параметров, указанных в <expression> ({1}) не соответствует указанному количеству параметров, указанных в <paramsCount> ({2}) для функции \"{3}\".";
        break;
    case ErrorType::InputSizeExceeded:
        message += "текстовое значение \"{1}\" превышает допустимую длину. Текущая длина - {2}, Ожидаемая - {3}.";
        break;
    case ErrorType::InputElementsExceeded:
        message += "элемент <{1}> превышает допустимое количество элементов. Текущее количество - {2}, Ожидаемое - {3}.";
        break;
    case ErrorType::UndefinedId:
        message += "идентификатор \"{1}\" в значении элемента <expression> не определен";
        break;
    case ErrorType::InvalidSymbol:
        message += "в значении элемента <expression> обнаружен символ \"{1}\".";
        break;
    case ErrorType::InputDataExprSizeExceeded:
        message += "в значении элемента <expression> превышено допустимое количество операций. Текущее - {1}, ожидается - 20.";
        break;
    case ErrorType::MissingOperand:
        message += "в значении элемента <expression>, у операции \"{1}\" отсутствует операнд.";
        break;
    case ErrorType::MissingOperations:
        message += "в значении элемента <expression>, у операнда \"{1}\" отсутствует операция.";
        break;
    case ErrorType::MultipleIncrementDecrement:
        message += "в значении элемента <expression>, значение элемента \"{1}\" не может быть инкрементировано или декрементировано более одного раза.";
        break;
    case ErrorType::NeverUsedElement:
        message += "элементы: {1} ни разу не встречаются в элементе <expression>.";
        break;
    case ErrorType::ParamsCountDescriptionDifference:
        message += "количество замен участков в <description> элемента <function> описанием входящих аргументов превышает значение атрибута \"paramsCount\" элемента <function>.";
        break;
    case ErrorType::NonUniqueName:
        message += "значение \"{1}\" для атрибута \"name\" для элемента <{2}> не уникальное";
        break;
    case ErrorType::InvalidName:
        message += "значение \"{1}\" атрибута \"name\" должно начинаться с латинской буквы или со специального символа \"_\" (нижнее подчеркивание) и содержать в себе только латинские буквы, цифры и специальный символ \"_\" (нижнее подчеркивание).";
        break;
    case ErrorType::UnidentifedType:
        message += "значение \"{1}\" атрибута \"type\" содержит неидентифицированный тип. Тип данных должен быть одним из поддерживаемых или может быть пользовательским типом данных, описанным в элементах <unions>; <structures>; <classes>; <enums>.";
        break;
    case ErrorType::InvalidType:
        message += "значение \"{1}\" атрибута \"type\" должно начинаться с латинской буквы или со специального символа \"_\" (нижнее подчеркивание) и содержать в себе только латинские буквы, цифры и специальный символ \"_\" (нижнее подчеркивание).";
        break;
    case ErrorType::InvalidParamsCount:
        message += "значение \"{1}\" атрибута \"paramsCount\" содержит неверное значение. Ожидается: положительное целое число от 0 до 20 включительно.";
        break;
    case ErrorType::MissingCases:
        message += "в элементе <description> отсутствует <case> с атрибутом \"type\" со значением \"{1}\".";
        break;
    case ErrorType::MissingReplacementArguments:
        message += "недостаток аргументов при замене в шаблоне \"{1}\"";
        break;
    case ErrorType::IncorrectCaseInPlaceHolder:
        message += "указанный падеж \"{1}\" в шаблоне описания содержит неверное значение. Ожидается: и, р, д, в, т, п.";
        break;
    case ErrorType::UnexpectedCaseType:
        message += "в элементе <case> получено значение атрибута “type” \"{1}\". Ожидается \"{2}\".";
        break;
    case ErrorType::VariableWithVoidType:
        message += "переменная \"{1}\" имеет недопустимый тип данных \"void\". тип данных \"void\" может быть только у функций";
        break;
    default:
        message += "неизвестная ошибка";
        break;
    }

    if(args.count() > 0) {
        message = replacePlaceholders(message, this->args);
    }

    return message;
}

ErrorType TEException::getErrorType() const
{
    return errorType;
}

int TEException::getLine() const
{
    return line;
}

QList<QString> TEException::getArgs() const
{
    return args;
}

QString TEException::replacePlaceholders(QString pattern, const QList<QString> args) const
{
    // Для каждого аргумента
    for (int i = 0; i < args.size(); i++) {
        // Формируем плейсхолдер вида {n} и т.д.
        QString placeholder = "{" + QString::number(i + 1) + "}";
        // Заменяем вхождение плейсхолдера на соответствующий ему аргумент
        pattern.replace(placeholder, args[i]);
    }
    return pattern;
}
