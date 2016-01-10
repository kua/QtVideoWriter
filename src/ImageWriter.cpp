#include "ImageWriter.h"
#include <QBuffer>
#include <QThread>

ImageWriter::ImageWriter(quint16 port, QObject* parent)
  : QTcpServer(parent)
  , m_disabled(true)
  , m_socket(new QTcpSocket(this))
{
  listen(QHostAddress::Any, port);
}

ImageWriter::~ImageWriter()
{
  pause();
}

void ImageWriter::incomingConnection(int socket)
{
  if(m_disabled)
    return;

  connect(m_socket, SIGNAL(disconnected()), this, SLOT(discardClient()));
  m_socket->setSocketDescriptor(socket);
}

void ImageWriter::pause()
{
  m_disabled = true;

  discardClient();
}

void ImageWriter::resume()
{
  m_disabled = false;
}

void ImageWriter::distribute(const QImage& img)
{
  if(m_disabled || !m_socket->isWritable())
    return;

  QBuffer buffer;
  buffer.open(QIODevice::WriteOnly);
  img.save(&buffer, "JPG");

  m_socket->write(buffer.data());
}

void ImageWriter::discardClient()
{
  m_socket->close();
}
