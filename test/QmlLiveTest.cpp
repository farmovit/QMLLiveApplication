#include "QmlLiveTest.h"
#include "private/ImportPathConfig_p.h"
#include <QtTest>
#include <QTemporaryFile>
#include <QDebug>

static constexpr auto rootQmlFile = QStringViewLiteral("../test/data/path1/main_root.qml");

QmlLiveTest::QmlLiveTest(QObject *parent)
    : QObject(parent) {
}

void QmlLiveTest::importPathTest() {
    static const QStringList testPaths = {
        QStringLiteral("/path/to/file"),
        QStringLiteral("../relative/path/to/file"),
        QStringLiteral("C:/path/to/file")
    };
    QTemporaryFile tempConfigPath;
    tempConfigPath.open();
    for (const auto &path: testPaths) {
        tempConfigPath.write(path.toUtf8());
        tempConfigPath.write("\n");
    }
    tempConfigPath.close();
    auto pathConfig = new qml_live::ImportPathConfig(this);
    pathConfig->load(tempConfigPath.fileName());
    QVERIFY2(pathConfig->paths() == testPaths, "Failed to parse config file");
}

QTEST_MAIN(QmlLiveTest)
