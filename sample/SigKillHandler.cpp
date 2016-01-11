#include "SigKillHandler.h"
#include <QCoreApplication>

#if (defined __USE_POSIX2) && (defined __USE_BSD)
#include <signal.h>
#include <sys/socket.h>
#include <unistd.h>
#endif // (defined __USE_POSIX2) && (defined __USE_BSD)

int SigKillHandler::s_fileDescriptorsPair[2] = {0, 0};

#if (defined __USE_POSIX2) && (defined __USE_BSD)
SigKillHandler::SigKillHandler(QObject* parent) : QObject(parent)
{
  if(::socketpair(AF_UNIX, SOCK_STREAM, 0, s_fileDescriptorsPair))
  {
    Q_ASSERT(!"Невозможно создать socketpair для обработки SIGINT.");
  }

  m_signalNotifier = new QSocketNotifier(s_fileDescriptorsPair[1], QSocketNotifier::Read, this);

  connect(m_signalNotifier, SIGNAL(activated(int)), this, SLOT(signalHandlerSlot()));

  signal(SIGINT, SigKillHandler::signalHandler);
  signal(SIGTERM, SigKillHandler::signalHandler);
  signal(SIGHUP, SigKillHandler::signalHandler);
}

void SigKillHandler::signalHandler(int unused)
{
  char a = 1;
  ::write(s_fileDescriptorsPair[0], &a, sizeof(a));
}

void SigKillHandler::signalHandlerSlot()
{
  m_signalNotifier->setEnabled(false);
  char tmp;
  ::read(s_fileDescriptorsPair[1], &tmp, sizeof(tmp));

  QCoreApplication::exit(EXIT_SUCCESS);

  m_signalNotifier->setEnabled(true);
}
#else
SigKillHandler::SigKillHandler(QObject* parent) : QObject(parent)
{
}

void SigKillHandler::signalHandler(int )
{
}

void SigKillHandler::signalHandlerSlot()
{
}
#endif // (defined __USE_POSIX2) && (defined __USE_BSD)

