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

CONFIG += c++1z

SOURCES += \
        constants.cpp \
        delegate/customersdelegate.cpp \
        delegate/projectinfodelegate.cpp \
        delegate/projectsdelegate.cpp \
        delegate/tableeditdelegate.cpp \
        main.cpp \
        model/customtablemodels.cpp \
        model/modelview.cpp \
        test/testdb.cpp \
        util/DBmaintainer/databasemaintainer.cpp \
        util/common/projectinfo.cpp \
        util/logger/logger.cpp \
        util/tools/postmessage.cpp \
        util/tools/validationtools.cpp \
        view/tabs/commontab.cpp \
        view/tabs/customerstab.cpp \
        view/tabs/projectstab.cpp \
        view/views/projectinfoview.cpp \
        view/views/tableview.cpp \
        view/widgets/timeindicator.cpp \
        view/windows/deleterecorddialog.cpp \
        view/windows/mainwindow.cpp \
        view/windows/projectinfowindow.cpp

HEADERS += \
    delegate/customersdelegate.h \
    delegate/projectinfodelegate.h \
    delegate/projectsdelegate.h \
    delegate/tableeditdelegate.h \
    model/customtablemodels.h \
    model/modelview.h \
    test/testdb.h \
    util/DBmaintainer/databasemaintainer.h \
    util/common/customerinfo.h \
    util/common/projectinfo.h \
    util/logger/logger.h \
    util/tools/fetchmodelinfo.h \
    util/tools/postmessage.h \
    util/tools/validationtools.h \
    view/tabs/commontab.h \
    view/tabs/customerstab.h \
    view/tabs/projectstab.h \
    view/views/projectinfoview.h \
    view/views/tableview.h \
    view/widgets/clickablelabel.h \
    view/widgets/timeindicator.h \
    view/windows/deleterecorddialog.h \
    view/windows/mainwindow.h \
    view/windows/projectinfowindow.h

INCLUDEPATH += \
    test \
    util \
    view \
    model \
    delegate \
    view/tabs \
    view/views \
    view/windows \
    view/widgets \
    util/DBmaintainer \
    util/common \
    util/logger \
    util/tools

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    emptydbpicture.qrc

QMAKE_CXXFLAGS += -Wno-padded
