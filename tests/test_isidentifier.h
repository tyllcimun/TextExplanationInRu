#ifndef TEST_ISIDENTIFIER_H
#define TEST_ISIDENTIFIER_H

#include <QObject>

class test_isIdentifier : public QObject
{
    Q_OBJECT
public:
    explicit test_isIdentifier(QObject *parent = nullptr);

private slots:
    void isIdentifier();
    void isIdentifier_data();

};

#endif // TEST_ISIDENTIFIER_H
