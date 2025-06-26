#ifndef TEST_ISREDUCIBLEUNARYSELFINVERSE_H
#define TEST_ISREDUCIBLEUNARYSELFINVERSE_H

#include <QObject>
#include "expressionnode.h"

class test_isReducibleUnarySelfInverse : public QObject
{
    Q_OBJECT
public:
    explicit test_isReducibleUnarySelfInverse(QObject *parent = nullptr);

private slots:
    void isReducibleUnarySelfInverse();
    void isReducibleUnarySelfInverse_data();
};

#endif // TEST_ISREDUCIBLEUNARYSELFINVERSE_H
