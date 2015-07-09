#include "SystemInclude.h"
#include "BcmController.h"

#include "cparser.h"
#include "cparser_priv.h"
#include "cparser_token.h"
#include "cparser_tree.h"

#include "LoadBalanceMgt.h"
#include "ChannelMgt.h"
#include "DhcpRelayMgt.h"
#include "UpdateMgt.h"
#include "DebugMgt.h"
#include "CliMain.h"

//for test
#include "CliCommon.h"
#include "SystemInfoMgt.h"
#include "CnuServiceMgt.h"
#include "CnuStatusMgt.h"
#include "StatisticsMgt.h"
#include "SnmpCfgMgt.h"

#include "BaseCommand.h"

cparser_result_t cparser_cmd_save_configure(cparser_context_t *context)
{
    if (SaveToCliConfig() != BASE_OK)
        return CPARSER_NOT_OK;

    if (!SaveSnmpCfg())
        return CPARSER_NOT_OK;

    return CPARSER_OK;
}

cparser_result_t cparser_cmd_recovery_factory_configure(cparser_context_t *context)
{
    //if (RestoreDefaultConfig() != BASE_OK)
    //return CPARSER_NOT_OK;
    RestoreDefaultConfig();

    return CPARSER_OK;
}


cparser_result_t
cparser_cmd_utest_load_balance_dynamic_create(cparser_context_t *context)
{
    return CPARSER_OK;
}

cparser_result_t
cparser_cmd_utest_load_balance_dynamic_clear(cparser_context_t *context)
{
    return CPARSER_OK;
}

cparser_result_t
cparser_cmd_utest_load_balance_dynamic_show(cparser_context_t *context)
{
    return CPARSER_OK;
}

cparser_result_t cparser_cmd_utest_load_balance_create(cparser_context_t *context)
{
//    const char* cmd[] =
//    {
//        "cable load-balance group 1 dynamic\n",
//        "cable load-balance group 1 add downstream 1\n",
//        "cable load-balance group 1 add downstream 2\n",
//        "cable load-balance group 1 add upstream 1\n",
//        "cable load-balance group 1 add upstream 2\n",
//        /* Add cnu mac ranges */
//        "cable load-balance group 1 add modem 01:01:01:01:01:01 01:01:01:01:01:02\n",
//        "cable load-balance group 1 add modem 01:01:01:01:01:04 01:01:01:01:01:05\n",
//        /* Add cnu mac exclude ranges */
//        "cable load-balance group exclude modem 01:01:01:01:02:01 01:01:01:01:02:02\n",
//        "cable load-balance group exclude modem 01:01:01:01:02:04 01:01:01:01:02:05\n",
//        "cable load-balance group 2 dynamic\n",
//        "cable load-balance group 3 dynamic\n",
//        "exit\n",
//    };

    CMCOAM_CmcAddrT* cmcId = cliGetCmc();
    uint8_t group = 1;
    uint8_t dsChannel[BRCM_CMC_MAX_DS_CH];
    uint8_t usChannel[BRCM_CMC_MAX_US_CH];

    CreateLoadBalanceGrp(group, DYNAMIC_LD_BLC, cmcId);
    dsChannel[0] = 1;
    AddDsChannelToLoadBalanceGrp(group, dsChannel, 1, cmcId);
    dsChannel[0] = 2;
    AddDsChannelToLoadBalanceGrp(group, dsChannel, 1, cmcId);

    usChannel[0] = 1;
    AddUsChannelToLoadBalanceGrp(group, usChannel, 1, cmcId);
    usChannel[0] = 2;
    AddUsChannelToLoadBalanceGrp(group, usChannel, 1, cmcId);

    MacAddressT startMac = {{0x1, 0x1, 0x1, 0x1, 0x1, 0x1}};
    MacAddressT endMac = {{0x1, 0x1, 0x1, 0x1, 0x1, 0x2}};
    AddCnuRangeToLoadBalanceGrp(group, &startMac, &endMac, cmcId);
    startMac.addr[5] = 0x4;
    endMac.addr[5] = 0x5;
    AddCnuRangeToLoadBalanceGrp(group, &startMac, &endMac, cmcId);

    startMac.addr[4] = 0x2;
    endMac.addr[4] = 0x2;
    startMac.addr[5] = 0x1;
    endMac.addr[5] = 0x2;
    AddCnuExcludeRange(&startMac, &endMac, cmcId);
    startMac.addr[5] = 0x4;
    endMac.addr[5] = 0x5;
    AddCnuExcludeRange(&startMac, &endMac, cmcId);

    group = 2;
    CreateLoadBalanceGrp(group, DYNAMIC_LD_BLC, cmcId);
    return CPARSER_OK;
}

cparser_result_t cparser_cmd_utest_load_balance_clear(cparser_context_t *context)
{
    ClearLoadBalanceConfig();
    return CPARSER_OK;
}

cparser_result_t cparser_cmd_utest_load_balance_show(cparser_context_t *context)
{
    UnitTestLoadBalanceMgtShow();
    return CPARSER_OK;
}

cparser_result_t cparser_cmd_debug_function_funcName(cparser_context_t *context,
        char **funcName_ptr)
{
    SetDebugStateOn(*funcName_ptr);
    return CPARSER_OK;
}

cparser_result_t cparser_cmd_no_debug_function_funcName(cparser_context_t *context,
        char **funcName_ptr)
{
    SetDebugStateOff(*funcName_ptr);
    return CPARSER_OK;
}

cparser_result_t cparser_cmd_utest_channel_data_create(cparser_context_t *context)
{
//US SIGNAL CABALITY TEST
#if 1

    CMCOAM_UpstreamIfcQualT FirstUsSigdata;
    CMCOAM_UpstreamIfcQualT NextUsSigdata;
    CMCOAM_UpstreamIfcQualT FindUsSigdata;

    MgtChannelGetFirstUsSignalQuality(&FirstUsSigdata);
    MgtChannelShowUsSignalQuality(&FirstUsSigdata);
    MgtChannelGetNextUsSignalQuality(&FirstUsSigdata , &NextUsSigdata);
    MgtChannelShowUsSignalQuality(&NextUsSigdata);
    MgtChannelFindUsSignalQuality(&FindUsSigdata , 2);
    MgtChannelShowUsSignalQuality(&FindUsSigdata);

#endif

//CNU STATUS INGO TEST
#if 0

    CMCOAM_PRINT("----------------FIRST CM SID : %d\n", now_cm_info_entry.cnu_index);
    CMCOAM_PRINT("					CM Mac Address : %02x%02x.%02x%02x.%02x%02x\n", now_cm_info_entry.cnu_mac[0], now_cm_info_entry.cnu_mac[1],
                 now_cm_info_entry.cnu_mac[2], now_cm_info_entry.cnu_mac[3], now_cm_info_entry.cnu_mac[4], now_cm_info_entry.cnu_mac[5]);
    CMCOAM_PRINT("	 Primary Downstream channel ID : %d\n", now_cm_info_entry.primary_ds_id);
    CMCOAM_PRINT("	   Primary Upstream channel ID : %d\n", now_cm_info_entry.primary_us_id);
    CMCOAM_PRINT("		Number of Upstream Channel : %d\n", now_cm_info_entry.up_ch_num);
    for(i=0; i<now_cm_info_entry.up_ch_num; i++) {
        up_chid = now_cm_info_entry.upstream[i].ch_id;
        CMCOAM_PRINT("			Upstream Channel ID(%u) : %u\n", up_chid, up_chid);
        CMCOAM_PRINT("		   Upstream Power Level(%u) : %.1f dBmv\n", up_chid, (now_cm_info_entry.upstream[i].power_level)/10.0);
        CMCOAM_PRINT("High Resolution Timing Offset(%u) : %d\n", up_chid, now_cm_info_entry.upstream[i].time_offset);
        CMCOAM_PRINT("	   Equalization Data Length(%u) : %u\n", up_chid, now_cm_info_entry.upstream[i].equ_data_len);
        if(now_cm_info_entry.upstream[i].equ_data_len > 0) {
            CMCOAM_PRINT("			  Equalization Data(%u) : \n", up_chid);
            //COAM_PRINT_BUF(now_cm_info_entry.upstream[i].equ_data, now_cm_info_entry.upstream[i].equ_data_len, 16);
        }
        CMCOAM_PRINT("			Unerrored Codewords(%u) : %llu\n", up_chid, now_cm_info_entry.upstream[i].unerr_code);
        CMCOAM_PRINT("			Corrected Codewords(%u) : %llu\n", up_chid, now_cm_info_entry.upstream[i].corrected_code);
        CMCOAM_PRINT("		Uncorrectable Codewords(%u) : %llu\n", up_chid, now_cm_info_entry.upstream[i].uncorrectted_code);
        CMCOAM_PRINT("				   Upstream SNR(%u) : %.1f dB\n", up_chid, (now_cm_info_entry.upstream[i].snr)/10.0);
        CMCOAM_PRINT("			  Micro-reflections(%u) : %d dBc\n", up_chid, now_cm_info_entry.upstream[i].micro_reflect);
    }
    CMCOAM_PRINT("	  Number of Downstream Channel : %u\n", now_cm_info_entry.ds_ch_num);
    if(now_cm_info_entry.ds_ch_num > 0) {
        CMCOAM_PRINT("			Downstream Channel IDs : ");
        for(i=0; i<now_cm_info_entry.ds_ch_num; i++)
            CMCOAM_PRINT("%u ", now_cm_info_entry.ds_ch_id[i]);
        CMCOAM_PRINT("\n");
    }
    CMCOAM_PRINT("			  CM IP Address Length : %u\n", now_cm_info_entry.ip_addr_len);
    if(now_cm_info_entry.ip_addr_len == 4) {
        CMCOAM_PRINT("					 CM IP Address : %u.%u.%u.%u\n", now_cm_info_entry.ip_addr[0], now_cm_info_entry.ip_addr[1], now_cm_info_entry.ip_addr[2], now_cm_info_entry.ip_addr[3]);

    } else {
        CMCOAM_PRINT("					 CM IP Address : ");
        //COAM_PRINT_BUF(now_cm_info_entry.ip_addr, now_cm_info_entry.ip_addr_len, 16);
    }
    CMCOAM_PRINT("						 Timestamp : %u\n", now_cm_info_entry.timestamp);
    CMCOAM_PRINT("	Upstream Service Flow ID Count : %u\n", now_cm_info_entry.us_sf_id_count);
    if(now_cm_info_entry.us_sf_id_count>0) {
        CMCOAM_PRINT("		 Upstream Service Flow IDs : ");
        for(i=0; i<now_cm_info_entry.us_sf_id_count; i++)
            CMCOAM_PRINT("%u,", now_cm_info_entry.us_sf_id[i]);
        CMCOAM_PRINT("\n");
    }
    CMCOAM_PRINT("Downstream Service Flow ID Count : %u\n", now_cm_info_entry.ds_sf_id_count);
    if(now_cm_info_entry.ds_sf_id_count > 0) {
        CMCOAM_PRINT("	   Downstream Service Flow IDs : ");
        for(i=0; i<now_cm_info_entry.ds_sf_id_count; i++)
            CMCOAM_PRINT("%u,", now_cm_info_entry.ds_sf_id[i]);
        CMCOAM_PRINT("\n");
    }
    CMCOAM_PRINT("						DSID Count : %u\n", now_cm_info_entry.dsid_count);
    if(now_cm_info_entry.dsid_count > 0) {
        CMCOAM_PRINT("							 DSIDs : ");
        for(i=0; i<now_cm_info_entry.dsid_count; i++)
            CMCOAM_PRINT("%u,", now_cm_info_entry.dsid[i]);
        CMCOAM_PRINT("\n");
    }
    CMCOAM_PRINT("	  Length of Link-Local Address : %u\n", now_cm_info_entry.link_addr_len);
    CMCOAM_PRINT("				Link-Local Address : ");
    //COAM_PRINT_BUF(now_cm_info_entry.link_addr, now_cm_info_entry.link_addr_len, 16);
    CMCOAM_PRINT("				  Service Group ID : %u\n", now_cm_info_entry.sg_id);
    CMCOAM_PRINT("				  Service Group ID : %u\n", now_cm_info_entry.sg_id);
    CMCOAM_PRINT("					 RCP-ID length : %u\n", now_cm_info_entry.rcpid_len);
    CMCOAM_PRINT("							RCP-ID : ");
    //COAM_PRINT_BUF(now_cm_info_entry.rcpid, now_cm_info_entry.rcpid_len, 16);
    CMCOAM_PRINT("							RCC-ID : %u\n", now_cm_info_entry.rccid);
    CMCOAM_PRINT("							RCS-ID : %u\n", now_cm_info_entry.rcsid);
    CMCOAM_PRINT("							TCS-ID : %u\n", now_cm_info_entry.tcsid);
    CMCOAM_PRINT("		 Last Registered Timestamp : %u\n", now_cm_info_entry.lr_timestamp);
    CMCOAM_PRINT("						  MTC Mode : %s\n", now_cm_info_entry.mtc ? "Enabled" : "Disabled");
    CMCOAM_PRINT("			 Fragmentation support : %s\n", now_cm_info_entry.fragmentation ? "Enabled" : "Disabled");
    CMCOAM_PRINT("			 Concatenation support : %s\n", now_cm_info_entry.concatenation ? "Enabled" : "Disabled");
    CMCOAM_PRINT("					   PHS support : %s\n", now_cm_info_entry.phs ? "Enabled" : "Disabled");
    CMCOAM_PRINT("			  Advanced PHY support : %s\n", now_cm_info_entry.adv_phy ? "Enabled" : "Disabled");
    CMCOAM_PRINT("		   Load Balancing Group ID : %u\n", now_cm_info_entry.lb_grp_id);
    CMCOAM_PRINT("			  QoS Provisioned mode : %s\n", now_cm_info_entry.qos_prov_mode ? "v1.1" : "v1.0");
    CMCOAM_PRINT("\n\n");

#if 0
    if(GetNextCMInfoMgt(&now_cm_info_entry,&next_cm_info_entry) != BASE_OK)
        return BASE_ERROR;

    CMCOAM_PRINT("	----------------SECOND CM SID : %d\n", next_cm_info_entry.cnu_index);
    CMCOAM_PRINT("					CM Mac Address : %02x%02x.%02x%02x.%02x%02x\n", next_cm_info_entry.cnu_mac[0], next_cm_info_entry.cnu_mac[1],
                 next_cm_info_entry.cnu_mac[2], next_cm_info_entry.cnu_mac[3], next_cm_info_entry.cnu_mac[4], next_cm_info_entry.cnu_mac[5]);
    CMCOAM_PRINT("	 Primary Downstream channel ID : %d\n", next_cm_info_entry.primary_ds_id);
    CMCOAM_PRINT("	   Primary Upstream channel ID : %d\n", next_cm_info_entry.primary_us_id);
    CMCOAM_PRINT("		Number of Upstream Channel : %d\n", next_cm_info_entry.up_ch_num);
    for(i=0; i<next_cm_info_entry.up_ch_num; i++) {
        up_chid = next_cm_info_entry.upstream[i].ch_id;
        CMCOAM_PRINT("			Upstream Channel ID(%u) : %u\n", up_chid, up_chid);
        CMCOAM_PRINT("		   Upstream Power Level(%u) : %.1f dBmv\n", up_chid, (next_cm_info_entry.upstream[i].power_level)/10.0);
        CMCOAM_PRINT("High Resolution Timing Offset(%u) : %d\n", up_chid, next_cm_info_entry.upstream[i].time_offset);
        CMCOAM_PRINT("	   Equalization Data Length(%u) : %u\n", up_chid, next_cm_info_entry.upstream[i].equ_data_len);
        if(next_cm_info_entry.upstream[i].equ_data_len > 0) {
            CMCOAM_PRINT("			  Equalization Data(%u) : \n", up_chid);
            CMCOAM_PRINT_BUF(next_cm_info_entry.upstream[i].equ_data, next_cm_info_entry.upstream[i].equ_data_len, 16);
        }
        CMCOAM_PRINT("			Unerrored Codewords(%u) : %llu\n", up_chid, next_cm_info_entry.upstream[i].unerr_code);
        CMCOAM_PRINT("			Corrected Codewords(%u) : %llu\n", up_chid, next_cm_info_entry.upstream[i].corrected_code);
        CMCOAM_PRINT("		Uncorrectable Codewords(%u) : %llu\n", up_chid, next_cm_info_entry.upstream[i].uncorrectted_code);
        CMCOAM_PRINT("				   Upstream SNR(%u) : %.1f dB\n", up_chid, (next_cm_info_entry.upstream[i].snr)/10.0);
        CMCOAM_PRINT("			  Micro-reflections(%u) : %d dBc\n", up_chid, next_cm_info_entry.upstream[i].micro_reflect);
    }
    CMCOAM_PRINT("	  Number of Downstream Channel : %u\n", next_cm_info_entry.ds_ch_num);
    if(next_cm_info_entry.ds_ch_num > 0) {
        CMCOAM_PRINT("			Downstream Channel IDs : ");
        for(i=0; i<next_cm_info_entry.ds_ch_num; i++)
            CMCOAM_PRINT("%u ", next_cm_info_entry.ds_ch_id[i]);
        CMCOAM_PRINT("\n");
    }
    CMCOAM_PRINT("			  CM IP Address Length : %u\n", next_cm_info_entry.ip_addr_len);
    if(next_cm_info_entry.ip_addr_len == 4) {
        CMCOAM_PRINT("					 CM IP Address : %u.%u.%u.%u\n", next_cm_info_entry.ip_addr[0], next_cm_info_entry.ip_addr[1], next_cm_info_entry.ip_addr[2], next_cm_info_entry.ip_addr[3]);

    } else {
        CMCOAM_PRINT("					 CM IP Address : ");
        CMCOAM_PRINT_BUF(next_cm_info_entry.ip_addr, next_cm_info_entry.ip_addr_len, 16);
    }
    CMCOAM_PRINT("						 Timestamp : %u\n", next_cm_info_entry.timestamp);
    CMCOAM_PRINT("	Upstream Service Flow ID Count : %u\n", next_cm_info_entry.us_sf_id_count);
    if(next_cm_info_entry.us_sf_id_count>0) {
        CMCOAM_PRINT("		 Upstream Service Flow IDs : ");
        for(i=0; i<next_cm_info_entry.us_sf_id_count; i++)
            CMCOAM_PRINT("%u,", next_cm_info_entry.us_sf_id[i]);
        CMCOAM_PRINT("\n");
    }
    CMCOAM_PRINT("Downstream Service Flow ID Count : %u\n", next_cm_info_entry.ds_sf_id_count);
    if(next_cm_info_entry.ds_sf_id_count > 0) {
        CMCOAM_PRINT("	   Downstream Service Flow IDs : ");
        for(i=0; i<next_cm_info_entry.ds_sf_id_count; i++)
            CMCOAM_PRINT("%u,", next_cm_info_entry.ds_sf_id[i]);
        CMCOAM_PRINT("\n");
    }
    CMCOAM_PRINT("						DSID Count : %u\n", next_cm_info_entry.dsid_count);
    if(next_cm_info_entry.dsid_count > 0) {
        CMCOAM_PRINT("							 DSIDs : ");
        for(i=0; i<next_cm_info_entry.dsid_count; i++)
            CMCOAM_PRINT("%u,", next_cm_info_entry.dsid[i]);
        CMCOAM_PRINT("\n");
    }
    CMCOAM_PRINT("	  Length of Link-Local Address : %u\n", next_cm_info_entry.link_addr_len);
    CMCOAM_PRINT("				Link-Local Address : ");
    CMCOAM_PRINT_BUF(next_cm_info_entry.link_addr, next_cm_info_entry.link_addr_len, 16);
    CMCOAM_PRINT("				  Service Group ID : %u\n", next_cm_info_entry.sg_id);
    CMCOAM_PRINT("				  Service Group ID : %u\n", next_cm_info_entry.sg_id);
    CMCOAM_PRINT("					 RCP-ID length : %u\n", next_cm_info_entry.rcpid_len);
    CMCOAM_PRINT("							RCP-ID : ");
    CMCOAM_PRINT_BUF(next_cm_info_entry.rcpid, next_cm_info_entry.rcpid_len, 16);
    CMCOAM_PRINT("							RCC-ID : %u\n", next_cm_info_entry.rccid);
    CMCOAM_PRINT("							RCS-ID : %u\n", next_cm_info_entry.rcsid);
    CMCOAM_PRINT("							TCS-ID : %u\n", next_cm_info_entry.tcsid);
    CMCOAM_PRINT("		 Last Registered Timestamp : %u\n", next_cm_info_entry.lr_timestamp);
    CMCOAM_PRINT("						  MTC Mode : %s\n", next_cm_info_entry.mtc ? "Enabled" : "Disabled");
    CMCOAM_PRINT("			 Fragmentation support : %s\n", next_cm_info_entry.fragmentation ? "Enabled" : "Disabled");
    CMCOAM_PRINT("			 Concatenation support : %s\n", next_cm_info_entry.concatenation ? "Enabled" : "Disabled");
    CMCOAM_PRINT("					   PHS support : %s\n", next_cm_info_entry.phs ? "Enabled" : "Disabled");
    CMCOAM_PRINT("			  Advanced PHY support : %s\n", next_cm_info_entry.adv_phy ? "Enabled" : "Disabled");
    CMCOAM_PRINT("		   Load Balancing Group ID : %u\n", next_cm_info_entry.lb_grp_id);
    CMCOAM_PRINT("			  QoS Provisioned mode : %s\n", next_cm_info_entry.qos_prov_mode ? "v1.1" : "v1.0");
    CMCOAM_PRINT("\n\n");
#endif

    MacAddressT Cm_mac;
    Cm_mac.addr[0] = 0x00;
    Cm_mac.addr[1] = 0x26;
    Cm_mac.addr[2] = 0x24;
    Cm_mac.addr[3] = 0xdb;
    Cm_mac.addr[4] = 0xeb;
    Cm_mac.addr[5] = 0x6d;

    if(FindCMInfoMgt(Cm_mac,&now_cm_info_entry) != BASE_OK)
        return BASE_ERROR;

    CMCOAM_PRINT("----------------THIRD CM SID : %d\n", now_cm_info_entry.cnu_index);
    CMCOAM_PRINT("					CM Mac Address : %02x%02x.%02x%02x.%02x%02x\n", now_cm_info_entry.cnu_mac[0], now_cm_info_entry.cnu_mac[1],
                 now_cm_info_entry.cnu_mac[2], now_cm_info_entry.cnu_mac[3], now_cm_info_entry.cnu_mac[4], now_cm_info_entry.cnu_mac[5]);
    CMCOAM_PRINT("	 Primary Downstream channel ID : %d\n", now_cm_info_entry.primary_ds_id);
    CMCOAM_PRINT("	   Primary Upstream channel ID : %d\n", now_cm_info_entry.primary_us_id);
    CMCOAM_PRINT("		Number of Upstream Channel : %d\n", now_cm_info_entry.up_ch_num);
    for(i=0; i<now_cm_info_entry.up_ch_num; i++) {
        up_chid = now_cm_info_entry.upstream[i].ch_id;
        CMCOAM_PRINT("			Upstream Channel ID(%u) : %u\n", up_chid, up_chid);
        CMCOAM_PRINT("		   Upstream Power Level(%u) : %.1f dBmv\n", up_chid, (now_cm_info_entry.upstream[i].power_level)/10.0);
        CMCOAM_PRINT("High Resolution Timing Offset(%u) : %d\n", up_chid, now_cm_info_entry.upstream[i].time_offset);
        CMCOAM_PRINT("	   Equalization Data Length(%u) : %u\n", up_chid, now_cm_info_entry.upstream[i].equ_data_len);
        if(now_cm_info_entry.upstream[i].equ_data_len > 0) {
            CMCOAM_PRINT("			  Equalization Data(%u) : \n", up_chid);
            //COAM_PRINT_BUF(now_cm_info_entry.upstream[i].equ_data, now_cm_info_entry.upstream[i].equ_data_len, 16);
        }
        CMCOAM_PRINT("			Unerrored Codewords(%u) : %llu\n", up_chid, now_cm_info_entry.upstream[i].unerr_code);
        CMCOAM_PRINT("			Corrected Codewords(%u) : %llu\n", up_chid, now_cm_info_entry.upstream[i].corrected_code);
        CMCOAM_PRINT("		Uncorrectable Codewords(%u) : %llu\n", up_chid, now_cm_info_entry.upstream[i].uncorrectted_code);
        CMCOAM_PRINT("				   Upstream SNR(%u) : %.1f dB\n", up_chid, (now_cm_info_entry.upstream[i].snr)/10.0);
        CMCOAM_PRINT("			  Micro-reflections(%u) : %d dBc\n", up_chid, now_cm_info_entry.upstream[i].micro_reflect);
    }
    CMCOAM_PRINT("	  Number of Downstream Channel : %u\n", now_cm_info_entry.ds_ch_num);
    if(now_cm_info_entry.ds_ch_num > 0) {
        CMCOAM_PRINT("			Downstream Channel IDs : ");
        for(i=0; i<now_cm_info_entry.ds_ch_num; i++)
            CMCOAM_PRINT("%u ", now_cm_info_entry.ds_ch_id[i]);
        CMCOAM_PRINT("\n");
    }
    CMCOAM_PRINT("			  CM IP Address Length : %u\n", now_cm_info_entry.ip_addr_len);
    if(now_cm_info_entry.ip_addr_len == 4) {
        CMCOAM_PRINT("					 CM IP Address : %u.%u.%u.%u\n", now_cm_info_entry.ip_addr[0], now_cm_info_entry.ip_addr[1], now_cm_info_entry.ip_addr[2], now_cm_info_entry.ip_addr[3]);

    } else {
        CMCOAM_PRINT("					 CM IP Address : ");
        //COAM_PRINT_BUF(now_cm_info_entry.ip_addr, now_cm_info_entry.ip_addr_len, 16);
    }
    CMCOAM_PRINT("						 Timestamp : %u\n", now_cm_info_entry.timestamp);
    CMCOAM_PRINT("	Upstream Service Flow ID Count : %u\n", now_cm_info_entry.us_sf_id_count);
    if(now_cm_info_entry.us_sf_id_count>0) {
        CMCOAM_PRINT("		 Upstream Service Flow IDs : ");
        for(i=0; i<now_cm_info_entry.us_sf_id_count; i++)
            CMCOAM_PRINT("%u,", now_cm_info_entry.us_sf_id[i]);
        CMCOAM_PRINT("\n");
    }
    CMCOAM_PRINT("Downstream Service Flow ID Count : %u\n", now_cm_info_entry.ds_sf_id_count);
    if(now_cm_info_entry.ds_sf_id_count > 0) {
        CMCOAM_PRINT("	   Downstream Service Flow IDs : ");
        for(i=0; i<now_cm_info_entry.ds_sf_id_count; i++)
            CMCOAM_PRINT("%u,", now_cm_info_entry.ds_sf_id[i]);
        CMCOAM_PRINT("\n");
    }
    CMCOAM_PRINT("						DSID Count : %u\n", now_cm_info_entry.dsid_count);
    if(now_cm_info_entry.dsid_count > 0) {
        CMCOAM_PRINT("							 DSIDs : ");
        for(i=0; i<now_cm_info_entry.dsid_count; i++)
            CMCOAM_PRINT("%u,", now_cm_info_entry.dsid[i]);
        CMCOAM_PRINT("\n");
    }
    CMCOAM_PRINT("	  Length of Link-Local Address : %u\n", now_cm_info_entry.link_addr_len);
    CMCOAM_PRINT("				Link-Local Address : ");
    //COAM_PRINT_BUF(now_cm_info_entry.link_addr, now_cm_info_entry.link_addr_len, 16);
    CMCOAM_PRINT("				  Service Group ID : %u\n", now_cm_info_entry.sg_id);
    CMCOAM_PRINT("				  Service Group ID : %u\n", now_cm_info_entry.sg_id);
    CMCOAM_PRINT("					 RCP-ID length : %u\n", now_cm_info_entry.rcpid_len);
    CMCOAM_PRINT("							RCP-ID : ");
    //COAM_PRINT_BUF(now_cm_info_entry.rcpid, now_cm_info_entry.rcpid_len, 16);
    CMCOAM_PRINT("							RCC-ID : %u\n", now_cm_info_entry.rccid);
    CMCOAM_PRINT("							RCS-ID : %u\n", now_cm_info_entry.rcsid);
    CMCOAM_PRINT("							TCS-ID : %u\n", now_cm_info_entry.tcsid);
    CMCOAM_PRINT("		 Last Registered Timestamp : %u\n", now_cm_info_entry.lr_timestamp);
    CMCOAM_PRINT("						  MTC Mode : %s\n", now_cm_info_entry.mtc ? "Enabled" : "Disabled");
    CMCOAM_PRINT("			 Fragmentation support : %s\n", now_cm_info_entry.fragmentation ? "Enabled" : "Disabled");
    CMCOAM_PRINT("			 Concatenation support : %s\n", now_cm_info_entry.concatenation ? "Enabled" : "Disabled");
    CMCOAM_PRINT("					   PHS support : %s\n", now_cm_info_entry.phs ? "Enabled" : "Disabled");
    CMCOAM_PRINT("			  Advanced PHY support : %s\n", now_cm_info_entry.adv_phy ? "Enabled" : "Disabled");
    CMCOAM_PRINT("		   Load Balancing Group ID : %u\n", now_cm_info_entry.lb_grp_id);
    CMCOAM_PRINT("			  QoS Provisioned mode : %s\n", now_cm_info_entry.qos_prov_mode ? "v1.1" : "v1.0");
    CMCOAM_PRINT("\n\n");
#endif

//SERVICE FLOW TEST
#if 0
    CMCOAM_CnuStatusT now_cm_info_entry,next_cm_info_entry;
    CMCOAM_ServiceFlowT nowServiceFlow, nextServiceFlow;

    if(GetFirstCMInfoMgt(&now_cm_info_entry) != BASE_OK)
        return BASE_ERROR;

    if(GetCMFirstServiceFlowMgt(&now_cm_info_entry , &nowServiceFlow) != BASE_OK)
        return BASE_ERROR;

    CMCOAM_Qos_PrintServiceFlowConfig(&nowServiceFlow);

    MacAddressT Cm_mac;
    Cm_mac.addr[0] = 0x00;
    Cm_mac.addr[1] = 0x26;
    Cm_mac.addr[2] = 0x24;
    Cm_mac.addr[3] = 0xdb;
    Cm_mac.addr[4] = 0xeb;
    Cm_mac.addr[5] = 0x6d;

    if(FindCMServiceFlowMgt(Cm_mac , 1 , &nowServiceFlow) != BASE_OK)
        return BASE_ERROR;

    CMCOAM_Qos_PrintServiceFlowConfig(&nowServiceFlow);

    if(GetCMNextServiceFlowMgt(&now_cm_info_entry , &nowServiceFlow, &nextServiceFlow) != BASE_OK)
        return BASE_ERROR;

    CMCOAM_Qos_PrintServiceFlowConfig(&nextServiceFlow);
#endif

//HOST INFO TEST
#if 0
    CMCOAM_CnuStatusT now_cm_info_entry;
    if(GetFirstCMInfoMgt(&now_cm_info_entry) != BASE_OK)
        return BASE_ERROR;

    MacAddressT Cm_mac;
    Cm_mac.addr[0] = 0x00;
    Cm_mac.addr[1] = 0x26;
    Cm_mac.addr[2] = 0x24;
    Cm_mac.addr[3] = 0xdb;
    Cm_mac.addr[4] = 0xeb;
    Cm_mac.addr[5] = 0x6d;

    CMCOAM_CmHostInfoT Cm_host1,Cm_host2;
    CMCOAM_CmHostInfoT now_host_info_entry,next_host_info_entry;

    Cm_host1.host_mac.addr[0] = 0x00;
    Cm_host1.host_mac.addr[1] = 0x01;
    Cm_host1.host_mac.addr[2] = 0x02;
    Cm_host1.host_mac.addr[3] = 0x03;
    Cm_host1.host_mac.addr[4] = 0x04;
    Cm_host1.host_mac.addr[5] = 0x05;
    Cm_host1.ipv4_addr = 0x11223344;

    Cm_host2.host_mac.addr[0] = 0x05;
    Cm_host2.host_mac.addr[1] = 0x04;
    Cm_host2.host_mac.addr[2] = 0x03;
    Cm_host2.host_mac.addr[3] = 0x02;
    Cm_host2.host_mac.addr[4] = 0x01;
    Cm_host2.host_mac.addr[5] = 0x00;
    Cm_host2.ipv4_addr = 0x44332211;

    Update_Cm_HostMgt(Cm_mac , Cm_host1);
    Update_Cm_HostMgt(Cm_mac , Cm_host2);

    GetFirst_Cm_HostMgt(&now_cm_info_entry ,&now_host_info_entry);
    GetNext_Cm_HostMgt(&now_cm_info_entry,&now_host_info_entry,&next_host_info_entry);

    Find_Cm_HostMgt(Cm_mac,Cm_host1.host_mac,&now_host_info_entry);

    CMCOAM_PRINT("--Find2 Host Mac Address : %02x%02x.%02x%02x.%02x%02x ", now_host_info_entry.host_mac.addr[0], now_host_info_entry.host_mac.addr[1], now_host_info_entry.host_mac.addr[2], now_host_info_entry.host_mac.addr[3], now_host_info_entry.host_mac.addr[4], now_host_info_entry.host_mac.addr[5]);
    CMCOAM_PRINT("--Find2 Host Ip Address : 0x%x\n",now_host_info_entry.ipv4_addr);

    Show_All_Cm_HostMgt();
#endif

//test map mac to ip
#if 0
    MacAddressT mac1;
    U32 ip1;
    MacAddressT mac2;
    U32 ip2;
    MacAddressT mac3;
    U32 ip3;
    U32 temp_ip;

    mac1.addr[0] = 0x00;
    mac1.addr[1] = 0x26;
    mac1.addr[2] = 0x24;
    mac1.addr[3] = 0xdb;
    mac1.addr[4] = 0xeb;
    mac1.addr[5] = 0x76;

    mac2.addr[0] = 0x02;
    mac2.addr[1] = 0x02;
    mac2.addr[2] = 0x02;
    mac2.addr[3] = 0x02;
    mac2.addr[4] = 0x02;
    mac2.addr[5] = 0x02;

    mac3.addr[0] = 0x03;
    mac3.addr[1] = 0x03;
    mac3.addr[2] = 0x03;
    mac3.addr[3] = 0x03;
    mac3.addr[4] = 0x03;
    mac3.addr[5] = 0x03;

    ip1 = 0xc0080133;
    ip2 = 0x22222222;
    ip3 = 0x33333333;

    AddMacToIpMapMgt(mac1 , ip1);
    AddMacToIpMapMgt(mac2 , ip2);
    AddMacToIpMapMgt(mac3 , ip3);

    SearchCnuMacToIpMapMgt(mac2 , &temp_ip);
    CMCOAM_PRINT("\n\r temp ip :0x%x !",temp_ip);

    DeleteMacToIpMapMgt(mac2);

    DumpCnuMacToIpMapMgt();
#endif

//test map mac to bandwitdth
#if 0

    DS_QoS_InfoT bandwitch1;
    DS_QoS_InfoT bandwitch2;
    DS_QoS_InfoT bandwitch3;
    DS_QoS_InfoT temp_bandwitch;
    //DS_QoS_InfoT first;
    //DS_QoS_InfoT  nest;
    //U32 a,b,c,d,e,f;

    bandwitch1.ds_qos_cnu_mac.addr[0] = 0x01;
    bandwitch1.ds_qos_cnu_mac.addr[1] = 0x01;
    bandwitch1.ds_qos_cnu_mac.addr[2] = 0x01;
    bandwitch1.ds_qos_cnu_mac.addr[3] = 0x01;
    bandwitch1.ds_qos_cnu_mac.addr[4] = 0x01;
    bandwitch1.ds_qos_cnu_mac.addr[5] = 0x01;

    bandwitch2.ds_qos_cnu_mac.addr[0] = 0x02;
    bandwitch2.ds_qos_cnu_mac.addr[1] = 0x02;
    bandwitch2.ds_qos_cnu_mac.addr[2] = 0x02;
    bandwitch2.ds_qos_cnu_mac.addr[3] = 0x02;
    bandwitch2.ds_qos_cnu_mac.addr[4] = 0x02;
    bandwitch2.ds_qos_cnu_mac.addr[5] = 0x02;

    bandwitch3.ds_qos_cnu_mac.addr[0] = 0x03;
    bandwitch3.ds_qos_cnu_mac.addr[1] = 0x03;
    bandwitch3.ds_qos_cnu_mac.addr[2] = 0x03;
    bandwitch3.ds_qos_cnu_mac.addr[3] = 0x03;
    bandwitch3.ds_qos_cnu_mac.addr[4] = 0x03;
    bandwitch3.ds_qos_cnu_mac.addr[5] = 0x03;

    bandwitch1.bandwidth= 11;
    bandwitch2.bandwidth = 22;
    bandwitch3.bandwidth = 33;

    AddMacToBandnwidthMapMgt(&bandwitch1);
    AddMacToBandnwidthMapMgt(&bandwitch2);
    AddMacToBandnwidthMapMgt(&bandwitch3);

    SearchCnuMacToBandnwidthMapMgt(bandwitch2.ds_qos_cnu_mac , &temp_bandwitch);
    CMCOAM_PRINT("\n\r temp bandwitch :%d !",temp_bandwitch.bandwidth);

    DeleteMacToBandnwidthMapMgt(bandwitch2.ds_qos_cnu_mac);

    DumpCnuMacToBandnwidthMapMgt();

#endif

#if 0

    DS_QoS_InfoT first;
    DS_QoS_InfoT    nest;
    U32 a,b,c,d,e,f;

    printf("\n\r==========================================\n\r");

    GetFirstDsQosMgt(&first);

    a = 0xff & first.ds_qos_cnu_mac.addr[0];
    b = 0xff & first.ds_qos_cnu_mac.addr[1];
    c = 0xff & first.ds_qos_cnu_mac.addr[2];
    d = 0xff & first.ds_qos_cnu_mac.addr[3];
    e = 0xff & first.ds_qos_cnu_mac.addr[4];
    f = 0xff & first.ds_qos_cnu_mac.addr[5];
    printf("First ::::\n");
    printf("%02x:%02x:%02x:%02x:%02x:%02x |",a,b,c,d,e,f);
    printf("Bandnwidth= %d\n\r",first.bandwidth);

    GetNextDsQosMgt(&first, &nest);

    a = 0xff & nest.ds_qos_cnu_mac.addr[0];
    b = 0xff & nest.ds_qos_cnu_mac.addr[1];
    c = 0xff & nest.ds_qos_cnu_mac.addr[2];
    d = 0xff & nest.ds_qos_cnu_mac.addr[3];
    e = 0xff & nest.ds_qos_cnu_mac.addr[4];
    f = 0xff & nest.ds_qos_cnu_mac.addr[5];
    printf("Next ::::\n");
    printf("%02x:%02x:%02x:%02x:%02x:%02x |",a,b,c,d,e,f);
    printf("Bandnwidth= %d\n\r",nest.bandwidth);

#endif

    return CPARSER_OK;
}

cparser_result_t cparser_cmd_utest_channel_data_clear(cparser_context_t *context)
{
    //ClearChannelConfig();
    DumpCnuMacToIpMapMgt();
    DumpCnuMacToBandnwidthMapMgt();
    return CPARSER_OK;
}

int AlarmCpuUsingHandlerMgt(CpuUsingInfoT* CpuUsingInfoPoint)
{
    CMCOAM_PRINT("Alarm CPU USING: %f !\n",CpuUsingInfoPoint->cpu_using);
    return 0;
}

int AlarmRamUsingHandlerMgt(RamUsingInfoT* RamUsingInfoPoint)
{
    CMCOAM_PRINT("Alarm Ram USING: %f !\n",RamUsingInfoPoint->ram_using);
    return 0;
}

int AlarmUsSigHandlerMgt(UsSignalQualityAlarmT* UsSigInfoPoint)
{
    int i;
    for(i=0; i<4; i++) {
        CMCOAM_PRINT("\n+++++++++++++++++++++++++++++++++++++++++++++\n");
        CMCOAM_PRINT("Chnnel Id = %u    Us SNR = %.1f dB    Max SNR = %u\n",
                     UsSigInfoPoint->us_sig[i].ch_id,  (UsSigInfoPoint->us_sig[i].snr)/10.0, UsSigInfoPoint->us_sig[i].max_snr);
    }

    return 0;
}

int AlarmMaxOnlieCmNumberHandleMgt(JudgeMaxCmOnlineNumberInfoT* JudgeMaxCmOnlinePoint)
{
    CMCOAM_PRINT("Online Cm now online  value : %d !\n",JudgeMaxCmOnlinePoint->cm_online_number);
    //CMCOAM_PRINT("Online Cm max value : %d !\n",JudgeMaxCmOnlinePoint->max_cm_online_number);
    return 0;	
}

int initAlarmCpuUsingHandlerMgt()
{
    return RegisterCpuUsingCallbackMgt(AlarmCpuUsingHandlerMgt);
}

int initAlarmRamUsingHandlerMgt()
{
    return RegisterRamUsingCallbackMgt(AlarmRamUsingHandlerMgt);
}

int initAlarmUsSigHandlerMgt()
{
    return RegisterUsSigCallbackMgt(AlarmUsSigHandlerMgt);
}

int initAlarmMaxOnlieCmNumberHandleMgt()
{
	return RegisterJudgeMaxCmOnlineNumberCallbackMgt(AlarmMaxOnlieCmNumberHandleMgt);
}

cparser_result_t cparser_cmd_utest_channel_data_show(cparser_context_t *context)
{
    //ShowChannelConfig();
    //initAlarmCpuUsingHandlerMgt();
    //initAlarmRamUsingHandlerMgt();
    //initAlarmUsSigHandlerMgt();
    initAlarmMaxOnlieCmNumberHandleMgt();
    return CPARSER_OK;
}

cparser_result_t cparser_cmd_show_wan_port_statistics(cparser_context_t *context)
{
	assert(context);
	WAN_PORT_STAT_CFG temp_wan_port_stat_entry;
	memset((unsigned char*)&temp_wan_port_stat_entry, 0, sizeof(WAN_PORT_STAT_CFG));
	MgtGetWanPortStat(&temp_wan_port_stat_entry);
	CMCOAM_PRINT("\ntest wan port stat : 0x%llx\n", temp_wan_port_stat_entry.snmpIfInOctets);
	CMCOAM_PRINT("test wan port stat : 0x%llx\n", temp_wan_port_stat_entry.snmpIfInNUcastPkts);
	return CPARSER_OK;
}

cparser_result_t cparser_cmd_download_rootfs_serverIp_fileName(
    cparser_context_t *context,
    char **serverIp_ptr,
    char **fileName_ptr)
{
    if (UpdateSwitchRootFs(*serverIp_ptr,*fileName_ptr) != BASE_OK)
        return CPARSER_NOT_OK;

    return CPARSER_OK;
}

cparser_result_t cparser_cmd_download_cmc_config_serverIp_fileName(
    cparser_context_t *context,
    char **serverIp_ptr,
    char **fileName_ptr)
{
    if (UpdateControllerConfigure(*serverIp_ptr,*fileName_ptr) != BASE_OK)
        return CPARSER_NOT_OK;

    return CPARSER_OK;

}

cparser_result_t cparser_cmd_download_switch_config_serverIp_fileName(
    cparser_context_t *context,
    char **serverIp_ptr,
    char **fileName_ptr)
{
    if (UpdateSwitchConfigure(*serverIp_ptr,*fileName_ptr) != BASE_OK)
        return CPARSER_NOT_OK;

    return CPARSER_OK;
}

cparser_result_t cparser_cmd_upload_cmc_config_serverIp_fileName(
    cparser_context_t *context,
    char **serverIp_ptr,
    char **fileName_ptr)
{
    if (SaveControllerConfigure(*serverIp_ptr,*fileName_ptr) != BASE_OK)
        return CPARSER_NOT_OK;

    return CPARSER_OK;
}

cparser_result_t cparser_cmd_upload_switch_config_serverIp_fileName(
    cparser_context_t *context,
    char **serverIp_ptr,
    char **fileName_ptr)
{
    if (SaveSwitchConfigure(*serverIp_ptr,*fileName_ptr) != BASE_OK)
        return CPARSER_NOT_OK;

    return CPARSER_OK;
}

typedef struct {
    const char*name;
    int32_t type;
} NameTypePair;

cparser_result_t cparser_cmd_update_type_serverIp_fileName_userName_password(
    cparser_context_t *context,
    char **type_ptr,
    char **serverIp_ptr,
    char **fileName_ptr,
    char **userName_ptr,
    char **password_ptr)
{
    static NameTypePair nameTypePair[] = {
        {"switch-config", FileTypeSwitchProfile},
        {"controller-config", FileTypeControllerProfile},
        {"rootfs", FileTypeRootFs}
    };
    size_t i;
    for (i = 0; i < sizeof(nameTypePair) / sizeof(NameTypePair); ++i) {
        if (strcmp(*type_ptr, nameTypePair[i].name) == 0) {
            BASE_ErrCode errCode;
            errCode = UpdateFromFtp(*serverIp_ptr, *userName_ptr, *password_ptr,
                                    *fileName_ptr, nameTypePair[i].type);
            if (errCode != BASE_OK) {
                printf("Failed!\n");
            }
            return CPARSER_OK;
        }
    }
    printf("Failed!\n");

    return CPARSER_OK;
}

/* Function: reboot controller & switch
 * refer to:
 * cparser_result_t cparser_cmd_exit(cparser_context_t *context)
 * cparser_result_t cparser_cmd_quit(cparser_context_t *context)
 */
cparser_result_t cparser_cmd_reboot(cparser_context_t *context)
{
    assert(context);
    StopCli();
    return CPARSER_OK;
}

cparser_result_t cparser_cmd_config_dhcp_relay_serverIp(cparser_context_t *context,
        uint32_t *serverIp_ptr)
{
    assert(context);
    struct in_addr server;
    server.s_addr = *serverIp_ptr;
    SetDhcpServerIp(server);

    return CPARSER_OK;
}

cparser_result_t cparser_cmd_config_syslog_ip_port(cparser_context_t *context,
    uint32_t *ip_ptr,
    int32_t *port_ptr)
{
    SyslogServer server;
    server.addr.s_addr = *ip_ptr;

    if (port_ptr == NULL)
        server.port = DefaultSyslogServerPort;
    else
        server.port = *port_ptr;

    if (AddSyslogServer(&server) != BASE_OK)
        printf("Failed!\n");

    return CPARSER_OK;
}

cparser_result_t cparser_cmd_config_no_syslog_ip(cparser_context_t *context,
    uint32_t *ip_ptr)
{
    struct in_addr addr;
    addr.s_addr = *ip_ptr;

    if (DelSyslogServer(addr) != BASE_OK)
        printf("Failed!\n");

    return CPARSER_OK;
}

cparser_result_t cparser_cmd_show_syslog_server(cparser_context_t *context)
{
    SyslogServer server;
    char buf[INET_ADDRSTRLEN];

    BASE_ErrCode ret;
    for (ret = GetFirstSyslogServer(&server);
        ret == BASE_OK;
        ret = GetNextSyslogServer(&server, &server))
    {
        inet_ntop(AF_INET, (void*)&server.addr, buf, INET_ADDRSTRLEN);
        printf("Syslog Sever IP %s, Port %d\n", buf, server.port);
    }
    return CPARSER_OK;
}

cparser_result_t cparser_cmd_show_reboot_level(cparser_context_t *context)
{
    printf("Reboot level : %d\n", GetRebootLevel());
    return CPARSER_OK;
}

cparser_result_t cparser_cmd_config_reboot_level_level(cparser_context_t *context,
    int32_t *level_ptr)
{
    if (*level_ptr > RebootLevelRebootCmc)
    {
        printf("Wrong value!\n");
        return CPARSER_OK;
    }
    SetRebootLevel(*level_ptr);
    return CPARSER_OK;
}
