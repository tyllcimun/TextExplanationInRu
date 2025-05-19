#ifndef TEST_ISCUSTOMTYPEWITHFILEDS_H
#define TEST_ISCUSTOMTYPEWITHFILEDS_H

#include <QObject>

class test_isCustomTypeWithFileds : public QObject
{
    Q_OBJECT
public:
    explicit test_isCustomTypeWithFileds(QObject *parent = nullptr);


private slots:
    void isCustomTypeWithFileds();
    void isCustomTypeWithFileds_data();
};

#endif // TEST_ISCUSTOMTYPEWITHFILEDS_H
