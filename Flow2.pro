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
    src/widget/qjdlabel.cpp \
    src/widget/qjdfuncationwidget.cpp \
    src/xml/qjdreadxmlmodule.cpp \
    src/xml/qjdreadxmlindex.cpp \
    src/qjdargu.cpp \
    src/widget/qjdareaheadwidget.cpp \
    src/widget/qjdfuncationheadwidget.cpp

HEADERS  += src/qjdmainwindow.h \
    src/widget/qjdmdi.h \
    src/widget/qjdpropertywidget.h \
    src/widget/qjdtabwidget.h \
    src/widget/qjdareawidget.h \
    src/widget/qjdlabel.h \
    src/widget/qjdfuncationwidget.h \
    src/xml/qjdreadxmlmodule.h \
    src/xml/qjdreadxmlindex.h \
    src/qjdargu.h \
    src/widget/qjdareaheadwidget.h \
    src/widget/qjdfuncationheadwidget.h

FORMS    += src/qjdmainwindow.ui
