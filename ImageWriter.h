#ifndef _ImageWriter_H_INCLUDED_
#define _ImageWriter_H_INCLUDED_

#include <QImage>
#include <QTcpServer>
#include <QTcpSocket>
#include <QThread>
#include <QScopedPointer>
#include <stdexcept>

class ImageWriter: public QTcpServer
{
  Q_OBJECT

public:
  ImageWriter(quint16 port, QObject* parent = 0);
  ~ImageWriter();

  Q_SLOT void pause();
  Q_SLOT void resume();
  Q_SLOT void distribute(const QImage& img);

protected:
  void incomingConnection(int socket);

private:
  Q_SLOT void discardClient();

  bool m_disabled;
  QTcpSocket* m_socket;
};

class ImageWriterLauncher:
  public QObject
{
  Q_OBJECT

public:
  ImageWriterLauncher(const int port)
    : m_port(port)
  {
    connect(&m_worker, SIGNAL (started()), this, SLOT (init()));
    moveToThread(&m_worker);
    m_worker.start();
  }

  ~ImageWriterLauncher()
  {
    if(m_worker.isRunning())
    {
      m_worker.quit();
      m_worker.wait();
    }
  }

  void save(const QImage& image)
  {
    if(m_imageWriter)
      QMetaObject::invokeMethod(m_imageWriter.data(), "distribute", Qt::QueuedConnection, Q_ARG(QImage, image));
  }

  void start()
  {
    if(m_imageWriter)
      QMetaObject::invokeMethod(m_imageWriter.data(), "resume", Qt::QueuedConnection);
    else
      throw std::runtime_error("Unable to start recording a video stream until the worker thread not started");
  }

  void stop()
  {
    if(m_imageWriter)
      QMetaObject::invokeMethod(m_imageWriter.data(), "pause", Qt::QueuedConnection);
  }

private:
  Q_SLOT void init()
  {
    m_imageWriter.reset(new ImageWriter(m_port, this));
  }

  const int m_port;
  QScopedPointer<ImageWriter> m_imageWriter;
  QThread m_worker;
}; // class ImageWriterRunner

#endif //_ImageWriter_H_INCLUDED_
