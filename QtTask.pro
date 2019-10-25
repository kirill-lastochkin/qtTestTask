#-------------------------------------------------
#
# Project created by QtCreator 2019-09-04T14:32:36
#
#-------------------------------------------------

QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtTask
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

CONFIG += c++14

SOURCES += \
        constants.cpp \
        delegate/customersdelegate.cpp \
        delegate/projectinfodelegate.cpp \
        delegate/projectsdelegate.cpp \
        delegate/tableeditdelegate.cpp \
        main.cpp \
        model/customtablemodel.cpp \
        test/testdb.cpp \
        util/databasemaintainer.cpp \
        util/logger.cpp \
        util/modelview.cpp \
        util/projectinfo.cpp \
        util/validationtools.cpp \
        view/commontab.cpp \
        view/customerstab.cpp \
        view/deleterecorddialog.cpp \
        view/mainwindow.cpp \
        view/projectinfoview.cpp \
        view/projectinfowindow.cpp \
        view/projectstab.cpp \
        view/tableview.cpp \
        view/timeindicator.cpp

HEADERS += \
    delegate/customersdelegate.h \
    delegate/projectinfodelegate.h \
    delegate/projectsdelegate.h \
    delegate/tableeditdelegate.h \
    model/customtablemodel.h \
    test/testdb.h \
    util/customerinfo.h \
    util/databasemaintainer.h \
    util/logger.h \
    util/modelview.h \
    util/projectinfo.h \
    util/validationtools.h \
    view/commontab.h \
    view/customerstab.h \
    view/deleterecorddialog.h \
    view/mainwindow.h \
    view/projectinfoview.h \
    view/projectinfowindow.h \
    view/projectstab.h \
    view/tableview.h \
    view/timeindicator.h

INCLUDEPATH += \
    test \
    util \
    view \
    model \
    delegate

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    emptydbpicture.qrc
