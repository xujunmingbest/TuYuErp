#ifndef MYSQLSTRUCT_H
#define MYSQLSTRUCT_H

#include <QString>
   contract_id` varchar(45) NOT NULL COMMENT '合同编号',
  `contract_address` varchar(100) NOT NULL COMMENT '合同签订地点',
  `jiesuan_xiaoxie` int(11) NOT NULL,
  `jiesuan_daxie` varchar(100) NOT NULL,
  `jia_name` varchar(100) NOT NULL,
  `jia_address` varchar(255) NOT NULL,
  `jia_faren` varchar(45) NOT NULL,
  `jia_agency` varchar(100) NOT NULL,
  `jia_date` date NOT NULL,
  `jia_bank` varchar(100) NOT NULL,
  `jia_account` varchar(45) NOT NULL,
  `jia_telephone` varchar(20) NOT NULL,
  `jia_fax` varchar(45) NOT NULL,
  `yi_name` varchar(100) NOT NULL,
  `yi_address` varchar(255) NOT NULL,
  `yi_faren` varchar(45) NOT NULL,
  `yi_agency` varchar(100) NOT NULL,
  `yi_date` date NOT NULL,
  `yi_bank` varchar(100) NOT NULL,
  `yi_account` varchar(45) NOT NULL,
  `yi_telephone` varchar(20) NOT NULL,
  `yi_fax_copy1` varchar(45) NOT NULL,
struct S_factory_contract{
    QString contract_id;   //
    QString contract_address; //
    QString jiesuan_xiaoxie; //
    QString jiesuan_daxie;

    QString jia_name;       //
    QString jia_address; //
    QString jia_faren; //
    QString jia_agency; //
    QString jia_date; //
    QString jia_bank; //
    QString jia_account; //
    QString jia_telephone;
    QString jia_fax; //


    QString yi_name;       //
    QString yi_address; //
    QString yi_faren; //
    QString yi_agency; //
    QString yi_date; //
    QString yi_bank; //
    QString yi_account; //
    QString yi_telephone;
    QString yi_fax; //
};



#endif // MYSQLSTRUCT_H
