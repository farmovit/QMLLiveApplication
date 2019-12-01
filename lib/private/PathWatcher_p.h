#pragma once

#include <QObject>
#include <QMap>

QT_BEGIN_NAMESPACE
class QFileSystemWatcher;
QT_END_NAMESPACE

namespace qml_live {

class ImportPathConfig;
class PathWatcher : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString rootQmlFile READ rootQmlFile WRITE setRootQmlFile NOTIFY rootQmlFileChanged)
    Q_PROPERTY(QStringList watchedPaths READ watchedPaths NOTIFY watchedPathsChanged)

public:
    explicit PathWatcher(QObject *parent = nullptr);
    QString rootQmlFile() const noexcept;
    QStringList watchedPaths() const noexcept;
    QStringList watchedFiles() const noexcept;

    static bool isFileQml(QStringView fileName);

public slots:
    void setRootQmlFile(const QString &rootQmlFile);

signals:
    void rootQmlFileChanged(const QString& rootQmlFile);
    void watchedPathsChanged();

private slots:
    void updateWatchedPaths();

private:
    QString m_rootQmlFile;
    QStringList m_WatchedPaths;
    ImportPathConfig *m_PathConfig;
    QFileSystemWatcher *m_FileSystemWatcher;
};

} // namespace qml_live
