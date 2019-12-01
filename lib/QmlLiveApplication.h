#pragma once

#include <QGuiApplication>
#include <memory>
#include "shared_export.h"

QT_BEGIN_NAMESPACE
class QQmlApplicationEngine;
QT_END_NAMESPACE

namespace qml_live {
class PathWatcher;
class SHARED_EXPORT QmlLiveApplication : public QGuiApplication {
    Q_OBJECT
public:
    QmlLiveApplication(const QString &applicationName, int &argc, char **argv);
    std::shared_ptr<QQmlApplicationEngine> applicationEngine() const noexcept;
    int exec();

public slots:
    void setRootQmlFilePath(const QString &rootQmlFilePath);
    QString rootQmlFileUrl() const;
    void restartApplication();
    void clearCache();

private:
    std::shared_ptr<QQmlApplicationEngine> m_ApplicationEnginePtr = nullptr;
    PathWatcher *m_PathWatcher = nullptr;
};

} // namespace qml_live
