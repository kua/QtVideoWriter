#ifndef _VideoWriter_H_INCLUDED_
#define _VideoWriter_H_INCLUDED_

#include "ImageWriter.h"
#include "FFmpegLauncher.h"
#include <QObject>
#include <QString>
#include <QImage>

class VideoWriter:
  public QObject
{
  Q_OBJECT

public:
  VideoWriter(const int communicationPort = 7777,
             const QString& workingDir = QString::fromUtf8("."),
             const QString& filePrefixName = QString::fromUtf8("video_"))
    : m_ffmpegRunner(communicationPort, workingDir, filePrefixName, this)
    , m_imageDistributor(communicationPort)
  {
  }

  Q_SLOT void save(const QImage& image)
  {
    m_imageDistributor.save(image);
  }

  Q_SLOT void start(const int fps = 25)
  try
  {
    m_imageDistributor.start();
    m_ffmpegRunner.start(fps);
  }
  catch(std::exception& ex)
  {
    qDebug() << ex.what();
  }

  Q_SLOT void stop()
  {
    m_imageDistributor.stop();
  }

  FFmpegLauncher m_ffmpegRunner;
  ImageWriterLauncher m_imageDistributor;
}; // class VideoWriter

#endif //_VideoWriter_H_INCLUDED_
