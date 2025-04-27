#include "codeentity.h"

Variable::Variable(const QString &name, const QString &type, const QHash<Case, QString> &description)
    : name(name), type(type), description(description) {}

QString Variable::toQString(const QString& startLine) const {

    QString result = startLine + "Variable:";
    result += "  name: " +          this->name;
    result += ";  type: " +         this->type;
    result += ";  description: " +  this->description.value(Case::Nominative);

    return result;
}

Function::Function(const QString &name, const QString &type, int paramsCount, const QHash<Case, QString> &description)
    : name(name), type(type), paramsCount(paramsCount), description(description) {}

QString Function::toQString(const QString& startLine) const {

    QString result = startLine + "Function:";
    result += "  name: " +          this->name;
    result += ";  type: " +         this->type;
    result += ";  description: " +  this->description.value(Case::Nominative);

    return result;
}

Union::Union(const QString &name, const QHash<QString, Variable> &variables, const QHash<QString, Function> &functions)
    : name(name), variables(variables), functions(functions) {}

QString Union::toQString(const QString& startLine) const {

    QString result = startLine + "Union:";
    result += " name: " + this->name;

    QHashIterator itV(this->variables);
    while(itV.hasNext())
        result += "\n" + itV.next().value().toQString(startLine + "    ");

    QHashIterator itF(this->functions);
    while(itF.hasNext())
        result += "\n" + itF.next().value().toQString(startLine + "    ");

    return result;
}

Structure::Structure(const QString &name, const QHash<QString, Variable> &variables, const QHash<QString, Function> &functions)
    : name(name), variables(variables), functions(functions) {}

QString Structure::toQString(const QString& startLine) const {

    QString result = startLine + "Structure:";
    result += " name: " + this->name;

    QHashIterator itV(this->variables);
    while(itV.hasNext())
        result += "\n" + itV.next().value().toQString(startLine + "    ");

    QHashIterator itF(this->functions);
    while(itF.hasNext())
        result += "\n" + itF.next().value().toQString(startLine + "    ");

    return result;
}

Class::Class(const QString &name, const QHash<QString, Variable> &variables, const QHash<QString, Function> &functions)
    : name(name), variables(variables), functions(functions) {}

QString Class::toQString(const QString& startLine) const {

    QString result = startLine + "Class:";
    result += " name: " + this->name;

    QHashIterator itV(this->variables);
    while(itV.hasNext())
        result += "\n" + itV.next().value().toQString(startLine + "    ");

    QHashIterator itF(this->functions);
    while(itF.hasNext())
        result += "\n" + itF.next().value().toQString(startLine + "    ");

    return result;
}

Enum::Enum(const QString &name, const QHash<QString, QHash<Case, QString> > &values)
    : name(name), values(values) {}

QString Enum::toQString(const QString& startLine) const {

    QString result = startLine + "Enum:";
    result += " name: " + name;

    QHashIterator itV(this->values);
    while(itV.hasNext()) {
        auto v = itV.next();
        result += "\n   " + startLine + v.key() + ": " + v.value().value(Case::Nominative);
    }
    return result;
}
