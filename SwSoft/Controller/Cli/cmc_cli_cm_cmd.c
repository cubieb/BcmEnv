//****************************************************************************
//
//  Copyright (c) 2011,2012  Broadcom Corporation
//  All Rights Reserved
//  No portions of this material may be reproduced in any form without the
//  written permission of: 
//          Broadcom Corporation
//          16251 Laguna Canyon Road
//          Irvine, California 92618
//  All information contained in this document is Broadcom Corporation
//  company private, proprietary, and trade secret.
//
//****************************************************************************
//

/* --- system includes ---*/
#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include "cparser.h"
#include "cparser_priv.h"
#include "cparser_token.h"
#include "cparser_tree.h"
#include <signal.h>
#include <stdarg.h>

/* --- project includes ---*/
#include "cli.h"
#include "cmcOam.h"
#include "cmcOamCm.h"
#include "cmcOamQos.h"

#include "CliCommon.h"
#include "CmMgt.h"

cparser_result_t
cparser_cmd_clear_cable_modem_mac(cparser_context_t *context, cparser_macaddr_t *mac_ptr)
{
    assert(context && mac_ptr);

    BASE_ErrCode	ret=0;
    MacAddressT		macAddr;
    memcpy(macAddr.addr, mac_ptr->octet, BRCM_MAC_LEN);

    ret = ClearCableModem(macAddr, cliGetCmc());
    return BaseErrCodeToCliResult(ret);
}

cparser_result_t
cparser_cmd_clear_cable_modem_all(cparser_context_t *context)
{
    assert(context);

    BASE_ErrCode	ret=0;
    MacAddressT		macAddr;

    memset(macAddr.addr, 0xff, BRCM_MAC_LEN);
    ret = ClearCableModem(macAddr, cliGetCmc());

    return BaseErrCodeToCliResult(ret);
}

cparser_result_t
cparser_cmd_show_cable_modem(cparser_context_t *context)
{
    CMCOAM_CmLstT cmList;
    assert(context);

    memset(&cmList, 0, sizeof(cmList));
    if(BASE_OK == CMCOAM_Cm_GetCnuList(&cmList, cliGetCmc()))
        CMCOAM_Cm_PrintCnuList(&cmList);

    return CPARSER_OK;
}

cparser_result_t
cparser_cmd_show_cable_modem_mac_classifier(cparser_context_t *context,
                                            cparser_macaddr_t *mac_ptr)
{
    MacAddressT		macAddr;

    assert(context&&mac_ptr);

    memcpy(&macAddr, mac_ptr->octet, sizeof(macAddr));

    if(CMCCTRL_Qos_ShowCmClassifier(macAddr, cliGetCmc()) != BASE_OK)
        return CPARSER_NOT_OK;

    return CPARSER_OK;

}

//show cable modem xx:xx:xx:xx:xx:xx service flow
cparser_result_t
cparser_cmd_show_cable_modem_mac_service_flow(cparser_context_t *context,
                                              cparser_macaddr_t *mac_ptr)
{
    MacAddressT		macAddr;

    assert(context&&mac_ptr);

    memcpy(&macAddr, mac_ptr->octet, sizeof(macAddr));

    if(CMCCTRL_Qos_ShowCmServiceFlow(macAddr, cliGetCmc()) != BASE_OK)
        return CPARSER_NOT_OK;

    return CPARSER_OK;
}

//show cable modem xx:xx:xx:xx:xx:xx status
cparser_result_t
cparser_cmd_show_cable_modem_mac_status(cparser_context_t *context,
                                        cparser_macaddr_t *mac_ptr)
{
    MacAddressT					macAddr;
    CMCOAM_GetCnuStatusRspT		rsp;

    assert(context&&mac_ptr);

    memset(&rsp, 0, sizeof(rsp));
    memcpy(&macAddr, mac_ptr->octet, sizeof(macAddr));

    if(BASE_OK == CMCOAM_Cm_GetCnuListStatus(&macAddr, 1, &rsp, cliGetCmc()))
    {
        CMCOAM_Cm_PrintCnuListStatus(&rsp);
        CMCOAM_Cm_FreeCnuListStatus(&rsp);
        return CPARSER_OK;
    }

    return CPARSER_NOT_OK;
}

cparser_result_t
cparser_cmd_show_cable_modem_status_all(cparser_context_t *context)
{
    CMCOAM_GetCnuStatusRspT rsp;
    CMCOAM_CmLstT           cmList;
    int i=0;

    assert(context);

    memset(&rsp, 0, sizeof(rsp));
    memset(&cmList, 0, sizeof(cmList));

    if(BASE_OK!=CMCOAM_Cm_GetCnuList(&cmList, cliGetCmc()))
        return CPARSER_NOT_OK;

    MacAddressT *pMac = (MacAddressT*)BASE_Malloc(cmList.num_of_cm * sizeof(MacAddressT));
    if(NULL == pMac)
        return CPARSER_NOT_OK;

    for(i=0; i<cmList.num_of_cm; i++)
    {
        memcpy((pMac+i)->addr, cmList.cm[i].mac.addr, sizeof(MacAddressT));
    }

    if(BASE_OK == CMCOAM_Cm_GetCnuListStatus(pMac, cmList.num_of_cm, &rsp, cliGetCmc()))
    {
        CMCOAM_Cm_PrintCnuListStatus(&rsp);
        CMCOAM_Cm_FreeCnuListStatus(&rsp);
    }

    BASE_Free(pMac);
    return CPARSER_OK;
}

cparser_result_t
cparser_cmd_show_cable_modem_mac_downstream(cparser_context_t *context,
                                            cparser_macaddr_t *mac_ptr)
{
    MacAddressT				macAddr;
    CMCOAM_GetCnuDsRspMsgT	rsp;

    assert(context&&mac_ptr);

    memset(&rsp, 0, sizeof(rsp));
    memcpy(&macAddr, mac_ptr->octet, sizeof(macAddr));

    if(CMCOAM_Cm_GetCnuDownstream(macAddr, &rsp, cliGetCmc()) != BASE_OK)
        return CPARSER_NOT_OK;

    BASE_ErrCode    ret;
    ret = CMCOAM_Cm_PrintCnuDownstream(&rsp);

    return BaseErrCodeToCliResult(ret);
}

cparser_result_t
cparser_cmd_show_cable_modem_mac_upstream(cparser_context_t *context,
                                          cparser_macaddr_t *mac_ptr)
{
    MacAddressT				macAddr;
    CMCOAM_GetCnuUsRspMsgT	rsp;

    assert(context && mac_ptr);

    memset(&rsp, 0, sizeof(rsp));
    memcpy(&macAddr, mac_ptr->octet, sizeof(macAddr));

    if(CMCOAM_Cm_GetCnuUpstream(macAddr, &rsp, cliGetCmc()) != BASE_OK)
        return CPARSER_NOT_OK;

    BASE_ErrCode    ret;
    ret = CMCOAM_Cm_PrintCnuUpstream(&rsp);

    return BaseErrCodeToCliResult(ret);
}

cparser_result_t
cparser_cmd_cable_modem_mac_dcc_chlst(cparser_context_t *context,
                                      cparser_macaddr_t *mac_ptr,
                                      char **chlst_ptr)
{
    MacAddressT     		macAddr;
    U8						ch_lst[BRCM_CMC_MAX_DS_CH];
    U8						num_of_ch=0;

    assert(context && mac_ptr && chlst_ptr && (*chlst_ptr));
    memset(&ch_lst, 0, sizeof(ch_lst));
    memcpy(&macAddr, mac_ptr->octet, sizeof(macAddr));

    char* token = strtok( *chlst_ptr, ",-|");
    while( (token != NULL) && (num_of_ch<BRCM_CMC_MAX_DS_CH) )
    {
        if(atoi(token)>BRCM_CMC_MAX_DS_CH)
            return CPARSER_NOT_OK;

        ch_lst[num_of_ch++] = atoi(token);

        /* Get next token: */
        token = strtok( NULL, ",-|");
    }

    BASE_ErrCode ret;
    ret = SetCnuDownstream(macAddr, ch_lst, num_of_ch, cliGetCmc());

    return BaseErrCodeToCliResult(ret);
}

cparser_result_t
cparser_cmd_cable_modem_mac_ucc_chlst(cparser_context_t *context,
                                      cparser_macaddr_t *mac_ptr,
                                      char **chlst_ptr)
{
    MacAddressT     		macAddr;
    U8						ch_lst[BRCM_CMC_MAX_US_CH];
    U8						num_of_ch=0;

	assert(context && mac_ptr && chlst_ptr && (*chlst_ptr));
	memset(&ch_lst, 0, sizeof(ch_lst));
	memcpy(&macAddr, mac_ptr->octet, sizeof(macAddr));
	
	char* token = strtok( *chlst_ptr, ",-|");
    while( (token != NULL) && (num_of_ch<BRCM_CMC_MAX_US_CH) )
    {
    	if(atoi(token)>BRCM_CMC_MAX_US_CH)
    		return CPARSER_NOT_OK;
    		
        ch_lst[num_of_ch++] = atoi(token);
        
        /* Get next token: */
        token = strtok( NULL, ",-|");
    }

    BASE_ErrCode ret;
    ret = SetCnuUpstream(macAddr, ch_lst, num_of_ch, cliGetCmc());

    return BaseErrCodeToCliResult(ret);
}  

cparser_result_t
cparser_cmd_clear_cable_modem_history(cparser_context_t *context)
{
    assert(context);

    BASE_ErrCode ret;
    ret = ClearCableModemHistory(cliGetCmc());

    return BaseErrCodeToCliResult(ret);
}

cparser_result_t
cparser_cmd_show_cable_modem_history(cparser_context_t *context)
{
    CMCOAM_CmHistoryEntryT cmHistoryEntry;
    assert(context);

    memset(&cmHistoryEntry, 0, sizeof(cmHistoryEntry));

    if(BASE_OK == CMCOAM_Cm_GetCableModemHistory(cliGetCmc(), &cmHistoryEntry))
    {
        CMCOAM_Cm_PrintCableModemHistory(&cmHistoryEntry);
        BASE_Free(cmHistoryEntry.pCmEntry);
    }

    return CPARSER_OK;
}


cparser_result_t
cparser_cmd_config_cable_cm_ctrl_mute_mac_channel_id_interval(cparser_context_t *context,
                                                              cparser_macaddr_t *mac_ptr,
                                                              uint32_t *channel_id_ptr,
                                                              uint32_t *interval_ptr)
{
    MacAddressT macAddr;
    assert(context && mac_ptr && channel_id_ptr && interval_ptr);

    memcpy(&macAddr, mac_ptr->octet, sizeof(macAddr));

    BASE_ErrCode ret;
    ret = SendCmCtlMute(macAddr, *channel_id_ptr, *interval_ptr, cliGetCmc());

    return BaseErrCodeToCliResult(ret);
}

cparser_result_t
cparser_cmd_config_cable_cm_ctrl_reinit_mac(cparser_context_t *context,
                                            cparser_macaddr_t *mac_ptr)
{
    MacAddressT macAddr;
    assert(context && mac_ptr);

    memcpy(&macAddr, mac_ptr->octet, sizeof(macAddr));

    BASE_ErrCode ret;
    ret = SendCmCtlReinit(macAddr, cliGetCmc());

    return BaseErrCodeToCliResult(ret);
}

cparser_result_t
cparser_cmd_config_cable_cm_ctrl_disable_forwarding_mac_forwarding(cparser_context_t *context,
                                                                   cparser_macaddr_t *mac_ptr,
                                                                   char **forwarding_ptr)
{
    MacAddressT macAddr;
    U8 forwarding;

    assert(context && mac_ptr && forwarding_ptr && (*forwarding_ptr));

    memcpy(&macAddr, mac_ptr->octet, sizeof(macAddr));
    forwarding = strncmp(*forwarding_ptr, "true", 4) ? 0 : 1;

    BASE_ErrCode ret;
    ret = SendCmCtlDisableForwarding(macAddr, forwarding, cliGetCmc());

    return BaseErrCodeToCliResult(ret);
}

cparser_result_t
cparser_cmd_show_cable_modem_mac_mmm_counter_clean(cparser_context_t *context,
                                                   cparser_macaddr_t *mac_ptr,
                                                   char **clean_ptr)
{
    return CPARSER_OK;
}

cparser_result_t
cparser_cmd_show_cable_modem_mmm_counter_all_clean(cparser_context_t *context,
                                                   char **clean_ptr)
{
    assert(context);
	
	CMCOAM_GetCmcCmMmmCntRspT rsp;
	CMCOAM_CmLstT             cmList;
	Bool                      bClean=FALSE;
	int i=0;
	
	assert(context);
	
	memset(&rsp, 0, sizeof(rsp));
	memset(&cmList, 0, sizeof(cmList));

	if(clean_ptr && *clean_ptr && !strncmp(*clean_ptr, "clean", 5))
		bClean = TRUE;

	if(BASE_OK != CMCOAM_Cm_GetCnuList(&cmList, cliGetCmc()))
		return CPARSER_NOT_OK;

	MacAddressT *pMac = (MacAddressT*)BASE_Malloc(cmList.num_of_cm * sizeof(MacAddressT));
	if(NULL == pMac)
		return CPARSER_NOT_OK;

	U8 num_of_cm = (cmList.num_of_cm > 255) ? 255 : cmList.num_of_cm;
	
	for(i=0; i<num_of_cm; i++)
	{
		memcpy((pMac+i)->addr, cmList.cm[i].mac.addr, sizeof(MacAddressT));
	}
	
	if(BASE_OK == CMCOAM_Cm_GetCmcCmMmmCnt(pMac, num_of_cm, bClean, &rsp, cliGetCmc()))
	{
		CMCOAM_Cm_PrintCmcCmMmmCnt(&rsp);
		CMCOAM_Cm_FreeCmcCmMmmCnt(&rsp);
	}
	
	BASE_Free(pMac);
	return CPARSER_OK;
}

#ifdef SUPPORT_DHCP_RELAY
cparser_result_t
cparser_cmd_show_cable_hosts(cparser_context_t *context)
{
	MacAddressT cm_mac;
	CMCOAM_GetCmHostsRspT rsp;
	
	assert(context);
	
	memset(cm_mac.addr, 0xff, sizeof(MacAddressT));	
	memset(&rsp, 0, sizeof(rsp));//important.
	
	if(BASE_OK != CMCOAM_Cm_GetCableModemHosts(&rsp, &cm_mac, cliGetCmc()))
		return BASE_OK;
	
	CMCOAM_Cm_PrintCableModemHosts(&rsp);

	return CMCOAM_Cm_FreeCableModemHosts(&rsp);
	
}

cparser_result_t
cparser_cmd_show_cable_modem_mac_hosts(cparser_context_t *context, cparser_macaddr_t *mac_ptr)
{
	MacAddressT cm_mac;
	CMCOAM_GetCmHostsRspT rsp;

	assert(context && mac_ptr);

	memcpy(cm_mac.addr, mac_ptr->octet, BRCM_MAC_LEN);
	memset(&rsp, 0, sizeof(rsp));//important.
	
	if(BASE_OK != CMCOAM_Cm_GetCableModemHosts(&rsp, &cm_mac, cliGetCmc()))
		return BASE_OK;
		
	CMCOAM_Cm_PrintCableModemHosts(&rsp);

	return CMCOAM_Cm_FreeCableModemHosts(&rsp);

}

#endif


