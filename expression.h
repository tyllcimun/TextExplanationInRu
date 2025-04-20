#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "codeentity.h"

#include <QHash>
#include <QString>


class Expression
{
public:
    QString ToQstring();

    void setExpression(const QString &newExpression);
    const QString* getExpression() const;

    const QHash<QString, Variable>* getVariables() const;
    void setVariables(const QHash<QString, Variable> &newVariables);
    const Variable getVarByName(QString name) const;

    const QHash<QString, Function>* getFunctions() const;
    void setFunctions(const QHash<QString, Function> &newFunctions);
    const Function getFuncByName(QString name) const;

    const QHash<QString, Union>* getUnions() const;
    void setUnions(const QHash<QString, Union> &newUnions);
    const Union getUnionByName(QString name) const;

    const QHash<QString, Structure>* getStructures() const;
    void setStructures(const QHash<QString, Structure> &newStructures);
    const Structure getStructByName(QString name) const;

    const QHash<QString, Class>* getClasses() const;
    void setClasses(const QHash<QString, Class> &newClasses);
    const Class getClassByName(QString name) const;

    const QHash<QString, Enum>* getEnums() const;
    void setEnums(const QHash<QString, Enum> &newEnums);
    const Enum getEnumByName(QString name) const;

    const Variable getVariableByNameFromCustomData(QString varName, QString dataName) const;
    const Function getFunctionByNameFromCustomData(QString varName, QString dataName) const;

private:
    QString expression; // Текстовое выражение
    QHash<QString, Variable> variables; //  Переменные
    QHash<QString, Function> functions; // функции
    // Пользовательские типы
    QHash<QString, Union> unions; // Объединения
    QHash<QString, Structure> structures; // Структуры
    QHash<QString, Class> classes; // Классы
    QHash<QString, Enum> enums; // Перечисления

};

#endif // EXPRESSION_H
