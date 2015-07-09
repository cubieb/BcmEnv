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
#include "SystemInclude.h"
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
#include "cmcOamSystem.h"
#include "CnuStatusMgt.h"
#include "SystemInfoMgt.h"
#include "cmcController.h"

cparser_result_t
cparser_cmd_clear_cmc(cparser_context_t *context)
{
    assert(context);

    //Send reset command.
    CMCOAM_System_ResetCmcBoard(cliGetCmc());

    //Unregister the selected(this) cmc.
    return CMCCTRL_UnRegisterCmc(cliGetCmc());
}

cparser_result_t
cparser_cmd_show_cmc_version(cparser_context_t *context)
{
    char cmcVersionArray[CMC_MAX_VERSION_LENGTH]= {0};
    U8 length = 0;

    assert(context);

    if (BASE_OK == CMCOAM_System_GetSoftwareVersion(cmcVersionArray,&length,cliGetCmc()))
        CMCOAM_PRINT("CMC Version: %s\n",cmcVersionArray);

    return BASE_OK;
}


cparser_result_t
cparser_cmd_show_cmc_max_cm(cparser_context_t *context)
{
    U16 cm_num=0;

    if (BASE_OK == CMCOAM_System_GetNumOfCm(&cm_num,cliGetCmc()))
        CMCOAM_PRINT("Max Number of CM: %u\n", cm_num);

    return BASE_OK;
}

cparser_result_t
cparser_cmd_config_cmc_max_cm_num(cparser_context_t *context, uint32_t *num_ptr)
{

    assert(context && num_ptr);

    if(*num_ptr > BRCM_CMC_MAX_CM) {
        BASE_ERR("Max CM should not greater than %d!", BRCM_CMC_MAX_CM);
        return BASE_ERROR;
    }

    return CMCOAM_System_SetNumOfCm(*num_ptr,cliGetCmc());
}

cparser_result_t
cparser_cmd_show_cmc_max_multicasts(cparser_context_t *context)
{
    U16 multicast_num=0;

    assert(context);

    if (BASE_OK != CMCOAM_System_GetNumOfMulticasts(&multicast_num,cliGetCmc())) {
        BASE_ERR("faild to get max multicasts!\n");
        return BASE_ERROR;
    }

    CMCOAM_PRINT("Max Number of Multicasts: %u\n", multicast_num);

    return BASE_OK;
}

cparser_result_t cparser_cmd_show_system_alarm_list(cparser_context_t *context)
{
    assert(context);

    return CMCOAM_System_GetAlarmList(cliGetCmc());
}

cparser_result_t cparser_cmd_show_system_log_data(cparser_context_t *context)
{
    assert(context);

    return CMCOAM_System_GetLogData(cliGetCmc());
}

cparser_result_t cparser_cmd_config_cmc_async_log_zone_level(cparser_context_t *context,
        char **zone_ptr, char **level_ptr)
{
    U8 zone=kLogZonel_Invalid;
    U8 level=kLogLevel_Invalid;

    assert(context && zone_ptr && (*zone_ptr) && level_ptr && (*level_ptr));

    zone = CMCOAM_System_GetLogZoneCodeByName(*zone_ptr);
    level = kLogLevel1;//TBD......

    if(kLogZonel_Invalid != zone) {
        return CMCOAM_System_SetAsyncLogConfig(zone, level, cliGetCmc());
    } else {
        BASE_ERR("Get Zone code(%s) failed!\n", *zone_ptr);
        return BASE_ERROR;
    }

}

cparser_result_t cparser_cmd_config_no_cmc_async_log(cparser_context_t *context)
{
    assert(context);
    return CMCOAM_System_DisableAllAsyncLog(cliGetCmc());
}


cparser_result_t cparser_cmd_show_system_crash_data(cparser_context_t *context)
{
    assert(context);

    return CMCOAM_System_GetCrashData(cliGetCmc());

}

cparser_result_t cparser_cmd_clear_system_crash_data(cparser_context_t *context)
{
    assert(context);

    return CMCOAM_System_ClearCrashData(cliGetCmc());
}


cparser_result_t cparser_cmd_show_system_async_log_config(cparser_context_t *context)
{
    assert(context);
    return CMCOAM_System_GetAsyncLogConfig(cliGetCmc());
}

//cmc tpid <hex:id>
cparser_result_t
cparser_cmd_config_cmc_custom_config_tpid_tpid(cparser_context_t *context, uint32_t *tpid_ptr)
{
    assert(context && tpid_ptr);
    return CMCOAM_System_SetCustomConfiguration(kTPID, (const void*)tpid_ptr, 2, cliGetCmc());
}

cparser_result_t
cparser_cmd_config_cmc_custom_config_map_time_automatic(cparser_context_t *context)

{
    const U16 min=3200;
    const U16 max=8000;

    assert(context);
    CMCOAM_System_SetCustomConfiguration(kMinMapTime, (const void*)&min, 2, cliGetCmc());
    CMCOAM_System_SetCustomConfiguration(kMaxMapTime, (const void*)&max, 2, cliGetCmc());

    return BASE_OK;
}

cparser_result_t
cparser_cmd_config_cmc_custom_config_map_time_min_max(cparser_context_t *context, uint32_t *min_ptr, uint32_t *max_ptr)
{
    assert(context && min_ptr && max_ptr);

    if(*min_ptr > *max_ptr) {
        BASE_ERR("Min map time should not greater than max map time!");
        return BASE_ERROR;
    }

    CMCOAM_System_SetCustomConfiguration(kMinMapTime, (const void*)min_ptr, 2, cliGetCmc());
    CMCOAM_System_SetCustomConfiguration(kMaxMapTime, (const void*)max_ptr, 2, cliGetCmc());

    return BASE_OK;
}

cparser_result_t
cparser_cmd_config_cmc_custom_config_init_rng_period_milis(cparser_context_t *context, uint32_t *milis_ptr)
{
    assert(context && milis_ptr);
    return CMCOAM_System_SetCustomConfiguration(kInitRngPeriod, (const void*)milis_ptr, 4, cliGetCmc());
}

cparser_result_t
cparser_cmd_config_cmc_custom_config_periodic_rng_period_milis(cparser_context_t *context,    uint32_t *milis_ptr)
{
    assert(context && milis_ptr);
    return CMCOAM_System_SetCustomConfiguration(kPeriodicRngPeriod, (const void*)milis_ptr, 4, cliGetCmc());
}

cparser_result_t
cparser_cmd_config_cmc_custom_config_range_backoff_automatic(cparser_context_t *context)
{
    const U8 start = 2;
    const U8 end = 3;
    assert(context);

    CMCOAM_System_SetCustomConfiguration(kRngBackoffStart, (const void*)&start, 1, cliGetCmc());
    CMCOAM_System_SetCustomConfiguration(kRngBackoffEnd, (const void*)&end, 1, cliGetCmc());
    return BASE_OK;
}

cparser_result_t
cparser_cmd_config_cmc_custom_config_range_backoff_start_end(cparser_context_t *context, uint32_t *start_ptr, uint32_t *end_ptr)
{
    assert(context && start_ptr && end_ptr);

    if((*start_ptr > 15) || (*end_ptr > 15)) {
        BASE_ERR("Range backoff value should not greater than 15!");
        return BASE_ERROR;
    }

    if(*start_ptr > *end_ptr) {
        BASE_ERR("Start value should not greater than end value!");
        return BASE_ERROR;
    }

    CMCOAM_System_SetCustomConfiguration(kRngBackoffStart, (const void*)start_ptr, 1, cliGetCmc());
    CMCOAM_System_SetCustomConfiguration(kRngBackoffEnd, (const void*)end_ptr, 1, cliGetCmc());

    return BASE_OK;
}

cparser_result_t
cparser_cmd_config_cmc_custom_config_data_backoff_automatic(cparser_context_t *context)
{

    const U8 start = 2;
    const U8 end = 8;

    assert(context);

    CMCOAM_System_SetCustomConfiguration(kDataBackoffStart, (const void*)&start, 1, cliGetCmc());
    CMCOAM_System_SetCustomConfiguration(kDataBackoffEnd, (const void*)&end, 1, cliGetCmc());
    return BASE_OK;

}

cparser_result_t
cparser_cmd_config_cmc_custom_config_data_backoff_start_end(cparser_context_t *context, uint32_t *start_ptr, uint32_t *end_ptr)
{
    assert(context && start_ptr && end_ptr);

    if((*start_ptr > 15) || (*end_ptr > 15)) {
        BASE_ERR("Range backoff value should not greater than 15!");
        return BASE_ERROR;
    }

    if(*start_ptr > *end_ptr) {
        BASE_ERR("Start value should not greater than End value!");
        return BASE_ERROR;
    }

    CMCOAM_System_SetCustomConfiguration(kDataBackoffStart, (const void*)start_ptr, 1, cliGetCmc());
    CMCOAM_System_SetCustomConfiguration(kDataBackoffEnd, (const void*)end_ptr, 1, cliGetCmc());
    return CPARSER_OK;
}

cparser_result_t cparser_cmd_config_cmc_custom_config_maplead_time_adj_micros(cparser_context_t *context, int32_t *micros_ptr)
{
    assert(context && micros_ptr);
    return CMCOAM_System_SetCustomConfiguration(kMapLeadTimeAdjustment, (const void*)micros_ptr, 2, cliGetCmc());
}

cparser_result_t
cparser_cmd_show_cmc_custom_config_subtype(cparser_context_t *context, char **subtype_ptr)
{
    U8 subtype = 0;

    assert(context);

    if(NULL == subtype_ptr)
        return CMCOAM_System_GetCmcCustomCfg(0, cliGetCmc());
    else {
        if(kIlleagalSubType == (subtype = CMCOAM_System_GetCmcCustomSubTypeByName(*subtype_ptr))) {
            BASE_ERR("Invalid subtyp string %s", *subtype_ptr);
            return BASE_ERROR;
        }

        return CMCOAM_System_GetCmcCustomCfg(subtype, cliGetCmc());

    }

}

//cmc upgrade firmware <File>
cparser_result_t
cparser_cmd_config_cmc_upgrade_firmware_filename(cparser_context_t *context, char **filename_ptr)
{
    int fw_len=0;
    FILE * stream = NULL;
    U8  *pFileBuf = NULL;
    int     numread=0;

    assert(context && filename_ptr && (*filename_ptr));

    fw_len = CMCOAM_Utility_GetFileSize(*filename_ptr);
    if(fw_len <= 0) {
        BASE_ERR("Get file(%s) size(%d) failed!", *filename_ptr, fw_len);
        return BASE_ERROR;
    }

    if(NULL==(pFileBuf=BASE_Malloc(fw_len))) {
        BASE_ERR("Allocate %d memory failed!", fw_len);
        return BASE_ERROR_NOMEM;
    }



    if( NULL != (stream = fopen( *filename_ptr, "r+b" )) ) {
        numread = fread(pFileBuf, sizeof(char), fw_len, stream);
        fclose(stream);
    }

    if((numread<=0) || (numread!=fw_len)) {
        BASE_ERR("Read File(%s) failed, readed %s, but file lenght is %d!", *filename_ptr, numread, fw_len);
        BASE_Free(pFileBuf);
        return BASE_ERROR;
    }

    CMCOAM_PRINT("Read %d bytes for file %s\n", numread, *filename_ptr);

    //Call API function...

    CMCOAM_System_SetCmcFirmwareUpgrade(pFileBuf, numread, cliGetCmc());


    BASE_Free(pFileBuf);
    return BASE_OK;



}

cparser_result_t
cparser_cmd_show_system_clock(cparser_context_t *context)
{
    BASE_DateTimeT dateTime;

    assert(context);

    memset(&dateTime, 0, sizeof(dateTime));

    if(BASE_OK == CMCOAM_System_GetTime(cliGetCmc(), &dateTime))
        return CMCOAM_System_PrintTime(&dateTime);

    return CPARSER_NOT_OK;
}

cparser_result_t
cparser_cmd_config_cmc_clock_time_year_week(cparser_context_t *context,char **time_ptr, char **year_ptr, char **week_ptr)
{
    return CPARSER_OK;
}

cparser_result_t
cparser_cmd_config_cmc_clock_default(cparser_context_t *context)
{
    BASE_DateTimeT dateTime;
    assert(context);

    memset(&dateTime, 0, sizeof(dateTime));

    //Use the controller host system time as cmc's
    if( BASE_OK == BASE_GetLocalDateTime(&dateTime))
        return CMCOAM_System_SetTime(cliGetCmc(), &dateTime);

    return CPARSER_NOT_OK;
}

cparser_result_t
cparser_cmd_config_cmc_broadcast_vid_vidlist(cparser_context_t *context, char **vidlist_ptr)
{
    U16 num_of_vid=0;
    U16 vid[BRCM_CMC_MAX_VID];

    assert(context&&vidlist_ptr&&(*vidlist_ptr));

    char* token = strtok( *vidlist_ptr, ",-|");
    memset(vid, 0, sizeof(vid));

    while( (token != NULL) && (num_of_vid<BRCM_CMC_MAX_VID) ) {
        vid[num_of_vid++] = atoi(token);

        /* Get next token: */
        token = strtok( NULL, ",-|");
    }
    return CMCOAM_System_SetBroadcastVid(cliGetCmc(), vid, num_of_vid, kVid_Broadcast_ON);

}

cparser_result_t
cparser_cmd_config_no_cmc_broadcast_vid_vidlist(cparser_context_t *context, char **vidlist_ptr)
{
    U16 num_of_vid=0;
    U16 vid[BRCM_CMC_MAX_VID];

    assert(context&&vidlist_ptr&&(*vidlist_ptr));

    char* token = strtok( *vidlist_ptr, ",-|");
    memset(vid, 0, sizeof(vid));

    while( (token != NULL) && (num_of_vid<BRCM_CMC_MAX_VID) ) {
        vid[num_of_vid++] = atoi(token);

        /* Get next token: */
        token = strtok( NULL, ",-|");
    }
    return CMCOAM_System_SetBroadcastVid(cliGetCmc(), vid, num_of_vid, kVid_Broadcast_OFF);

}

cparser_result_t
cparser_cmd_show_cmc_broadcast_vid(cparser_context_t *context)
{
    U16 num_of_vid=0;
    U16 vid[BRCM_CMC_MAX_VID];

    assert(context);

    if(BASE_OK == CMCOAM_System_GetBroadcastVid(cliGetCmc(), vid, &num_of_vid)) {
        int i=0;
        CMCOAM_PRINT("Current VIDs in brodcast domain: ");
        for(i=0; i<num_of_vid; i++)
            CMCOAM_PRINT("%d ", vid[i]);
        CMCOAM_PRINT("\n");
    }

    return BASE_OK;
}

cparser_result_t
cparser_cmd_show_cmc_mmm_counter_chLst_clean(cparser_context_t *context,
        char **chLst_ptr, char **clean_ptr)
{
    assert(context);

    Bool bClean=FALSE;
    CMCOAM_GetMmmCntRspMsgT rsp;

    U8 chId[BRCM_CMC_MAX_US_CH];
    U8 chNum = 0;

    if(clean_ptr && *clean_ptr && !strncmp(*clean_ptr, "clean", 5))
        bClean = TRUE;

    memset(chId, 0, sizeof(chId));
    memset(&rsp, 0, sizeof(rsp));

    if(chLst_ptr && *chLst_ptr) {
        char* token = strtok( *chLst_ptr, ",-|");
        while( (token != NULL) && (chNum<BRCM_CMC_MAX_DS_CH) ) {
            if((atoi(token)<=BRCM_CMC_MAX_DS_CH) && (atoi(token)>0))
                chId[chNum++] = atoi(token);

            /* Get next token: */
            token = strtok( NULL, ",-|");
        }

    } else {
        //Get all channel settings.
        chNum = 0;
    }


    if(BASE_OK == CMCOAM_System_GetMmmCounts(cliGetCmc(), chId, chNum, bClean, &rsp)) {
        return CMCOAM_System_PrintMmmCounts(&rsp);
    }
    return BASE_OK;
}

cparser_result_t
cparser_cmd_show_cmc_map_counter_chLst_clean(cparser_context_t *context,
        char **chLst_ptr, char **clean_ptr)
{
    assert(context);

    Bool bClean=FALSE;
    CMCOAM_GetMapCntRspMsgT rsp;

    U8 chId[BRCM_CMC_MAX_US_CH];
    U8 chNum = 0;

    if(clean_ptr && *clean_ptr && !strncmp(*clean_ptr, "clean", 5))
        bClean = TRUE;

    memset(chId, 0, sizeof(chId));
    memset(&rsp, 0, sizeof(rsp));

    if(chLst_ptr && *chLst_ptr) {
        char* token = strtok( *chLst_ptr, ",-|");
        while( (token != NULL) && (chNum<BRCM_CMC_MAX_US_CH) ) {
            if((atoi(token)<=BRCM_CMC_MAX_US_CH) && (atoi(token)>0))
                chId[chNum++] = atoi(token);

            /* Get next token: */
            token = strtok( NULL, ",-|");
        }

    } else {
        //Get all channel settings.
        chNum = 0;
    }

    if(BASE_OK == CMCOAM_System_GetMapCounts(cliGetCmc(), chId, chNum, bClean, &rsp)) {
        return CMCOAM_System_PrintMapCounts(&rsp);
    }
    return BASE_OK;
}

#ifdef SUPPORT_DHCP_RELAY
cparser_result_t
cparser_cmd_config_cmc_dhcp_relay_relayip_interface_start(cparser_context_t *context,
        uint32_t *relayip_ptr, char **interface_ptr)
{
    assert(context && relayip_ptr);
    return CMCOAM_System_StartDhcpRelay(*relayip_ptr, *interface_ptr);
}

cparser_result_t
cparser_cmd_config_no_cmc_dhcp_relay_start(cparser_context_t *context)
{
    assert(context);
    return CMCOAM_System_StopDhcpRelay();
}
#endif

cparser_result_t
cparser_cmd_show_cpu_using(cparser_context_t *context)
{
    assert(context);
    float cpu_using_number;
    BASE_ErrCode ret=BASE_ERROR;

    ret = GetCpuUsingMgt(&cpu_using_number);
    if(ret == BASE_OK) {
        CMCOAM_PRINT("Cpu Using Number : %f\n" , cpu_using_number);
        return CPARSER_OK;
    } else
        return CPARSER_NOT_OK;
}

cparser_result_t
cparser_cmd_show_max_cpu_using(cparser_context_t *context)
{
    assert(context);
    float cpu_using_number;
    BASE_ErrCode ret=BASE_ERROR;

    ret = GetMaxCpuUsingMgt(&cpu_using_number);
    if(ret == BASE_OK) {
        CMCOAM_PRINT("Cpu Max Using Number : %f\n" , cpu_using_number);
        return CPARSER_OK;
    } else
        return CPARSER_NOT_OK;
}

cparser_result_t
cparser_cmd_show_max_cm_online_number(cparser_context_t *context)
{
    assert(context);
    int max_cm_online_number;
    BASE_ErrCode ret=BASE_ERROR;

    ret = GetMaxCnuOnlieMgt(&max_cm_online_number);
    if(ret == BASE_OK) {
        CMCOAM_PRINT("Cm Max Online Number : %d\n" , max_cm_online_number);
        return CPARSER_OK;
    } else
        return CPARSER_NOT_OK;
}

cparser_result_t
cparser_cmd_show_cm_online_number(cparser_context_t *context)
{
    assert(context);
    int cm_online_number;
    BASE_ErrCode ret=BASE_ERROR;

    ret = GetCnuOnlieMgt(&cm_online_number);
    if(ret == BASE_OK) {
        CMCOAM_PRINT("Cm Online Number : %d\n" , cm_online_number);
        return CPARSER_OK;
    } else
        return CPARSER_NOT_OK;
}

cparser_result_t
cparser_cmd_show_max_ram_using(cparser_context_t *context)
{
    assert(context);
    unsigned long  max_ram_using;
    BASE_ErrCode ret=BASE_ERROR;

    ret = GetMaxRamUsingMgt(&max_ram_using);
    if(ret == BASE_OK) {
        CMCOAM_PRINT("Max Ram Using : %ld\n" , max_ram_using);
        return CPARSER_OK;
    } else
        return CPARSER_NOT_OK;
}


cparser_result_t
cparser_cmd_show_ram_using(cparser_context_t *context)
{
    assert(context);
    unsigned long ram_using;
    BASE_ErrCode ret=BASE_ERROR;

    ret = GetRamUsingMgt(&ram_using);
    if(ret == BASE_OK) {
        CMCOAM_PRINT("Ram Using : %ld\n" , ram_using);
        return CPARSER_OK;
    } else
        return CPARSER_NOT_OK;
}

cparser_result_t cparser_cmd_config_set_max_cpu_using_max_cpu_using(cparser_context_t *context,
        double *max_cpu_using_ptr)
{
    assert(context);
    float max_cpu_using = *max_cpu_using_ptr;
    BASE_ErrCode ret=BASE_ERROR;

    ret = SetMaxCpuUsingMgt(max_cpu_using);
    if(ret == BASE_OK)
        return CPARSER_OK;
    else
        return CPARSER_NOT_OK;

}

cparser_result_t cparser_cmd_config_set_max_cm_online_number_max_cpu_online(cparser_context_t *context,
        int32_t *max_cpu_online_ptr)
{
    assert(context);
    int max_cpu_onlie = *max_cpu_online_ptr;
    BASE_ErrCode ret=BASE_ERROR;

    ret = SetMaxCnuOnlieMgt(max_cpu_onlie);
    if(ret == BASE_OK)
        return CPARSER_OK;
    else
        return CPARSER_NOT_OK;

}

cparser_result_t cparser_cmd_config_set_max_ram_using_max_ram_using(cparser_context_t *context,
        int32_t *max_ram_using_ptr)
{
    assert(context);
    int max_ram_using = *max_ram_using_ptr;
    BASE_ErrCode ret=BASE_ERROR;

    ret = SetMaxRamUsingMgt(max_ram_using);
    if(ret == BASE_OK)
        return CPARSER_OK;
    else
        return CPARSER_NOT_OK;
}

cparser_result_t cparser_cmd_show_max_us_snr(cparser_context_t *context)
{
    assert(context);
    BASE_ErrCode ret;
    unsigned int max_snr;
    int i;	

    //1.2.3.4
    CMCOAM_PRINT("\n");
    for(i = 1; i <= 4; i++) {
        ret = GetMaxUsSigMgt(i, &max_snr);
	 if(ret != BASE_OK)
        	return CPARSER_NOT_OK;	
	 CMCOAM_PRINT("Us Signal channel %d  max snr : %d dB\n" , i , max_snr);	
    }

    return CPARSER_OK;
}

cparser_result_t cparser_cmd_config_set_max_us_channel_us_channel_snr_db_us_snr(cparser_context_t *context,
        uint32_t *us_channel_ptr,
        int32_t *us_snr_ptr)

{
    assert(context && us_channel_ptr && us_snr_ptr);
    BASE_ErrCode ret;
    unsigned int channel = *us_channel_ptr;
    unsigned int max_snr = *us_snr_ptr;

    ret = SetMaxUsSigMgt(channel, max_snr);
    if(ret == BASE_OK)
        return CPARSER_OK;
    else
        return CPARSER_NOT_OK;
}

