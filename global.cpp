#include "global.h"

CUser *CUser::getInstance()
{
    if (local_instance == nullptr)
    {
        local_instance = new CUser();
    }
    return local_instance;
}


QString CUser::get_auth(QString MoKuaiName){
    auto mi = m_authmap.find(MoKuaiName);
    if(mi != m_authmap.end())
    {
        return mi.value();
    }
    return "";
}
