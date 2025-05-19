#ifndef TEST_GETEXPLANATIONINEN_H
#define TEST_GETEXPLANATIONINEN_H

#include <QObject>

class test_getExplanationInRu : public QObject
{
    Q_OBJECT
public:
    explicit test_getExplanationInRu(QObject *parent = nullptr);

private slots:
    void getExplanationInRu();
    void getExplanationInRu_data();

};

#endif // TEST_GETEXPLANATIONINEN_H
