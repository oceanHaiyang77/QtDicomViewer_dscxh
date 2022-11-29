#include "mainwindow.h"
#include "ui_mainwindow.h"

#include<iostream>
#include<string.h>

#include <QFileDialog>
#include <QDir>
#include <QString.h>
#include <QMessageBox>


// vtk header files needed
#include<vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingFreeType);
#define vtkRenderingCore_AUTOINIT 3(vtkInteractionStyle,vtkRenderingFreeType,vtkRenderingOpenGL2)
#define vtkRenderingVolume_AUTOINIT 1(vtkRenderingVolumeOpenGL2)
#include "vtkMarchingCubes.h"
#include "vtkStripper.h"
#include "vtkPolyDataMapper.h"
#include "vtkProperty.h"
#include "vtkCamera.h"
#include "vtkBoxWidget.h"
#include "vtkSmartPointer.h" 
#include "vtkTriangleFilter.h"
#include "vtkMassProperties.h"
#include "vtkSmoothPolyDataFilter.h"
#include "vtkPolyDataNormals.h"
#include "vtkContourFilter.h"
#include "vtkRecursiveDividingCubes.h"
#include "vtkSTLWriter.h"
#include <vtkInteractorStyleImage.h>
#include <vtkActor2D.h>
#include <vtkTextProperty.h>
#include <vtkTextMapper.h>
#include <sstream>
#include <vtkSmartPointer.h>
#include <vtkObjectFactory.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkDataObjectToTable.h>
#include <vtkElevationFilter.h>
#include "vtkGenericOpenGLRenderWindow.h"
#include <vtkPolyDataMapper.h>
#include <vtkQtTableView.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkSphereSource.h>
#include <vtkCubeSource.h>
#include <vtkCylinderSource.h>
#include <vtkImageData.h>
#include <vtkDICOMImageReader.h>
#include <vtkImageGaussianSmooth.h>
#include <vtkImageMarchingCubes.h>
#include <vtkSTLWriter.h>
#include <vtkActor.h>
#include <vtkImageViewer2.h>
#include "vtkCommand.h"
#include "vtkSliderWidget.h"
#include "vtkSliderRepresentation2D.h"
#include "vtkProperty.h"
#include <vtkFillHolesFilter.h>
#include <vtkPolyDataNormals.h>
#include <vtkConnectivityFilter.h>
#include <vtkDataSetMapper.h>
#include <vtkProperty.h>
#include <vtkUnstructuredGrid.h>
#include <vtkPolyData.h>
#include <vtkPointData.h>
#include <vtkCellData.h>
#include <vtkCellIterator.h>
#include <vtkGenericCell.h>
#include <vtkNamedColors.h>
#include <vtkPolyDataConnectivityFilter.h>
#include <vtkQuadricDecimation.h>
#include <vtkDecimatePro.h>
#include <vtkMarchingCubes.h>
#include <vtkStripper.h>
#include <vtkSmoothPolyDataFilter.h>
#include <vtkNew.h>



// itk header files needed
#include "itkGDCMImageIO.h"
#include "itkGDCMSeriesFileNames.h"
#include "itkImageSeriesReader.h"
#include "itkImageSeriesWriter.h"
#include "itkImageToVTKImageFilter.h"








// 切片状态信息
class StatusMessage {
public:
   static std::string Format(int slice, int maxSlice) {
      std::stringstream tmp;
      tmp << " Slice Number " << slice + 1 << "/" << maxSlice + 1;
      return tmp.str();
   }
};

// 切片交互方式
class myVtkInteractorStyleImage : public vtkInteractorStyleImage
{
public:
   static myVtkInteractorStyleImage* New();
   vtkTypeMacro(myVtkInteractorStyleImage, vtkInteractorStyleImage);

protected:
   vtkImageViewer2* _ImageViewer;
   vtkTextMapper* _StatusMapper;
   int _Slice;
   int _MinSlice;
   int _MaxSlice;

public:
   void SetImageViewer(vtkImageViewer2* imageViewer) {
      _ImageViewer = imageViewer;
      _MinSlice = imageViewer->GetSliceMin();
      _MaxSlice = imageViewer->GetSliceMax();
      _Slice = _MaxSlice/2;
      cout << "Slicer: Min = " << _MinSlice << ", Max = " << _MaxSlice << std::endl;
   }

   void SetStatusMapper(vtkTextMapper* statusMapper) {
      _StatusMapper = statusMapper;
   }


protected:
   void MoveSliceForward() {
      if(_Slice < _MaxSlice) {
         _Slice += 1;
         cout << "MoveSliceForward::Slice = " << _Slice << std::endl;
         _ImageViewer->SetSlice(_Slice);
         std::string msg = StatusMessage::Format(_Slice, _MaxSlice);
         _StatusMapper->SetInput(msg.c_str());
         _ImageViewer->Render();
      }
   }

   void MoveSliceBackward() {
      if(_Slice > _MinSlice) {
         _Slice -= 1;
         cout << "MoveSliceBackward::Slice = " << _Slice << std::endl;
         _ImageViewer->SetSlice(_Slice);
         std::string msg = StatusMessage::Format(_Slice, _MaxSlice);
         _StatusMapper->SetInput(msg.c_str());
         _ImageViewer->Render();
      }
   }


   virtual void OnKeyDown() {
      std::string key = this->GetInteractor()->GetKeySym();
      if(key.compare("Up") == 0) {
         //cout << "Up arrow key was pressed." << endl;
         MoveSliceForward();
      }
      else if(key.compare("Down") == 0) {
         //cout << "Down arrow key was pressed." << endl;
         MoveSliceBackward();
      }
      // forward event
      vtkInteractorStyleImage::OnKeyDown();
   }


   virtual void OnMouseWheelForward() {
      //std::cout << "Scrolled mouse wheel forward." << std::endl;
      MoveSliceForward();
      // don't forward events, otherwise the image will be zoomed 
      // in case another interactorstyle is used (e.g. trackballstyle, ...)
      // vtkInteractorStyleImage::OnMouseWheelForward();
   }


   virtual void OnMouseWheelBackward() {
      //std::cout << "Scrolled mouse wheel backward." << std::endl;
      if(_Slice > _MinSlice) {
         MoveSliceBackward();
      }
      // don't forward events, otherwise the image will be zoomed 
      // in case another interactorstyle is used (e.g. trackballstyle, ...)
      // vtkInteractorStyleImage::OnMouseWheelBackward();
   }
};

// 等待消息窗口
class MyMessageBox : public QMessageBox {
protected:
	void showEvent(QShowEvent* event) {
		QMessageBox::showEvent(event);

		QWidget* textField = findChild<QWidget*>("qt_msgbox_label");

		if (textField != NULL)

		{

			textField->setMinimumSize(240, 20);
		}

		QMessageBox::showEvent(event);
	}
};


vtkStandardNewMacro(myVtkInteractorStyleImage);

// 使用滑块选择切片

//class sliceSliderCallback:: public vtkCommand
//{
//public
//	static sliceSliderCallback *New()
//	{
//		return new sliceSliderCallback;
//	}
//	void Execute(vtkObject *caller, unsigned long, void *)
//	{
//		vtkSliderWidget *sliderWidget = reinterpret_cast<vtkSliderWidget*>(caller);
//		this->sliderer->SetsliSlice(value);
//	}
//	sliceSliderCallback():sliderer(nullptr) {}	
//}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

	vtkNew<vtkGenericOpenGLRenderWindow> renderWindowLeft;
	this->ui->qvtkWidgetLeft->SetRenderWindow(renderWindowLeft);
	//vtkNew<vtkGenericOpenGLRenderWindow> renderWindowRight;
	//this->ui->qvtkWidgetRight->SetRenderWindow(renderWindowRight);
	//vtkNew<vtkGenericOpenGLRenderWindow> renderWindowDLeft;
	//this->ui->qvtkWidgetDLeft->SetRenderWindow(renderWindowDLeft);

	
	connect(this->ui->action_close,SIGNAL(triggered()), this, SLOT(closeThis()));
	
	connect(this->ui->action_input, SIGNAL(triggered()), this, SLOT(input()));
	
	connect(this->ui->action_exportInf, SIGNAL(triggered()),this, SLOT(exportInf()));
	
	connect(this->ui->action_anatomyView, SIGNAL(triggered()),this, SLOT(anatomyView()));
	
	//connect(this->ui->action_exportSTL, SIGNAL(triggered()),this, SLOT(exportSTL()));
	
	//connect(this->ui->MarchingCubes, SIGNAL(triggered()), this, SLOT(modelMaker_marchingCubes()));
	
	//connect(this->ui->DividingCubes, SIGNAL(triggered()),this, SLOT(model_autoRepair()));

	connect(this->ui->action_3dView, SIGNAL(triggered()), this, SLOT(threeDimViewTrue()));
	connect(this->ui->action_thisproject, SIGNAL(triggered()), this, SLOT(aboutThis()));
}


MainWindow::~MainWindow()
{
    delete ui;
}

// 关闭
void MainWindow::closeThis()
{
	this->close();
}

// 导入dcm
void MainWindow::input()
{
	ui->text_inf->clear();

	// set path (support chinese)
	QDir dir;
	QString file_path = QFileDialog::getExistingDirectory( this, "read file from:");
	if ( file_path.isEmpty() == true ) return;

	QByteArray ba = file_path.toLocal8Bit();
	const char *file_path_str = ba.data();
	
	QString file_path_str_q;
	file_path_str_q = file_path_str;
	ui->text_inf->append("read file from:\n" + file_path_str_q);

	// print wait msg
	MyMessageBox waitMsg;
	waitMsg.setWindowTitle(tr("please wait!"));
	waitMsg.show();


	
	// read dcm from 'file_path_str' using itk
	using PixelType = short;
	constexpr unsigned int Dimension = 3;
	using ImageType = itk::Image<PixelType, Dimension>;
	using ReaderType = itk::ImageSeriesReader<ImageType>;

	using ImageIOType = itk::GDCMImageIO;
	using NamesGeneratorType = itk::GDCMSeriesFileNames;

	auto gdcmIO = ImageIOType::New();
	auto namesGenerator = NamesGeneratorType::New();

	namesGenerator->SetInputDirectory(file_path_str);

	const ReaderType::FileNamesContainer& filenames =
		namesGenerator->GetInputFileNames();

	size_t numberOfFileNames = filenames.size();
	std::cout << numberOfFileNames << std::endl;
	//for (unsigned int fni = 0; fni < numberOfFileNames; ++fni)
	//{
	//    std::cout << "filename # " << fni << " = ";
	//    std::cout << filenames[fni] << std::endl;
	//}

	auto reader = ReaderType::New();

	reader->SetImageIO(gdcmIO);
	reader->SetFileNames(filenames);

	try
	{
		reader->Update();
	}
	catch (const itk::ExceptionObject& excp)
	{
		std::cerr << "Exception thrown while writing the image" << std::endl;
		std::cerr << excp << std::endl;
	}

	// convert itkFile to vtk (using itk filter)
	typedef itk::ImageToVTKImageFilter<ImageType> FilterType;
	FilterType::Pointer vtkfilter = FilterType::New();
	vtkfilter->SetInput(reader->GetOutput());
	try
	{
		vtkfilter->Update();
	}
	catch (itk::ExceptionObject& e)
	{
		std::cerr << "exception in file reader" << std::endl;
		std::cerr << e << std::endl;
	}

	//// flip axes y (itk and vtk are on the contrary)
	//vtkSmartPointer<vtkImageFlip> imageflip = vtkSmartPointer<vtkImageFlip>::New();
	//imageflip->SetInputData(filter->GetOutput());
	//imageflip->SetFilteredAxes(1);
	//imageflip->Update();

	this->filter = vtkfilter;
	this->readState = 1;

	waitMsg.close();

	// open slice view automatically
	//this->anatomyView();
}

// 打印信息
void MainWindow::exportInf()
{
	if (readStateCheck() == 0)
	{
		return ;
	}


	//vtkImageData* readerImg = nullptr;
	//readerImg = this->reader->GetOutput(); 
	//
	//// 确认读取到文件并输出文件信息
	//readerImg->GetDimensions(this->dim);
	//if ( dim[0] > 2 || dim[1] > 2 || dim[2] > 2 )
	//	{
	//		
	//		QString dim0 = QString::number(dim[0]);
	//		QString dim1 = QString::number(dim[1]);
	//		QString dim2 = QString::number(dim[2]);
	//		QString space = " ";
	//		ui->text_inf->append("Dimensions:" + space + dim0 + space + dim1 + space + dim2);
	//		
	//		QString fileExtensions = this->reader-> GetFileExtensions();
	//		ui->text_inf->append("fileExtensions: " + fileExtensions);
	//		
	//		QString descriptiveName = this->reader->GetDescriptiveName();
	//		ui->text_inf->append("descriptiveName: " + descriptiveName);
	//		
	//		double* pixelSpacing = this->reader->GetPixelSpacing();
	//		QString pixelS = QString::number(*pixelSpacing,10,5);
	//		ui->text_inf->append("pixelSpacing: " + pixelS);
	//		
	//		int width = this->reader->GetWidth();
	//		QString wid = QString::number(width);
	//		ui->text_inf->append("width: " + wid);
	//		
	//		int height = this->reader->GetHeight();
	//		QString heig = QString::number(height);
	//		ui->text_inf->append("height: " + heig);
	//		
	//		float* imagePositionPatient = this->reader->GetImagePositionPatient();
	//		QString imPP = QString::number(*imagePositionPatient,10,5);
	//		ui->text_inf->append("imagePositionPatient: " + imPP);
	//		
	//		float* imageOrientationPatient = this->reader->GetImageOrientationPatient();
	//		QString imOP = QString::number(*imageOrientationPatient,10,5);
	//		ui->text_inf->append("imageOrientationPatient: " + imOP);
	//		
	//		int bitsAllocated = this->reader->GetBitsAllocated();
	//		QString bitsA = QString::number(bitsAllocated);
	//		ui->text_inf->append("bitsAllocated: " + bitsA);
	//		
	//		int pixelRepresentation = this->reader->GetPixelRepresentation();
	//		QString pixelR = QString::number(pixelRepresentation);
	//		ui->text_inf->append("pixelRepresentation: " + pixelR);
	//		
	//		int numberOfComponents = this->reader->GetNumberOfComponents();
	//		QString numberO = QString::number(numberOfComponents);
	//		ui->text_inf->append("numberOfComponents: " + numberO);

	//		QString transferSyntaxUID = this->reader->GetTransferSyntaxUID();
	//		ui->text_inf->append("transferSyntaxUID: " + transferSyntaxUID);

	//		float rescaleSlope = this->reader->GetRescaleSlope();
	//		QString rescaleS = QString::number(rescaleSlope, 10, 5);
	//		ui->text_inf->append("rescaleSlope: " + rescaleS);
	//	 
	//		float rescaleOffset = this->reader->GetRescaleOffset();
	//		QString rescaleO = QString::number(rescaleOffset, 10, 5);
	//		ui->text_inf->append("rescaleOffset: " + rescaleO);

	//		QString patientName = this->reader->GetPatientName();
	//		ui->text_inf->append("patientName: " + patientName);

	//		QString studyUID = this->reader->GetStudyUID();
	//		ui->text_inf->append("studyUID: " + studyUID);

	//		QString studyID = this->reader->GetStudyID();
	//		ui->text_inf->append("studyID: " + studyID);
	//		
	//		float gantryAngle = this->reader->GetGantryAngle();
	//		QString gantryA = QString::number(gantryAngle, 10, 5);
	//		ui->text_inf->append("gantryAngle: " + gantryA);
	//	}	
}

// 横断面（矢状面、冠状面）视图
void MainWindow::anatomyView()
{
	// check read state
	if (readStateCheck() == 0)
	{
		return;
	}



	MyMessageBox waitMsg;
	waitMsg.setWindowTitle(tr("please wait!"));
	waitMsg.show();
	//用vtkImageViewer2获取数据各个方向的切片
	//横断面面视图
	vtkSmartPointer<vtkImageViewer2> imageViewerLeft =
	vtkSmartPointer<vtkImageViewer2>::New();
	imageViewerLeft->SetInputData(this->filter->GetOutput());
	imageViewerLeft->SetSliceOrientationToXY();	
	imageViewerLeft->SetSlice(dim[2]/2);
	imageViewerLeft->SetRenderWindow(ui->qvtkWidgetLeft->GetRenderWindow());
	////矢状面视图
	//vtkSmartPointer<vtkImageViewer2> imageViewerRight =
	//vtkSmartPointer<vtkImageViewer2>::New();
	//imageViewerRight->SetInputConnection(this->reader->GetOutputPort());
	//imageViewerRight->SetSliceOrientationToXZ();
	//imageViewerRight->SetSlice(dim[0]/2);
	//imageViewerRight->SetRenderWindow(ui->qvtkWidgetRight->GetRenderWindow());	
	////冠状面视图
	//vtkSmartPointer<vtkImageViewer2> imageViewerDLeft =
	//vtkSmartPointer<vtkImageViewer2>::New();
	//imageViewerDLeft->SetInputConnection(this->reader->GetOutputPort());
	//imageViewerDLeft->SetSlice(dim[1]/2);
	//imageViewerDLeft->SetSliceOrientationToYZ();
	//imageViewerDLeft->SetRenderWindow(ui->qvtkWidgetDLeft->GetRenderWindow());
	
	//横断面当前切片数
	vtkSmartPointer<vtkTextProperty> sliceTextProp1 = vtkSmartPointer<vtkTextProperty>::New();
	sliceTextProp1->SetFontFamilyToCourier();
	sliceTextProp1->SetFontSize(15);
	sliceTextProp1->SetVerticalJustificationToBottom();
	sliceTextProp1->SetJustificationToLeft();

	vtkSmartPointer<vtkTextMapper> sliceTextMapper1 = vtkSmartPointer<vtkTextMapper>::New();
	std::string msg1 = StatusMessage::Format(dim[2]/2, imageViewerLeft->GetSliceMax());
	sliceTextMapper1->SetInput(msg1.c_str());
	sliceTextMapper1->SetTextProperty(sliceTextProp1);

	vtkSmartPointer<vtkActor2D> sliceTextActor1 = vtkSmartPointer<vtkActor2D>::New();
	sliceTextActor1->SetMapper(sliceTextMapper1);
	sliceTextActor1->SetPosition(90, 10);
	
	////冠状面当前切片数
	//vtkSmartPointer<vtkTextProperty> sliceTextProp2 = vtkSmartPointer<vtkTextProperty>::New();
	//sliceTextProp2->SetFontFamilyToCourier();
	//sliceTextProp2->SetFontSize(15);
	//sliceTextProp2->SetVerticalJustificationToBottom();
	//sliceTextProp2->SetJustificationToLeft();

	//vtkSmartPointer<vtkTextMapper> sliceTextMapper2 = vtkSmartPointer<vtkTextMapper>::New();
	//std::string msg2 = StatusMessage::Format(dim[0]/2, imageViewerRight->GetSliceMax());
	//sliceTextMapper2->SetInput(msg2.c_str());
	//sliceTextMapper2->SetTextProperty(sliceTextProp2);

	//vtkSmartPointer<vtkActor2D> sliceTextActor2 = vtkSmartPointer<vtkActor2D>::New();
	//sliceTextActor2->SetMapper(sliceTextMapper2);
	//sliceTextActor2->SetPosition(90, 10);

	////矢状面当前切片数
	//vtkSmartPointer<vtkTextProperty> sliceTextProp3 = vtkSmartPointer<vtkTextProperty>::New();
	//sliceTextProp3->SetFontFamilyToCourier();
	//sliceTextProp3->SetFontSize(15);
	//sliceTextProp3->SetVerticalJustificationToBottom();
	//sliceTextProp3->SetJustificationToLeft();

	//vtkSmartPointer<vtkTextMapper> sliceTextMapper3 = vtkSmartPointer<vtkTextMapper>::New();
	//std::string msg3 = StatusMessage::Format(dim[1]/2, imageViewerDLeft->GetSliceMax());
	//sliceTextMapper3->SetInput(msg3.c_str());
	//sliceTextMapper3->SetTextProperty(sliceTextProp3);

	//vtkSmartPointer<vtkActor2D> sliceTextActor3 = vtkSmartPointer<vtkActor2D>::New();
	//sliceTextActor3->SetMapper(sliceTextMapper3);
	//sliceTextActor3->SetPosition(90, 10);
	
	//提示文本
	vtkSmartPointer<vtkTextProperty> usageTextProp = vtkSmartPointer<vtkTextProperty>::New();
	usageTextProp->SetFontFamilyToCourier();
	usageTextProp->SetFontSize(14);
	usageTextProp->SetVerticalJustificationToTop();
	usageTextProp->SetJustificationToLeft();

	vtkSmartPointer<vtkTextMapper> usageTextMapper = vtkSmartPointer<vtkTextMapper>::New();
	usageTextMapper->SetInput("- Slice with mouse wheel or Up/Down-Key\n- Zoom with pressed right mouse button\n  while dragging\n- adjust the window width with pressed\n  left mouse button while dragging");
	usageTextMapper->SetTextProperty(usageTextProp);

	vtkSmartPointer<vtkActor2D> usageTextActor = vtkSmartPointer<vtkActor2D>::New();
	usageTextActor->SetMapper(usageTextMapper);
	usageTextActor->GetPositionCoordinate()->SetCoordinateSystemToNormalizedDisplay();
	usageTextActor->GetPositionCoordinate()->SetValue( 0.05, 0.95);

	//切片方向文本
	vtkSmartPointer<vtkTextProperty> transverseTextProp = vtkSmartPointer<vtkTextProperty>::New();
	transverseTextProp->SetFontFamilyToCourier();
	transverseTextProp->SetFontSize(12);
	transverseTextProp->SetVerticalJustificationToBottom();
	transverseTextProp->SetJustificationToLeft();

	vtkSmartPointer<vtkTextMapper> transverseTextMapper = vtkSmartPointer<vtkTextMapper>::New();
	transverseTextMapper->SetInput("transverse");
	transverseTextMapper->SetTextProperty(transverseTextProp);
	transverseTextMapper->GetTextProperty()->SetColor(0.1, 0.1, 0.1);

	vtkSmartPointer<vtkActor2D> transverseTextActor = vtkSmartPointer<vtkActor2D>::New();
	transverseTextActor->SetMapper(transverseTextMapper);
	transverseTextActor->SetPosition(5, 10);
	
	//交互
	//窗口交互器
	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor1 =
	vtkSmartPointer<vtkRenderWindowInteractor>::New();
	//vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor2 =
	//vtkSmartPointer<vtkRenderWindowInteractor>::New();
	//vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor3 =
	//vtkSmartPointer<vtkRenderWindowInteractor>::New();
	//交互风格
	vtkSmartPointer<myVtkInteractorStyleImage> myInteractorStyle1 =
	vtkSmartPointer<myVtkInteractorStyleImage>::New();
	myInteractorStyle1->SetImageViewer(imageViewerLeft);
	myInteractorStyle1->SetStatusMapper(sliceTextMapper1);

	//vtkSmartPointer<myVtkInteractorStyleImage> myInteractorStyle2 =
	//vtkSmartPointer<myVtkInteractorStyleImage>::New();
	//myInteractorStyle2->SetImageViewer(imageViewerRight);
	//myInteractorStyle2->SetStatusMapper(sliceTextMapper2);
	
	//vtkSmartPointer<myVtkInteractorStyleImage> myInteractorStyle3 =
	//vtkSmartPointer<myVtkInteractorStyleImage>::New();
	//myInteractorStyle3->SetImageViewer(imageViewerDLeft);
	//myInteractorStyle3->SetStatusMapper(sliceTextMapper3);
	
	//将交互风格添加到窗口交互器
	imageViewerLeft->SetupInteractor(renderWindowInteractor1);
	renderWindowInteractor1->SetInteractorStyle(myInteractorStyle1);
	//imageViewerRight->SetupInteractor(renderWindowInteractor2);
	//renderWindowInteractor2->SetInteractorStyle(myInteractorStyle2);
	//imageViewerDLeft->SetupInteractor(renderWindowInteractor3);
	//renderWindowInteractor3->SetInteractorStyle(myInteractorStyle3);

	//把文本添加到绘制窗口
	imageViewerLeft->GetRenderer()->AddActor2D(sliceTextActor1);
	//imageViewerRight->GetRenderer()->AddActor2D(sliceTextActor2);
	//imageViewerDLeft->GetRenderer()->AddActor2D(sliceTextActor3);
	imageViewerLeft->GetRenderer()->AddActor2D(usageTextActor);
	imageViewerLeft->GetRenderer()->AddActor2D(transverseTextActor);
	


	waitMsg.close();

	//渲染
	imageViewerLeft->Render();
	//imageViewerRight->Render();
	//imageViewerDLeft->Render();
	renderWindowInteractor1->Start(); 
	//renderWindowInteractor2->Start();
	//renderWindowInteractor3->Start();
}

// 导出 STL 文件
void MainWindow::exportSTL()
{

	// check read state
	if (readStateCheck() == 0)
	{
		return;
	}



//	//导出stl文件用于3D打印
//	std::string filename = "out.stl";
//	vtkSTLWriter *stlWriter = vtkSTLWriter::New();
//	stlWriter->SetFileName(filename.c_str());
//	stlWriter->SetInputConnection(this->poly_repair->GetOutputPort());
//	stlWriter->Write();
}









 // 3D视图
void MainWindow::threeDimViewTrue()
{
	// check read state
	if (readStateCheck() == 0)
	{
		return;
	}

	// print wait msg
	MyMessageBox waitMsg;
	waitMsg.setWindowTitle(tr("please wait!"));
	waitMsg.show();

	std::cout << "3dView:" << std::endl;
	std::string filename = "out_Smooth.stl";//设置输出文件路径

	//MC算法
	//抽取等值面为骨头的信息
	vtkSmartPointer< vtkMarchingCubes > boneExtractor =
		vtkSmartPointer< vtkMarchingCubes >::New();
	boneExtractor->SetInputData(this->filter->GetOutput());
	boneExtractor->SetValue(this->buldPara.thresholdLow, this->buldPara.thresholdHigh);
	boneExtractor->Update();

	std::cout << "building 3D model..." << std::endl;
	//剔除旧的或废除的数据单元，提高绘制速度(可略去这一步)
	vtkSmartPointer< vtkStripper > boneStripper =
		vtkSmartPointer< vtkStripper >::New(); //三角带连接
	boneStripper->SetInputConnection(boneExtractor->GetOutputPort());
	boneStripper->Update();

	//平滑滤波
	vtkSmartPointer<vtkSmoothPolyDataFilter> pSmoothPolyDataFilter = vtkSmartPointer<vtkSmoothPolyDataFilter>::New();
	pSmoothPolyDataFilter->SetInputConnection(boneStripper->GetOutputPort());
	//pSmoothPolyDataFilter->SetNumberOfIterations(m_nNumberOfIterations);
	pSmoothPolyDataFilter->SetRelaxationFactor(0.05);

	vtkSmartPointer<vtkPolyDataNormals> pPolyDataNormals = vtkSmartPointer<vtkPolyDataNormals>::New();
	pPolyDataNormals->SetInputConnection(pSmoothPolyDataFilter->GetOutputPort());
	//pPolyDataNormals->SetFeatureAngle(m_nFeatureAngle);

	std::cout << "saving to .stl file..." << std::endl;
	//将模型输出到入STL文件
	vtkSmartPointer<vtkSTLWriter> stlWriter =
		vtkSmartPointer<vtkSTLWriter>::New();
	stlWriter->SetFileName(filename.c_str());
	stlWriter->SetInputConnection(pPolyDataNormals->GetOutputPort());
	stlWriter->Write();

	waitMsg.close();

	std::cout << "ploting..." << std::endl;
	//建立映射
	vtkSmartPointer< vtkPolyDataMapper > boneMapper =
		vtkSmartPointer< vtkPolyDataMapper >::New();
	boneMapper->SetInputData(pPolyDataNormals->GetOutput());
	boneMapper->ScalarVisibilityOff();
	//建立角色
	vtkSmartPointer< vtkActor > bone =
		vtkSmartPointer< vtkActor >::New();
	bone->SetMapper(boneMapper);

	bone->GetProperty()->SetDiffuseColor(1.0, 1.0, 1.0);
	bone->GetProperty()->SetSpecular(.3);
	bone->GetProperty()->SetSpecularPower(20);

	std::cout << "2..." << std::endl;
	//定义绘制器
	vtkSmartPointer< vtkRenderer > aRenderer =
		vtkSmartPointer< vtkRenderer >::New();
	//定义绘制窗口
	vtkSmartPointer< vtkRenderWindow > renWin =
		vtkSmartPointer< vtkRenderWindow >::New();
	renWin->AddRenderer(aRenderer);
	//定义窗口交互器
	vtkSmartPointer< vtkRenderWindowInteractor > iren =
		vtkSmartPointer< vtkRenderWindowInteractor >::New();
	iren->SetRenderWindow(renWin);

	std::cout << "3..." << std::endl;
	//创建一个camera
	vtkSmartPointer< vtkCamera > aCamera =
		vtkSmartPointer< vtkCamera >::New();
	aCamera->SetViewUp(0, 0, -1);
	aCamera->SetPosition(0, 1, 0);
	aCamera->SetFocalPoint(0, 0, 0);

	aRenderer->AddActor(bone);
	aRenderer->SetActiveCamera(aCamera);
	aRenderer->ResetCamera();
	aCamera->Dolly(1.5);
	aRenderer->SetBackground(0, 0, 0);
	aRenderer->ResetCameraClippingRange();

	std::cout << "4..." << std::endl;
	//将3D模型渲染到绘制窗口
	iren->Initialize();
	iren->Start();
	
}






void MainWindow::threeDimView()
{
	//vtkRenderer* ren = vtkRenderer::New();
	//ui->qvtkWidgetDLeft->GetRenderWindow()->AddRenderer(ren);
	//vtkSmartPointer<vtkRenderWindowInteractor> iren = ui->qvtkWidgetDLeft->GetRenderWindow()->GetInteractor();
	//iren->SetRenderWindow(ui->qvtkWidgetDLeft->GetRenderWindow());
	///*               导入数据，添加变量条等程序                */

	////刷新渲染
	//ui->qvtkWidgetDLeft->GetRenderWindow()->Render();
	////初始化交互器
	//iren->Initialize();
	////启动交互器
	//iren->Start();
	
}

// 检查文件读取状态
int MainWindow::readStateCheck()
{
	if (this->readState == 0)
	{
		MyMessageBox waitMsg;
		waitMsg.setWindowTitle(tr("warning!"));
		waitMsg.setText(tr("please input dcm file first."));
		waitMsg.exec();
		return 0;
	}
	else
	{
		return 1;
	}
}












// about 信息窗口
class AboutMessageBox : public QMessageBox {
protected:
	void showEvent(QShowEvent* event) {
		QMessageBox::showEvent(event);

		QWidget* textField = findChild<QWidget*>("qt_msgbox_label");

		if (textField != NULL)

		{

			textField->setMinimumSize(640, 480);
		}

		QMessageBox::showEvent(event);
	}
};

// 打印 about 信息窗口
void MainWindow::aboutThis()
{
	AboutMessageBox aboutMsg;
	aboutMsg.setWindowTitle(tr("about this project"));
	QString aboutText = 
		"项目源代码已上传 github, 链接：...\n重建算法";
	aboutMsg.setText(aboutText);
	aboutMsg.exec();


}