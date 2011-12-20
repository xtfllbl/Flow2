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
    src/widget/qjdtreewidget.cpp \
    src/widget/qjdlabel.cpp \
    src/widget/qjdfuncationwidget.cpp \
    src/xml/readxmlmodule.cpp \
    src/xml/readxmlindex.cpp

HEADERS  += src/qjdmainwindow.h \
    src/widget/qjdmdi.h \
    src/widget/qjdpropertywidget.h \
    src/widget/qjdtabwidget.h \
    src/widget/qjdtreewidget.h \
    src/widget/qjdlabel.h \
    src/widget/qjdfuncationwidget.h \
    src/xml/readxmlmodule.h \
    src/xml/readxmlindex.h

FORMS    += src/qjdmainwindow.ui
