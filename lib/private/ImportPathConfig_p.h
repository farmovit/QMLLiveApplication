#pragma once

#include <QObject>
#include "shared_export.h"

QT_BEGIN_NAMESPACE
class QFileSystemWatcher;
QT_END_NAMESPACE

namespace qml_live {

class SHARED_EXPORT ImportPathConfig : public QObject
{
    Q_OBJECT

public:
    explicit ImportPathConfig(QObject *parent = nullptr);
    QStringList paths() const noexcept;

public slots:
    bool load(const QString &configPath);

signals:
    void pathsChanged();

private:
    void updateWatchedConfigFile(const QString &configPath);

private:
    QStringList m_Paths;
    QFileSystemWatcher *m_FileSystemWatcher;
};

} // namespace qml_live
