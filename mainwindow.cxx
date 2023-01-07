#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <string.h>
#include <string>

#include <QFileDialog>
#include <QDir>
#include <QString.h>
#include <QMessageBox>
#include <QDebug>
#include <QSpinBox>
#include <QDialogButtonBox>
#include <QLabel>
#include <QThread>

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
#include <vtkImageFlip.h>

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
         MoveSliceForward();
      }
      else if(key.compare("Down") == 0) {
         MoveSliceBackward();
      }
      vtkInteractorStyleImage::OnKeyDown();
   }


   virtual void OnMouseWheelForward() {
      MoveSliceForward();
   }


   virtual void OnMouseWheelBackward() {
      if(_Slice > _MinSlice) {
         MoveSliceBackward();
      }
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

// about 信息窗口
class AboutMessageBox : public QMessageBox {
protected:
	void showEvent(QShowEvent* event) {
		QMessageBox::showEvent(event);

		QWidget* textField = findChild<QWidget*>("qt_msgbox_label");

		if (textField != NULL)

		{
			textField->setMinimumSize(640, 240);
		}

		QMessageBox::showEvent(event);
	}
};

// 提醒信息窗口
class NoticeMessageBox : public QMessageBox {
protected:
	void showEvent(QShowEvent* event) {
		QMessageBox::showEvent(event);

		QWidget* textField = findChild<QWidget*>("qt_msgbox_label");

		if (textField != NULL)

		{
			textField->setMinimumSize(640, 20);
		}

		QMessageBox::showEvent(event);
	}
};

vtkStandardNewMacro(myVtkInteractorStyleImage);












MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
		
	connect(this->ui->action_close,SIGNAL(triggered()), this, SLOT(closeThis()));
	
	connect(this->ui->action_input, SIGNAL(triggered()), this, SLOT(input()));
	
	connect(this->ui->action_exportInf, SIGNAL(triggered()),this, SLOT(exportInf()));
	
	connect(this->ui->action_anatomyView, SIGNAL(triggered()),this, SLOT(sliceView()));
	
	connect(this->ui->action_exportStl, SIGNAL(triggered()),this, SLOT(exportSTL()));

	connect(this->ui->action_3dView, SIGNAL(triggered()), this, SLOT(threeDimView()));

	connect(this->ui->action_thisproject, SIGNAL(triggered()), this, SLOT(aboutThis()));

	connect(this->ui->action_buildparm, SIGNAL(triggered()), this, SLOT(setPara()));

	//connect(this->ui->action_test, SIGNAL(triggered()), this, SLOT(test()));
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

	reader->Update();
	//try
	//{
	//	reader->Update();
	//}
	//catch (const itk::ExceptionObject& excp)
	//{
	//	std::cerr << "Exception thrown while writing the image" << std::endl;
	//	std::cerr << excp << std::endl;
	//}

	// store tagInfo to hash
	using DictionaryType = itk::MetaDataDictionary;
	const DictionaryType& dictionary = gdcmIO->GetMetaDataDictionary();
	using MetaDataStringType = itk::MetaDataObject<std::string>;

	QHash<QString, QString> *dcm_tig = &(this->infoHash);
	for (auto ite = dictionary.Begin(); ite != dictionary.End(); ++ite) 
	{
		QString id = QString::fromStdString(ite->first);
		itk::MetaDataObjectBase::Pointer entry = ite->second;
		MetaDataStringType::ConstPointer entry_value =
			dynamic_cast<const MetaDataStringType*>(ite->second.GetPointer());
		std::string key_string;
		itk::GDCMImageIO::GetLabelFromTag(id.toStdString().c_str(), key_string);
		QString key = QString::fromStdString(key_string);
		QString value = "null";
		if (entry_value != nullptr)
		{
			value = QString::fromStdString(entry_value->GetMetaDataObjectValue());
		}
		else 
		{
			value = "null";
		}
		dcm_tig->insert(key, value);
	}

	// convert itkFile to vtk (using itk filter)
	typedef itk::ImageToVTKImageFilter<ImageType> FilterType;
	FilterType::Pointer vtkfilter = FilterType::New();
	vtkfilter->SetInput(reader->GetOutput());
	vtkfilter->Update();
	//try
	//{
	//	vtkfilter->Update();
	//}
	//catch (itk::ExceptionObject& e)
	//{
	//	std::cerr << "exception in file reader" << std::endl;
	//	std::cerr << e << std::endl;
	//}

	//// flip axes y (itk and vtk are on the contrary)
	//vtkSmartPointer<vtkImageFlip> imageflip = vtkSmartPointer<vtkImageFlip>::New();
	//imageflip->SetInputData(filter->GetOutput());
	//imageflip->SetFilteredAxes(1);
	//imageflip->Update();

	this->filter = vtkfilter;
	this->readState = 1;

	waitMsg.close();

	ui->text_inf->append("Successfully load dcm files!\n");

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

	QHash<QString, QString> ::const_iterator it;
	for (it = this->infoHash.begin(); it != this->infoHash.end(); it++)
	{
		QString keyMsg = it.key();
		keyMsg.insert(0, "<b>");
		keyMsg.append(":</b>");
		keyMsg.append(it.value());
		this->ui->text_inf->append(keyMsg);

		//std::cout << it.key().toStdString() << ":" << it.value().toStdString() << std::endl;
	}
}

// 切片视图
void MainWindow::sliceView()
{
	// check read state
	if (readStateCheck() == 0)
	{
		return;
	}

	MyMessageBox waitMsg;
	waitMsg.setWindowTitle(tr("please wait!"));
	waitMsg.show();
	//获取切片
	vtkSmartPointer<vtkImageViewer2> imageViewerLeft =
	vtkSmartPointer<vtkImageViewer2>::New();
	imageViewerLeft->SetInputData(this->filter->GetOutput());
	imageViewerLeft->SetSliceOrientationToXY();	
	imageViewerLeft->SetSlice(1);
	imageViewerLeft->SetRenderWindow(ui->qvtkWidgetLeft->GetRenderWindow());
	
	//横断面当前切片数
	vtkSmartPointer<vtkTextProperty> sliceTextProp1 = vtkSmartPointer<vtkTextProperty>::New();
	sliceTextProp1->SetFontFamilyToCourier();
	sliceTextProp1->SetFontSize(15);
	sliceTextProp1->SetVerticalJustificationToBottom();
	sliceTextProp1->SetJustificationToLeft();

	vtkSmartPointer<vtkTextMapper> sliceTextMapper1 = vtkSmartPointer<vtkTextMapper>::New();
	std::string msg1 = StatusMessage::Format(1, imageViewerLeft->GetSliceMax());
	sliceTextMapper1->SetInput(msg1.c_str());
	sliceTextMapper1->SetTextProperty(sliceTextProp1);

	vtkSmartPointer<vtkActor2D> sliceTextActor1 = vtkSmartPointer<vtkActor2D>::New();
	sliceTextActor1->SetMapper(sliceTextMapper1);
	sliceTextActor1->SetPosition(90, 10);
	
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
	
	//窗口交互器
	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor1 =
	vtkSmartPointer<vtkRenderWindowInteractor>::New();
	vtkSmartPointer<myVtkInteractorStyleImage> myInteractorStyle1 =
	vtkSmartPointer<myVtkInteractorStyleImage>::New();
	myInteractorStyle1->SetImageViewer(imageViewerLeft);
	myInteractorStyle1->SetStatusMapper(sliceTextMapper1);
	
	//将交互风格添加到窗口交互器
	imageViewerLeft->SetupInteractor(renderWindowInteractor1);
	renderWindowInteractor1->SetInteractorStyle(myInteractorStyle1);


	//把文本添加到绘制窗口
	imageViewerLeft->GetRenderer()->AddActor2D(sliceTextActor1);
	imageViewerLeft->GetRenderer()->AddActor2D(usageTextActor);
	imageViewerLeft->GetRenderer()->AddActor2D(transverseTextActor);
	

	waitMsg.close();

	//渲染
	imageViewerLeft->Render();
	renderWindowInteractor1->Start(); 

}

// 导出 STL 文件
void MainWindow::exportSTL()
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
	std::string filename = "out_smooth.stl";//设置输出文件路径


	//抽取等值面
	vtkSmartPointer< vtkMarchingCubes > boneExtractor =
		vtkSmartPointer< vtkMarchingCubes >::New();
	boneExtractor->SetInputData(this->filter->GetOutput());
	boneExtractor->SetValue(this->buildPara.thresholdLow, this->buildPara.thresholdHigh); //设置提取的等值信息
	boneExtractor->Update();

	vtkSmartPointer< vtkStripper > boneStripper =
		vtkSmartPointer< vtkStripper >::New();
	boneStripper->SetInputConnection(boneExtractor->GetOutputPort());
	boneStripper->Update();

	std::cout << "saving to .stl file..." << std::endl;
	//将模型输出到入STL文件
	vtkSmartPointer<vtkSTLWriter> stlWriter =
		vtkSmartPointer<vtkSTLWriter>::New();
	stlWriter->SetFileName(filename.c_str());
	stlWriter->SetInputData(boneStripper->GetOutput());
	stlWriter->Write();


	waitMsg.close();

	NoticeMessageBox aboutMsg;
	QString aboutText = "the STL file have been writen to 'out_smooth.stl'";
	aboutMsg.setText(aboutText);
	aboutMsg.exec();
	
}

// 3D视图
void MainWindow::threeDimView()
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
	
	//抽取等值面
	vtkSmartPointer< vtkMarchingCubes > boneExtractor =
		vtkSmartPointer< vtkMarchingCubes >::New();
	boneExtractor->SetInputData(this->filter->GetOutput());
	boneExtractor->SetValue(this->buildPara.thresholdLow, this->buildPara.thresholdHigh); //设置提取的等值信息
	boneExtractor->Update();

	//剔除旧的或废除的数据单元
	vtkSmartPointer< vtkStripper > boneStripper =
		vtkSmartPointer< vtkStripper >::New();
	boneStripper->SetInputConnection(boneExtractor->GetOutputPort());
	boneStripper->Update();

	//建立映射
	vtkSmartPointer< vtkPolyDataMapper > boneMapper =
		vtkSmartPointer< vtkPolyDataMapper >::New();
	boneMapper->SetInputData(boneStripper->GetOutput());
	boneMapper->ScalarVisibilityOff();

	//建立角色
	vtkSmartPointer< vtkActor > bone =
		vtkSmartPointer< vtkActor >::New();
	bone->SetMapper(boneMapper);

	bone->GetProperty()->SetDiffuseColor(1.0, 1.0, 1.0);
	bone->GetProperty()->SetSpecular(.3);
	bone->GetProperty()->SetSpecularPower(20);

	//定义绘制器
	vtkSmartPointer< vtkRenderer > aRenderer =
		vtkSmartPointer< vtkRenderer >::New();
	this->ui->qvtkWidgetLeft->GetRenderWindow()->AddRenderer(aRenderer);

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

	waitMsg.close();
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

// 打印 about 信息窗口
void MainWindow::aboutThis()
{
	AboutMessageBox aboutMsg;
	aboutMsg.setWindowTitle(tr("about this project"));
	std::string aboutText;
	aboutText += "source code has been upload to github,linking:\n";
	aboutText += "https://github.com/oceanHaiyang77/QtDicomViewer_dscxh \n";
	aboutText += "\nour method:vtkMarchingCubes(MC)";
	aboutMsg.setText(QString::fromStdString(aboutText));
	aboutMsg.exec();
}

// 设置重建参数
void MainWindow::setPara()
{
	QDialog dialog(this);
	QFormLayout form(&dialog);
	form.addRow(new QLabel("User input:"));
	// Value1
	QString value1 = QString("thresholdLow: ");
	QSpinBox *spinbox1 = new QSpinBox(&dialog);
	spinbox1->setRange(0, 999);
	spinbox1->setValue(this->buildPara.thresholdLow);
	form.addRow(value1, spinbox1);
	// Value2
	QString value2 = QString("thresholdHigh: ");
	QSpinBox* spinbox2 = new QSpinBox(&dialog);
	spinbox2->setRange(0, 999);
	spinbox2->setValue(this->buildPara.thresholdHigh);
	form.addRow(value2, spinbox2);
	// Add Cancel and OK button
	QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
		Qt::Horizontal, &dialog);
	form.addRow(&buttonBox);
	QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
	QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

	// Process when OK button is clicked
	if (dialog.exec() == QDialog::Accepted) 
	{
		//std::cout << "spinbox1:" << spinbox1->text().toInt() << std::endl;
		this->buildPara.thresholdLow = spinbox1->text().toInt();
		this->buildPara.thresholdHigh = spinbox2->text().toInt();
	}
}













void MainWindow::test()
{
	//QMessageBox msgBox;
	//msgBox.setText("please wait");
	//msgBox.show();
	//QThread::sleep(3);
	//msgBox.close();
	
	
	//QString msg = "message";
	//this->ui->text_inf->append(msg);
	//msg.insert(0, "<b>");
	//msg.append("</b>");
	//this->ui->text_inf->append(msg);

	//this->ui->qvtkWidgetLeft->;
}