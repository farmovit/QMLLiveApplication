#include "PathWatcher_p.h"
#include "ImportPathConfig_p.h"
#include <QQmlEngine>
#include <QFileInfo>
#include <QFile>
#include <QDirIterator>
#include <QFileSystemWatcher>
#include <QDebug>

namespace {
constexpr std::array qmlFileExtensions = {
    QStringViewLiteral("qml"),
    QStringViewLiteral("ui.qml")
};
constexpr auto configPathsFileName = QStringViewLiteral("qmllive_config_paths.conf");
} // namespace

namespace qml_live {

PathWatcher::PathWatcher(QObject *parent)
    : QObject(parent),
      m_PathConfig(new ImportPathConfig(this)),
      m_FileSystemWatcher(new QFileSystemWatcher(this))
{
    connect(m_FileSystemWatcher, &QFileSystemWatcher::directoryChanged, this, [this](const QString &dir) {
        Q_UNUSED(dir)
        updateWatchedPaths();
    });
    connect(m_FileSystemWatcher, &QFileSystemWatcher::fileChanged, this, [this](const QString &file) {
        if (isFileQml(file))
        {
            emit watchedPathsChanged();
        }
        else
        {
            updateWatchedPaths();
        }
    });
    connect(m_PathConfig, &ImportPathConfig::pathsChanged, this, &PathWatcher::updateWatchedPaths);
}

QString PathWatcher::rootQmlFile() const noexcept
{
    return m_rootQmlFile;
}

QStringList PathWatcher::watchedPaths() const noexcept
{
    return m_FileSystemWatcher->directories();
}

QStringList PathWatcher::watchedFiles() const noexcept
{
    return m_FileSystemWatcher->files();
}

void PathWatcher::setRootQmlFile(const QString &rootQmlFile)
{
    QString filePath;
    if (QUrl url(rootQmlFile); url.isValid())
    {
        filePath = url.toLocalFile();
    }
    filePath = QFileInfo(filePath).absoluteFilePath();
    if (filePath.isEmpty() || !QFile(filePath).exists())
    {
        filePath = QFileInfo(rootQmlFile).absoluteFilePath();
    }
    if (isFileQml(filePath)
            && m_rootQmlFile != filePath
            && QFile(filePath).exists())
    {
        m_rootQmlFile = filePath;
        emit rootQmlFileChanged(m_rootQmlFile);
        updateWatchedPaths();
    }
}

void PathWatcher::updateWatchedPaths() {
    m_PathConfig->load(QStringLiteral("%1/%2").arg(QFileInfo(m_rootQmlFile).absolutePath()).arg(configPathsFileName));
    auto paths = m_PathConfig->paths();
    paths.append(QFileInfo(m_rootQmlFile).absolutePath());
    const auto &watchedDirs = m_FileSystemWatcher->directories();
    if (watchedDirs != paths)
    {
        if (!watchedDirs.isEmpty()) {
            m_FileSystemWatcher->removePaths(watchedDirs);
        }
        m_FileSystemWatcher->addPaths(paths);
    }
    const auto &watchedFiles = m_FileSystemWatcher->files();
    if (!watchedFiles.isEmpty()) {
        m_FileSystemWatcher->removePaths(watchedFiles);
    }
    for (const auto& path: paths)
    {
        QDirIterator it(path, QDir::AllEntries | QDir::NoDotAndDotDot,
                        QDirIterator::Subdirectories | QDirIterator::FollowSymlinks);
        while (it.hasNext())
        {
            const auto& file = it.next();
            if (isFileQml(file)) {
                m_FileSystemWatcher->addPath(file);
            }
        }
    }
    if (QSet<QString>::fromList(watchedFiles) != QSet<QString>::fromList(m_FileSystemWatcher->files())) {
        emit watchedPathsChanged();
    }
}

bool PathWatcher::isFileQml(QStringView fileName)
{
    QFileInfo fileInfo(fileName.toString());
    const auto extension = fileInfo.completeSuffix();
    return std::find_if(qmlFileExtensions.cbegin(), qmlFileExtensions.cend(),
                        [&extension](const auto &ext) {
        return ext.toString() == extension;
    }) != qmlFileExtensions.cend();
}

} // namespace qml_live
