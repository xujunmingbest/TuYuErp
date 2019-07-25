#include "MysqlTableConfig.h"

MysqlTableConfig *MysqlTableConfig::local_instance = nullptr;

MysqlTableConfig* MysqlTableConfig::getInstance(){
    if (local_instance == nullptr)
    {
        local_instance = new MysqlTableConfig();
        local_instance->LoadConfig();
    }
    return local_instance;
}

#include <QDir>
#include <QDebug>
#include <QSettings>
#include <QMessageBox>
void MysqlTableConfig::LoadConfig(){
    //±éÀúÎÄ¼þ¼Ð
    TableConfigdata.clear();
    QString DirPath ="./MysqlTableConfig/";
    QDir dir(DirPath);
    QStringList nameFilters;
    nameFilters << "*.ini";
    QStringList files = dir.entryList(nameFilters, QDir::Files|QDir::Readable, QDir::Name);
    for(int i = 0;i<files.size();i++){
       QString inipath =  DirPath+files.at(i);
       QSettings settings(inipath,QSettings::IniFormat);
       QSet<QString> set;
       for(int j = 0;j< settings.allKeys().size();j++){
           settings.allKeys().at(j);
           set.insert(settings.allKeys().at(j));
       }
       TableConfigdata.insert(files[i].remove(".ini"),set);
    }
    qDebug() << TableConfigdata;
}

bool MysqlTableConfig::is_legal_table(QString TableName){
    if(TableConfigdata.count(TableName) == 0) return false;
    else return true;
}

QSet<QString> MysqlTableConfig::get_table_columns(QString TableName){
    return  TableConfigdata.value(TableName);
}
