/****************************************************************************
** Copyright (C) 2010 CERN 
** All rights reserved.
** Contact: CernVM project (cernvm-talk@cern.ch)
** Author: Artem Harutyunyan (Artem.Harutyunyan@cern.ch)
****************************************************************************/

#include "CVMIConfig.h"
#include <QDir>
#include <QTextStream>
#include <QDebug>

CVMIConfig::CVMIConfig() :
confFileName("cvminst.cnf")
{

  qDebug() << "_conf_ ()";
  // Create the config file and populate it with default values
  QChar separator = QDir::separator();
  QString homeDir = QDir::homePath() + separator; 
  confDirPath = homeDir + ".cernvm" + separator; 
 
  QDir confDir;

  if (! confDir.exists(confDirPath))
    confDir.mkpath(confDirPath); 

  confFilePath = confDirPath + confFileName;
 
  if (! confDir.exists(confFilePath))
  {
    qDebug() << "_conf_ " << confFilePath << "does not exist";
    reset();
    return;
  }

  read(); 
}

void CVMIConfig::reset()
{
  QChar separator = QDir::separator();
  imageDir = confDirPath + "images" + separator;
  vmDir   = confDirPath;

  #ifdef Q_OS_MAC
    hypervisorDir = "/Applications/";
  #endif

  #ifdef Q_OS_WIN32
    //hypervisorDir = "C:\\\\Program Files\\\\Oracle\\\\VirtualBox\\\\";
    hypervisorDir = "C:/Program Files/Oracle/VirtualBox/";
    qDebug() << hypervisorDir;
  #endif
  
  #ifdef Q_OS_LINUX
    hypervisorDir = "/usr/";
  #endif

  qDebug() << hypervisorDir;
  showDevReleases = "false";

  QFile confFile (confFilePath); 
  QString confString;
  Q_ASSERT(confFile.open (QIODevice::WriteOnly));
  QTextStream out(&confFile); 

  out << "imageDir = " << imageDir << "\n";
  out << "vmDir = " << vmDir << "\n";
  out << "hypervisorDir = " << hypervisorDir << "\n";
  out << "showDevReleases = " << showDevReleases << "\n";
  confFile.close();
}

void CVMIConfig::write()
{
  qDebug() << "_conf_ write";
  QFile confFile (confFilePath); 
  QString confString;
  Q_ASSERT(confFile.open (QIODevice::WriteOnly));
  QTextStream out(&confFile); 

  out << "imageDir = " << imageDir << "\n";
  out << "vmDir = " << vmDir << "\n";
  out << "hypervisorDir = " << hypervisorDir << "\n";
  out << "showDevReleases = " << showDevReleases << "\n";
  confFile.close();
}

void CVMIConfig::read()
{
  qDebug() << "_conf_ read";

  QFile confFile (confFilePath);
  QString confString;
  Q_ASSERT(confFile.open(QIODevice::ReadOnly));
  QTextStream in(&confFile);

  QRegExp rx;
  QString var;
  QString val;
  confString = in.readLine();
 
  while (!confString.isNull())
  {
    rx.setPattern ("^\\s*(\\w+)\\s*=\\s*(\\S.*\\S)\\s*$");
 
    qDebug() << "_conf_ read configString " << confString;  
  
    if (rx.indexIn(confString) != -1 )
    {
      var = rx.cap(1);
      val = rx.cap(2);
      
      if (var.compare("imageDir") == 0)
      {
        imageDir = val;
        qDebug() << "_conf_ read imageDir " << imageDir;
      }
      else if (var.compare("vmDir") == 0)
      {
        vmDir = val;
        qDebug() << "_conf_ read vmDir " << vmDir;
      }
      else if (var.compare("hypervisorDir") == 0)
      {
        hypervisorDir = val;
        qDebug() << "_conf_ read hypervisorDir " << hypervisorDir;
      }
      else if (var.compare("showDevReleases") == 0)
      {
        showDevReleases = val; 
        qDebug() << "_conf_ read showDevReleases " << showDevReleases;
      }
    }
    confString = in.readLine();
  } 
  
  confFile.close();
}

QString CVMIConfig::getImageDir()
  { return imageDir; }
  
QString CVMIConfig::getVMDir()
  { return vmDir; }

QString CVMIConfig::getHypervisorDir()
  { return hypervisorDir; }

QString CVMIConfig::getShowDevReleases()
  { return showDevReleases; } 
 
void CVMIConfig::setImageDir (QString dir)
{
  qDebug() << "_conf_ imageDir " << dir; 
  imageDir = dir; 
  write(); 
}
  
void CVMIConfig::setVMDir (QString dir)
{  
  qDebug() << "_conf_ vmDir " << dir; 
  vmDir = dir; 
  write(); 
}

void CVMIConfig::setHypervisorDir (QString dir)
{ 
  qDebug() << "_conf_ hypervisorDir " << dir; 
  hypervisorDir = dir; 
  write(); 
}

void CVMIConfig::setShowDevReleases (QString show)
{
  qDebug() << "_conf_ showDevReleases " << show;
  showDevReleases = show;
  write();
}

