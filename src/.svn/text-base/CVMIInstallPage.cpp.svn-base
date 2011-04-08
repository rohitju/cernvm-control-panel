/****************************************************************************
** Copyright (C) 2010 CERN 
** All rights reserved.
** Contact: CernVM project (cernvm-talk@cern.ch)
** Author: Artem Harutyunyan (Artem.Harutyunyan@cern.ch)
****************************************************************************/

#include "CVMIInstallPage.h"
#include "CVMIConfig.h"
#include "CVMIControlVBox.h"

CVMIInstallPage::CVMIInstallPage (QWidget* parent) : QWidget(parent) 
{
  installDir = dir.absolutePath() + dir.separator();
  // Label
  label = new QLabel;//(tr("Downloading CernVM image"));

  // Progress bar
  progressBar = new QProgressBar;  

  // Done button
  doneButton = new QPushButton(tr("Done"));
  doneButton->setEnabled(false);
  Q_ASSERT(connect(doneButton, SIGNAL(clicked()), this, SLOT(donePressed())));
 
  // Layout
  QGroupBox* progressGroupBox = new QGroupBox(tr("CernVM deployment status"));
  QGridLayout* progressLayout = new QGridLayout;
  
  progressLayout->addWidget(label, 0, 0, 1, 3, Qt::AlignTop);
  progressLayout->addWidget(progressBar, 1, 0, 1, 3, Qt::AlignTop);
  progressLayout->addWidget(doneButton, 2, 2);  

  progressGroupBox->setLayout(progressLayout);

  QGridLayout* mainLayout = new QGridLayout;
  mainLayout->addWidget(progressGroupBox, 0, 0, 3, 0, Qt::AlignTop);
  //mainLayout->addWidget(doneButton, 1, 2, Qt::AlignTop);

  setLayout(mainLayout);
}

CVMIExitStatus CVMIInstallPage::checkConfig()
{
  // Check that image storage and VM deployment directoried are writeable
  CVMIConfig conf;
  CVMIExitStatus status;
  QDir dir;

  confImageStorageDir = conf.getImageDir() + dir.separator();
  confWorkDir = conf.getVMDir() + dir.separator(); 
  
  dir.mkpath(confImageStorageDir);
  QFile imageFile(confImageStorageDir + "img");
  if (imageFile.open(QIODevice::WriteOnly))
  {
    imageFile.remove();
    imageFile.close();
  }
  else 
  {
    imageFile.close();  
    status.isOK = false;
    status.msg = tr("Image download directory ") + confImageStorageDir + tr(" is not writeable. Please check CernVM Installer preferences.");
    qDebug() << "_checkConfig_ Could not write to the image store directory";
    return status; 
  }
  
  dir.mkpath(confWorkDir);
  QFile workDirFile(confWorkDir + "hdd");
  if (workDirFile.open(QIODevice::WriteOnly))
  {
    workDirFile.remove();
    workDirFile.close();
  }
  else
  {
    workDirFile.close();
    status.isOK = false;
    status.msg = tr("VM deployment directory ") + confWorkDir + tr(" is not writeable. Please check CernVM Installer preferences.");
    qDebug() << "_checkConfig_ Could not write to the VM deployment directory";
    return status;
  } 

  // Check that hypervisor is installed
  CVMIControlVM* vm = new CVMIControlVBox();

  if (vm->checkHypervisor() != true)
  {
    status.isOK = false;
    status.msg = tr("Hypervisor installtion can not be located. Please check CernVM Installer preferences.");
    qDebug() << "_checkConfig_ Could not find hypervisor";
    return status;
  }

  status.isOK = true;
  qDebug() << "_checkConfig_ OK"; 
  return status;
} 

void CVMIInstallPage::start(CVMIRelease vmRelease, CVMIInstanceConfig vmInstance)
{
  currentRelease = vmRelease;
  currentInstanceConfig = vmInstance;
  label->setText(tr("Downloading ") + vmRelease.name);
  startDownload();
}

void CVMIInstallPage::startDownload()
{
  imageURL = currentRelease.url;
  QString edition = currentRelease.edition;

  QFileInfo fileInfo(imageURL.path());
  QString   fileName = fileInfo.fileName();
  QString   fullPath;

  CVMIConfig conf; 

  confImageStorageDir = conf.getImageDir() + dir.separator();

  fullPath = confImageStorageDir + edition + fileName;

  if (QFile::exists(fullPath))
  {
    if (QMessageBox::question (0, tr("HTTP"),
                                  tr("There already exists a file called %1. Overwrite?").arg(fullPath),
                                  QMessageBox::Yes|QMessageBox::No, QMessageBox::No)
        == QMessageBox::No)
    {

      #ifdef Q_OS_WIN32
        unzip();
      #else
        untar();
      #endif
      return;
    }
  }

  dir.mkpath (confImageStorageDir);  
  compressedImage = new QFile (fullPath);
 
  if (!compressedImage->open (QIODevice::WriteOnly))
  {
    QMessageBox::critical(0, tr("HTTP"),
                             tr("Unable to save the file %1: %2.")
                             .arg(fileName).arg(compressedImage->errorString()));  

    delete compressedImage;
    compressedImage = 0;
    //emit done();
    doneButton->setEnabled(true);
    return;
  }
  
  httpStart (); 
}

void CVMIInstallPage::httpFinished()
{
  compressedImage->flush();
  compressedImage->close();

  QVariant redirectionTarget = httpReply->attribute(QNetworkRequest::RedirectionTargetAttribute);
  
  if (httpReply->error() != QNetworkReply::NoError)
  {
    compressedImage->remove();
    QMessageBox::critical(0, tr("HTTP"), tr("Download failed: %1.").arg(httpReply->errorString()));
    //emit done();
    doneButton->setEnabled(true);
    //downloadInProgress = false;
  }
  else if (!redirectionTarget.isNull())
  {
      QUrl redirectUrl = imageURL.resolved(redirectionTarget.toUrl());
      imageURL = redirectUrl;
      httpReply->deleteLater();
      
      // Rename the file after redirect
      QFileInfo fileInfo(imageURL.path());
      QString fileName = fileInfo.fileName();
      QString edition = currentRelease.edition;
      compressedImage->remove(); 
      delete compressedImage;
   
      QString fullPath = confImageStorageDir + edition + fileName;
      dir.mkpath (confImageStorageDir);      

      qDebug() << "_http_ Finished. Redirect full path: " << fullPath << " " << fileName;

      if (QFile::exists(fullPath))
      { 
        if (QMessageBox::question(0, tr("HTTP"),
                                     tr("There already exisis a file called %1. Overwrite?").arg(fullPath),
                                     QMessageBox::Yes|QMessageBox::No, QMessageBox::No)
            == QMessageBox::No)
            {
              #ifdef Q_OS_WIN32
                unzip();
              #else
                untar();
              #endif
              return;
            }
      }
        
      compressedImage = new QFile (fullPath);

      if (!compressedImage->open(QIODevice::WriteOnly))
      {
        QMessageBox::critical(0, tr("HTTP"), tr("Unable to save the file %1: %2.")
                                 .arg(fileName).arg(compressedImage->errorString()));

        delete compressedImage;
        compressedImage = 0;
        //emit done();
        doneButton->setEnabled(true);
        return;
      } 

      if (!compressedImage->isOpen())
        compressedImage->open(QIODevice::WriteOnly);
      
      compressedImage->resize(0);

      httpStart();
      return;
  } 

  else 
  { 
      qDebug() << "_http_ Finished";
      #ifdef Q_OS_WIN32
        unzip();
      #else
        untar();
      #endif
  }

  httpReply->deleteLater();
  httpReply = 0;
  delete compressedImage;
  compressedImage = 0;
}

void CVMIInstallPage::httpStart()
{  
  httpReply = qnam.get(QNetworkRequest(imageURL));

  Q_ASSERT(connect (httpReply, SIGNAL(finished()), this, SLOT(httpFinished())));
  Q_ASSERT(connect (httpReply, SIGNAL(readyRead()), this, SLOT(httpReadyRead())));
  Q_ASSERT(connect (httpReply, SIGNAL(downloadProgress(qint64,qint64)), 
           this, SLOT (httpUpdateProgress(qint64, qint64))));

  qDebug() << "_http_ Start " <<  imageURL;
}

void CVMIInstallPage::httpReadyRead() 
{
  if (compressedImage)
    compressedImage->write(httpReply->readAll());
};

void CVMIInstallPage::httpUpdateProgress (qint64 bytesRead, qint64 bytesTotal)
{
  progressBar->setMaximum(100);
  progressBar->setMinimum(0);
  progressBar->setValue(100*bytesRead/bytesTotal);
}

void CVMIInstallPage::setImagePaths()
{
  qDebug() << "_imagePaths_ ()";  

  QString id = QUuid::createUuid().toString();

  // Remove curly braces 
  id.replace (0,1,"");
  id.replace (id.length() - 1, 1, "");

  QFileInfo fileInfo(imageURL.path());
  QString   fileName = fileInfo.fileName();
  QString   edition = currentRelease.edition;
  imagePath = confImageStorageDir + edition + fileName;

  CVMIConfig conf;
  confWorkDir = conf.getVMDir() + dir.separator(); 

  tmpUncompressedImageDir = confWorkDir + "/" + id + "/";
  tmpUncompressedImage = tmpUncompressedImageDir + fileName + ".uncompressed";

  dir.mkpath (tmpUncompressedImageDir);
}

void CVMIInstallPage::unzip()
{
  qDebug() << "_unzip_ ()";
  setImagePaths();

  Q_ASSERT(connect(&process, SIGNAL(finished(int, QProcess::ExitStatus)),
                   this, SLOT(unzipDone(int, QProcess::ExitStatus))));

  process.setStandardOutputFile (tmpUncompressedImage) ;
  qDebug() << "_unzip_ Image path: " << imagePath;

  #ifdef Q_OS_WIN32
    QDir dir; 
    process.start(installDir + "gzip.exe", QStringList() << "-d" << "--to-stdout" <<  imagePath);
  #else
    process.start("gzip", QStringList() << "-d" << "--to-stdout" <<  imagePath);
  #endif
  label->setText(tr("Uncompressing ") + currentRelease.name);
  progressBar->setMaximum(0);
  progressBar->setMinimum(0);
}

void CVMIInstallPage::unzipDone(int exitCode, QProcess::ExitStatus exitStatus)
{
  qDebug() << "_unzip_ Done";

  if (exitCode == 0 && exitStatus == QProcess::NormalExit)
  {
    process.disconnect();
    
    untar();    
  }
  else
  {  
    QFileInfo fileInfo(imageURL.path());
    QString   fileName = fileInfo.fileName();
    QMessageBox::warning (0, tr("Failed to unzip"),
                             tr("Problem occured while unzipping %1. Please download the file again").arg(fileName));
    qDebug() << "_unzip_ Error. Exit code: " << exitCode;
    setImagePaths();
    qDebug() << "_unzip_ Removing corrupted file " << imagePath;
    dir.remove(imagePath); 
    process.disconnect();
    cleanup(tmpUncompressedImageDir);
    //emit done();
    doneButton->setEnabled(true);
  }
}

void CVMIInstallPage::untar()
{
  qDebug() << "_untar_ ()";  
 
  #ifndef Q_OS_WIN32
  setImagePaths();
  #endif 

  Q_ASSERT(connect(&process, SIGNAL(finished(int, QProcess::ExitStatus)),
                   this, SLOT(untarDone(int, QProcess::ExitStatus))));

  process.setStandardOutputFile (tmpUncompressedImageDir + "/tar.out") ;
  #ifdef Q_OS_WIN32
    QDir dir;
    tmpUncompressedImageDir.replace("\\","\\\\");
    tmpUncompressedImageDir.replace("/","\\\\");

    tmpUncompressedImage.replace("\\", "\\\\");
    tmpUncompressedImage.replace("/", "\\\\");

    dir.setCurrent (tmpUncompressedImageDir);
 
    process.start(installDir + "tar.exe", QStringList() << "-C" << "." << "-xf" << tmpUncompressedImage);
    qDebug() << "_untar_ Start. Args: "                 << "-C" << "." << "-xf" << tmpUncompressedImage;
  #else
    process.start("tar", QStringList() << "-C" << tmpUncompressedImageDir << "-zxf" << imagePath);
    qDebug() <<  "_untar_ Start. Args: " << "-C " << tmpUncompressedImageDir << " -zxf" << imagePath;
  #endif
  label->setText(tr("Uncompressing ") + currentRelease.name);
  progressBar->setMaximum(0);
  progressBar->setMinimum(0);
}

void CVMIInstallPage::untarDone(int exitCode, QProcess::ExitStatus exitStatus)
{ 
  qDebug() << "_untar_ Done";

  if (exitCode == 0 && exitStatus == QProcess::NormalExit)
  {
    Q_ASSERT (process.disconnect());
    dir.remove(tmpUncompressedImage);
    label->setText("Configuring CernVM instance");

    currentInstanceConfig.name.append ("-" + QDateTime::currentDateTime().toString("yyyyMMddhhmmss"));
    CVMIControlVM* vm = new CVMIControlVBox(&tmpUncompressedImageDir, currentInstanceConfig);
    Q_ASSERT(connect(vm, SIGNAL(vmReady()), this, SLOT(vmReady())));
    Q_ASSERT(connect(vm, SIGNAL(error(QString)), this, SLOT(vmError(QString)))); 
    vm->createVM();
  }
  else
  {  
    QFileInfo fileInfo(imageURL.path());
    QString   fileName = fileInfo.fileName();
    QMessageBox::warning (0, tr("Failed to untar"),
                             tr("Problem occured while unzipping %1. Please download the file again").arg(fileName));
    qDebug() << "_untar_ Error: Exit code: " << exitCode;
    setImagePaths();
    qDebug() << "_untar_ Removing corrupted file " << imagePath;
    dir.remove(imagePath);
    Q_ASSERT (process.disconnect());
    cleanup (tmpUncompressedImageDir);
    //emit done();
    doneButton->setEnabled(true);
  }
}

void CVMIInstallPage::vmError(QString msg)
{
  label->setText("A problem occured during creation of " + currentRelease.name);
  progressBar->setMinimum(0);
  progressBar->setMaximum(10);
  progressBar->setValue(10);
  doneButton->setEnabled(true);
  QMessageBox::critical(0, "VM creation error","Error: " + msg);
}

void CVMIInstallPage::vmReady()
{
  label->setText(currentRelease.name + tr(" was sucessfully created"));
  progressBar->setMinimum(0);
  progressBar->setMaximum(10);
  progressBar->setValue(10);
  doneButton->setEnabled(true);
}

void CVMIInstallPage::donePressed()
{
  doneButton->setEnabled(false);
  emit done();
}

bool CVMIInstallPage::cleanup(QString d)
{
  QDir tmpDir (d);
  bool result = true;

  qDebug() << "_cleanup_ " << d;
  
  if (tmpDir.exists(d))
  {
    Q_FOREACH(QFileInfo info, tmpDir.entryInfoList (QDir::NoDotAndDotDot | QDir::Files | QDir::AllDirs, QDir::DirsFirst) ) 
    {
      qDebug() << "_cleanup_ Absolute path: " << info.absoluteFilePath();

      if (info.isDir()) 
        result = cleanup(info.absoluteFilePath());
      else
        result = QFile::remove(info.absoluteFilePath());
    }
    
    if (result != true)
      return result;

    result = tmpDir.rmdir(d);
  }

  return result;
}


