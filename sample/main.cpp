#include "VideoWriter.h"
#include "SigKillHandler.h"
#include <QImage>
#include <QTimer>
#include <QDateTime>
#include <QPainter>
#include <QApplication>

void drawTimeStamp(QImage& image)
{
  const QRectF TIMESTAMP_POSITION(QPointF(-0.4, -0.4), QPointF(0.5, -0.5));

  QPainter painter(&image);
  painter.setPen(Qt::red);
  painter.setFont(QFont("Arial", 60.0 / 1000.0 * image.size().height() ));

  const QRect textRect(10, 10, 900,100);

  painter.drawText(textRect, Qt::AlignLeft, QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss.zzz "));
}

class ImageTestGenerator : public QObject
{
public:
  ImageTestGenerator()
    : QObject(NULL)
    , m_image(":/sample/test.jpg")
  {
    QTimer::singleShot(1000, &m_saver, SLOT(start()));
    startTimer(50);
  }

protected:
  void timerEvent(QTimerEvent * event)
  {
    QImage imageWithTime(m_image);

    drawTimeStamp(imageWithTime);

    m_saver.save(imageWithTime);
  }

private:
  const QImage m_image;
  VideoWriter m_saver;
};

int main(int argc, char *argv[])
{
  QApplication app(argc, argv); //for the possibility of drawing on Qimage with a QPainter.
  SigKillHandler sigkillHandler;

  Q_INIT_RESOURCE(sample);

  ImageTestGenerator generator;

  return app.exec();
}
