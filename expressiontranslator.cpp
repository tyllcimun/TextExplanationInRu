#include "expressiontranslator.h"

ExpressionTranslator::ExpressionTranslator() {}

QHash<Case, QString> ExpressionTranslator::getExplanation(const QHash<Case, QString> &description, const QList<QHash<Case, QString> > &arguments)
{

}

QString ExpressionTranslator::replacePlaceholders(const QString &pattern, const QList<QHash<Case, QString> > &args, QRegularExpression &placeholderRegex)
{

}
