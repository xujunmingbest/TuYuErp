#-------------------------------------------------
#
# Project created by QtCreator 2019-07-22T22:51:03
#
#-------------------------------------------------

QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TuYuErp
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    global.cpp \
    MysqlOperate/mysqloperate.cpp \
    MysqlTableConfig/MysqlTableConfig.cpp \
    login.cpp \
    FactoryContract/addfactorycontract.cpp \
    FactoryContract/findfactorycontract.cpp \
    FactoryContract/lookfactorycontract.cpp \
    Mianliao/miaoliaoruku.cpp \
    Mianliao/findmianliao.cpp \
    FuLiao/fuliaochuruku.cpp \
    ChengPin/chengpinchuruku.cpp \
    Mianliao/mianliaoedit.cpp \
    Mianliao/mianliao_chuku_edit.cpp \
    FuLiao/findfuliao.cpp \
    FuLiao/chukuedit.cpp \
    FuLiao/rukuedit.cpp \
    ChengPin/findchengpin.cpp \
    ChengPin/chengpinrukuedit.cpp \
    ChengPin/chengpinchukuedit.cpp \
    ZhiXiangContract/zhixiangcontract.cpp \
    ZhiXiangContract/findzhixiangcontract.cpp \
    DindanContract/dindancontract.cpp \
    picture_client/qtclient.cpp \
    DindanContract/finddingdancontract.cpp \
    YeWuManage/kehucontract.cpp \
    YeWuManage/findkehucontract.cpp

HEADERS  += mainwindow.h \
    global.h \
    MysqlOperate/mysqloperate.h \
    MysqlOperate/mysqlstruct.h \
    MysqlTableConfig/MysqlTableConfig.h \
    login.h \
    FactoryContract/addfactorycontract.h \
    FactoryContract/findfactorycontract.h \
    FactoryContract/lookfactorycontract.h \
    Mianliao/miaoliaoruku.h \
    Mianliao/findmianliao.h \
    FuLiao/fuliaochuruku.h \
    ChengPin/chengpinchuruku.h \
    Mianliao/mianliaoedit.h \
    Mianliao/mianliao_chuku_edit.h \
    FuLiao/findfuliao.h \
    FuLiao/chukuedit.h \
    FuLiao/rukuedit.h \
    ChengPin/findchengpin.h \
    ChengPin/chengpinrukuedit.h \
    ChengPin/chengpinchukuedit.h \
    ZhiXiangContract/zhixiangcontract.h \
    ZhiXiangContract/findzhixiangcontract.h \
    DindanContract/dindancontract.h \
    picture_client/qtclient.h \
    picture_client/msg_def.h \
    DindanContract/finddingdancontract.h \
    YeWuManage/kehucontract.h \
    YeWuManage/findkehucontract.h

FORMS    += mainwindow.ui \
    login.ui \
    FactoryContract/addfactorycontract.ui \
    FactoryContract/findfactorycontract.ui \
    FactoryContract/lookfactorycontract.ui \
    Mianliao/miaoliaoruku.ui \
    Mianliao/findmianliao.ui \
    FuLiao/fuliaochuruku.ui \
    ChengPin/chengpinchuruku.ui \
    Mianliao/mianliaoedit.ui \
    Mianliao/mianliao_chuku_edit.ui \
    FuLiao/findfuliao.ui \
    FuLiao/chukuedit.ui \
    FuLiao/rukuedit.ui \
    ChengPin/findchengpin.ui \
    ChengPin/chengpinrukuedit.ui \
    ChengPin/chengpinchukuedit.ui \
    ZhiXiangContract/zhixiangcontract.ui \
    ZhiXiangContract/findzhixiangcontract.ui \
    DindanContract/dindancontract.ui \
    DindanContract/finddingdancontract.ui \
    YeWuManage/kehucontract.ui \
    YeWuManage/findkehucontract.ui
