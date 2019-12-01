#include "QmlLiveApplication.h"
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QProcess>
#include "private/PathWatcher_p.h"

namespace {
constexpr auto main_qml_url = QStringViewLiteral("qrc:/main.qml");
} // namespace

namespace qml_live {

QmlLiveApplication::QmlLiveApplication(const QString &applicationName, int &argc, char **argv)
    : QGuiApplication(argc, argv),
      m_ApplicationEnginePtr(std::make_shared<QQmlApplicationEngine>()),
      m_PathWatcher(new PathWatcher(this))
{
    setOrganizationName(QStringLiteral("org_%1").arg(applicationName));
    setOrganizationDomain(QStringLiteral("%1.com").arg(applicationName));
    setApplicationName(applicationName);
    setApplicationDisplayName(applicationName);
    m_ApplicationEnginePtr->rootContext()->setContextProperty(QStringLiteral("PathWatcher"), m_PathWatcher);
    m_ApplicationEnginePtr->rootContext()->setContextProperty(QStringLiteral("LiveApp"), this);
}

void QmlLiveApplication::setRootQmlFilePath(const QString &rootQmlFilePath)
{
    m_PathWatcher->setRootQmlFile(rootQmlFilePath);
}

QString QmlLiveApplication::rootQmlFileUrl() const
{
    return QUrl::fromLocalFile(m_PathWatcher->rootQmlFile()).toString();
}

std::shared_ptr<QQmlApplicationEngine> QmlLiveApplication::applicationEngine() const noexcept
{
    return m_ApplicationEnginePtr;
}

void QmlLiveApplication::restartApplication()
{
    qApp->quit();
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
}

void QmlLiveApplication::clearCache()
{
    m_ApplicationEnginePtr->clearComponentCache();
}

int QmlLiveApplication::exec()
{
    m_ApplicationEnginePtr->load(main_qml_url.toString());
    if (m_ApplicationEnginePtr->rootObjects().isEmpty())
    {
        return -1;
    }
    connect(m_PathWatcher, &PathWatcher::watchedPathsChanged, this, [this]()
    {
        QMetaObject::invokeMethod(m_ApplicationEnginePtr->rootObjects().first(), "reload", Q_ARG(QVariant, rootQmlFileUrl()));
    });
    QMetaObject::invokeMethod(m_ApplicationEnginePtr->rootObjects().first(), "reload", Q_ARG(QVariant, rootQmlFileUrl()));
    return QGuiApplication::exec();
}

} // namespace qml_live
