#ifndef _FFmpegLauncher_H_INCLUDED_
#define _FFmpegLauncher_H_INCLUDED_

#include <QProcess>
#include <QObject>
#include <QTimer>

class FFmpegLauncher: public QObject
{
  Q_OBJECT

public:
  FFmpegLauncher(const int serverPort, const QString& workingDir, const QString& filePrefixName, QObject* parent = 0);
  ~FFmpegLauncher();

  void start(int fps);

private:
  Q_SLOT void launch();
  Q_SLOT void onStarted();
  Q_SLOT void onFinished(int exitCode, QProcess::ExitStatus exitStatus);
  Q_SLOT void onLaunchError(QProcess::ProcessError error);

  int m_fps;
  QProcess m_process;
  const QString m_fileNameTemplate;
  const QString m_commandTemplate;
  QString m_lastFileName;
  QTimer m_timer;
};

#endif //_FFmpegLauncher_H_INCLUDED_
