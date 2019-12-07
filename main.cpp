/*			
1)			
Переходим в папку: C:\Qt\Qt5.13.2\5.13.2\msvc2017_64			
именно msvc2017_64. Там все нужное лежит.			
2)			
свойства проекта. "С/С++" -> "Общие" -> "Дополнительные каталоги включаемый файлов"    туда указываем путь до include.			
	C:\Qt\Qt5.13.2\5.13.2\msvc2017_64\include		
3) "Компоновщик" -> "Общие"  -> "Дополнительные каталоги библиотек"   туда указываем папку lib			
	C:\Qt\Qt5.13.2\5.13.2\msvc2017_64\lib		
4) 			
 "Компоновщик" -> "Ввод" ->"Дополнительные зависимости"  включаем нужные lib (Qt5Widgetsd.lib;Qt5Cored.lib;)			
	если debug сборка - выбираем debug dll типо: Qt5Widgetsd.lib;Qt5Cored.lib;		
			
			
Использую:
Qt5Widgetsd.lib
Qt5Cored.lib
Qt5MultimediaWidgetsd.lib
Qt5Multimediad.lib
Qt5Guid.lib
*/
// ConsoleApplication1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <iostream>
#include "QtCore/qstring.h"
#include "QtCore/qdebug.h"

#include "QtWidgets/qapplication.h"
#include "QtWidgets/qmessagebox.h"//	#include <QMessageBox>

#include "QtMultimedia/qcamera.h" //#include <QCamera> 
#include "QtMultimedia/qcameraimagecapture.h" //#include <QCameraImageCapture>
#include "QtMultimedia/qcamerainfo.h"//#include <QCameraInfo>
#include "QtCore/qbuffer.h" //#include <QBuffer>

#include "QtMultimediaWidgets/qcameraviewfinder.h" //#include <QCameraViewfinder>

#include "QtGui/qimage.h"//#include <QImage>
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
	if (0==cameras.size()) {
		QMessageBox msgBox;
		msgBox.setText("Not found camera");
		msgBox.exec();
		return 1;
	}

	QCamera *cam = new QCamera;

	cam->setCaptureMode(QCamera::CaptureStillImage);

	QCameraViewfinder *viewfinder = new QCameraViewfinder;

	viewfinder->show();

	QCameraImageCapture *cap = new QCameraImageCapture(cam);
	cap->setCaptureDestination(QCameraImageCapture::CaptureToBuffer);

	cam->setViewfinder(viewfinder);

	
	QObject::connect(cap, &QCameraImageCapture::imageCaptured, [=](int id, QImage img) {
		int static counter = 0;
		//QByteArray buf;
		//QBuffer buffer(&buf);
		//buffer.open(QIODevice::WriteOnly);
		//img.save(&buffer, "PNG");
		QString filePath = "./img" + QString("").setNum(counter)+".png";
		if (img.save(filePath, "PNG")) {
			qDebug("good save");
		}
		++counter;
	});

	QObject::connect(cap, &QCameraImageCapture::readyForCaptureChanged, [=](bool state) {
		if (state == true) {
			cam->searchAndLock();
			cap->capture();
			cam->unlock();
		}
	});

	cam->start();
	
	qDebug() << "All end";
	return a.exec();
}
