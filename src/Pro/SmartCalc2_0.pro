QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ../Controller/ControllerCalculator.cpp \
    ../Controller/ControllerCredit.cpp \
    ../Controller/ControllerGraph.cpp \
    ../Model/MainModel.cpp \
    ../Model/ModelCalculator.cpp \
    ../Model/ModelCredit.cpp \
    ../Model/ModelGraph.cpp \
    ../View/credit.cpp \
    ../View/graph.cpp \
    ../View/main.cpp \
    ../View/mainwindow.cpp \
    ../View/qcustomplot_1.cpp \
    ../View/qcustomplot_2.cpp

HEADERS += \
    ../Controller/ControllerCredit.h \
    ../Controller/ControllerGraph.h \
    ../Model/MainModel.h \
    ../Controller/ControllerCalculator.h \
    ../Model/ModelCalculator.h \
    ../Model/ModelCredit.h \
    ../Model/ModelGraph.h \
    ../View/credit.h \
    ../View/graph.h \
    ../View/mainwindow.h \
    ../View/qcustomplot.h

FORMS += \
    ../View/credit.ui \
    ../View/graph.ui \
    ../View/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    SmartCalc2_0.pro.user
