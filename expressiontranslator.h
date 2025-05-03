#ifndef EXPRESSIONTRANSLATOR_H
#define EXPRESSIONTRANSLATOR_H

#include "codeentity.h"
#include <QString>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QRegularExpressionMatchIterator>

#include <QHash>
#include <QString>

class ExpressionTranslator
{
public:
    ExpressionTranslator();

    static QHash<Case, QString> getExplanation(const QHash<Case, QString> &description, const QList<QHash<Case, QString>> &arguments);
    static QString replacePlaceholders(const QString &pattern, const QList<QHash<Case, QString>> &args, QRegularExpression& placeholderRegex);
};

#endif // EXPRESSIONTRANSLATOR_H
