#-------------------------------------------------
#
# Project created by QtCreator 2017-12-25T19:13:53
#
#-------------------------------------------------

QT       += core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

LIBS += `pkg-config --libs --cflags opencv`

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    dialog.cpp \
    cvwidget.cpp \
    imageList.cpp \
    color/color_colorspace.cpp \
    color/color_histogram.cpp \
    color/color_interpolate.cpp \
    expect/expect.cpp \
    filter/filter_basic.cpp \
    filter/filter_difference.cpp \
    filter/filter_smooth.cpp \
    matrix/matrix_conv.cpp \
    matrix/matrix_math.cpp \
    morphology/morphology_basic.cpp \
    dialog/confirmdialog.cpp \
    dialog/valuedialog.cpp \
    dialog/kerneldialog.cpp \
    dialog/messagedialog.cpp \
    dialog/choicedialog.cpp \
    dialog/histogramdialog.cpp \
    dialog/hsvdialog.cpp \
    dialog/pointsdialog.cpp

HEADERS += \
        mainwindow.h \
    dialog.h \
    cvwidget.h \
    imageList.h \
    color/color_colorspace.h \
    color/color_histogram.h \
    color/color_interpolate.h \
    expect/expect.h \
    filter/filter_basic.h \
    filter/filter_difference.h \
    filter/filter_smooth.h \
    matrix/matrix_conv.h \
    matrix/matrix_map.h \
    matrix/matrix_math.h \
    morphology/morphology_basic.h \
    dialog/confirmdialog.h \
    dialog/valuedialog.h \
    dialog/kerneldialog.h \
    dialog/messagedialog.h \
    dialog/choicedialog.h \
    dialog/histogramdialog.h \
    dialog/hsvdialog.h \
    dialog/pointsdialog.h

FORMS += \
        mainwindow.ui \
    dialog.ui \
    dialog/confirmdialog.ui \
    dialog/valuedialog.ui \
    dialog/kerneldialog.ui \
    dialog/messagedialog.ui \
    dialog/choicedialog.ui \
    dialog/histogramdialog.ui \
    dialog/hsvdialog.ui \
    dialog/pointsdialog.ui

DISTFILES += \
    1.png
