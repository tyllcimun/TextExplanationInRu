#ifndef TEST_GETEXPLANATION_H
#define TEST_GETEXPLANATION_H

#include <QObject>

class test_getExplanation : public QObject
{
    Q_OBJECT
public:
    explicit test_getExplanation(QObject *parent = nullptr);

private slots:
    void getExplanation();
    void getExplanation_data();
};

#endif // TEST_GETEXPLANATION_H
