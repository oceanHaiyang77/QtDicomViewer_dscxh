#include "mainwindow.h"
#include <QApplication>

#include <QSurfaceFormat>
#include "QVTKOpenGLWidget.h"

int main(int argc, char *argv[])
{
	QSurfaceFormat::setDefaultFormat(QVTKOpenGLWidget::defaultFormat());
	
	// Qt window
    QApplication a(argc, argv);
    MainWindow w;
	w.setWindowTitle("DicomViewer_dscxh");
    w.show();

    return a.exec();
}