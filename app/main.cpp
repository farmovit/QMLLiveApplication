#include "QmlLiveApplication.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    qml_live::QmlLiveApplication app(QStringLiteral("test_application"), argc, argv);
    app.setRootQmlFilePath(QStringLiteral("../../test/additional/Add.qml"));

    return app.exec();
}
