#ifndef TEST_removeConsecutiveDuplicates_H
#define TEST_removeConsecutiveDuplicates_H

#include <QObject>

class test_removeConsecutiveDuplicates : public QObject
{
    Q_OBJECT
public:
    explicit test_removeConsecutiveDuplicates(QObject *parent = nullptr);

private slots:
    void removeConsecutiveDuplicates();
    void removeConsecutiveDuplicates_data();

};

#endif // TEST_removeConsecutiveDuplicates_H
