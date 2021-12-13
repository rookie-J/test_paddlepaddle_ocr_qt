#-------------------------------------------------
#
# Project created by QtCreator 2021-02-08T21:17:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = paddleOCRDemo
TEMPLATE = app
CONFIG += c++11
DESTDIR=$$OUT_PWD

DEPEN=$$PWD/inference

DES=$$DESTDIR/inference
QMAKE_PRE_LINK += $(COPY_DIR) $$shell_path($$DEPEN) $$shell_path($$DES) \
    && $(COPY) $$shell_path($$PWD/config.txt) $$shell_path($$OUT_PWD/config.txt) \
    && $(COPY) $$shell_path($$PWD/ppocr_keys_v1.txt) $$shell_path($$OUT_PWD/ppocr_keys_v1.txt)

PADDLE_PATH = /home/jixiaoxu/paddle_inference #use your own path of paddle_inference

INCLUDEPATH += $$PADDLE_PATH/paddle/include \
    $$PADDLE_PATH/third_party/install/cryptopp/include \
    $$PADDLE_PATH/third_party/install/glog/include \
    $$PADDLE_PATH/third_party/install/mkldnn/include \
    $$PADDLE_PATH/third_party/install/mklml/include \
    $$PADDLE_PATH/third_party/install/protobuf/include \
    $$PADDLE_PATH/third_party/install/xxhash/include

#you may need to modify the version of opencv
#LIBS += -L/lib/x86_64-linux-gnu -lopencv_core -lopencv_imgproc -lopencv_imgcodecs

LIBS += -L$$PADDLE_PATH/paddle/lib -lpaddle_inference
LIBS += $$PADDLE_PATH/third_party/install/glog/lib/libglog.a
LIBS += -L$$PADDLE_PATH/third_party/install/mkldnn/lib -lmkldnn
LIBS += -L$$PADDLE_PATH/third_party/install/mklml/lib -liomp5 -lmklml_intel
LIBS += $$PADDLE_PATH/third_party/install/protobuf/lib/libprotobuf.a
LIBS += $$PADDLE_PATH/third_party/install/xxhash/lib/libxxhash.a

CONFIG+=link_pkgconfig

PKGCONFIG += opencv

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS


SOURCES += \
#        main.cpp \
#        mainwindow.cpp \
    paddle/clipper.cpp \
    paddle/config.cpp \
    paddle/ocr_cls.cpp \
    paddle/ocr_det.cpp \
    paddle/ocr_rec.cpp \
    paddle/postprocess_op.cpp \
    paddle/preprocess_op.cpp \
    paddle/utility.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
#        mainwindow.h \
    paddle/clipper.h \
    paddle/config.h \
    paddle/ocr_cls.h \
    paddle/ocr_det.h \
    paddle/ocr_rec.h \
    paddle/postprocess_op.h \
    paddle/preprocess_op.h \
    paddle/utility.h \
    mainwindow.h

FORMS += \
        mainwindow.ui
