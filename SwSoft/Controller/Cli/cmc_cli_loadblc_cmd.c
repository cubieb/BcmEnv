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
#include "cmcOamLoadBalancing.h"

#include "CliCommon.h"
#include "LoadBalanceMgt.h"

/* create a load balance group with full settings.
 * cable load-balance group full <UINT:id>
 */
cparser_result_t
cparser_cmd_config_cable_load_balance_group_full_id(cparser_context_t *context,
                                                    uint32_t *id_ptr)
{
    LoadBalGrpCfgT  grpCfg;
	char				sc_input_buf[32];
	int					i, temp = 0;
	MacAddressT			mac;
	
	assert(context && id_ptr);

	//Init. the variables.
	memset(&grpCfg, 0, sizeof(LoadBalGrpCfgT));
	memset(sc_input_buf, 0, sizeof(sc_input_buf));

	grpCfg.id = *id_ptr;

	//printf("Enter the Load Balancing Type [static|dyn] : ");
    printf("Enter the Load Balancing Type [static] : "); //support static only
	//scanf("%s", sc_input_buf);
	cliGetString(sc_input_buf, sizeof(sc_input_buf));
	
	if(strcmp(sc_input_buf, "dyn") == 0)
	    grpCfg.type = DYNAMIC_LD_BLC;
	else//default use static.
	    grpCfg.type = STATIC_LD_BLC;

	//Downstream.
	do{
		printf("Enter the number of Downstrem Channels : ");	
		//scanf("%d", &temp);
		cliGetString(sc_input_buf, sizeof(sc_input_buf));
		temp = atoi(sc_input_buf);
	}while(temp > BRCM_CMC_MAX_DS_CH);
	grpCfg.dsChNumber = (U8)temp;

	if(grpCfg.dsChNumber > 0)
	{
	    for(i=0; i<grpCfg.dsChNumber; i++)
	    {
	        do
	        {
	            printf("Enter the Downstrem Channels %d : ", i+1);
	            //scanf("%d", &temp);
	            cliGetString(sc_input_buf, sizeof(sc_input_buf));
	            temp = atoi(sc_input_buf);
	        }while(temp>BRCM_CMC_MAX_DS_CH);
	        grpCfg.dsChList[i] = (U8)temp;
	    }
	}

	//Upstream.
	do{
	    printf("Enter the number of Upstream Channels : ");
	    //scanf("%d", &temp);
	    cliGetString(sc_input_buf, sizeof(sc_input_buf));
	    temp = atoi(sc_input_buf);
	}while(temp > BRCM_CMC_MAX_US_CH);
	grpCfg.usChNumber = (U8)temp;

	if(grpCfg.usChNumber > 0)
	{
	    for(i=0; grpCfg.usChNumber; i++)
	    {
	        do
	        {
	            printf("Enter the Upstream Channels %d : ", i+1);
	            //scanf("%d", &temp);
	            cliGetString(sc_input_buf, sizeof(sc_input_buf));
	            temp = atoi(sc_input_buf);

	        }while(temp>BRCM_CMC_MAX_US_CH);
	        grpCfg.usChList[i] = (U8)temp;
	    }
	}	

	//Mac Range.
	printf("Enter the number of Mac Ranges : ");	
	//scanf("%d", &temp);
	cliGetString(sc_input_buf, sizeof(sc_input_buf));
	temp = atoi(sc_input_buf);
	grpCfg.macRangeNum = (U8)temp;

	if(grpCfg.macRangeNum > 0)
	{
	    for(i=0; i<grpCfg.macRangeNum; i++)
	    {
	        do
	        {
	            printf("Enter the start Mac address %d: ", i+1);
	            //scanf("%s", sc_input_buf);
	            cliGetString(sc_input_buf, sizeof(sc_input_buf));
	        }while(cliParseCmMacAddr(sc_input_buf, &mac));
	        memcpy(grpCfg.macRange[i].start, &mac, BRCM_MAC_LEN);

	        do
	        {
	            printf("Enter the end Mac address %d(enter \"0\" to ignore),  : ", i+1);
	            //scanf("%s", sc_input_buf);
	            cliGetString(sc_input_buf, sizeof(sc_input_buf));
	        }while(cliParseCmMacAddr(sc_input_buf, &mac) && (atoi(sc_input_buf)!=0));
	        memcpy(grpCfg.macRange[i].end, &mac, BRCM_MAC_LEN);
	    }
	}

	BASE_ErrCode    ret;
	ret = SetFullLoadBalancingGrp(&grpCfg, cliGetCmc());

	return BaseErrCodeToCliResult(ret);
}


/* create a load balance group
 * //cable load-balance group <UINT:id> <LIST:static,dynamic:type>
 * cable load-balance group <UINT:id> <LIST:static:type> // static supported only now
 */
cparser_result_t
cparser_cmd_config_cable_load_balance_group_id_type(cparser_context_t *context,
                                                    uint32_t *id_ptr,
                                                    char **type_ptr)
{
    U8	method=STATIC_LD_BLC;

    assert(context && id_ptr && type_ptr && (*type_ptr));

    if(!strncmp(*type_ptr, "dynamic", 7))
        method = DYNAMIC_LD_BLC;
    else
        method = STATIC_LD_BLC;
 
    BASE_ErrCode    ret;
    ret = CreateLoadBalanceGrp((uint8_t)*id_ptr, method, cliGetCmc());

    return BaseErrCodeToCliResult(ret);
}

/* add downstream channel(s) to a load balance group, use \",\" to sepreate different channels
 * cable load-balance group <UINT:id> add downstream <STRING:chlst>
 */
cparser_result_t
cparser_cmd_config_cable_load_balance_group_id_add_downstream_chlst(cparser_context_t *context,
                                                                    uint32_t *id_ptr,
                                                                    char **chlst_ptr)
{
	U8	num_of_ch = 0;
	U8	ch_id[BRCM_CMC_MAX_DS_CH];
	
	assert(id_ptr && chlst_ptr && (*chlst_ptr));
	
	memset(ch_id, 0, sizeof(ch_id));
	char* token = strtok( *chlst_ptr, ",-|");
    while( (token != NULL) && (num_of_ch<BRCM_CMC_MAX_DS_CH) )
    {
    	if(atoi(token)>BRCM_CMC_MAX_DS_CH)
    		return CPARSER_NOT_OK;
    		
        ch_id[num_of_ch++] = atoi(token);
        
        /* Get next token: */
        token = strtok( NULL, ",-|");
    }

    BASE_ErrCode    ret;
    ret = AddDsChannelToLoadBalanceGrp((uint8_t)*id_ptr, ch_id, num_of_ch, cliGetCmc());

    return BaseErrCodeToCliResult(ret);
}

/*
 * add upstream channel(s) to a load balance group, use \",\" to sepreate different channels
 * cable load-balance group <UINT:id> add upstream <STRING:chlst>
 */
cparser_result_t
cparser_cmd_config_cable_load_balance_group_id_add_upstream_chlst(cparser_context_t *context,
    uint32_t *id_ptr,
    char **chlst_ptr)
{
	U8	num_of_ch = 0;
	U8	ch_id[BRCM_CMC_MAX_US_CH];
	
	assert(id_ptr && chlst_ptr && (*chlst_ptr));
	
	memset(ch_id, 0, sizeof(ch_id));
	char* token = strtok( *chlst_ptr, ",-|");
    while( (token != NULL) && (num_of_ch<BRCM_CMC_MAX_US_CH) )
    {
    	if(atoi(token)>BRCM_CMC_MAX_US_CH)
    		return CPARSER_NOT_OK;
    		
        ch_id[num_of_ch++] = atoi(token);
        
        /* Get next token: */
        token = strtok( NULL, ",-|");
    }
	
    BASE_ErrCode    ret;
    ret = AddUsChannelToLoadBalanceGrp((uint8_t)*id_ptr, ch_id, num_of_ch, cliGetCmc());

    return BaseErrCodeToCliResult(ret);
}

cparser_result_t cparser_cmd_config_cable_load_balance_group_id_add_modem_start_mac_end_mac(cparser_context_t *context,
    uint32_t *id_ptr,
    char **start_mac_ptr,
    char **end_mac_ptr)
{
	MacAddressT	start_mac, end_mac;
	
	assert(id_ptr && start_mac_ptr && (*start_mac_ptr));//end_mac_ptr is optional

	memset(&start_mac, 0, sizeof(start_mac));
	memset(&end_mac, 0, sizeof(end_mac));

	//command example.
	// 1) OUI case: add load balancing group cnu config 3 001018
	// 2)one cnu mac case: add load balancing group cnu config 3 0010.1811.1111
	// 3)cnu mac rang case: add load balancing group cnu config 3 0010.1811.1133
	if(NULL == end_mac_ptr)//case 1 and 2.
	{
		if(cliParseCmMacAddr(*start_mac_ptr, &start_mac) != BASE_OK)
			return CPARSER_ERR_INVALID_PARAMS;
		//start mac and end mac are the same.
		memcpy(&end_mac, &start_mac, sizeof(MacAddressT));
	}
	else if(NULL != end_mac_ptr)//case 3.
	{
		if((cliParseCmMacAddr(*start_mac_ptr, &start_mac) != BASE_OK) ||
			(cliParseCmMacAddr(*end_mac_ptr, &end_mac) != BASE_OK))
			return CPARSER_ERR_INVALID_PARAMS;
	}
	else
		return CPARSER_ERR_INVALID_PARAMS;

	BASE_ErrCode    ret;
    ret = AddCnuRangeToLoadBalanceGrp((uint8_t)*id_ptr, &start_mac, &end_mac, cliGetCmc());

    return BaseErrCodeToCliResult(ret);
}

cparser_result_t
cparser_cmd_config_cable_load_balance_group_exclude_modem_start_mac_end_mac(cparser_context_t *context,
    char **start_mac_ptr,
    char **end_mac_ptr)
{
	MacAddressT	start_mac, end_mac;
	
	assert(start_mac_ptr && (*start_mac_ptr));//end_mac_ptr is optional

	memset(&start_mac, 0, sizeof(start_mac));
	memset(&end_mac, 0, sizeof(end_mac));

	//command example.
	// 1) OUI case: add load balancing group cnu config 3 001018
	// 2)one cnu mac case: add load balancing group cnu config 3 0010.1811.1111
	// 3)cnu mac rang case: add load balancing group cnu config 3 0010.1811.1133
	if(NULL == end_mac_ptr)//case 1 and 2.
	{
		if(cliParseCmMacAddr(*start_mac_ptr, &start_mac) != BASE_OK)
			return CPARSER_ERR_INVALID_PARAMS;
		//start mac and end mac are the same.
		memcpy(&end_mac, &start_mac, sizeof(MacAddressT));
	}
	else if(NULL != end_mac_ptr)//case 3.
	{
		if((cliParseCmMacAddr(*start_mac_ptr, &start_mac) != BASE_OK) ||
			(cliParseCmMacAddr(*end_mac_ptr, &end_mac) != BASE_OK))
			return CPARSER_ERR_INVALID_PARAMS;
	}
	else
		return CPARSER_ERR_INVALID_PARAMS;

	BASE_ErrCode    ret;
    ret = AddCnuExcludeRange(&start_mac, &end_mac, cliGetCmc());

    return BaseErrCodeToCliResult(ret);
}

cparser_result_t
cparser_cmd_config_no_cable_load_balance_group_id(cparser_context_t *context,
    uint32_t *id_ptr)
{
	assert(context && id_ptr);
	
	//return CMCOAM_Group_CreateDestroyLoadBalancingGrp(*id_ptr, STATIC_LD_BLC, LD_BLC_DESTROY,cliGetCmc());
	BASE_ErrCode    ret;
	ret = RemoveLoadBalanceGrp((uint8_t)*id_ptr, cliGetCmc());
	return BaseErrCodeToCliResult(ret);
}

cparser_result_t
cparser_cmd_config_no_cable_load_balance_group_id_add_downstream_chlst(cparser_context_t *context,
    uint32_t *id_ptr,
    char **chlst_ptr)
{
	U8	num_of_ch = 0;
	U8	ch_id[BRCM_CMC_MAX_DS_CH];
	
	assert(id_ptr && chlst_ptr && (*chlst_ptr));
	
	memset(ch_id, 0, sizeof(ch_id));
	char* token = strtok( *chlst_ptr, ",-|");
    while( (token != NULL) && (num_of_ch<BRCM_CMC_MAX_DS_CH) )
    {
    	if(atoi(token)>BRCM_CMC_MAX_DS_CH)
    		return CPARSER_NOT_OK;
    		
        ch_id[num_of_ch++] = atoi(token);
        
        /* Get next token: */
        token = strtok( NULL, ",-|");
    }
	
    BASE_ErrCode    ret;
    ret = RemoveDsChannelFromLoadBalanceGrp((uint8_t)*id_ptr, ch_id, num_of_ch, cliGetCmc());

    return BaseErrCodeToCliResult(ret);
}

cparser_result_t
cparser_cmd_config_no_cable_load_balance_group_id_add_upstream_chlst(cparser_context_t *context,
    uint32_t *id_ptr,
    char **chlst_ptr)
{
	U8	num_of_ch = 0;
	U8	ch_id[BRCM_CMC_MAX_US_CH];
	
	assert(id_ptr && chlst_ptr && (*chlst_ptr));
	
	memset(ch_id, 0, sizeof(ch_id));
	char* token = strtok( *chlst_ptr, ",-|");
    while( (token != NULL) && (num_of_ch<BRCM_CMC_MAX_US_CH) )
    {
    	if(atoi(token)>BRCM_CMC_MAX_US_CH)
    		return CPARSER_NOT_OK;
    		
        ch_id[num_of_ch++] = atoi(token);
        
        /* Get next token: */
        token = strtok( NULL, ",-|");
    }
	
    BASE_ErrCode    ret;
    ret = RemoveUsChannelFromLoadBalanceGrp((uint8_t)*id_ptr, ch_id, num_of_ch, cliGetCmc());

    return BaseErrCodeToCliResult(ret);
}

cparser_result_t
cparser_cmd_config_no_cable_load_balance_group_id_add_modem_start_mac_end_mac(cparser_context_t *context,
    uint32_t *id_ptr,
    char **start_mac_ptr,
    char **end_mac_ptr)
{
	MacAddressT	start_mac, end_mac;
	
	assert(id_ptr && start_mac_ptr && (*start_mac_ptr));//end_mac_ptr is optional

	memset(&start_mac, 0, sizeof(start_mac));
	memset(&end_mac, 0, sizeof(end_mac));

	//command example.
	// 1) OUI case: add load balancing group cnu config 3 001018
	// 2)one cnu mac case: add load balancing group cnu config 3 0010.1811.1111
	// 3)cnu mac rang case: add load balancing group cnu config 3 0010.1811.1133
	if(NULL == end_mac_ptr)//case 1 and 2.
	{
		if(cliParseCmMacAddr(*start_mac_ptr, &start_mac) != BASE_OK)
			return CPARSER_ERR_INVALID_PARAMS;
		//start mac and end mac are the same.
		memcpy(&end_mac, &start_mac, sizeof(MacAddressT));
	}
	else if(NULL != end_mac_ptr)//case 3.
	{
		if((cliParseCmMacAddr(*start_mac_ptr, &start_mac) != BASE_OK) ||
			(cliParseCmMacAddr(*end_mac_ptr, &end_mac) != BASE_OK))
			return CPARSER_ERR_INVALID_PARAMS;
	}
	else
		return CPARSER_ERR_INVALID_PARAMS;

	BASE_ErrCode    ret;
	ret = RemoveCnuRangeFromLoadBalanceGrp((uint8_t)*id_ptr, &start_mac, &end_mac, cliGetCmc());

	return BaseErrCodeToCliResult(ret);
}

cparser_result_t
cparser_cmd_config_no_cable_load_balance_group_exclude_modem_start_mac_end_mac(cparser_context_t *context,
    char **start_mac_ptr,
    char **end_mac_ptr)
{
	MacAddressT	start_mac, end_mac;
	
	assert(start_mac_ptr && (*start_mac_ptr));//end_mac_ptr is optional

	memset(&start_mac, 0, sizeof(start_mac));
	memset(&end_mac, 0, sizeof(end_mac));

	//command example.
	// 1) OUI case: add load balancing group cnu config 3 001018
	// 2)one cnu mac case: add load balancing group cnu config 3 0010.1811.1111
	// 3)cnu mac rang case: add load balancing group cnu config 3 0010.1811.1133
	if(NULL == end_mac_ptr)//case 1 and 2.
	{
		if(cliParseCmMacAddr(*start_mac_ptr, &start_mac) != BASE_OK)
			return CPARSER_ERR_INVALID_PARAMS;
		//start mac and end mac are the same.
		memcpy(&end_mac, &start_mac, sizeof(MacAddressT));
	}
	else if(NULL != end_mac_ptr)//case 3.
	{
		if((cliParseCmMacAddr(*start_mac_ptr, &start_mac) != BASE_OK) ||
			(cliParseCmMacAddr(*end_mac_ptr, &end_mac) != BASE_OK))
			return CPARSER_ERR_INVALID_PARAMS;
	}
	else
		return CPARSER_ERR_INVALID_PARAMS;

    BASE_ErrCode    ret;
    ret = RemoveCnuExcludeRange(&start_mac, &end_mac, cliGetCmc());

    return BaseErrCodeToCliResult(ret);
}



cparser_result_t
cparser_cmd_show_cable_load_balancing_group_id_full(cparser_context_t *context, uint32_t *id_ptr)
{
	CMCOAM_GetFullLdBlcGrpRspT 	lb_grp_info;
	assert(context&&id_ptr);

	memset(&lb_grp_info, 0, sizeof(lb_grp_info));

	if(CMCOAM_Group_GetFullLoadBalancingGrpSettings(*id_ptr, &lb_grp_info, cliGetCmc()) != BASE_OK)
		return BASE_ERROR;

	return CMCOAM_Group_PrintFullLoadBalancingGrpSettings(&lb_grp_info, cliGetCmc());
}


cparser_result_t
cparser_cmd_show_cable_load_balancing_group(cparser_context_t *context)
{
    assert(context);

    CMCOAM_ShowLdBlcGrpRspT rspMsg;
    memset(&rspMsg, 0, sizeof(rspMsg));

    if(CMCOAM_Group_GetLoadBalancingGrp(&rspMsg, cliGetCmc()) != BASE_OK)
    	return BASE_ERROR;

    return CMCOAM_Group_PrintLoadBalancingGrp(&rspMsg, cliGetCmc());
}

cparser_result_t
cparser_cmd_show_cable_load_balancing_group_id_downstream(cparser_context_t *context, uint32_t *id_ptr)
{
	CMCOAM_GetLdBlcDsUsChRsqT 	rspMsg;
	
	assert(context && id_ptr);
	memset(&rspMsg, 0, sizeof(rspMsg));

	if(CMCOAM_Group_GetDsUsFromLoadBalancingGrp(*id_ptr, GetLoadBlcGrpDsRequest, &rspMsg, cliGetCmc()) != BASE_OK)
		return BASE_ERROR;
	
	return CMCOAM_Group_PrintDsUsFromLoadBalancingGrp(&rspMsg, cliGetCmc());
}

cparser_result_t
cparser_cmd_show_cable_load_balancing_group_id_upstream(cparser_context_t *context, uint32_t *id_ptr)
{
	CMCOAM_GetLdBlcDsUsChRsqT 	rspMsg;

	assert(context && id_ptr);
	memset(&rspMsg, 0, sizeof(rspMsg));	

	if(CMCOAM_Group_GetDsUsFromLoadBalancingGrp(*id_ptr, GetLoadBlcGrpUsRequest, &rspMsg,cliGetCmc()) != BASE_OK)
		return BASE_ERROR;
	
	return CMCOAM_Group_PrintDsUsFromLoadBalancingGrp(&rspMsg,cliGetCmc());
}

cparser_result_t
cparser_cmd_show_cable_load_balancing_group_id_modem(cparser_context_t *context, uint32_t *id_ptr)
{
	CMCOAM_GetLdBlcGrpCnuConfigRspT rspMsg;
	int 							i=0;
	
	assert(context && id_ptr);
	memset(&rspMsg, 0, sizeof(rspMsg));
	

	if(CMCOAM_Group_GetLoadBalancingGrpCnuConfig(*id_ptr, &rspMsg, cliGetCmc()) != BASE_OK)
		return BASE_ERROR;
	
	//Print the Response Message..
	printf("Load Balancing Group ID number = %u\n", rspMsg.lb_grp_id);
	printf("             Number of entries = %u\n", rspMsg.num_of_entries);

	for(i=0; i<rspMsg.num_of_entries; i++)
	{
		printf("      Starting CNU MAC address = %02x%02x.%02x%02x.%02x%02x\n",
			rspMsg.cnu_mac[i].start[0],rspMsg.cnu_mac[i].start[1],rspMsg.cnu_mac[i].start[2],
			rspMsg.cnu_mac[i].start[3],rspMsg.cnu_mac[i].start[4],rspMsg.cnu_mac[i].start[5]);
		printf("        Ending CNU MAC address = %02x%02x.%02x%02x.%02x%02x\n\n",
			rspMsg.cnu_mac[i].end[0],rspMsg.cnu_mac[i].end[1],rspMsg.cnu_mac[i].end[2],
			rspMsg.cnu_mac[i].end[3],rspMsg.cnu_mac[i].end[4],rspMsg.cnu_mac[i].end[5]);
	}	
	return BASE_OK;
}

cparser_result_t
cparser_cmd_show_cable_load_balancing_group_id_active_modem(cparser_context_t *context,
    uint32_t *id_ptr)
{
	CMCOAM_GetLdBlcGrpActCnuRspT 	rspMsg;
	int								i=0;
	assert(context && id_ptr);
	memset(&rspMsg, 0, sizeof(rspMsg));
	
	if(CMCOAM_Group_GetLoadBalancingGrpActiveCnuConfig(*id_ptr, &rspMsg, cliGetCmc()) != BASE_OK)
		return BASE_ERROR;

	//Print the respMessage.
	//Print the Response code.
	CMCOAM_PRINT("Load Balancing Group ID number = %u\n", rspMsg.lb_grp_id);
	CMCOAM_PRINT("                Number of Cnus = %u\n", rspMsg.num_of_cnu);

	for(i=0; i<rspMsg.num_of_cnu; i++)
	{
		CMCOAM_PRINT("               CNU MAC address = %02x%02x.%02x%02x.%02x%02x\n",
			rspMsg.cnu_mac[i][0], rspMsg.cnu_mac[i][1], rspMsg.cnu_mac[i][2], 
			rspMsg.cnu_mac[i][3], rspMsg.cnu_mac[i][4], rspMsg.cnu_mac[i][5]);
	}	
		
	return BASE_OK;
}

cparser_result_t
cparser_cmd_show_cable_load_balancing_group_excluded_modem(cparser_context_t *context)
{
	assert(context);
	
	CMCOAM_GetExcludedCnusConfigRspT rsp;
	memset(&rsp, 0, sizeof(rsp));

	if(CMCOAM_Group_GetExcludeCnusConfiguration(&rsp, cliGetCmc()) != BASE_OK)
		return BASE_ERROR;

	return CMCOAM_Group_PrintExcludeCnusConfiguration(&rsp, cliGetCmc());	
}

cparser_result_t
cparser_cmd_show_cable_load_balancing_group_excluded_active_modem(cparser_context_t *context)
{
	assert(context);
	
	CMCOAM_GetExcludedActCnusRspT rsp;
	memset(&rsp, 0, sizeof(rsp));


	if(CMCOAM_Group_GetExcludeActiveCnus(&rsp, cliGetCmc()) != BASE_OK)
		return BASE_ERROR;

	return CMCOAM_Group_PrintExcludeActiveCnus(&rsp,cliGetCmc());
}


//Dynamic Load balancing...
cparser_result_t
cparser_cmd_show_cable_load_balancing_dynamic_config(cparser_context_t *context)
{
    CMCOAM_GetLdBlcDynCfgRspT rsp;
    assert(context);
    memset(&rsp, 0, sizeof(rsp));

    BASE_ErrCode ret;
    ret = GetDynamicLoadBalanceConfig( &rsp );
    if ( BASE_OK != ret )
    {
        BASE_ERR("Get Dynamic LoadBalance Config Failed ! \n");
        return BASE_ERROR;
    }  

    return CMCOAM_Group_PrintLoadBalancingDynConfig(&rsp);
}

cparser_result_t
cparser_cmd_config_cable_load_balance_dynamic_method_value(cparser_context_t *context, char **value_ptr)
{
    U8 method;
    assert(context && value_ptr && *value_ptr);

    /*0=Disabled, 1=Dynamic (run-time statistics), 2=Static (minimum reserved rate)*/
    if(!strncmp(*value_ptr, "disable", 7))
    	method = LdBlcMethodDisabled;
    else if (!strncmp(*value_ptr, "dynamic", 7))
    	method = LdBlcMethodDynamic;
    else if (!strncmp(*value_ptr, "static", 6))	
    	method = LdBlcMethodStatic;
    else
    {
    	BASE_ERR("Invalid method!");
    	return BASE_ERROR;
    }

    BASE_ErrCode ret;
    ret = SetDynamicLoadBalanceMethod( &method );

    return BaseErrCodeToCliResult(ret);
}

cparser_result_t
cparser_cmd_config_cable_load_balance_dynamic_period_value(cparser_context_t *context, uint32_t *value_ptr)
{
    U32 period;
    assert(context && value_ptr);

    /*Time in seconds between load balancing checks. 
       Minimum time is 30 seconds. (0=no change)*/
    period = (*value_ptr < 30) ? 30 : *value_ptr;  

    BASE_ErrCode ret;
    ret = SetDynamicLoadBalancePeriod( &period );

    return BaseErrCodeToCliResult(ret);
}

cparser_result_t
cparser_cmd_config_cable_load_balance_dynamic_weighted_average_period_value(cparser_context_t *context, uint32_t *value_ptr)
{
    assert(context && value_ptr);

    BASE_ErrCode ret;
    ret = SetDynamicLoadWeightedAveragePeriod( value_ptr );

    return BaseErrCodeToCliResult(ret);
}

cparser_result_t
cparser_cmd_config_cable_load_balance_dynamic_overload_threshold_percent_value(cparser_context_t *context, uint32_t *value_ptr)
{
    assert(context && value_ptr);

    BASE_ErrCode ret;
    ret = SetDynamicLoadOverloadThreshold( value_ptr );

    return BaseErrCodeToCliResult(ret);
}

cparser_result_t
cparser_cmd_config_cable_load_balance_dynamic_difference_threshold_percent_value(cparser_context_t *context, uint32_t *value_ptr)
{
    assert(context && value_ptr);

    BASE_ErrCode ret;
    ret = SetDynamicLoadDiffThreshold( value_ptr );

    return BaseErrCodeToCliResult(ret);
}

cparser_result_t
cparser_cmd_config_cable_load_balance_dynamic_max_moves_value(cparser_context_t *context, uint32_t *value_ptr)
{
    CMCOAM_GetLdBlcDynCfgRspT dynLdBlcCfg;

    assert(context && value_ptr);

    memset(&dynLdBlcCfg, 0, sizeof(dynLdBlcCfg));

    /*Maximum number of modems allowed to be moved per load balancer period. (0=no change)*/
    if(*value_ptr == 0) /*no change...*/
    	return BASE_OK;
    
    BASE_ErrCode ret;
    ret = SetDynamicLoadMaxMove( value_ptr );

    return BaseErrCodeToCliResult(ret);	
}

cparser_result_t
cparser_cmd_config_cable_load_balance_dynamic_min_hold_time_value(cparser_context_t *context, uint32_t *value_ptr)
{
    CMCOAM_GetLdBlcDynCfgRspT dynLdBlcCfg;

    assert(context && value_ptr);

    memset(&dynLdBlcCfg, 0, sizeof(dynLdBlcCfg));

    /*Minimum number of seconds that must elapse before moving the 
       same modem to a different channel. (0=no change)*/
    if(*value_ptr == 0) /*no change...*/
    	return BASE_OK;

    BASE_ErrCode ret;
    ret = SetDynamicLoadMinHoldTime( value_ptr );

    return BaseErrCodeToCliResult(ret);	
}

cparser_result_t
cparser_cmd_config_cable_load_balance_dynamic_ranging_override_mode_value(cparser_context_t *context, char **value_ptr)
{
    assert(context && value_ptr && *value_ptr);
    U8 RangeOverrideMode;

    	/*1=Enabled (default), 2=Disabled. When enabled, 
       a modem can be directed to a different channel via 
       RNG-RSP prior to registration (0=no change)*/
    if(!strncmp(*value_ptr, "enable", 6))
    	RangeOverrideMode= RangOverrideModeEnable;
    else if (!strncmp(*value_ptr, "disable", 7))	
    	RangeOverrideMode = RangOverrideModeEnable;
    else
    {
    	BASE_ERR("Invalid parameter!");
    	return BASE_ERROR;
    }

    BASE_ErrCode ret;
    ret = SetDynamicLoadRangeOverrideMode( &RangeOverrideMode );

    return BaseErrCodeToCliResult(ret);	
}

cparser_result_t
cparser_cmd_config_cable_load_balance_dynamic_atdma_dcc_init_tech_value(cparser_context_t *context, uint32_t *value_ptr)
{
    assert(context && value_ptr);
    U8 AtdmaDccInitTech;

    /*DCC initialization technique used for ATDMA mode (valid values are 1, 2, 3, and 4.*/
    if((*value_ptr > 4) || (*value_ptr < 1))
    {
    	BASE_ERR("Only Inti-Tech 1,2,3,4 are supported for ATDMA");
    	return BASE_ERROR;
    }
    AtdmaDccInitTech = *value_ptr;

    BASE_ErrCode ret;
    ret = SetDynamicLoadAtdmaDccInitTechMode( &AtdmaDccInitTech );

    return BaseErrCodeToCliResult(ret);
}

cparser_result_t
cparser_cmd_config_cable_load_balance_dynamic_scdma_dcc_init_tech_value(cparser_context_t *context, uint32_t *value_ptr)
{
    assert(context && value_ptr);
    U8 ScdmaDccInitTech;

    /*DCC initialization technique used for SCDMA mode (valid values are 1, 2, and 3.*/
    if((*value_ptr > 3) || (*value_ptr < 1))
    {
    	BASE_ERR("Only Inti-Tech 1,2,3 are supported for SCDMA");
    	return BASE_ERROR;
    }
    ScdmaDccInitTech = *value_ptr;

    BASE_ErrCode ret;
    ret = SetDynamicLoadScdmaDccInitTechMode( &ScdmaDccInitTech );

    return BaseErrCodeToCliResult(ret);	
}

cparser_result_t
cparser_cmd_config_cable_load_balance_dynamic_atdma_dbc_init_tech_value(cparser_context_t *context, uint32_t *value_ptr)
{
    assert(context && value_ptr);
    U8 AtdmaDbcInitTech;

    /*DBC initialization technique used for ATDMA mode (valid values are 1, 2, 3, and 4.*/
    if((*value_ptr > 4) || (*value_ptr < 1))
    {
    	BASE_ERR("Only Inti-Tech 1,2,3,4 are supported for ATDMA");
    	return BASE_ERROR;
    }
    AtdmaDbcInitTech = *value_ptr;

    BASE_ErrCode ret;
    ret = SetDynamicLoadAtdmaDbcInitTechMode( &AtdmaDbcInitTech );

    return BaseErrCodeToCliResult(ret);	
}

cparser_result_t
cparser_cmd_config_cable_load_balance_dynamic_scdma_dbc_init_tech_value(cparser_context_t *context, uint32_t *value_ptr)
{
    assert(context && value_ptr);
    U8 ScdmaDbcInitTech;

    /*DBC initialization technique used for SCDMA mode (valid values are 1, 2, and 3.*/
    if((*value_ptr > 3) || (*value_ptr < 1))
    {
    	BASE_ERR("Only Inti-Tech 1,2,3 are supported for SCDMA");
    	return BASE_ERROR;
    }
    ScdmaDbcInitTech = *value_ptr;

    BASE_ErrCode ret;
    ret = SetDynamicLoadScdmaDbcInitTechMode( &ScdmaDbcInitTech );

    return BaseErrCodeToCliResult(ret);	
}


