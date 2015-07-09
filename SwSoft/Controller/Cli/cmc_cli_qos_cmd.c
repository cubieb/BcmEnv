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
#include "dsxMsgHandler.h"
#include "DsQosConfigMgt.h"
#include "CnuServiceMgt.h"
#include "CnuStatusMgt.h"

//create service flow xx:xx:xx:xx:xx:xx cos qos_tlv_file
cparser_result_t
cparser_cmd_config_cable_qos_service_flow_create_mac_cos_filename(cparser_context_t *context,
    cparser_macaddr_t *mac_ptr,
    uint32_t *cos_ptr,
    char **filename_ptr)
{
	int		numread=0;
	FILE	*stream;
	U8		file_buf[ 4000 ];
	MacAddressT		macAddr;
	
	assert(context && mac_ptr && cos_ptr && (*filename_ptr));
	memcpy(&macAddr, mac_ptr->octet, sizeof(macAddr));

	if(*cos_ptr > 7)
	{
		printf("Bad COS value, should not greater than 7\n");
		return BASE_ERROR_PARAM;
	}

	if( (stream = fopen( *filename_ptr, "r+b" )) != NULL )
	{
		numread = fread(file_buf, sizeof(char), 4000, stream);
		fclose(stream);	
	}
	else
	{
		BASE_ERR("Open file %s failed!", *filename_ptr);
		return BASE_ERROR_PARAM;
	}


	if( numread )
	{
		// non-zero number of bytes read.
		// special case:  last byte of config file should be
		// mutant type code 0xff.  it is not followed by a
		// length byte.  our TLV parser is not cool with
		// mutant T, no V!  we will check for the 0xff manually,
		// and if it is present, we will fudge the useful bytes
		// param to the BcmOctetBuffer structure.
		U8 last_byte = file_buf[ numread - 1 ];
		if( last_byte == 0xff )
		{
                // what we expect...
                numread--;
		}
	}
	
	return CMCCTRL_Dsx_CreateServiceFlow(macAddr,*cos_ptr,file_buf,numread, cliGetCmc());		
}

#if 1
cparser_result_t
cparser_cmd_config_cable_qos_service_flow_change_mac_usfid_dsfid_filename(cparser_context_t *context,
    cparser_macaddr_t *mac_ptr,
    uint32_t *usfid_ptr,
    uint32_t *dsfid_ptr,
    char **filename_ptr)
{
	int		numread=0;
	FILE	*stream;
	U8		file_buf[ 4000 ];
	MacAddressT		macAddr;
	assert(context && mac_ptr && usfid_ptr && dsfid_ptr && (*filename_ptr));
	memcpy(&macAddr, mac_ptr->octet, sizeof(macAddr));
	
	
	if( (stream = fopen( *filename_ptr, "r+b" )) != NULL )
	{
		numread = fread(file_buf, sizeof(char), 4000, stream);
		fclose(stream);	
	}
	else
	{
		BASE_ERR("Open file %s failed!", *filename_ptr);
		return BASE_ERROR_PARAM;
	}

	if( numread )
	{
		// non-zero number of bytes read.
		// special case:  last byte of config file should be
		// mutant type code 0xff.  it is not followed by a
		// length byte.  our TLV parser is not cool with
		// mutant T, no V!  we will check for the 0xff manually,
		// and if it is present, we will fudge the useful bytes
		// param to the BcmOctetBuffer structure.
		U8 last_byte = file_buf[ numread - 1 ];
		if( last_byte == 0xff )
		{
                // what we expect...
                numread--;
		}
	}
	
	return CMCCTRL_Dsx_ChangeServiceFlow(macAddr, *usfid_ptr, *dsfid_ptr, file_buf, numread, cliGetCmc());

}
#else
//change service flow xx:xx:xx:xx:xx:xx qos_tlv_file
cparser_result_t
cparser_cmd_config_cable_qos_service_flow_change_mac_filename(cparser_context_t *context,
    cparser_macaddr_t *mac_ptr,
    char **filename_ptr)
{
	int		numread=0;
	FILE	*stream;
	U8		file_buf[ 4000 ];
	MacAddressT		macAddr;
	memcpy(&macAddr, mac_ptr->octet, sizeof(macAddr));
	
	assert(context && mac_ptr && (*filename_ptr));

	
	if( (stream = fopen( *filename_ptr, "r+b" )) != NULL )
	{
		numread = fread(file_buf, sizeof(char), 4000, stream);
		fclose(stream);	
	}
	else
	{
		BASE_ERR("Open file %s failed!", *filename_ptr);
		return BASE_ERROR_PARAM;
	}


	if( numread )
	{
		// non-zero number of bytes read.
		// special case:  last byte of config file should be
		// mutant type code 0xff.  it is not followed by a
		// length byte.  our TLV parser is not cool with
		// mutant T, no V!  we will check for the 0xff manually,
		// and if it is present, we will fudge the useful bytes
		// param to the BcmOctetBuffer structure.
		U8 last_byte = file_buf[ numread - 1 ];
		if( last_byte == 0xff )
		{
                // what we expect...
                numread--;
		}
	}
	
	return CMCCTRL_Dsx_ChangeServiceFlow(macAddr, file_buf, numread, cliGetCmc());
}
#endif
//clear service flow xx:xx:xx:xx:xx:xx us_sfid ds_sfid
cparser_result_t
cparser_cmd_config_cable_qos_service_flow_clear_mac_usfid_dsfid(cparser_context_t *context,
    cparser_macaddr_t *mac_ptr,
    uint32_t *usfid_ptr,
    uint32_t *dsfid_ptr)
{
	MacAddressT		macAddr;
	
	assert(context && mac_ptr && usfid_ptr && dsfid_ptr);

	memcpy(&macAddr, mac_ptr->octet, sizeof(macAddr));
	
	
	return CMCCTRL_Dsx_DestroyServiceFlow(macAddr, *usfid_ptr, *dsfid_ptr, cliGetCmc());
}

cparser_result_t
cparser_cmd_show_cable_downstream_bonding_group(cparser_context_t *context)
{
	return CMCOAM_Qos_GetDsBondingGroup(cliGetCmc());
}

cparser_result_t
cparser_cmd_show_cable_upstream_bonding_group(cparser_context_t *context)
{
	return CMCOAM_Qos_GetUsBondingGroup(cliGetCmc());
}


cparser_result_t
cparser_cmd_show_cable_qos_service_flow_config_sfidlst(cparser_context_t *context,
    char **sfidlst_ptr)
{
	
	U16 num_of_sf=0;
	U32 sf_id[BRCM_CMC_MAX_DSX_SF];
	CMCOAM_ServiceFlowT *pSf=NULL;
	U8 ret_sf_cnt=0;
	int i=0;
	
	assert(context&&sfidlst_ptr&&(*sfidlst_ptr));
	
	char* token = strtok( *sfidlst_ptr, ",-|");
	memset(sf_id, 0, sizeof(sf_id));
	
    while( (token != NULL) && (num_of_sf<BRCM_CMC_MAX_DSX_SF) )
    {    		
        sf_id[num_of_sf++] = atoi(token);
        
        /* Get next token: */
        token = strtok( NULL, ",-|");
    }

	if((num_of_sf > 0) &&
		(NULL != (pSf=(CMCOAM_ServiceFlowT*)BASE_Malloc(num_of_sf * sizeof(CMCOAM_ServiceFlowT)))) && 
		((ret_sf_cnt=CMCOAM_Qos_GetServiceFlowConfig(sf_id, num_of_sf, pSf, cliGetCmc())) > 0))
	{
		for(i=0; i<ret_sf_cnt; i++)
		{
			CMCOAM_Qos_PrintServiceFlowConfig(pSf+i);
			BASE_Free((pSf+i)->pCluster);
		}
	}

	if(NULL!=pSf)
		BASE_Free(pSf);
	return BASE_OK;
}

cparser_result_t
cparser_cmd_show_cable_qos_service_flow_statistics_sfidlst(cparser_context_t *context,
    char **sfidlst_ptr)
{
	U16 num_of_sf=0;
	U32 sf_id[BRCM_CMC_MAX_DSX_SF];
	
	assert(context&&sfidlst_ptr&&(*sfidlst_ptr));
	
	char* token = strtok( *sfidlst_ptr, ",-|");
	memset(sf_id, 0, sizeof(sf_id));
	
    while( (token != NULL) && (num_of_sf<BRCM_CMC_MAX_DSX_SF) )
    {		
        sf_id[num_of_sf++] = atoi(token);
        
        /* Get next token: */
        token = strtok( NULL, ",-|");
    }
    return CMCOAM_Qos_GetServiceFlowStatistics(sf_id, num_of_sf, cliGetCmc());
}

cparser_result_t
cparser_cmd_show_cable_qos_classifier_config_sfidlst(cparser_context_t *context,
    char **sfidlst_ptr)
{
	U16 num_of_sf=0;
	U32 sf_id[BRCM_CMC_MAX_DSX_SF];
	
	assert(context&&sfidlst_ptr&&(*sfidlst_ptr));
	
	char* token = strtok( *sfidlst_ptr, ",-|");
	memset(sf_id, 0, sizeof(sf_id));
	
    while( (token != NULL) && (num_of_sf<BRCM_CMC_MAX_DSX_SF) )
    {		
        sf_id[num_of_sf++] = atoi(token);
        
        /* Get next token: */
        token = strtok( NULL, ",-|");
    }
	
	return CMCOAM_Qos_GetPktClassifierConfig(sf_id, num_of_sf, cliGetCmc());

}


cparser_result_t 
cparser_cmd_config_cable_qos_service_flow_class_name_name_add_filename(cparser_context_t *context, char **name_ptr, char **filename_ptr)
{
	int		numread=0;
	FILE	*stream;
	U8		file_buf[ 4000 ];
	
	assert(context && name_ptr && (*name_ptr) && filename_ptr && (*filename_ptr));
	
	if( (stream = fopen( *filename_ptr, "r+b" )) != NULL )
	{
		numread = fread(file_buf, sizeof(char), 4000, stream);
		fclose(stream);	
	}
	else
	{
		BASE_ERR("Open file %s failed!", *filename_ptr);
		return BASE_ERROR_PARAM;
	}

	if( numread )
	{
		// non-zero number of bytes read.
		// special case:  last byte of config file should be
		// mutant type code 0xff.  it is not followed by a
		// length byte.  our TLV parser is not cool with
		// mutant T, no V!  we will check for the 0xff manually,
		// and if it is present, we will fudge the useful bytes
		// param to the BcmOctetBuffer structure.
		U8 last_byte = file_buf[ numread - 1 ];
		if( last_byte == 0xff )
		{
                // what we expect...
                numread--;
		}
	}

	return CMCCTRL_Qos_SetServiceFlowClassName((const U8*)*name_ptr, ADD_CLS_NAME, file_buf, numread, cliGetCmc());
}

cparser_result_t
cparser_cmd_config_no_cable_qos_service_flow_class_name_name(cparser_context_t *context, char **name_ptr)
{
	assert(context && name_ptr && (*name_ptr));
	
	return CMCCTRL_Qos_SetServiceFlowClassName((const U8*)*name_ptr, DELETE_CLS_NAME, NULL, 0, cliGetCmc());
}

cparser_result_t
cparser_cmd_config_cable_qos_service_flow_reserve_filename(cparser_context_t *context,
    char **filename_ptr)
{
	int		numread=0;
	FILE	*stream;
	U8		file_buf[ 4000 ];
	
	assert(context && filename_ptr && (*filename_ptr));
	
	if( (stream = fopen( *filename_ptr, "r+b" )) != NULL )
	{
		numread = fread(file_buf, sizeof(char), 4000, stream);
		fclose(stream);	
	}
	else
	{
		BASE_ERR("Open file %s failed!", *filename_ptr);
		return BASE_ERROR_PARAM;
	}

	if( numread )
	{
		// non-zero number of bytes read.
		// special case:  last byte of config file should be
		// mutant type code 0xff.  it is not followed by a
		// length byte.  our TLV parser is not cool with
		// mutant T, no V!  we will check for the 0xff manually,
		// and if it is present, we will fudge the useful bytes
		// param to the BcmOctetBuffer structure.
		U8 last_byte = file_buf[ numread - 1 ];
		if( last_byte == 0xff )
		{
                // what we expect...
                numread--;
		}
	}
	
	return CMCCTRL_Qos_ReserveServiceFlow(file_buf, numread, cliGetCmc());
}



//change current existing service flows max sustained rate.
cparser_result_t
cparser_cmd_config_cable_qos_service_flow_change_max_sustained_rate_mac_sfid_rate_bps(cparser_context_t *context,
cparser_macaddr_t *mac_ptr,
uint32_t *sfid_ptr,
uint32_t *rate_bps_ptr)
{	
	MacAddressT		macAddr;
	
	assert(context && sfid_ptr && rate_bps_ptr);
	memcpy(&macAddr, mac_ptr->octet, sizeof(macAddr));

	return CMCOAM_Qos_ChangeServiceFlowMaxSustainedRate(macAddr, *sfid_ptr, *rate_bps_ptr, cliGetCmc());
}

//change current existing service flows qos set.
cparser_result_t
cparser_cmd_config_cable_qos_service_flow_change_qos_set_mac_sfid_qos_set(cparser_context_t *context,
cparser_macaddr_t *mac_ptr,
uint32_t *sfid_ptr,
uint32_t *qos_set_ptr)
{	
	MacAddressT		macAddr;
	
	assert(context && sfid_ptr && qos_set_ptr);
	memcpy(&macAddr, mac_ptr->octet, sizeof(macAddr));

	if(*qos_set_ptr > 7 || *qos_set_ptr<1)
	{
		BASE_ERR("Bad Qos Set Parameter!");
		return BASE_ERROR;
	}

	return CMCOAM_Qos_ChangeServiceFlowQosSet(macAddr, *sfid_ptr, *qos_set_ptr, cliGetCmc());
}

cparser_result_t
cparser_cmd_config_cable_qos_test_loop_time_max_cm(cparser_context_t *context, uint32_t *loop_time_ptr, uint32_t *max_cm_ptr)
{
	assert(context);

	U8 maxCmCntInOneLoop = max_cm_ptr ? *max_cm_ptr : 5;
	U32 loopTime = loop_time_ptr ? *loop_time_ptr : 1;

	CMCOAM_Qos_CmtsInitDsxAutoTest(maxCmCntInOneLoop, loopTime, cliGetCmc());
	return CPARSER_OK;
}

cparser_result_t
cparser_cmd_show_ds_qos_bandwidth(cparser_context_t *context)
{
	assert(context);
	DumpCnuMacToBandnwidthMapMgt();

	return CPARSER_OK;
}

cparser_result_t cparser_cmd_config_add_ds_qos_mac_address_mac_bandwidth_M_bandwidth(cparser_context_t *context,
    cparser_macaddr_t *mac_ptr,
    int32_t *bandwidth_ptr)
{
	MacAddressT macAddr;
	uint32_t ipaddress;
	assert(context && mac_ptr);
	BASE_ErrCode ret=BASE_ERROR;	

	memcpy(&macAddr, mac_ptr->octet, sizeof(macAddr));

	//add cm ds qos para to map mac to bandwidth
	DS_QoS_InfoT new_ds_qos;
	memset(&new_ds_qos, 0, sizeof(DS_QoS_InfoT));

	memcpy(&(new_ds_qos.ds_qos_cnu_mac), &macAddr, sizeof(MacAddressT));
	new_ds_qos.bandwidth = *bandwidth_ptr;
	
	if(AddMacToBandnwidthMapMgt(&new_ds_qos) != BASE_OK)
	{
		BASE_ERR(" AddMacToBandnwidthMapMgt! \n");
		return CPARSER_NOT_OK;
	}

	//search ip through mac in map mac to ip
	if(SearchCnuMacToIpMapMgt(macAddr , &ipaddress) != BASE_OK)
	{
		//BASE_ERR(" SearchCnuMacToIpMapMgt! \n");
		return CPARSER_NOT_OK;
	}

	//test
	//MgtDeleteDsQosARule(ipaddress);

	//add qos rules 
	ret = MgtAddDsQosARule(ipaddress, *bandwidth_ptr);

	if(ret != BASE_OK)
		return CPARSER_NOT_OK;    

	return CPARSER_OK;	
}

cparser_result_t cparser_cmd_config_delete_ds_qos_mac_address_mac(cparser_context_t *context,
    cparser_macaddr_t *mac_ptr)
{
	MacAddressT macAddr;
	uint32_t ipaddress;	
	assert(context && mac_ptr);
	BASE_ErrCode ret=BASE_ERROR;	

	memcpy(&macAddr, mac_ptr->octet, sizeof(macAddr));

	//search ip through mac in map mac to ip
	if(SearchCnuMacToIpMapMgt(macAddr , &ipaddress) != BASE_OK)
	{
		BASE_ERR(" SearchCnuMacToIpMapMgt! \n");
		return CPARSER_NOT_OK;
	}

	ret = MgtDeleteDsQosARule(ipaddress);

	if(ret != BASE_OK)
		return CPARSER_NOT_OK;  
	else
	{
		//add cm ds qos para to map mac to bandwidth
		if(DeleteMacToBandnwidthMapMgt(macAddr) != BASE_OK)
		{
			BASE_ERR(" DeleteMacToBandnwidthMapMgt! \n");
			return CPARSER_NOT_OK;
		}
	}

	return CPARSER_OK;
}

cparser_result_t cparser_cmd_config_show_downstream_qos_rules(cparser_context_t *context)
{
	assert(context);
	BASE_ErrCode ret=BASE_ERROR;		

	ret = MgtShowDsQosARule();

	if(ret != BASE_OK)
		return CPARSER_NOT_OK;  

	return CPARSER_OK;	
}

