#pragma once

#include <QObject>

class QmlLiveTest : public QObject
{
    Q_OBJECT
public:
    explicit QmlLiveTest(QObject *parent = nullptr);
private slots:
    void importPathTest();
};
