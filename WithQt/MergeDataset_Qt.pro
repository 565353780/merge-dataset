QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

win32{
DESTDIR = ../bin_win
}
unix{
DESTDIR = ../bin_linux
}

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    MainWindow.cpp

HEADERS += \
    MainWindow.h

FORMS += \
    MainWindow.ui

# OpenCV
INCLUDEPATH += \
    /home/chli/OpenCV/opencv-3.4.0/build/installed/include \
    /home/chli/OpenCV/opencv-3.4.0/build/installed/include/opencv \
    /home/chli/OpenCV/opencv-3.4.0/build/installed/include/opencv2

LIBS += \
    -L/home/chli/OpenCV/opencv-3.4.0/build/installed/lib \
    -lopencv_highgui \
    -lopencv_core \
    -lopencv_imgproc \
    -lopencv_imgcodecs \
    -lopencv_video \
    -lopencv_videoio

# MergeCVMat
SOURCES += \
#    MergeCVMat/Example.cpp \
    MergeCVMat/MergeCVMat.cpp

HEADERS += \
#    MergeCVMat/Example.h \
    MergeCVMat/MergeCVMat.h

# MergeDataset
SOURCES += \
    MergeDataset/Example.cpp \
    MergeDataset/MergeDataset.cpp

HEADERS += \
    MergeDataset/Example.h \
    MergeDataset/MergeDataset.h

TRANSLATIONS += \
    MergeDataset_zh_CN.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
