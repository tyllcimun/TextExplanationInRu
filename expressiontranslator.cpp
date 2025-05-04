#include "expressiontranslator.h"

const QHash<OperationType, QHash<Case, QString>> ExpressionTranslator::Templates = {
    {
        OperationType::Addition, {
            {Case::Nominative, "сумма {1 (р)} и {2 (р)}"},
            {Case::Genitive, "суммы {1 (р)} и {2 (р)}"},
            {Case::Dative, "сумме {1 (р)} и {2 (р)}"},
            {Case::Accusative, "сумму {1 (р)} и {2 (р)}"},
            {Case::Instrumental, "суммой {1 (р)} и {2 (р)}"},
            {Case::Prepositional, "сумме {1 (р)} и {2 (р)}"}
        }
    },
    {
        OperationType::Subtraction, {
            {Case::Nominative, "разность {1 (р)} и {2 (р)}"},
            {Case::Genitive, "разности {1 (р)} и {2 (р)}"},
            {Case::Dative, "разности {1 (р)} и {2 (р)}"},
            {Case::Accusative, "разность {1 (р)} и {2 (р)}"},
            {Case::Instrumental, "разностью {1 (р)} и {2 (р)}"},
            {Case::Prepositional, "разности {1 (р)} и {2 (р)}"}
        }
    },
    {
        OperationType::And, {
            {Case::Nominative, "{1 (и)} и {2 (и)}"},
            {Case::Genitive, "{1 (р)} и {2 (р)}"},
            {Case::Dative, "{1 (д)} и {2 (д)}"},
            {Case::Accusative, "{1 (в)} и {2 (в)}"},
            {Case::Instrumental, "{1 (т)} и {2 (т)}"},
            {Case::Prepositional, "{1 (п)} и {2 (п)}"}
        }
    },
    {
        OperationType::Or, {
            {Case::Nominative, "{1 (и)} или {2 (и)}"},
            {Case::Genitive, "{1 (р)} или {2 (р)}"},
            {Case::Dative, "{1 (д)} или {2 (д)}"},
            {Case::Accusative, "{1 (в)} или {2 (в)}"},
            {Case::Instrumental, "{1 (т)} или {2 (т)}"},
            {Case::Prepositional, "{1 (п)} или {2 (п)}"}
        }
    },
    {
        OperationType::LessThanOrEqual, {
            {Case::Nominative, "{1 (и)} меньше или равно {2 (д)}"},
            {Case::Genitive, "{1 (р)} меньше или равно {2 (д)}"},
            {Case::Dative, "{1 (д)} меньше или равно {2 (д)}"},
            {Case::Accusative, "{1 (в)} меньше или равно {2 (д)}"},
            {Case::Instrumental, "{1 (т)} меньше или равно {2 (д)}"},
            {Case::Prepositional, "{1 (п)} меньше или равно {2 (д)}"}
        }
    },
    {
        OperationType::GreaterThan, {
            {Case::Nominative, "{1 (и)} больше {2 (р)}"},
            {Case::Genitive, "{1 (р)} больше {2 (р)}"},
            {Case::Dative, "{1 (д)} больше {2 (р)}"},
            {Case::Accusative, "{1 (в)} больше {2 (р)}"},
            {Case::Instrumental, "{1 (т)} больше {2 (р)}"},
            {Case::Prepositional, "{1 (п)} больше {2 (р)}"}
        }
    },
    {
        OperationType::NotEqual, {
            {Case::Nominative, "{1 (и)} не равно {2 (д)}"},
            {Case::Genitive, "{1 (р)} не равно {2 (д)}"},
            {Case::Dative, "{1 (д)} не равно {2 (д)}"},
            {Case::Accusative, "{1 (в)} не равно {2 (д)}"},
            {Case::Instrumental, "{1 (т)} не равно {2 (д)}"},
            {Case::Prepositional, "{1 (п)} не равно {2 (д)}"}
        }
    },
    {
        OperationType::Equal, {
            {Case::Nominative, "{1 (и)} равно {2 (д)}"},
            {Case::Genitive, "{1 (р)} равно {2 (д)}"},
            {Case::Dative, "{1 (д)} равно {2 (д)}"},
            {Case::Accusative, "{1 (в)} равно {2 (д)}"},
            {Case::Instrumental, "{1 (т)} равно {2 (д)}"},
            {Case::Prepositional, "{1 (п)} равно {2 (д)}"}
        }
    },
    {
        OperationType::LessThan, {
            {Case::Nominative, "{1 (и)} меньше {2 (р)}"},
            {Case::Genitive, "{1 (р)} меньше {2 (р)}"},
            {Case::Dative, "{1 (д)} меньше {2 (р)}"},
            {Case::Accusative, "{1 (в)} меньше {2 (р)}"},
            {Case::Instrumental, "{1 (т)} меньше {2 (р)}"},
            {Case::Prepositional, "{1 (п)} меньше {2 (р)}"}
        }
    },
    {
        OperationType::GreaterThanOrEqual, {
            {Case::Nominative, "{1 (и)} больше или равно {2 (д)}"},
            {Case::Genitive, "{1 (р)} больше или равно {2 (д)}"},
            {Case::Dative, "{1 (д)} больше или равно {2 (д)}"},
            {Case::Accusative, "{1 (в)} больше или равно {2 (д)}"},
            {Case::Instrumental, "{1 (т)} больше или равно {2 (д)}"},
            {Case::Prepositional, "{1 (п)} больше или равно {2 (д)}"}
        }
    },
    {
        OperationType::Multiplication, {
            {Case::Nominative, "произведение {1 (р)} и {2 (р)}"},
            {Case::Genitive, "произведения {1 (р)} и {2 (р)}"},
            {Case::Dative, "произведению {1 (р)} и {2 (р)}"},
            {Case::Accusative, "произведение {1 (р)} и {2 (р)}"},
            {Case::Instrumental, "произведением {1 (р)} и {2 (р)}"},
            {Case::Prepositional, "произведении {1 (р)} и {2 (р)}"}
        }
    },
    {
        OperationType::Division, {
            {Case::Nominative, "частное {1 (р)} и {2 (р)}"},
            {Case::Genitive, "частного {1 (р)} и {2 (р)}"},
            {Case::Dative, "частному {1 (р)} и {2 (р)}"},
            {Case::Accusative, "частное {1 (р)} и {2 (р)}"},
            {Case::Instrumental, "частным {1 (р)} и {2 (р)}"},
            {Case::Prepositional, "частном {1 (р)} и {2 (р)}"}
        }
    },
    {
        OperationType::Modulus, {
            {Case::Nominative, "остаток от деления {1 (р)} на {2 (р)}"},
            {Case::Genitive, "остатка от деления {1 (р)} на {2 (р)}"},
            {Case::Dative, "остатку от деления {1 (р)} на {2 (р)}"},
            {Case::Accusative, "остаток от деления {1 (р)} на {2 (р)}"},
            {Case::Instrumental, "остатком от деления {1 (р)} на {2 (р)}"},
            {Case::Prepositional, "остатке от деления {1 (р)} на {2 (р)}"}
        }
    },
    {
        OperationType::Not, {
            {Case::Nominative, "не {1 (и)}"},
            {Case::Genitive, "не {1 (р)}"},
            {Case::Dative, "не {1 (д)}"},
            {Case::Accusative, "не {1 (в)}"},
            {Case::Instrumental, "не {1 (т)}"},
            {Case::Prepositional, "не {1 (п)}"}
        }
    },
    {
        OperationType::UnaryMinus, {
            {Case::Nominative, "отрицание {1 (р)}"},
            {Case::Genitive, "отрицания {1 (р)}"},
            {Case::Dative, "отрицанию {1 (р)}"},
            {Case::Accusative, "отрицание {1 (р)}"},
            {Case::Instrumental, "отрицанием {1 (р)}"},
            {Case::Prepositional, "отрицании {1 (р)}"}
        }
    },
    {
        OperationType::AddressOf, {
            {Case::Nominative, "адрес элемента {1 (р)}"},
            {Case::Genitive, "адреса элемента {1 (р)}"},
            {Case::Dative, "адресу элемента {1 (р)}"},
            {Case::Accusative, "адрес элемента {1 (р)}"},
            {Case::Instrumental, "адресом элемента {1 (р)}"},
            {Case::Prepositional, "адресе элемента {1 (р)}"}
        }
    },
    {
        OperationType::Dereference, {
            {Case::Nominative, "{1 (и)}"},
            {Case::Genitive, "{1 (р)}"},
            {Case::Dative, "{1 (д)}"},
            {Case::Accusative, "{1 (в)}"},
            {Case::Instrumental, "{1 (т)}"},
            {Case::Prepositional, "{1 (п)}"}
        }
    },
    {
        OperationType::ArrayAccess, {
            {Case::Nominative, "элемент под индексом {2 (и)} массива {1 (р)}"},
            {Case::Genitive, "элемента под индексом {2 (и)} массива {1 (р)}"},
            {Case::Dative, "элементу под индексом {2 (и)} массива {1 (р)}"},
            {Case::Accusative, "элемент под индексом {2 (и)} массива {1 (р)}"},
            {Case::Instrumental, "элементом под индексом {2 (и)} массива {1 (р)}"},
            {Case::Prepositional, "элементе под индексом {2 (и)} массива {1 (р)}"}
        }
    },
    {
        OperationType::FieldAccess, {
            {Case::Nominative, "{2 (и)} {1 (р)}"},
            {Case::Genitive, "{2 (р)} {1 (р)}"},
            {Case::Dative, "{2 (д)} {1 (р)}"},
            {Case::Accusative, "{2 (в)} {1 (р)}"},
            {Case::Instrumental, "{2 (т)} {1 (р)}"},
            {Case::Prepositional, "{2 (п)} {1 (р)}"}
        }
    },
    {
        OperationType::PointerFieldAccess, {
            {Case::Nominative, "{2 (и)} {1 (р)}"},
            {Case::Genitive, "{2 (р)} {1 (р)}"},
            {Case::Dative, "{2 (д)} {1 (р)}"},
            {Case::Accusative, "{2 (в)} {1 (р)}"},
            {Case::Instrumental, "{2 (т)} {1 (р)}"},
            {Case::Prepositional, "{2 (п)} {1 (р)}"}
        }
    },
    {
        OperationType::StaticMemberAccess, {
            {Case::Nominative, "{2 (и)}"},
            {Case::Genitive, "{2 (р)}"},
            {Case::Dative, "{2 (д)}"},
            {Case::Accusative, "{2 (в)}"},
            {Case::Instrumental, "{2 (т)}"},
            {Case::Prepositional, "{2 (п)}"}
        }
    },
    {
        OperationType::PrefixIncrement, {
            {Case::Nominative, "инкрементировать {1 (и)}, а затем {2 (в)}"},
            {Case::Genitive, "инкрементировать {1 (и)}, а затем {2 (в)}"},
            {Case::Dative, "инкрементировать {1 (и)}, а затем {2 (в)}"},
            {Case::Accusative, "инкрементировать {1 (и)}, а затем {2 (в)}"},
            {Case::Instrumental, "инкрементировать {1 (и)}, а затем {2 (в)}"},
            {Case::Prepositional, "инкрементировать {1 (и)}, а затем {2 (в)}"}
        }
    },
    {
        OperationType::PostfixIncrement, {
            {Case::Nominative, "получить {2 (в)}, а затем инкрементировать {1 (и)}"},
            {Case::Genitive, "получить {2 (в)}, а затем инкрементировать {1 (и)}"},
            {Case::Dative, "получить {2 (в)}, а затем инкрементировать {1 (и)}"},
            {Case::Accusative, "получить {2 (в)}, а затем инкрементировать {1 (и)}"},
            {Case::Instrumental, "получить {2 (в)}, а затем инкрементировать {1 (и)}"},
            {Case::Prepositional, "получить {2 (в)}, а затем инкрементировать {1 (и)}"}
        }
    },
    {
        OperationType::PrefixDecrement, {
            {Case::Nominative, "декрементировать {1 (и)}, а затем {2 (в)}"},
            {Case::Genitive, "декрементировать {1 (и)}, а затем {2 (в)}"},
            {Case::Dative, "декрементировать {1 (и)}, а затем {2 (в)}"},
            {Case::Accusative, "декрементировать {1 (и)}, а затем {2 (в)}"},
            {Case::Instrumental, "декрементировать {1 (и)}, а затем {2 (в)}"},
            {Case::Prepositional, "декрементировать {1 (и)}, а затем {2 (в)}"}
        }
    },
    {
        OperationType::PostfixDecrement, {
            {Case::Nominative, "получить {2 (в)}, а затем декрементировать {1 (и)}"},
            {Case::Genitive, "получить {2 (в)}, а затем декрементировать {1 (и)}"},
            {Case::Dative, "получить {2 (в)}, а затем декрементировать {1 (и)}"},
            {Case::Accusative, "получить {2 (в)}, а затем декрементировать {1 (и)}"},
            {Case::Instrumental, "получить {2 (в)}, а затем декрементировать {1 (и)}"},
            {Case::Prepositional, "получить {2 (в)}, а затем декрементировать {1 (и)}"}
        }
    },
    {
        OperationType::Assignment, {
            {Case::Nominative, "присваивание {1 (д)} значения {2 (р)}"},
            {Case::Genitive, "присваивания {1 (д)} значения {2 (р)}"},
            {Case::Dative, "присваиванию {1 (д)} значения {2 (р)}"},
            {Case::Accusative, "присваивание {1 (д)} значения {2 (р)}"},
            {Case::Instrumental, "присваиванием {1 (д)} значения {2 (р)}"},
            {Case::Prepositional, "присваивании {1 (д)} значения {2 (р)}"}
        }
    },
    {
        OperationType::AdditionAssignment, {
            {Case::Nominative, "присваивание {1 (д)} суммы {1 (р)} и {2 (р)}"},
            {Case::Genitive, "присваивания {1 (д)} суммы {1 (р)} и {2 (р)}"},
            {Case::Dative, "присваиванию {1 (д)} суммы {1 (р)} и {2 (р)}"},
            {Case::Accusative, "присваивание {1 (д)} суммы {1 (р)} и {2 (р)}"},
            {Case::Instrumental, "присваиванием {1 (д)} суммы {1 (р)} и {2 (р)}"},
            {Case::Prepositional, "присваивании {1 (д)} суммы {1 (р)} и {2 (р)}"}
        }
    },
    {
        OperationType::SubtractionAssignment, {
            {Case::Nominative, "присваивание {1 (д)} разности {1 (р)} и {2 (р)}"},
            {Case::Genitive, "присваивания {1 (д)} разности {1 (р)} и {2 (р)}"},
            {Case::Dative, "присваиванию {1 (д)} разности {1 (р)} и {2 (р)}"},
            {Case::Accusative, "присваивание {1 (д)} разности {1 (р)} и {2 (р)}"},
            {Case::Instrumental, "присваиванием {1 (д)} разности {1 (р)} и {2 (р)}"},
            {Case::Prepositional, "присваивании {1 (д)} разности {1 (р)} и {2 (р)}"}
        }
    },
    {
        OperationType::MultiplicationAssignment, {
            {Case::Nominative, "присваивание {1 (д)} произведения {1 (р)} и {2 (р)}"},
            {Case::Genitive, "присваивания {1 (д)} произведения {1 (р)} и {2 (р)}"},
            {Case::Dative, "присваиванию {1 (д)} произведения {1 (р)} и {2 (р)}"},
            {Case::Accusative, "присваивание {1 (д)} произведения {1 (р)} и {2 (р)}"},
            {Case::Instrumental, "присваиванием {1 (д)} произведения {1 (р)} и {2 (р)}"},
            {Case::Prepositional, "присваивании {1 (д)} произведения {1 (р)} и {2 (р)}"}
        }
    },
    {
        OperationType::DivisionAssignment, {
            {Case::Nominative, "присваивание {1 (д)} частного {1 (р)} и {2 (р)}"},
            {Case::Genitive, "присваивания {1 (д)} частного {1 (р)} и {2 (р)}"},
            {Case::Dative, "присваиванию {1 (д)} частного {1 (р)} и {2 (р)}"},
            {Case::Accusative, "присваивание {1 (д)} частного {1 (р)} и {2 (р)}"},
            {Case::Instrumental, "присваиванием {1 (д)} частного {1 (р)} и {2 (р)}"},
            {Case::Prepositional, "присваивании {1 (д)} частного {1 (р)} и {2 (р)}"}
        }
    },
    {
        OperationType::ModulusAssignment, {
            {Case::Nominative, "присваивание {1 (д)} остатка от деления {1 (р)} и {2 (р)}"},
            {Case::Genitive, "присваивания {1 (д)} остатка от деления {1 (р)} и {2 (р)}"},
            {Case::Dative, "присваиванию {1 (д)} остатка от деления {1 (р)} и {2 (р)}"},
            {Case::Accusative, "присваивание {1 (д)} остатка от деления {1 (р)} и {2 (р)}"},
            {Case::Instrumental, "присваиванием {1 (д)} остатка от деления {1 (р)} и {2 (р)}"},
            {Case::Prepositional, "присваивании {1 (д)} остатка от деления {1 (р)} и {2 (р)}"}
        }
    }
};

ExpressionTranslator::ExpressionTranslator() {}

QHash<Case, QString> ExpressionTranslator::getExplanation(const QHash<Case, QString> &description, const QList<QHash<Case, QString> > &arguments)
{
    QHash<Case, QString> pattern = {};
    QRegularExpression placeholderRegex(R"(\{\s*(\d+)\s*\(\s*([а-яА-ЯёЁ])\s*\)\s*\})");

    // Подставить аргументы во все падежи
    for (Case c : {Case::Nominative, Case::Genitive, Case::Dative,
                   Case::Accusative, Case::Instrumental, Case::Prepositional}) {
        pattern.insert(c, replacePlaceholders(description[c], arguments, placeholderRegex));
    }

    return pattern;
}

QString ExpressionTranslator::replacePlaceholders(const QString &pattern, const QList<QHash<Case, QString> > &args, QRegularExpression &placeholderRegex)
{
    QString patternCopy = pattern;
    QRegularExpressionMatchIterator it = placeholderRegex.globalMatch(pattern);

    // Пока есть вхождения плейсхолдера
    while (it.hasNext()) {
        QRegularExpressionMatch match = it.next();
        int index = match.captured(1).toInt() - 1;
        QString caseStr = match.captured(2);

        if (index >= 0 && index < args.size()) {
            Case caseEnum = parseCase(caseStr);
            QString replacement = args[index].value(caseEnum);

            // Заменить плейсхолдер в результирующей строке на соответствующий аргумент в указанном падеже
            patternCopy.replace(match.captured(0), replacement);
        }
    }
    return patternCopy;
}

Case ExpressionTranslator::parseCase(const QString &caseChar) {
    if (caseChar == "и")      return Case::Nominative;      // Именительный
    else if (caseChar == "р") return Case::Genitive;        // Родительный
    else if (caseChar == "д") return Case::Dative;          // Дательный
    else if (caseChar == "в") return Case::Accusative;      // Винительный
    else if (caseChar == "т") return Case::Instrumental;    // Творительный
    else if (caseChar == "п") return Case::Prepositional;   // Предложный
}
