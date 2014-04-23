#-------------------------------------------------
#
# Project created by QtCreator 2013-10-08T15:00:53
#
#-------------------------------------------------

QT       += core
QT       += gui
QT       += widgets
TARGET = GenderRecognition
#CONFIG   -= console
CONFIG   -= app_bundle


#Custom configuration options to aid modifying build
#Note that the opencv build with dll's (dynamic) is not tested
#CONFIG += static_build
CONFIG += dynamic_build
#CONFIG += sqlite_support

#OPENCV_INSTALL_DIR = C:/opencv241Built/install/


#OPENCV_DIR = C:/opencv241Dynamic/
#OPENCV_INSTALL_DIR = $$OPENCV_DIR/install/
#OPENCV_3RD_PARTY_DIR = $$OPENCV_DIR/3rdparty/lib/
OPENCV_DIR = C:/opencv-2.4.8/build/
OPENCV_INSTALL_DIR = $$OPENCV_DIR
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


INCLUDEPATH += $$SQLITE_LIB_DIR
LIBS += $$SQLITE_LIB_DIR/sqlite3.dll


    LIBS +=$$OPENCV_INSTALL_DIR/lib/libopencv_core248d.dll.a
    LIBS +=$$OPENCV_INSTALL_DIR/lib/libopencv_highgui248d.dll.a
    LIBS +=$$OPENCV_INSTALL_DIR/lib/libopencv_imgproc248d.dll.a
    LIBS +=$$OPENCV_INSTALL_DIR/lib/libopencv_features2d248d.dll.a
    LIBS +=$$OPENCV_INSTALL_DIR/lib/libopencv_calib3d248d.dll.a
    LIBS +=$$OPENCV_INSTALL_DIR/lib/libopencv_ml248d.dll.a

    LIBS +=$$OPENCV_INSTALL_DIR/lib/libopencv_contrib248d.dll.a
    LIBS +=$$OPENCV_INSTALL_DIR/lib/libopencv_features2d248d.dll.a
    LIBS +=$$OPENCV_INSTALL_DIR/lib/libopencv_flann248d.dll.a
    LIBS +=$$OPENCV_INSTALL_DIR/lib/libopencv_gpu248d.dll.a
    LIBS +=$$OPENCV_INSTALL_DIR/lib/libopencv_legacy248d.dll.a
    LIBS +=$$OPENCV_INSTALL_DIR/lib/libopencv_ml248d.dll.a
    LIBS +=$$OPENCV_INSTALL_DIR/lib/libopencv_nonfree248d.dll.a
    LIBS +=$$OPENCV_INSTALL_DIR/lib/libopencv_objdetect248d.dll.a
    LIBS +=$$OPENCV_INSTALL_DIR/lib/libopencv_photo248d.dll.a
    LIBS +=$$OPENCV_INSTALL_DIR/lib/libopencv_ts248d.a
    LIBS +=$$OPENCV_INSTALL_DIR/lib/libopencv_video248d.dll.a
    LIBS +=$$OPENCV_INSTALL_DIR/lib/libopencv_stitching248d.dll.a
    LIBS +=$$OPENCV_INSTALL_DIR/lib/libopencv_videostab248d.dll.a
    LIBS +=$$OPENCV_INSTALL_DIR/lib/libopencv_highgui248d.dll.a


LIBS +=$$OPENCV_3RD_PARTY_DIR/liblibjasperd.a
LIBS +=$$OPENCV_3RD_PARTY_DIR/liblibtiffd.a
LIBS +=$$OPENCV_3RD_PARTY_DIR/libzlibd.a
LIBS +=$$OPENCV_3RD_PARTY_DIR/liblibpngd.a
LIBS +=$$OPENCV_3RD_PARTY_DIR/liblibjpegd.a



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

