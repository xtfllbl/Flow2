#-------------------------------------------------
#
# Project created by QtCreator 2011-11-09T13:30:50
#
#-------------------------------------------------

QT       += core gui
QT += xml

TARGET = flow
TEMPLATE = app

MOC_DIR += ./GeneratedFiles/moc
OBJECTS_DIR += ./GeneratedFiles/ofile
RCC_DIR += ./GeneratedFiles
UI_DIR += ./src

SOURCES += main.cpp\
    src/qjdmainwindow.cpp \
    src/widget/qjdmdi.cpp \
    src/widget/qjdpropertywidget.cpp \
    src/widget/qjdtabwidget.cpp \
    src/widget/qjdareawidget.cpp \
    src/widget/QJD/qjdlabel.cpp \
    src/widget/qjdfuncationwidget.cpp \
    src/xml/qjdreadxmlindex.cpp \
    src/qjdargu.cpp \
    src/widget/qjdareaheadwidget.cpp \
    src/widget/qjdfuncationheadwidget.cpp \
    src/widget/qjdmdisublistwidget.cpp \
    src/widget/qjdnewarea.cpp \
    src/widget/qjdnewline.cpp \
    src/widget/qjdnewflow.cpp \
    src/widget/qjdfunarguwidget.cpp \
    src/widget/QJD/qjdfilesaveline.cpp \
    src/widget/QJD/qjdfilereadline.cpp \
    src/widget/QJD/qjdlineedit.cpp \
    src/widget/QJD/qjdcheckbox.cpp \
    src/widget/QJD/qjdspinbox.cpp \
    src/widget/QJD/qjdcombobox.cpp \
    src/widget/QJD/qjdradiobutton.cpp \
    src/widget/QJD/qjdbuttongroup.cpp \
    src/widget/QJD/qjdpushbutton.cpp \
    src/widget/qjdprocesswidget.cpp

HEADERS  += src/qjdmainwindow.h \
    src/widget/qjdmdi.h \
    src/widget/qjdpropertywidget.h \
    src/widget/qjdtabwidget.h \
    src/widget/qjdareawidget.h \
    src/widget/QJD/qjdlabel.h \
    src/widget/qjdfuncationwidget.h \
    src/xml/qjdreadxmlindex.h \
    src/qjdargu.h \
    src/widget/qjdareaheadwidget.h \
    src/widget/qjdfuncationheadwidget.h \
    src/widget/qjdmdisublistwidget.h \
    src/widget/qjdnewarea.h \
    src/widget/qjdnewline.h \
    src/widget/qjdnewflow.h \
    src/widget/qjdfunarguwidget.h \
    src/widget/QJD/qjdfilesaveline.h \
    src/widget/QJD/qjdfilereadline.h \
    src/widget/QJD/qjdlineedit.h \
    src/widget/QJD/qjdcheckbox.h \
    src/widget/QJD/qjdspinbox.h \
    src/widget/QJD/qjdcombobox.h \
    src/widget/QJD/qjdradiobutton.h \
    src/widget/QJD/qjdbuttongroup.h \
    src/widget/QJD/qjdpushbutton.h \
    src/widget/qjdprocesswidget.h

FORMS    += src/qjdmainwindow.ui

RESOURCES += \
    images.qrc
