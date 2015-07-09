
#include "SystemInclude.h"
#include "BcmController.h"

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include "UpdateMgt.h"
#include "gsDslFtp.h"

using namespace std;

CxxBeginNameSpace(Snmp)
CxxBeginNameSpace(GsDslFtp)

void InitGsDslFtp(void)
{
    const oid managedObjectType[] = { 1,3,6,1,4,1,22764,7900,3,1,7,1 };
    const oid address[] = { 1,3,6,1,4,1,22764,7900,3,1,7,2 };
    const oid userName[] = { 1,3,6,1,4,1,22764,7900,3,1,7,3 };
    const oid userPwd[] = { 1,3,6,1,4,1,22764,7900,3,1,7,4 };
    const oid filePath[] = { 1,3,6,1,4,1,22764,7900,3,1,7,5 };
    const oid fileName[] = { 1,3,6,1,4,1,22764,7900,3,1,7,6 };
    const oid loadType[] = { 1,3,6,1,4,1,22764,7900,3,1,7,7 };
    const oid downloadStatus[] = { 1,3,6,1,4,1,22764,7900,3,1,7,8 };
    const oid addressType[] = { 1,3,6,1,4,1,22764,7900,3,1,7,9 };
//    const oid ipv6Address[] = { 1,3,6,1,4,1,22764,7900,3,1,7,10 };
    const oid protocolType[] = { 1,3,6,1,4,1,22764,7900,3,1,7,11 };
//    const oid managedObjectSubType[] = { 1,3,6,1,4,1,22764,7900,3,1,7,12 };

    netsnmp_handler_registration* handler;

    handler = netsnmp_create_handler_registration("gsDslFtpManagedObjectType",
        HandleManagedObjectType, managedObjectType, OID_LENGTH(managedObjectType),
        HANDLER_CAN_RWRITE);
    netsnmp_register_scalar(handler);

    handler = netsnmp_create_handler_registration("gsDslFtpAddress",
        HandleAddress, address, OID_LENGTH(address), HANDLER_CAN_RWRITE);
    netsnmp_register_scalar(handler);

    handler = netsnmp_create_handler_registration("gsDslFtpUserName",
        HandleUserName, userName, OID_LENGTH(userName), HANDLER_CAN_RWRITE);
    netsnmp_register_scalar(handler);

    handler = netsnmp_create_handler_registration("gsDslFtpUserPwd",
        HandleUserPwd, userPwd, OID_LENGTH(userPwd), HANDLER_CAN_RWRITE);
    netsnmp_register_scalar(handler);

    handler = netsnmp_create_handler_registration("gsDslFtpFilePath",
        HandleFilePath, filePath, OID_LENGTH(filePath), HANDLER_CAN_RWRITE);
    netsnmp_register_scalar(handler);

    handler = netsnmp_create_handler_registration("gsDslFtpFileName",
        HandleFileName, fileName, OID_LENGTH(fileName), HANDLER_CAN_RWRITE);
    netsnmp_register_scalar(handler);

    handler = netsnmp_create_handler_registration("gsDslFtpLoadType",
        HandleLoadType, loadType, OID_LENGTH(loadType), HANDLER_CAN_RWRITE);
    netsnmp_register_scalar(handler);

    handler = netsnmp_create_handler_registration("gsDslDownloadStatus",
        HandleDownloadStatus, downloadStatus, OID_LENGTH(downloadStatus),
        HANDLER_CAN_RONLY);
    netsnmp_register_scalar(handler);

    handler = netsnmp_create_handler_registration("gsDslFtpAddressType",
        HandleAddressType, addressType, OID_LENGTH(addressType),
        HANDLER_CAN_RWRITE);
    netsnmp_register_scalar(handler);

//    handler = netsnmp_create_handler_registration("gsDslFtpIpv6Address",
//        HandleIpv6Address, HandleIpv6Address, OID_LENGTH(HandleIpv6Address),
//        HANDLER_CAN_RWRITE);
//    netsnmp_register_scalar(handler);

    handler = netsnmp_create_handler_registration("gsDslFtpProtocolType",
        HandleProtocolType, protocolType, OID_LENGTH(protocolType),
        HANDLER_CAN_RWRITE);
    netsnmp_register_scalar(handler);

//    handler = netsnmp_create_handler_registration("gsDslFtpManagedObjectSubType",
//        HandleManagedObjectSubType,
//        managedObjectSubType, OID_LENGTH(managedObjectSubType),
//        HANDLER_CAN_RWRITE);
//    netsnmp_register_scalar(handler);
}

pair<int, const char*> objTypeNames[] =
{
    make_pair(FileTypeSwitchProfile, (const char*)"switch"),
    make_pair(FileTypeControllerProfile, (const char*)"controller"),
    make_pair(FileTypeRootFs, (const char*)"rootfs")
};
const size_t objTypeNum = sizeof(objTypeNames) / sizeof(pair<int, const char*>);

template<class T>
struct CompareObjType : public binary_function<pair<int, const char*>, T, bool>
{};

template<>
struct CompareObjType<int> : public binary_function<pair<int, const char*>, int, bool>
{
    bool operator()(const pair<int, const char*>& left, int right) const
    {
        return (left.first == right);
    }
};

template<>
struct CompareObjType<const char*> : public binary_function<pair<int, const char*>, const char*, bool>
{
    bool operator()(const pair<int, const char*>& left, const char* right) const
    {
        return memcmp(left.second, right, strlen(left.second)) == 0;
    }
};

struct in_addr ftpIp = {0};
char userName[MaxUserName];
char userPassword[MaxUserPassword];
char fileName[MaxFileName];
char filePath[MaxFilePath];
#define LoadTypeUpload   1
#define LoadTypeDownload 2
int32_t loadType = 0;
int32_t fileType = 0;

int HandleManagedObjectType(netsnmp_mib_handler *handler,
    netsnmp_handler_registration *reginfo,
    netsnmp_agent_request_info *reqinfo,
    netsnmp_request_info *requests)
{
    int ret;
    pair<int, const char*>* iter;

    switch(reqinfo->mode)
    {
    case MODE_GET:
        iter = find_if(objTypeNames,
                       objTypeNames + objTypeNum,
                       bind2nd(CompareObjType<int>(), fileType));
        if (iter != objTypeNames + objTypeNum)
        {
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR,
                iter->second, strlen(iter->second));
        }
        else
        {
            netsnmp_set_request_error(reqinfo, requests, SNMP_ERR_WRONGVALUE);
        }
        break;
    case MODE_SET_RESERVE1:
        ret = netsnmp_check_vb_max_size(requests->requestvb, MaxObjectType - 1);
        if (ret != SNMP_ERR_NOERROR)
        {
            netsnmp_set_request_error(reqinfo, requests, ret);
        }
        iter = find_if(objTypeNames,
            objTypeNames + objTypeNum,
            bind2nd(CompareObjType<const char*>(), (const char*)requests->requestvb->val.string));
        if (iter == objTypeNames + objTypeNum)
        {
            netsnmp_set_request_error(reqinfo, requests, SNMP_ERR_WRONGVALUE);
        }
        break;
    case MODE_SET_RESERVE2:
    case MODE_SET_FREE:
            break;
    case MODE_SET_ACTION:
        iter = find_if(objTypeNames,
            objTypeNames + objTypeNum,
            bind2nd(CompareObjType<const char*>(), (const char*)requests->requestvb->val.string));
        fileType = iter->first;
        break;
    case MODE_SET_COMMIT:
        break;

    default:
        /* we should never get here, so this is a really bad error */
        snmp_log(LOG_ERR, "unknown mode (%d) in %s\n", reqinfo->mode, __func__);
        return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}

int HandleAddress(netsnmp_mib_handler *handler,
    netsnmp_handler_registration *reginfo,
    netsnmp_agent_request_info *reqinfo,
    netsnmp_request_info *requests)
{
    switch(reqinfo->mode)
    {
    case MODE_GET:
        snmp_set_var_typed_value(requests->requestvb, ASN_IPADDRESS,
            &ftpIp.s_addr, sizeof(uint32_t));
        break;
    case MODE_SET_RESERVE1:
    case MODE_SET_RESERVE2:
    case MODE_SET_FREE:
            break;
    case MODE_SET_ACTION:
        ftpIp.s_addr = *requests->requestvb->val.integer;
        break;
    case MODE_SET_COMMIT:
        break;

    default:
        /* we should never get here, so this is a really bad error */
        snmp_log(LOG_ERR, "unknown mode (%d) in %s\n", reqinfo->mode, __func__);
        return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}

int HandleUserName(netsnmp_mib_handler *handler,
    netsnmp_handler_registration *reginfo,
    netsnmp_agent_request_info *reqinfo,
    netsnmp_request_info *requests)
{
    int ret;
    switch(reqinfo->mode)
    {
    case MODE_GET:
        snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR,
            userName, strlen(userName));
        break;
    case MODE_SET_RESERVE1:
    case MODE_SET_RESERVE2:
        ret = netsnmp_check_vb_max_size(requests->requestvb, MaxUserName - 1);
        if ( ret != SNMP_ERR_NOERROR )
        {
            netsnmp_set_request_error(reqinfo, requests, ret );
        }
        break;
    case MODE_SET_FREE:
        break;
    case MODE_SET_ACTION:
        memcpy(userName, requests->requestvb->val.string, requests->requestvb->val_len);
        userName[requests->requestvb->val_len] = '\0';
        break;
    case MODE_SET_COMMIT:
        break;

    default:
        /* we should never get here, so this is a really bad error */
        snmp_log(LOG_ERR, "unknown mode (%d) in %s\n", reqinfo->mode, __func__);
        return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}

int HandleUserPwd(netsnmp_mib_handler *handler,
    netsnmp_handler_registration *reginfo,
    netsnmp_agent_request_info *reqinfo,
    netsnmp_request_info *requests)
{
    int ret;
    switch(reqinfo->mode)
    {
    case MODE_GET:
        snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR,
            userPassword, strlen(userPassword));
        break;
    case MODE_SET_RESERVE1:
    case MODE_SET_RESERVE2:
        ret = netsnmp_check_vb_max_size(requests->requestvb, MaxUserPassword - 1);
        if ( ret != SNMP_ERR_NOERROR )
        {
            netsnmp_set_request_error(reqinfo, requests, ret );
        }
        break;
    case MODE_SET_FREE:
        break;
    case MODE_SET_ACTION:
        memcpy(userPassword, requests->requestvb->val.string, requests->requestvb->val_len);
        userPassword[requests->requestvb->val_len] = '\0';
        break;
    case MODE_SET_COMMIT:
        break;

    default:
        /* we should never get here, so this is a really bad error */
        snmp_log(LOG_ERR, "unknown mode (%d) in %s\n", reqinfo->mode, __func__);
        return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}

int HandleFilePath(netsnmp_mib_handler *handler,
    netsnmp_handler_registration *reginfo,
    netsnmp_agent_request_info *reqinfo,
    netsnmp_request_info *requests)
{
    int ret;
    switch(reqinfo->mode)
    {
    case MODE_GET:
        snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR, filePath, strlen(filePath));
        break;
    case MODE_SET_RESERVE1:
    case MODE_SET_RESERVE2:
        ret = netsnmp_check_vb_max_size(requests->requestvb, MaxFilePath - 1);
        if ( ret != SNMP_ERR_NOERROR )
        {
            netsnmp_set_request_error(reqinfo, requests, ret);
        }
        break;
    case MODE_SET_FREE:
        break;
    case MODE_SET_ACTION:
        memcpy(fileName, requests->requestvb->val.string, requests->requestvb->val_len);
        filePath[requests->requestvb->val_len] = '\0';
        break;
    case MODE_SET_COMMIT:
        break;

    default:
        /* we should never get here, so this is a really bad error */
        snmp_log(LOG_ERR, "unknown mode (%d) in %s\n", reqinfo->mode, __func__);
        return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}

int HandleFileName(netsnmp_mib_handler *handler,
    netsnmp_handler_registration *reginfo,
    netsnmp_agent_request_info *reqinfo,
    netsnmp_request_info *requests)
{
    int ret;
    switch(reqinfo->mode)
    {
    case MODE_GET:
        snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR, fileName, strlen(fileName));
        break;
    case MODE_SET_RESERVE1:
    case MODE_SET_RESERVE2:
        ret = netsnmp_check_vb_max_size(requests->requestvb, MaxFileName - 1);
        if ( ret != SNMP_ERR_NOERROR )
        {
            netsnmp_set_request_error(reqinfo, requests, ret );
        }
        break;
    case MODE_SET_FREE:
        break;
    case MODE_SET_ACTION:
        memcpy(fileName, requests->requestvb->val.string, requests->requestvb->val_len);
        fileName[requests->requestvb->val_len] = '\0';
        break;
    case MODE_SET_COMMIT:
        break;

    default:
        /* we should never get here, so this is a really bad error */
        snmp_log(LOG_ERR, "unknown mode (%d) in %s\n", reqinfo->mode, __func__);
        return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}

int HandleLoadType(netsnmp_mib_handler *handler,
    netsnmp_handler_registration *reginfo,
    netsnmp_agent_request_info *reqinfo,
    netsnmp_request_info *requests)
{
    int ret;
    switch(reqinfo->mode)
    {
    case MODE_GET:
        snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER, &loadType, sizeof(int32_t));
        break;
    case MODE_SET_RESERVE1:
    case MODE_SET_RESERVE2:
        ret = netsnmp_check_vb_range(requests->requestvb, LoadTypeUpload, LoadTypeDownload);
        if (ret != SNMP_ERR_NOERROR)
        {
            netsnmp_set_request_error(reqinfo, requests, ret );
        }
        break;
    case MODE_SET_FREE:
        break;
    case MODE_SET_ACTION:
        loadType = *requests->requestvb->val.integer;
        break;
    case MODE_SET_COMMIT:
        break;

    default:
        /* we should never get here, so this is a really bad error */
        snmp_log(LOG_ERR, "unknown mode (%d) in %s\n", reqinfo->mode, __func__);
        return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}

int HandleDownloadStatus(netsnmp_mib_handler *handler,
    netsnmp_handler_registration *reginfo,
    netsnmp_agent_request_info *reqinfo,
    netsnmp_request_info *requests)
{
    int32_t status = GetUpdateStatus();
    switch(reqinfo->mode)
    {
    case MODE_GET:
        snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER, &status, sizeof(int32_t));
        break;

    default:
        /* we should never get here, so this is a really bad error */
        snmp_log(LOG_ERR, "unknown mode (%d) in %s\n", reqinfo->mode, __func__);
        return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}

int HandleAddressType(netsnmp_mib_handler *handler,
    netsnmp_handler_registration *reginfo,
    netsnmp_agent_request_info *reqinfo,
    netsnmp_request_info *requests)
{
    int32_t type = 1;
    switch(reqinfo->mode)
    {
    case MODE_GET:
        snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER, &type, sizeof(int32_t));
        break;

    default:
        /* we should never get here, so this is a really bad error */
        snmp_log(LOG_ERR, "unknown mode (%d) in %s\n", reqinfo->mode, __func__);
        return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}

int HandleProtocolType(netsnmp_mib_handler *handler,
    netsnmp_handler_registration *reginfo,
    netsnmp_agent_request_info *reqinfo,
    netsnmp_request_info *requests)
{
    string fileFullName;
    int ret, protocolType = 1;
    char ipStr[INET_ADDRSTRLEN];
    switch(reqinfo->mode)
    {
    case MODE_GET:
        snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER, &protocolType, sizeof(int32_t));
        break;
    case MODE_SET_RESERVE1:
        ret = netsnmp_check_vb_range(requests->requestvb, 1, 1);
        if (ret != SNMP_ERR_NOERROR)
        {
            netsnmp_set_request_error(reqinfo, requests, ret);
        }
        break;
    case MODE_SET_RESERVE2:
    case MODE_SET_FREE:
        break;
    case MODE_SET_ACTION:
        inet_ntop(AF_INET, (void*)&ftpIp, ipStr, INET_ADDRSTRLEN);
        fileFullName.append(filePath);
        fileFullName.append("/");
        fileFullName.append(fileName);
        AsynUpdateFromFtp(ipStr, userName, userPassword, fileFullName.c_str(), fileType);
        break;
    case MODE_SET_COMMIT:
        break;

    default:
        /* we should never get here, so this is a really bad error */
        snmp_log(LOG_ERR, "unknown mode (%d) in %s\n", reqinfo->mode, __func__);
        return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}

CxxEndNameSpace /* Snmp */
CxxEndNameSpace /* GsDslFtp */
