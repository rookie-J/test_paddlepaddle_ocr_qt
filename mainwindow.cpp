#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QImage>
#include <QFileDialog>
#include <QDebug>

#include <string>
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->splitter->widget(0)->resize(512,512);
    m_label=new QLabel(ui->widget);

    PaddleOCR::OCRConfig config("./config.txt");
    qDebug() << "GPU :" << config.use_gpu;
    m_det=new PaddleOCR::DBDetector(config.det_model_dir, config.use_gpu, config.gpu_id,
                     config.gpu_mem, config.cpu_math_library_num_threads,
                     config.use_mkldnn, config.max_side_len, config.det_db_thresh,
                     config.det_db_box_thresh, config.det_db_unclip_ratio,
                     config.visualize, config.use_tensorrt, config.use_fp16);

    m_rec=new PaddleOCR::CRNNRecognizer(config.rec_model_dir, config.use_gpu, config.gpu_id,
                       config.gpu_mem, config.cpu_math_library_num_threads,
                       config.use_mkldnn, config.char_list_file,
                       config.use_tensorrt, config.use_fp16);

    ui->checkBox_imgvis->setChecked(true);
    QObject::connect(ui->pushButton_setImage,&QPushButton::clicked,this,&MainWindow::SetImage);
    QObject::connect(ui->checkBox_imgvis,&QCheckBox::stateChanged,[this](){
        m_det->SetVisualize(ui->checkBox_imgvis->isChecked());
    });
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_det;
    delete m_rec;
}

void MainWindow::SetImage()
{
    ui->textBrowser->clear();
    m_imgPath=QFileDialog::getOpenFileName(this,"Choose Image","","Image File(*.png *.jpg *.bmp *.jpeg)");
    if(m_imgPath.size()==0)
    {
        return;
    }
    QTime qtime;
    qtime.start();
    Mat srcimg = imread(m_imgPath.toLocal8Bit().constData(), IMREAD_COLOR);
    qDebug() << "time: " << qtime.elapsed() << "ms\n";
    std::vector<std::vector<std::vector<int>>> boxes;
    cv::Mat img_vis=m_det->Run(srcimg, boxes);
    qDebug() << "time: " << qtime.elapsed() << "ms\n";
    std::vector<std::string> res=m_rec->Run(boxes, srcimg, nullptr);
    qDebug() << "time: " << qtime.elapsed() << "ms\n";
    m_label->resize(ui->widget->size());
    if(ui->checkBox_imgvis->isChecked())
    {
        QImage img((const uchar*)(img_vis.data), img_vis.cols, img_vis.rows, img_vis.cols * img_vis.channels(), QImage::Format_RGB888);
        m_label->setPixmap(QPixmap::fromImage(img.scaled(m_label->size(),Qt::KeepAspectRatio)));
    }else{
        QImage img(m_imgPath);
        m_label->setPixmap(QPixmap::fromImage(img.scaled(m_label->size(),Qt::KeepAspectRatio)));
    }

    m_label->show();
    for(auto s:res)
    {
        ui->textBrowser->append(s.data());
    }
}
