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


struct buildParameter {
	int thresholdLow;
	int thresholdHigh;
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
	void anatomyView();
	void exportSTL();
	void threeDimView();
	void threeDimViewTrue();
	int readStateCheck();
	void aboutThis();
	

private:
    Ui::MainWindow *ui;
	//vtkSmartPointer<vtkDICOMImageReader> reader = vtkSmartPointer<vtkDICOMImageReader>::New();
	//vtkSmartPointer<vtkImageMarchingCubes> poly_cube = vtkSmartPointer<vtkImageMarchingCubes>::New();
	//vtkSmartPointer<vtkImageMarchingCubes> poly_repair = vtkSmartPointer<vtkImageMarchingCubes>::New();


	//using PixelType = short;
	//unsigned int Dimension = 3;
	using ImageType = itk::Image<short, 3U>;
	typedef itk::ImageToVTKImageFilter<ImageType> FilterType;
	FilterType::Pointer filter = FilterType::New();

	// readState: 0 means not read yet, 1 means read done 
	int readState = 0;

	buildParameter buldPara = { 0, 400 };

	int dim[3];
};




#endif // MAINWINDOW_H
