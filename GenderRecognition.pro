#-------------------------------------------------
#
# Project created by QtCreator 2013-10-08T15:00:53
#
#-------------------------------------------------

QT       += core

QT       += gui

TARGET = GenderRecognition
CONFIG   += console
CONFIG   -= app_bundle



#Custom configuration options to aid modifying build
#Note that the opencv build with dll's (dynamic) is not tested
#CONFIG += static_build
CONFIG += dynamic_build
#CONFIG += sqlite_support

#OPENCV_INSTALL_DIR = C:/opencv241Built/install/

OPENCV_DIR = C:/opencv241Dynamic/
OPENCV_INSTALL_DIR = $$OPENCV_DIR/install/
OPENCV_3RD_PARTY_DIR = $$OPENCV_DIR/3rdparty/lib/

#OPENCV_DIR = H:/VivaWork/opencv241Built/
#OPENCV_INSTALL_DIR = H:/VivaWork/opencv241Built/install/
#OPENCV_3RD_PARTY_DIR= H:/VivaWork/opencv241Built/3rdparty/lib/



SQLITE_LIB_DIR = C:/sqlite/
BOOST_DIR = C:/builtBoost/


#CONFIG += -static #-static-libgcc


TEMPLATE = app

INCLUDEPATH +=$$PWD/src/
INCLUDEPATH +=$$PWD/src/Face_api
INCLUDEPATH +=$$PWD/src/tracking
INCLUDEPATH +=$$PWD/src/persistence
INCLUDEPATH +=$$PWD/src/ui

SOURCES += \
    src/main.cpp \
    src/Face_api/flandmark_detector.cpp \
    src/Face_api/facevue.cpp \
    src/Face_api/faceRecognition.cpp \
    src/Face_api/cascadedetect.cpp \
    src/histogram1d.cpp \
    src/persistence/sqlitedb.cpp \
    src/tracking/detectionevent.cpp \
    src/tracking/trackedface.cpp \
    src/utils.cpp \
    src/ui/facetrackingwindow.cpp \
    src/tracking/objecttracker.cpp \
    src/tracking/facetracker.cpp \
    src/ui/demomanagerwindow.cpp \
    src/ui/addemowindow.cpp

HEADERS += \
    src/Face_api/flandmark_detector.h \
    src/Face_api/facevue.h \
    src/Face_api/faceRecognition.h \
    src/Face_api/cascadedetect.hpp \
    src/histogram1d.h \
    src/tracking/coloredrect.h \
    src/tracking/detectioninformation.h \
    src/tracking/trackedface.h \
    src/tracking/trackingstatus.h \
    src/persistence/dbinterface.h \
    src/tracking/detectionevent.h \
    src/persistence/sqlitedb.h \
    src/utils.h \
    src/flags.h \
    src/ui/facetrackingwindow.h \
    src/tracking/objecttracker.h \
    src/tracking/facetracker.h \
    src/ui/demomanagerwindow.h \
    src/ui/addemowindow.h


INCLUDEPATH +=$$OPENCV_INSTALL_DIR/include
INCLUDEPATH +=$$OPENCV_INSTALL_DIR/include/opencv
INCLUDEPATH +=$$OPENCV_INSTALL_DIR/include/opencv2

#INCLUDEPATH += $$BOOST_DIR/include
#INCLUDEPATH += $$BOOST_DIR/include/boost-1_54




LIBS += -L$$OPENCV_INSTALL_DIR/lib
LIBS += -L$$BOOST_DIR/lib


static_build {

    LIBS +=$$OPENCV_INSTALL_DIR/lib/libopencv_imgproc241.a
    LIBS +=$$OPENCV_INSTALL_DIR/lib/libopencv_calib3d241.a
    LIBS +=$$OPENCV_INSTALL_DIR/lib/libopencv_video241.a
    LIBS +=$$OPENCV_INSTALL_DIR/lib/libopencv_core241.a
    LIBS +=$$OPENCV_INSTALL_DIR/lib/libopencv_imgproc241.a
    LIBS +=$$OPENCV_INSTALL_DIR/lib/libopencv_video241.a
    LIBS +=$$OPENCV_INSTALL_DIR/lib/libopencv_contrib241.a
    LIBS +=$$OPENCV_INSTALL_DIR/lib/libopencv_features2d241.a
    LIBS +=$$OPENCV_INSTALL_DIR/lib/libopencv_legacy241.a
    LIBS +=$$OPENCV_INSTALL_DIR/lib/libopencv_ml241.a
    LIBS +=$$OPENCV_INSTALL_DIR/lib/libopencv_nonfree241.a
    LIBS +=$$OPENCV_INSTALL_DIR/lib/libopencv_objdetect241.a
    LIBS +=$$OPENCV_INSTALL_DIR/lib/libopencv_videostab241.a
    LIBS +=$$OPENCV_INSTALL_DIR/lib/libopencv_highgui241.a

}




INCLUDEPATH += $$SQLITE_LIB_DIR
LIBS += $$SQLITE_LIB_DIR/sqlite3.dll



dynamic_build {


    LIBS +=$$OPENCV_INSTALL_DIR/lib/libopencv_core241.dll.a
    LIBS +=$$OPENCV_INSTALL_DIR/lib/libopencv_highgui241.dll.a
    LIBS +=$$OPENCV_INSTALL_DIR/lib/libopencv_imgproc241.dll.a
    LIBS +=$$OPENCV_INSTALL_DIR/lib/libopencv_features2d241.dll.a
    LIBS +=$$OPENCV_INSTALL_DIR/lib/libopencv_calib3d241.dll.a
    LIBS +=$$OPENCV_INSTALL_DIR/lib/libopencv_ml241.dll.a

    LIBS +=$$OPENCV_INSTALL_DIR/lib/libopencv_contrib241.dll.a
    LIBS +=$$OPENCV_INSTALL_DIR/lib/libopencv_features2d241.dll.a
    LIBS +=$$OPENCV_INSTALL_DIR/lib/libopencv_flann241.dll.a
    LIBS +=$$OPENCV_INSTALL_DIR/lib/libopencv_gpu241.dll.a
    LIBS +=$$OPENCV_INSTALL_DIR/lib/libopencv_legacy241.dll.a
    LIBS +=$$OPENCV_INSTALL_DIR/lib/libopencv_ml241.dll.a
    LIBS +=$$OPENCV_INSTALL_DIR/lib/libopencv_nonfree241.dll.a
    LIBS +=$$OPENCV_INSTALL_DIR/lib/libopencv_objdetect241.dll.a
    LIBS +=$$OPENCV_INSTALL_DIR/lib/libopencv_photo241.dll.a
    LIBS +=$$OPENCV_INSTALL_DIR/lib/libopencv_ts241.a
    LIBS +=$$OPENCV_INSTALL_DIR/lib/libopencv_video241.dll.a
    LIBS +=$$OPENCV_INSTALL_DIR/lib/libopencv_stitching241.dll.a
    LIBS +=$$OPENCV_INSTALL_DIR/lib/libopencv_videostab241.dll.a
    LIBS +=$$OPENCV_INSTALL_DIR/lib/libopencv_highgui241.dll.a

}

##LIBS +=$$PWD/Face_api/facevue.o
##LIBS +=$$PWD/Face_api/flandmark_detector.o
##LIBS +=$$PWD/Face_api/cascadedetect.o

#LIBS +=$$PWD/opencv_dlls/libopencv_imgproc241.a##2?
#LIBS +=$$PWD/opencv_dlls/libopencv_calib3d241.a
#LIBS +=$$PWD/opencv_dlls/libopencv_video241.a###2?
#LIBS +=$$PWD/opencv_dlls/libopencv_core241.a
#LIBS +=$$PWD/opencv_dlls/libopencv_imgproc241.a
#LIBS +=$$PWD/opencv_dlls/libopencv_video241.a

#LIBS +=$$PWD/opencv_dlls/libopencv_contrib241.a
#LIBS +=$$PWD/opencv_dlls/libopencv_features2d241.a
#LIBS +=$$PWD/opencv_dlls/libopencv_legacy241.a
#LIBS +=$$PWD/opencv_dlls/libopencv_ml241.a
#LIBS +=$$PWD/opencv_dlls/libopencv_nonfree241.a
#LIBS +=$$PWD/opencv_dlls/libopencv_objdetect241.a
#LIBS +=$$PWD/opencv_dlls/libopencv_videostab241.a
#LIBS +=$$PWD/opencv_dlls/libopencv_highgui241.a
#LIBS +=$$PWD/opencv_dlls/libvfw32.a
#LIBS +=$$PWD/opencv_dlls/libcomctl32.a
#LIBS +=$$PWD/opencv_dlls/libavcodec.a
#LIBS +=$$PWD/opencv_dlls/libavformat.a
#LIBS +=$$PWD/opencv_dlls/libavcore64.a
#LIBS +=$$PWD/opencv_dlls/libole32.a
#LIBS +=$$PWD/opencv_dlls/libavifil32.a
#LIBS +=$$PWD/opencv_dlls/libuuid.a
#LIBS +=$$PWD/opencv_dlls/liboleaut32.a
#LIBS +=$$PWD/opencv_dlls/libolepro32.a




LIBS +=$$OPENCV_3RD_PARTY_DIR/liblibjasper.a
LIBS +=$$OPENCV_3RD_PARTY_DIR/liblibtiff.a
LIBS +=$$OPENCV_3RD_PARTY_DIR/libzlib.a
LIBS +=$$OPENCV_3RD_PARTY_DIR/liblibpng.a
LIBS +=$$OPENCV_3RD_PARTY_DIR/liblibjpeg.a



LIBS += -lvfw32
LIBS += -lcomctl32
#LIBS += -lavcodec
#LIBS += -lavformat
#LIBS += -lavcore64
LIBS += -lgdi32
LIBS += -lole32
LIBS += -lavifil32
LIBS += -luuid
LIBS += -loleaut32
LIBS += -lolepro32

#LIBS +=$$PWD/Face_api/libFaceVue.a



