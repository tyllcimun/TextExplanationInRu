#ifndef TEST_EXPRESSIONTONODES_H
#define TEST_EXPRESSIONTONODES_H

#include <QObject>

class test_expressionToNodes : public QObject
{
    Q_OBJECT
public:
    explicit test_expressionToNodes(QObject *parent = nullptr);

private slots:
    void expressionToNodes();
    void expressionToNodes_data();
};

#endif // TEST_EXPRESSIONTONODES_H
