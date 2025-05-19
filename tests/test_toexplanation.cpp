#include "qobject.h"
#include "test_toexplanation.h"
#include "expression.h"
#include "expressionnode.h"
#include "qtestcase.h"
#include "codeentity.h"
#include "teexception.h"
#include <QString>
#include <QHash>
#include <QTest>

Q_DECLARE_METATYPE(Case)
using CaseExplanationHash = QHash<Case, QString>;
Q_DECLARE_METATYPE(CaseExplanationHash)

test_toExplanation::test_toExplanation(QObject *parent)
    : QObject{parent}
{}

void test_toExplanation::toExplanation()
{
}


void test_toExplanation::toExplanation_data()
{
}
