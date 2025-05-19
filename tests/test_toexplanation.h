#ifndef TEST_TOEXPLANATION_H
#define TEST_TOEXPLANATION_H
#include "expression.h"
#include "expressionnode.h"
#include "qtestcase.h"
#include "codeentity.h"
#include <QObject>

class test_toExplanation : public QObject
{
    Q_OBJECT
public:
    explicit test_toExplanation(QObject *parent = nullptr);

private slots:
    void toExplanation();
    void toExplanation_data();
};

#endif // TEST_TOEXPLANATION_H
