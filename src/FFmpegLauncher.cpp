#include "FFmpegLauncher.h"
#include <QDateTime>
#include <QDebug>

namespace
{
  QString createFileNameTemplate(const QString& workingDir, const QString& filePrefixName)
  {
    return workingDir + QString::fromUtf8("/") + filePrefixName + QString::fromUtf8("%1.avi");
  }

  //For example: ffmpeg -y -f image2pipe -vcodec bmp -i tcp://127.0.0.1:7777/ -r 12 -vcodec mpeg4 /tmp/1111.avi
  QString createCommand(const int serverPort, const QString& fileNameTemplate)
  {
    QString retVal(
      QString::fromUtf8("ffmpeg -y -f image2pipe -vcodec mjpeg -r %2")
      + QString::fromUtf8(" -i tcp://127.0.0.1:") + QString::number(serverPort) + QString::fromUtf8("/")
      + QString::fromUtf8(" -vcodec libx264 -preset ultrafast ") + fileNameTemplate);
    return retVal;
  }
} // namespace

FFmpegLauncher::FFmpegLauncher(const int serverPort, const QString& workingDir, const QString& filePrefixName, QObject* parent)
  : QObject(parent)
  , m_fps(1)
  , m_process(this)
  , m_fileNameTemplate(createFileNameTemplate(workingDir, filePrefixName))
  , m_commandTemplate(createCommand(serverPort, m_fileNameTemplate))
  , m_timer(this)
{
  m_process.setProcessChannelMode(QProcess::MergedChannels);

  m_timer.setSingleShot(true);

  connect(&m_process, SIGNAL(started()), this, SLOT(onStarted()));
  connect(&m_process, SIGNAL(error(QProcess::ProcessError)), this, SLOT(onLaunchError(QProcess::ProcessError)));
  connect(&m_process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(onFinished(int, QProcess::ExitStatus)));
  connect(&m_timer,   SIGNAL(timeout()), this, SLOT(launch()));
}

FFmpegLauncher::~FFmpegLauncher()
{
  m_timer.stop();
  m_process.waitForFinished();
}

void FFmpegLauncher::start(int fps)
{
  m_fps = fps ? fps : 1;

  if(m_process.state() == QProcess::NotRunning)
  {
    launch();
  }
}

void FFmpegLauncher::launch()
{
  const QString currentDateTime(QDateTime::currentDateTime().toString("yyyyMMddThhmmss.zzz"));
  const QString commandName(m_commandTemplate.arg(currentDateTime).arg(m_fps));

  m_lastFileName = m_fileNameTemplate.arg(currentDateTime);

  qDebug() << QString::fromUtf8("Launch ffmpeg: ") + commandName;

  m_process.start(commandName);
}

void FFmpegLauncher::onFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
  if(exitStatus == QProcess::NormalExit)
  {
    qDebug() << QString::fromUtf8("Stop writing video file ") + m_lastFileName;
  }
  else
  {
    const int TIMEOUT = 1000;

    m_timer.start(TIMEOUT);

    qDebug() << QString::fromUtf8("ffmpeg process for writing video file  ") + m_lastFileName
             + QString::fromUtf8(" has aborted, restart attempt will be performed through ")
             + QString::number(TIMEOUT) + QString::fromUtf8(" milliseconds.");
  }
}

void FFmpegLauncher::onLaunchError(QProcess::ProcessError error)
{
  if (error == QProcess::FailedToStart)
  {
    qDebug() << QString::fromUtf8("Unable to launch ffmpeg process for creating video file ") + m_lastFileName;
  }
}

void FFmpegLauncher::onStarted()
{
  qDebug() << QString::fromUtf8("Start writing video file ") + m_lastFileName;
}
