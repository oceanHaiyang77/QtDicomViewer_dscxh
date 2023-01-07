#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "itkGDCMImageIO.h"
#include "itkGDCMSeriesFileNames.h"
#include "itkImageSeriesReader.h"
#include "itkImageSeriesWriter.h"
#include "itkImageToVTKImageFilter.h"
#include <vtkSmartPointer.h>
#include <vtkDICOMImageReader.h>
#include <vtkImageMarchingCubes.h>
#include <vtkPolyData.h>
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

//结构体包含重建参数
struct buildParameter {
	int thresholdLow;
	int thresholdHigh;
	float relaxFactor;
};



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
	
private slots:
	void closeThis();
	void input();
	void exportInf();
	void sliceView();
	void exportSTL();
	void threeDimView();
	int readStateCheck();
	void aboutThis();
	void setPara();
	void test();
	

private:
    Ui::MainWindow *ui;

	using ImageType = itk::Image<short, 3U>;
	typedef itk::ImageToVTKImageFilter<ImageType> FilterType;
	FilterType::Pointer filter = FilterType::New();

	// readState: 0 means not read yet, 1 means read done 
	int readState = 0;

	buildParameter buildPara = { 0, 400, 0.05 };

	// 信息表
	QHash<QString, QString> infoHash;

};




#endif
