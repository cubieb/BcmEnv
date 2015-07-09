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
#include "cmcOamChannel.h"

#include "ChannelMgt.h"

cparser_result_t
cparser_cmd_show_cable_downstream_channel(cparser_context_t *context, uint32_t *channel_ptr)
{
    U8 channel_id=0;
    CMCOAM_GetDsCfgRspMsgT	rspmsg;

    memset(&rspmsg, 0, sizeof(rspmsg));
    assert(context);

    if(NULL == channel_ptr)
        channel_id = 0;
    else
        channel_id = *channel_ptr;

    if(CMCOAM_Channel_GetDownstreamSettings(channel_id, &rspmsg, cliGetCmc()) != BASE_OK)
        return BASE_ERROR;

    return CMCOAM_Channel_PrintDownstreamSettings(&rspmsg);
}

cparser_result_t
cparser_cmd_show_cable_upstream_channel(cparser_context_t *context, uint32_t *channel_ptr)
{
    U8 channel_id=0;
    CMCOAM_GetUsCfgRspMsgT	rspmsg;

    memset(&rspmsg, 0, sizeof(rspmsg));
    assert(context);

    if(NULL == channel_ptr)
        channel_id = 0;
    else
        channel_id = *channel_ptr;

    if(CMCOAM_Channel_GetUpstreamSettings(channel_id, &rspmsg,cliGetCmc()) != BASE_OK)
        return BASE_ERROR;

    return CMCOAM_Channel_PrintUpstreamSettings(&rspmsg);
}

cparser_result_t 
cparser_cmd_config_cable_downstream_channel_shutdown(cparser_context_t *context, uint32_t *channel_ptr)
{
    BASE_ErrCode ret=BASE_ERROR;

    assert(context&&channel_ptr);

    ret = MgtChannelSetDownstreamSettingsEnabled(*channel_ptr,0);

    if(ret == BASE_OK)
    	return CPARSER_OK;
    else
	return CPARSER_NOT_OK;
}

cparser_result_t 
cparser_cmd_config_no_cable_downstream_channel_shutdown(cparser_context_t *context, uint32_t *channel_ptr)
{
    BASE_ErrCode ret=BASE_ERROR;	
	
    assert(context&&channel_ptr);
    ret = MgtChannelSetDownstreamSettingsEnabled(*channel_ptr,1);

    if(ret == BASE_OK)
    	return CPARSER_OK;
    else
	return CPARSER_NOT_OK;    	
}

cparser_result_t
cparser_cmd_config_cable_downstream_channel_mode_mode(cparser_context_t *context, uint32_t *channel_ptr, char **mode_ptr)
{
    U8 mode=0;
    BASE_ErrCode ret=BASE_ERROR;

    assert(context && channel_ptr && mode_ptr && (*mode_ptr));

    if(!strncmp(*mode_ptr, "docsis", 6))
        mode=1;
    else if(!strncmp(*mode_ptr, "ipqam", 5))
        mode=2;
    else
        {
            printf("example: cable downstream <channel> mode docsis|ipqam\n");
            return ret;
        }
    ret = MgtChannelSetDownstreamSettingsMode(*channel_ptr, mode);

    if(ret == BASE_OK)
    	return CPARSER_OK;
    else
	return CPARSER_NOT_OK; 

}

cparser_result_t
cparser_cmd_config_cable_downstream_channel_annex_annex_mode_symbolrate(cparser_context_t *context, uint32_t *channel_ptr, char **annex_mode_ptr, char **symbolrate_ptr)
{
    U8 annex=0;
    BASE_ErrCode ret=BASE_ERROR;
    
    assert(context && channel_ptr && annex_mode_ptr && (*annex_mode_ptr));
    if(!strncmp(*annex_mode_ptr, "a", 1))
        {
            if(symbolrate_ptr && (*symbolrate_ptr))
                {
                    if(!strncmp(*symbolrate_ptr, "6875000", 7))
                        annex=ANNEX_A_6875;
                    else if(!strncmp(*symbolrate_ptr, "6900000", 7))
                        annex=ANNEX_A_6900;
                    else
                        annex=ANNEX_A;
                }
            else
                annex=ANNEX_A;
        }
    else if(!strncmp(*annex_mode_ptr, "b", 1))
            {
                if(symbolrate_ptr && (*symbolrate_ptr))
                    {
                        BASE_ERR("Only allow to configure symbol rate in Annex A!");
                        return BASE_ERROR_PARAM;
                    }
                annex=ANNEX_B;
            }
            else
                {
                    printf("example: cable downstream <channel> annex a|b [symbolRate]\n");
                    return ret;
                }
    ret = MgtChannelSetDownstreamSettingsAnnexMode(*channel_ptr, annex);
    	
    if(ret == BASE_OK)
    	return CPARSER_OK;
    else
	return CPARSER_NOT_OK; 
}

cparser_result_t cparser_cmd_config_cable_downstream_channel_freq_freq(cparser_context_t *context, uint32_t *channel_ptr, uint32_t *freq_ptr)
{
    BASE_ErrCode ret=BASE_ERROR;
	
    assert(context && channel_ptr && freq_ptr);
	
    ret = MgtChannelSetDownstreamSettingsFreq(*channel_ptr, *freq_ptr);	

   
    	
    if(ret == BASE_OK)
    	return CPARSER_OK;
    else
	return CPARSER_NOT_OK; 	
}

cparser_result_t
cparser_cmd_config_cable_downstream_freq_auto_assign_start_step_mode(cparser_context_t *context, uint32_t *start_ptr, uint32_t *step_ptr, char **mode_ptr)
{
    BASE_ErrCode ret=BASE_ERROR;

    U8 assign_mode;
    assert(context && start_ptr && step_ptr && mode_ptr && (*mode_ptr));
    if(!strncmp(*mode_ptr, "increase", 8))
        assign_mode=1;
    else if(!strncmp(*mode_ptr, "decrease", 8))
        assign_mode=2;
    else
        {
            printf("only increase or decrase are supported!\n");
            return BASE_OK;
        }
    //return CMCOAM_Channel_AutoAssignDownstreamSettingsFreq(*start_ptr, *step_ptr,assign_mode,cliGetCmc());
    ret = MgtChannelAutoSetDownstreamSettingsFreq(*start_ptr, *step_ptr,assign_mode);

    if(ret == BASE_OK)
    	return CPARSER_OK;
    else
	return CPARSER_NOT_OK; 	    
    
}

cparser_result_t
cparser_cmd_config_cable_downstream_channel_interleaver_interleaver(cparser_context_t *context, uint32_t *channel_ptr, char **interleaver_ptr)
{
    BASE_ErrCode ret=BASE_ERROR;
    int Interleaver = 0;
    assert(context && channel_ptr && interleaver_ptr &&(*interleaver_ptr));
    Interleaver = CMCOAM_Channel_GetDownstreamInterleaverCodeByName(*interleaver_ptr);
    if(Interleaver < 0)
        {
            BASE_ERR("Get interlever code by name:%s failed!", *interleaver_ptr);
            return BASE_ERROR;
        }
    ret = MgtChannelSetDownstreamSettingsInterleaver(*channel_ptr, (U8)Interleaver);
   
    	
    if(ret == BASE_OK)
    	return CPARSER_OK;
    else
	return CPARSER_NOT_OK; 		
}

cparser_result_t cparser_cmd_config_cable_downstream_channel_interleaver(cparser_context_t *context, uint32_t *channel_ptr)
{
    return CPARSER_OK;
}
cparser_result_t cparser_cmd_config_cable_downstream_channel_modulation_modulation(cparser_context_t *context, uint32_t *channel_ptr, char **modulation_ptr)
{
    U8 modulation=0;
    BASE_ErrCode ret=BASE_ERROR;
    assert(context && channel_ptr && modulation_ptr && (*modulation_ptr));

    if(!strncmp(*modulation_ptr, "64QAM", 5))
        modulation=0;
    else if(!strncmp(*modulation_ptr, "256QAM", 6))
        modulation=1;
    else if(!strncmp(*modulation_ptr, "1024QAM",7))
        modulation=2;
    else
        {
            printf("downstream modulation type should be 64QAM,256QAM or 1024QAM!\n");
            return ret;
        }
    ret = MgtChannelSetDownstreamSettingsModulation(*channel_ptr, modulation);
   
    	
    if(ret == BASE_OK)
    	return CPARSER_OK;
    else
	return CPARSER_NOT_OK; 	

}

cparser_result_t
cparser_cmd_config_cable_downstream_channel_power_power(cparser_context_t *context, uint32_t *channel_ptr, int32_t *power_ptr)
{
    BASE_ErrCode ret=BASE_ERROR;
    assert(context && channel_ptr && power_ptr);
    ret =  MgtChannelSetDownstreamSettingsPowerLevel(*channel_ptr, *power_ptr);
   
    	
    if(ret == BASE_OK)
    	return CPARSER_OK;
    else
	return CPARSER_NOT_OK; 		
}

cparser_result_t
cparser_cmd_config_cable_downstream_auto_test_num_interval(cparser_context_t *context, uint32_t *num_ptr, uint32_t *interval_ptr)
{
    U16 num=0;
    U16 interval=0;
    assert(context);
    num = (num_ptr!=NULL) ? *num_ptr : 32;
    interval = (interval_ptr!=NULL) ? *interval_ptr : 0;
    return CMCOAM_Channel_SetDownstreamAutoTest(num, interval, cliGetCmc());
}

cparser_result_t
cparser_cmd_config_cable_upstream_channel_shutdown(cparser_context_t *context, uint32_t *channel_ptr)
{
    BASE_ErrCode ret=BASE_ERROR;
    assert(context && channel_ptr);
    ret = MgtChannelSetUpstreamSettingsEnabled(*channel_ptr , 0);
   
    	
    if(ret == BASE_OK)
    	return CPARSER_OK;
    else
	return CPARSER_NOT_OK; 	
}

cparser_result_t 
cparser_cmd_config_no_cable_upstream_channel_shutdown(cparser_context_t *context, uint32_t *channel_ptr)
{
    BASE_ErrCode ret=BASE_ERROR;
    assert(context && channel_ptr);
    ret = MgtChannelSetUpstreamSettingsEnabled(*channel_ptr , 1);
   
    	
    if(ret == BASE_OK)
    	return CPARSER_OK;
    else
	return CPARSER_NOT_OK; 	
}

cparser_result_t
cparser_cmd_config_cable_upstream_channel_freq_freq(cparser_context_t *context, uint32_t *channel_ptr, uint32_t *freq_ptr)
{
    BASE_ErrCode ret=BASE_ERROR;
    assert(context && channel_ptr && freq_ptr);
    ret = MgtChannelSetUpstreamSettingsFreq(*channel_ptr, *freq_ptr);
   
    	
    if(ret == BASE_OK)
    	return CPARSER_OK;
    else
	return CPARSER_NOT_OK; 		
}

cparser_result_t
cparser_cmd_config_cable_upstream_freq_auto_assign_start_step_mode(cparser_context_t *context, uint32_t *start_ptr, uint32_t *step_ptr, char **mode_ptr)
{
    U8 assign_mode;
    assert(context && start_ptr && step_ptr && mode_ptr && (*mode_ptr));
    if(!strncmp(*mode_ptr, "increase", 8))
    	assign_mode=1;
    else if(!strncmp(*mode_ptr, "decrease", 8))
    	assign_mode=2;
    else
    {
    	printf("only increase or decrase are supported!\n");
    	return BASE_OK;
    }
    //return CMCOAM_Channel_AutoAssignUpstreamSettingsFreq(*start_ptr, *step_ptr,assign_mode,cliGetCmc());
    return MgtChannelAutoSetUpstreamSettingsFreq(*start_ptr, *step_ptr,assign_mode);
}

cparser_result_t
cparser_cmd_config_cable_upstream_channel_channel_width_width(cparser_context_t *context, uint32_t *channel_ptr, char **width_ptr)
{
    BASE_ErrCode ret=BASE_ERROR;
    assert(context && channel_ptr && width_ptr && (*width_ptr));
    ret = MgtChannelSetUpstreamSettingsChannelWidth(*channel_ptr, atoi(*width_ptr));
   
    	
    if(ret == BASE_OK)
    	return CPARSER_OK;
    else
	return CPARSER_NOT_OK; 		
}

#if 1
cparser_result_t
cparser_cmd_config_cable_upstream_channel_profile_profile_type_group(cparser_context_t *context,
    uint32_t *channel_ptr,
    char **profile_ptr,
    char **type_ptr,    
    char **group_ptr)

{
    U8 channel_profile=0;
    char type[32];
    char group[32];

    assert(context && channel_ptr && profile_ptr && (*profile_ptr));

    memset(type, 0, sizeof(type));
    memset(group, 0, sizeof(group));

    //default use scdma.
    if(NULL == type_ptr)
    	strcpy(type, "scdma");
    else
    	strcpy(type, *type_ptr);

    //default use high noise.
    if(NULL == group_ptr)
    {
    	if(!strncmp(type, "atdma", 5))
    		strcpy(group, "medium_noise");
    	else
    		strcpy(group, "high_noise");
    }
    else
    	strcpy(group, *group_ptr);

    if(!strncmp(type, "scdma", 5) && !strncmp(group, "high_noise", 10))
    {
    	// SCDMA High Noise Profiles (lower throughput, but higher protection against noise)
    	// 0 = QPSK
    	// 1 = 16QAM
    	// 2 = 64QAM
    	// 3 = 256QAM
    	if(!strncmp(*profile_ptr, "QPSK", 4))
    		channel_profile=0;
    	else if(!strncmp(*profile_ptr, "16QAM", 5))
    		channel_profile=1;
    	else if(!strncmp(*profile_ptr, "64QAM",5))
    		channel_profile=2;
    	else if(!strncmp(*profile_ptr, "256QAM",6))
    		channel_profile=3;	
    	else
    	{
    		printf("upstream profile type should be QPSK,16QAM,64QAM or 256QAM for SCDMA High Noise Profiles!\n");
    		return BASE_ERROR;
    	}
    }
    else if(!strncmp(type, "scdma", 5) && !strncmp(group, "medium_noise", 12))
    {
    	// SCDMA Medium Noise Profiles (medium throughput, medium protection against noise):
    	// 4 = QPSK
    	// 5 = 16QAM
    	// 6 = 64QAM
    	// 7 = 256QAM
    	if(!strncmp(*profile_ptr, "QPSK", 4))
    		channel_profile=4;
    	else if(!strncmp(*profile_ptr, "16QAM", 5))
    		channel_profile=5;
    	else if(!strncmp(*profile_ptr, "32QAM", 5))
    		channel_profile=16;
    	else if(!strncmp(*profile_ptr, "64QAM",5))
    		channel_profile=6;
    	else if(!strncmp(*profile_ptr, "256QAM",6))
    		channel_profile=7;	
    	else
    	{
    		printf("upstream profile type should be QPSK,16QAM,32QAM,64QAM or 256QAM for SCDMA Medium Noise Profiles!\n");
    		return BASE_ERROR;
    	}

    }
    else if(!strncmp(type, "scdma", 5) && !strncmp(group, "low_noise", 9))
    {
    	// SCDMA Low Noise Profiles (high throughput, low protection against noise):
    	// 8 = 64QAM
    	// 9 = 256QAM
    	if(!strncmp(*profile_ptr, "64QAM",5))
    		channel_profile=8;
    	else if(!strncmp(*profile_ptr, "256QAM",6))
    		channel_profile=9;	
    	else
    	{
    		printf("upstream profile type should be 64QAM or 256QAM for SCDMA LOW Noise Profiles!\n");
    		return BASE_ERROR;
    	}
    }
    else if(!strncmp(type, "atdma", 5) && !strncmp(group, "medium_noise", 12))
    {
        // ATDMA Medium Noise Profiles (medium throughput, medium protection against noise):
        // 10 = QPSK
    	// 11 = 16QAM
    	// 12 = 64QAM
    	// 13 = 256QAM
    	if(!strncmp(*profile_ptr, "QPSK", 4))
    		channel_profile=10;
    	else if(!strncmp(*profile_ptr, "16QAM", 5))
    		channel_profile=11;
    	else if(!strncmp(*profile_ptr, "64QAM",5))
    		channel_profile=12;
    	else if(!strncmp(*profile_ptr, "256QAM",6))
    		channel_profile=13;	
    	else
    	{
    		printf("upstream profile type should be QPSK,16QAM,64QAM or 256QAM for ATDMA Medium Noise Profiles!\n");
    		return BASE_ERROR;
    	}

    }
    else if(!strncmp(type, "scdma", 5) && !strncmp(group, "low_latency", 11))
    {
    	// SCDMA Low Noise Profiles (high throughput, low protection against noise):
    	// 8 = 64QAM
    	// 9 = 256QAM
    	if(!strncmp(*profile_ptr, "64QAM",5))
    		channel_profile=14;
    	else if(!strncmp(*profile_ptr, "256QAM",6))
    		channel_profile=15;	
    	else
    	{
    		printf("upstream profile type should be 64QAM or 256QAM for SCDMA LOW Latency Profiles!\n");
    		return BASE_ERROR;
    	}
    }
    else if(!strncmp(type, "atdma", 5) && (!strncmp(group, "high_noise", 10) || !strncmp(group, "low_noise", 9) || !strncmp(group, "low_latency", 11)))
    {
    	printf("\"high noise\" \"low noise\" and \"low_latency\" profile are not supported in \"atdma mode\"\n");
    	return BASE_ERROR;
    }
    else
    {
    	printf("Error paramters! %s-%s-%s, \n", type, group, *profile_ptr);
    	return BASE_ERROR;
    }

    //return CMCOAM_Channel_SetUpstreamSettingsChannelProfile(*channel_ptr, channel_profile, cliGetCmc());
   return  MgtChannelSetUpstreamSettingsChannelProfile(*channel_ptr, channel_profile);
}
#else
//cable upstream <channel> profile <profile>
cparser_result_t 
cparser_cmd_config_cable_upstream_channel_profile_profile(cparser_context_t *context, uint32_t *channel_ptr, char **profile_ptr)
{
    U8 channel_profile=0;

    assert(context && channel_ptr && profile_ptr && (*profile_ptr));


    if(!strncmp(*profile_ptr, "QPSK", 4))
    	channel_profile=0;
    else if(!strncmp(*profile_ptr, "16QAM", 5))
    	channel_profile=1;
    else if(!strncmp(*profile_ptr, "64QAM",5))
    	channel_profile=2;
    else if(!strncmp(*profile_ptr, "256QAM",6))
    	channel_profile=3;	
    else
    {
    	printf("upstream profile type should be QPSK,16QAM,64QAM or 256QAM!\n");
    	return BASE_ERROR;
    }

    return CMCOAM_Channel_SetUpstreamSettingsChannelProfile(*channel_ptr, channel_profile, cliGetCmc());
}
#endif

cparser_result_t
cparser_cmd_config_cable_upstream_channel_power_level_power(cparser_context_t *context, uint32_t *channel_ptr, int32_t *power_ptr)
{
    BASE_ErrCode ret=BASE_ERROR;
    
    assert(context && channel_ptr && power_ptr);	
    ret = MgtChannelSetUpstreamSettingsPowerLevel(*channel_ptr, *power_ptr);
   
    	
    if(ret == BASE_OK)
	return CPARSER_OK;
    else
	return CPARSER_NOT_OK; 
}

cparser_result_t
cparser_cmd_config_cable_upstream_channel_docsis30_shutdown(cparser_context_t *context, uint32_t *channel_ptr)
{
    BASE_ErrCode ret=BASE_ERROR;

    assert(context && channel_ptr);
    return MgtChannelSetUpstreamSettingsD30Mode(*channel_ptr, 0);
   
    	
    if(ret == BASE_OK)
	return CPARSER_OK;
    else
	return CPARSER_NOT_OK;     
}

cparser_result_t
cparser_cmd_config_no_cable_upstream_channel_docsis30_shutdown(cparser_context_t *context, uint32_t *channel_ptr)
{
    BASE_ErrCode ret=BASE_ERROR;

    assert(context && channel_ptr);
    ret = MgtChannelSetUpstreamSettingsD30Mode(*channel_ptr, 1);
   
    	
    if(ret == BASE_OK)
	return CPARSER_OK;
    else
	return CPARSER_NOT_OK;        
}

cparser_result_t 
cparser_cmd_config_cable_upstream_channel_channel_type_type(cparser_context_t *context, uint32_t *channel_ptr, char **type_ptr)
{
    BASE_ErrCode ret=BASE_ERROR;

    U8 ch_type=0;
    if (context->parser->user_buf !=  NULL) 
    {
        //do save to configure file action
        printf("do save to configure file action\n");
        return BASE_OK;
    }  

    assert(context && type_ptr && (*type_ptr));
    if(!strncmp(*type_ptr, "atdma", 5))
    	ch_type=2;
    else if(!strncmp(*type_ptr, "scdma", 5))
    	ch_type=3;
    else
    {
    	printf("Only support ATDMA or SCDMA mode!\n");
    	return BASE_ERROR;
    }	
    ret = MgtChannelSetUpstreamChannelType(*channel_ptr, ch_type);
   
    	
    if(ret == BASE_OK)
	return CPARSER_OK;
    else
	return CPARSER_NOT_OK;       
}

cparser_result_t
cparser_cmd_show_cable_upstream_channel_power_level(cparser_context_t *context, uint32_t *channel_ptr)
{
    BASE_ErrCode ret=BASE_ERROR;

    CMCOAM_GetUsInputPowerRspMsgT	rsp;
    memset(&rsp, 0, sizeof(rsp));
    assert(context && channel_ptr);
    if(CMCOAM_Channel_GetUpstreamInputPower(*channel_ptr, &rsp, cliGetCmc()) != BASE_OK)
    	return BASE_ERROR;
    ret = CMCOAM_Channel_PrintUpstreamInputPower(&rsp);
   
    	
    if(ret == BASE_OK)
	return CPARSER_OK;
    else
	return CPARSER_NOT_OK;        
}

cparser_result_t 
cparser_cmd_show_cmc_interface_summary(cparser_context_t *context)
{
    assert(context);
    CMCOAM_CmcInterfaceInfoT 	ifc;
    memset(&ifc, 0, sizeof(ifc));
    if(CMCOAM_Channel_GetCmcInterfaces(&ifc, cliGetCmc()) != BASE_OK)
    	return BASE_ERROR;
    return CMCOAM_Channel_PrintCmcInterfaces(&ifc);
}

cparser_result_t 
cparser_cmd_show_cmc_interface_id_subtype(cparser_context_t *context, uint32_t *id_ptr, char **subtype_ptr)
{
    int channel_id;
    assert(context && subtype_ptr && *subtype_ptr);	
    channel_id = (NULL==id_ptr) ? 0 : *id_ptr;
    if(!strncmp(*subtype_ptr, "utilization", strlen("utilization")))
    {
    	CMCOAM_GetCmcInfUtilizationRspT rsp;
    	if(CMCOAM_Channel_GetCmcInterfaceUtilization(channel_id, &rsp, cliGetCmc()) != BASE_OK)
    		return BASE_ERROR;
    	return CMCOAM_Channel_PrintCmcInterfaceUtilization(&rsp);
    }
    else if(!strncmp(*subtype_ptr, "statistics", strlen("statistics")))
    {
    	CMCOAM_GetCmcInfStatisticsRspT	rsp;
    	if(CMCOAM_Channel_GetCmcInterfaceStatistics(channel_id, &rsp, cliGetCmc()) != BASE_OK)
    		return BASE_ERROR;
    	
    	return CMCOAM_Channel_PrintCmcInterfaceStatistics(&rsp);
    }
    else
    	return BASE_ERROR;
}

cparser_result_t 
cparser_cmd_show_cmc_interface_subtype(cparser_context_t *context, char **subtype_ptr)
{
    assert(context && subtype_ptr && *subtype_ptr);	
    return cparser_cmd_show_cmc_interface_id_subtype(context, NULL, subtype_ptr);
}

cparser_result_t 
cparser_cmd_show_cable_upstream_channel_signal_quality(cparser_context_t *context,  uint32_t *channel_ptr)
{
	CMCOAM_GetSignalQualityRspT	rsp;
	memset(&rsp, 0, sizeof(rsp));
	
	assert(context && channel_ptr);

	if(CMCOAM_Channel_GetSignalQuality(*channel_ptr, &rsp, cliGetCmc()) != BASE_OK)
		return BASE_ERROR;
	
    return CMCOAM_Channel_PrintSignalQuality(&rsp);
}

cparser_result_t 
cparser_cmd_show_cable_upstream_fft_config(cparser_context_t *context)
{
    assert(context);
    CMCOAM_UsFftConfig config;
    memset(&config, 0, sizeof(config));
    if(BASE_OK == CMCOAM_Channel_GetUsFftConfig(&config, cliGetCmc()) )
    	CMCOAM_Channel_PrintUsFftConfig(&config);
    else
    	BASE_ERR("Get upstream fft configuration failed!");
    return BASE_OK;
}

cparser_result_t 
cparser_cmd_config_cable_upstream_fft_averaging_shutdown(cparser_context_t *context)
{
    assert(context);
    CMCOAM_UsFftConfig config;
    memset(&config, 0, sizeof(config));
    if(BASE_OK == CMCOAM_Channel_GetUsFftConfig(&config, cliGetCmc()) )
    {
    	config.fft_averaging = 0;//disable.
    	//return CMCOAM_Channel_SetUsFftConfig(&config, cliGetCmc());
	return MgtChannelSetUsFftConfig(config.fft_averaging);
    }
    return BASE_OK;
}

cparser_result_t 
cparser_cmd_config_no_cable_upstream_fft_averaging_shutdown(cparser_context_t *context)
{
    assert(context);
    CMCOAM_UsFftConfig config;
    memset(&config, 0, sizeof(config));
    if(BASE_OK == CMCOAM_Channel_GetUsFftConfig(&config, cliGetCmc()) )
    {
    	config.fft_averaging = 1;//enable
    	//return CMCOAM_Channel_SetUsFftConfig(&config, cliGetCmc());
	return MgtChannelSetUsFftConfig(config.fft_averaging);
    }
    return BASE_OK;
}

cparser_result_t cparser_cmd_show_cable_upstream_fft_data(cparser_context_t *context)
{
    assert(context);
    CMCOAM_UsFftData data;	
    memset(&data, 0, sizeof(data));
    if(BASE_OK == CMCOAM_Channel_GetUsFftData(&data, cliGetCmc()) )
    {
    	CMCOAM_Channel_PrintUsFftData(&data);
    	CMCOAM_Channel_FreeUsFftData(&data);
    }
    return BASE_OK;	
}

