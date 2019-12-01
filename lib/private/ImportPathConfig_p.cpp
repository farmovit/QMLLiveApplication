#include "ImportPathConfig_p.h"
#include <QFileSystemWatcher>
#include <QFileInfo>
#include <QDir>
#include <QDebug>

namespace qml_live {

ImportPathConfig::ImportPathConfig(QObject *parent)
    : QObject(parent),
      m_FileSystemWatcher(new QFileSystemWatcher(this))
{
    connect(m_FileSystemWatcher, &QFileSystemWatcher::fileChanged, this, &ImportPathConfig::load);
}

bool ImportPathConfig::load(const QString &configPath)
{
    QFile configFile(configPath);
    if (!configFile.exists())
    {
        qWarning() << "Config file was not set: " << configPath;
        return false;
    }
    if (!configFile.open(QFile::ReadOnly))
    {
        qWarning() << "Cannot read config file " << configFile.errorString();
        return false;
    }
    updateWatchedConfigFile(configPath);
    auto paths = QString::fromUtf8(configFile.readAll()).simplified().split(QChar(QChar::Space));
    std::transform(std::begin(paths), std::end(paths), std::begin(paths), [&configPath](auto& path)
    {
        if (!QFile(path).exists()) {
            return QDir(QFileInfo(configPath).absolutePath()).absoluteFilePath(path);
        }
        return path;
    });
    if (paths != m_Paths)
    {
        m_Paths = paths;
        emit pathsChanged();
    }
    return true;
}

QStringList ImportPathConfig::paths() const noexcept
{
    return m_Paths;
}

void ImportPathConfig::updateWatchedConfigFile(const QString &configPath)
{
    const auto &files = m_FileSystemWatcher->files();
    if (files.contains(configPath))
    {
        return;
    }
    if (!files.isEmpty())
    {
        m_FileSystemWatcher->removePaths(files);
    }
    m_FileSystemWatcher->addPath(configPath);
}

} // namespace qml_live
