#ifndef _SigKillHandler_H_
#define _SigKillHandler_H_

#include <QSocketNotifier>

class SigKillHandler: public QObject
{
  Q_OBJECT

public:
  SigKillHandler(QObject* parent = 0);

  static void signalHandler(int unused);
  Q_SLOT void signalHandlerSlot();

private:
  static int s_fileDescriptorsPair[2];
  QSocketNotifier* m_signalNotifier;
};

#endif //_SigKillHandler_H_
