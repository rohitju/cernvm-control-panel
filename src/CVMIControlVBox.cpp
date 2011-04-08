/****************************************************************************
** Copyright (C) 2010 CERN 
** All rights reserved.
** Contact: CernVM project (cernvm-talk@cern.ch)
** Author: Artem Harutyunyan (Artem.Harutyunyan@cern.ch)
****************************************************************************/

#include <QDebug>
#include <QDir>
#include <QDateTime>

#include "CVMIControlVBox.h"
#include "CVMIConfig.h"

CVMIControlVBox::CVMIControlVBox (QString* path, CVMIInstanceConfig vmConfig) : CVMIControlVM (path, vmConfig),
vmConfigData (vmConfig)
{
  Q_ASSERT(connect(&process, SIGNAL(finished(int, QProcess::ExitStatus)),
                   this, SLOT(processFinished(int, QProcess::ExitStatus))));

  initPaths();
  canFail = false;
}

CVMIControlVBox::CVMIControlVBox()
  { initPaths(); }

void CVMIControlVBox::initPaths()
{
  CVMIConfig conf;
  QString vboxInstallDir = conf.getHypervisorDir();
  QChar separator = QDir::separator();

  #ifdef Q_OS_WIN32
    vboxManageExecutable = vboxInstallDir + separator + "VBoxManage.exe";
    vboxGuestAdditionsIso = vboxInstallDir + separator + "VBoxGuestAdditions.iso";  
  #endif
  #ifdef Q_OS_LINUX
    vboxManageExecutable = vboxInstallDir + separator + "/bin/VBoxManage";
    vboxGuestAdditionsIso = vboxInstallDir + separator + "/share/virtualbox/VBoxGuestAdditions.iso";  
  #endif
  #ifdef Q_OS_MAC
    vboxManageExecutable = vboxInstallDir + separator + "/VirtualBox.app/Contents/MacOS/VBoxManage";
    vboxGuestAdditionsIso = vboxInstallDir + separator + "/VirtualBox.app/Contents/MacOS/VBoxGuestAdditions.iso";
  #endif
}

bool CVMIControlVBox::checkHypervisor()
{
  QDir dir;
  qDebug() << vboxManageExecutable;
  qDebug() << dir.exists(vboxManageExecutable);
  if (dir.exists(vboxManageExecutable))
    return true;
    
  return false;
}

void CVMIControlVBox::createVM()
{
  // Find .vmdk 
  QString* uncompressedImagePath = getImagePath();
  QString  vmOsType = "Linux26"; // we assume the image is 32 bit
  QString  vmName;

  qDebug() << "_VBox_ createvm The image is ready: " << *uncompressedImagePath << ""; 

  // Find image directory, extract the name and architectue
  QStringList filter;
  filter << "cernvm-*";
  QDir dir(*uncompressedImagePath);
  dir.setNameFilters (filter);

  Q_FOREACH(QFileInfo info, dir.entryInfoList (QDir::NoDotAndDotDot | QDir::Dirs ) ) 
  {
    vmName = info.absoluteFilePath();
    vmImageFullPath = vmName;
    //vmName.replace(QRegExp("\\\\+"), "/");
    vmName = QFileInfo(vmName).canonicalFilePath();
    vmName.replace(info.canonicalPath() /*+ dir.separator()*/, "");

    if (vmName.contains("x86_64"))
      vmOsType += "_64";  

    vmImageFullPath += dir.separator() + vmName + ".vmdk";   
  } 
  
  //QDateTime now = QDateTime::currentDateTime();
  executable = vboxManageExecutable;
  args.clear();
  step = 0;

  //vmName = vmName + "-" + now.toString("yyyyMMddhhmmss");  

  args << "createvm" 
       << "--name" <<  vmConfigData.name
       << "--ostype" << vmOsType
       << "--register";
  qDebug() << "_vbox_ createVM " << executable << args; 
  startProcess();
}
               
void CVMIControlVBox::startProcess() 
{
  qDebug() << executable << " " << args << " ";
  process.start(executable, args);
  process.waitForFinished(); 
}

void CVMIControlVBox::readyReadStdout()
{

}
               
void CVMIControlVBox::configureVM() 
{
  if (step == 1)
  {
    // Configure VM now
    args.clear();
    args << "modifyvm"       << vmConfigData.name
         << "--memory"       << vmConfigData.ramSize 
         << "--cpus"         << vmConfigData.cpuCount
         << "--vram"         << "32"
         << "--acpi"         << "on"
         << "--ioapic"       << "on"
         << "--boot1"        << "disk" << "--boot2" << "none" << "--boot3" << "none" << "--boot4" << "none"
         << "--nic1"         << "nat"
         << "--natdnsproxy1" << "on"
         << "--nic2"         << "hostonly" << "--hostonlyadapter2";
    
    //Network settings (OS specific) 
    //
    #ifdef Q_OS_WIN32 
      args << "VirtualBox Host-Only Ethernet Adapter"; 
    #else 
      args << "vboxnet0"; 
    #endif
    
    startProcess();
  }
  if (step == 2)
  {
    // Configure VM storage (phase 1)
    args.clear();
    args << "storagectl" << vmConfigData.name
         << "--name" << "\"IDE Controller\"" << "--add" << "ide" << "--controller" << "PIIX4";

    startProcess();
  }
  else if (step == 3)
  {
    // Configure VM storage (phase 2)
    args.clear();
    args << "openmedium" << "disk" << vmImageFullPath;

    canFail = true;
    startProcess();
  }
  else if (step == 4)
  {
    args.clear(); // Attach HDD to the VM
    args << "storageattach" << vmConfigData.name 
         << "--storagectl" << "\"IDE Controller\""
         << "--port" << "0"
         << "--device" << "0"
         << "--type" << "hdd"
         << "--medium" << vmImageFullPath;

    startProcess();
  }
  else if (step == 5)
  {
    args.clear();
    args << "openmedium" << "dvd" << vboxGuestAdditionsIso;

    canFail = true;
    startProcess();
  }
  else if (step == 6)
  {
     args.clear(); // Attach GuestAdditions to the VM
     args << "storageattach" << vmConfigData.name 
          << "--storagectl" << "\"IDE Controller\""
          << "--port" << "1"
          << "--device" << "0"
          << "--type" << "dvddrive"
          << "--medium" << vboxGuestAdditionsIso;
 
    startProcess();   
  }
  else if (step == 7)
  {
    args.clear();
    args << "startvm" << vmConfigData.name; 

    startProcess();
  }

}               
void CVMIControlVBox::processFinished(int exitCode, QProcess::ExitStatus exitStatus) 
{
  ++step;
  qDebug() << step << " finished " << "canFail" << canFail << "code" << exitCode << " " << executable << " " << args.join(" "); 
  if ((exitCode == 0 && exitStatus == QProcess::NormalExit ) || canFail == true) 
  {
    if (step < 8) 
    {
      process.close();
      canFail = false;
      configureVM();
      return;
    }
    emit vmReady();
  } 
  else
  {
    // Report an error here;
    process.close();  
    qDebug() << "_VBox_ processFinished() error";
    emit error (tr("Execution of ") + executable + " " + args.join(" ") + tr("failed.")); 
  }
}

