#ifndef MYSQLSTRUCT_H
#define MYSQLSTRUCT_H

#include <QString>

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


struct S_factory_product{
    QString contract_id;   //
    QString name; //
    QString color; //
    QString spec;

    QString maogao;       //
    QString menfu; //
    QString danwei; //
    QString shuliang; //
    QString danjia; //
    QString jine; //
    QString comment; //
};


struct S_mianliao_ruku{
    QString ruku_id;   //
    QString date; //
    QString name; //
    QString menfu;

    QString mishu;       //
    QString gongjin; //
    QString juanshu; //
    QString kezhong; //
    QString color; //
    QString other_fee; //
    QString danjia; //

    QString zongjine;       //
    QString gongyingshang; //
    QString comment; //
};


struct S_mianliao_chuku{
    QString ruku_id;   //
    QString date; //
    QString mishu; //
    QString gongjing;

    QString juanshu;       //
    QString dingdanhao; //
    QString lingquren; //
    QString targetto; //
    QString comment; //
};
#endif // MYSQLSTRUCT_H
