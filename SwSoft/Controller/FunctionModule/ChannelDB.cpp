#include <stdio.h>

#include "SystemInclude.h"
#include "Debug.h"
#include "ChannelDB.h"

using namespace std;

DownStreamChannel downStreamChannelInfo[BRCM_CMC_MAX_DS_CH];
UpStreamChannel upStreamChannelInfo[BRCM_CMC_MAX_US_CH];

UpStreamSignalQuality upStreamSignalQualityInfo[BRCM_CMC_MAX_US_CH];

signed short upStreamPowerLevelInfo[BRCM_CMC_MAX_US_CH];//tenths dBmv
U8 fftAveraging;

//ds
/*************************************************************************/

BASE_ErrCode
SetDownStreamChannelEnable(U8 channelId,U8 enable)
{
    U32  step = (downStreamChannelInfo[0].annex == 1) ? 6000000 : 8000000;

    if(enable !=0 && enable !=1 && enable !=2) {
        dbgstrm << "Invalid parameter!" << endl;
        return BASE_ERROR_PARAM;
    }

    //dbgstrm << "enable=" <<  (U32)(enable) << endl;

    if(channelId>BRCM_CMC_MAX_DS_CH) {
        dbgstrm << "Invalid parameter!" << endl;
        return BASE_ERROR_PARAM;
    }

    if(channelId != 0) {
        downStreamChannelInfo[channelId-1].ds_ch_id = channelId;
        downStreamChannelInfo[channelId-1].enabled = enable;

        if(enable == 1) {
            downStreamChannelInfo[channelId-1].modulatoin= 1;//256QAM
            downStreamChannelInfo[channelId-1].power_level= 450;//45dBmv
            downStreamChannelInfo[channelId-1].freq = 440000000+((U32)channelId-1)*step;
        } else if(enable ==0) {
            downStreamChannelInfo[channelId-1].modulatoin= 0;//64QAM
            downStreamChannelInfo[channelId-1].power_level= 0;//0 dBmv
            downStreamChannelInfo[channelId-1].freq = 0;
        }
    } else {
        for( int i = 0; i<BRCM_CMC_MAX_DS_CH ; ++i ) {
            downStreamChannelInfo[i].ds_ch_id = i+1;
            downStreamChannelInfo[i].enabled = enable;

            if(enable == 1) {
                downStreamChannelInfo[i].modulatoin= 1;//256QAM
                downStreamChannelInfo[i].power_level= 450;//45dBmv
                downStreamChannelInfo[i].freq = 440000000+i*step;
            } else if(enable ==0) {
                downStreamChannelInfo[i].modulatoin= 0;//64QAM
                downStreamChannelInfo[i].power_level= 0;//0 dBmv
                downStreamChannelInfo[i].freq = 0;
            }
            dbgstrm << "channelId=" <<  (U32)(downStreamChannelInfo[i].ds_ch_id) << endl;
            dbgstrm << "enable=" << (U32)(downStreamChannelInfo[i].enabled) << endl;
        }
    }

    dbgstrm << "channelId=" <<  (U32)(downStreamChannelInfo[channelId-1].ds_ch_id) << endl;
    dbgstrm << "enable=" << (U32)(downStreamChannelInfo[channelId-1].enabled) << endl;

    return BASE_OK;
}

BASE_ErrCode
GetDownStreamChannelEnable(U32 channelId,U8* enablePoint)
{
    if((channelId>BRCM_CMC_MAX_DS_CH)||(enablePoint == NULL)) {
        dbgstrm << "Invalid parameter!" << endl;
        return BASE_ERROR_PARAM;
    }

    *enablePoint = downStreamChannelInfo[channelId-1].enabled;
    return BASE_OK;
}

/*************************************************************************/

BASE_ErrCode
SetDownStreamChannelMode(U8 channelId,U8 mode)
{
    if(downStreamChannelInfo[channelId-1].enabled == 0)
        return BASE_OK;

    if(channelId>BRCM_CMC_MAX_DS_CH) {
        dbgstrm << "Invalid parameter! " << endl;
        return BASE_ERROR_PARAM;
    }

    if(mode!=1 && mode!=2) {
        dbgstrm << "Invalid parameter! " << endl;
        return BASE_ERROR_PARAM;
    }

    if(channelId != 0)
        downStreamChannelInfo[channelId-1].enabled= mode;
    else {
        for( int i = 0; i<BRCM_CMC_MAX_DS_CH ; ++i ) {
            downStreamChannelInfo[i].annex = mode;
        }
    }
    return BASE_OK;
}

BASE_ErrCode
GetDownStreamChannelMode(U8 channelId,U8* mode)
{
    if(((channelId<1)&&(channelId>BRCM_CMC_MAX_DS_CH)||(mode == NULL))) {
        dbgstrm << "Invalid parameter!" << endl;
        return BASE_ERROR_PARAM;
    }

    *mode = downStreamChannelInfo[channelId-1].annex;
    return BASE_OK;
}

/*************************************************************************/

BASE_ErrCode
SetDownstreamChannleAnnexMode(U8 channelId,U8 annex)
{
    if(downStreamChannelInfo[channelId-1].enabled == 0)
        return BASE_OK;

    if(channelId>BRCM_CMC_MAX_DS_CH) {
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }

    if(annex!=ANNEX_A && annex!=ANNEX_B && annex!=ANNEX_A_6875 && annex!=ANNEX_A_6900) {
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }

    if(channelId != 0)
        downStreamChannelInfo[channelId-1].annex= annex;
    else {
        for( int i = 0; i<BRCM_CMC_MAX_DS_CH ; ++i ) {
            downStreamChannelInfo[i].annex = annex;
        }
    }
    return BASE_OK;
}

BASE_ErrCode
GetDownstreamChannleAnnexMode(U8 channelId,U8* annex)
{
    if((channelId>BRCM_CMC_MAX_DS_CH)||(annex == NULL)) {
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }

    *annex = downStreamChannelInfo[channelId-1].annex;
    return BASE_OK;
}

/*************************************************************************/

BASE_ErrCode
SetDownStreamChannelFrequency(U8 channelId,U32 freq)
{
    if((downStreamChannelInfo[channelId-1].enabled == 0)||(channelId == 0))
        return BASE_OK;

    if(downStreamChannelInfo[channelId-1].enabled == 0)
        return BASE_OK;

    if(channelId>BRCM_CMC_MAX_DS_CH) {
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }

    downStreamChannelInfo[channelId-1].freq = freq;
    return BASE_OK;
}

BASE_ErrCode
GetDownStreamChannelFrequency(U8 channelId, U32* freq)
{
    if(((channelId<1)&&(channelId>BRCM_CMC_MAX_DS_CH)||(freq == NULL))) {
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }

    *freq = downStreamChannelInfo[channelId-1].freq;
    return BASE_OK;
}

/*************************************************************************/

BASE_ErrCode
AutoSetDownStreamChannelFrequency(U32 freq, U32 step, U8 mode)
{
    int i;

    for(i=0; i<BRCM_CMC_MAX_DS_CH; i++) {
        if(downStreamChannelInfo[i].enabled == 0)
            break;

        if(1==mode)//increase
            downStreamChannelInfo[i].freq = freq+(i*step);
        else
            downStreamChannelInfo[i].freq = freq-(i*step);
    }
    return BASE_OK;
}

BASE_ErrCode
AutoGetDownStreamChannelFrequency(U32* freq, U32* step, U8* mode)
{
    if((freq == NULL)||(step == NULL)||(mode == NULL)) {
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }

    downStreamChannelInfo[0].freq < downStreamChannelInfo[1].freq ? *mode = 1 : *mode =0;
    //*mode = 1;//default is 1 increase
    *freq = downStreamChannelInfo[0].freq;//first channel freq
    if(*mode == 1)
        *step = downStreamChannelInfo[1].freq - downStreamChannelInfo[0].freq;
    else
        *step = downStreamChannelInfo[0].freq - downStreamChannelInfo[1].freq;
    return BASE_OK;
}

/*************************************************************************/

BASE_ErrCode
SetDownstreamChannelInterleaver(U8 channelId,U8 Interleaver)
{
    if(channelId>BRCM_CMC_MAX_DS_CH) {
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }

    if(Interleaver>14) {
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }

    if(channelId==0) {
        for(int i=0; i<BRCM_CMC_MAX_DS_CH; i++)
            downStreamChannelInfo[i].interleaver= Interleaver;
    } else {
        downStreamChannelInfo[channelId-1].interleaver = Interleaver;
    }
    return BASE_OK;
}

BASE_ErrCode
GetDownstreamChannelInterleaver(U8 channelId, U8* Interleaver)
{
    if((channelId>BRCM_CMC_MAX_DS_CH)||(Interleaver == NULL)) {
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }

    *Interleaver = downStreamChannelInfo[channelId-1].interleaver;
    return BASE_OK;
}

/*************************************************************************/

BASE_ErrCode
SetDownStreamChannelModulation(U8 channelId,U8 modulation)
{
    if(downStreamChannelInfo[channelId-1].enabled == 0)
        return BASE_OK;

    if(channelId>BRCM_CMC_MAX_DS_CH) {
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }

    if(modulation>2) {
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }

    if(channelId==0) {
        for(int i=0; i<BRCM_CMC_MAX_DS_CH; i++)
            downStreamChannelInfo[i].modulatoin= modulation;
    } else
        downStreamChannelInfo[channelId-1].modulatoin = modulation;
    return BASE_OK;
}

BASE_ErrCode
GetDownStreamChannelModulation(U8 channelId, U8* modulation)
{
    if((channelId>BRCM_CMC_MAX_DS_CH)||(modulation == NULL)) {
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }

    *modulation = downStreamChannelInfo[channelId-1].interleaver;
    return BASE_OK;
}

/*************************************************************************/

BASE_ErrCode
SetDownStreamChannelPowerLevel(U8 channelId,U32 power)
{
    if(downStreamChannelInfo[channelId-1].enabled == 0)
        return BASE_OK;

    if(power > 600 ) {
        dbgstrm << "Invalid power level "<< power <<"(<=600 tenth of dBmv)\n" << endl;
        return BASE_ERROR_PARAM;
    }

    if(channelId>BRCM_CMC_MAX_DS_CH) {
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }

    int i = 0;
    if(channelId==0) {
        for(i=0; i<16; i++)
            downStreamChannelInfo[i].power_level= power;
    } else {
        downStreamChannelInfo[channelId-1].power_level = power;
    }
    return BASE_OK;
}

BASE_ErrCode
GetDownStreamChannelPowerLevel(U8 channelId, U32* power)
{
    if((channelId>BRCM_CMC_MAX_DS_CH)||(power == NULL)) {
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }

    *power = downStreamChannelInfo[channelId-1].power_level;
    return BASE_OK;
}

//us
/*************************************************************************/

BASE_ErrCode
SetUpStreamChannelEnabled(U8 channelId,U8 enable)
{
    if(channelId>BRCM_CMC_MAX_US_CH) {
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }

    if(enable!=0 && enable!=1) {
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }

    if(channelId == 0) {
        for(int i = 0; i<BRCM_CMC_MAX_US_CH ; i++ ) {
            upStreamChannelInfo[i].us_ch_id = i+1;
            upStreamChannelInfo[i].enabled = enable;
        }
    } else {
        upStreamChannelInfo[channelId-1].us_ch_id= channelId;
        upStreamChannelInfo[channelId-1].enabled = enable;
    }
    return BASE_OK;
}

BASE_ErrCode
GetUpStreamChannelEnabled(U8 channelId,U8* enable)
{
    if((channelId>BRCM_CMC_MAX_US_CH)||(enable == NULL)) {
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }

    *enable = upStreamChannelInfo[channelId-1].enabled;
    return BASE_OK;
}

/*************************************************************************/

BASE_ErrCode
SetUpStreamChannelFrequency(U8 channelId,U32 freq)
{
    if(channelId>BRCM_CMC_MAX_US_CH) {
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }

    upStreamChannelInfo[channelId-1].freq = freq;
    return BASE_OK;
}

BASE_ErrCode
GetUpStreamChannelFrequency(U8 channelId, U32* freq)
{
    if((channelId>BRCM_CMC_MAX_US_CH)||(freq == NULL)) {
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }

    *freq = upStreamChannelInfo[channelId-1].freq;
    return BASE_OK;
}

/*************************************************************************/

BASE_ErrCode
AutoSetUpStreamChannelFrequency(U32 freq, U32 step, U8 mode)
{
    int i;

    for(i=0; i<BRCM_CMC_MAX_US_CH; i++) {
        if(1==mode)//increase
            upStreamChannelInfo[i].freq = freq+(i*step);
        else
            upStreamChannelInfo[i].freq = freq-(i*step);
    }
    return BASE_OK;
}

BASE_ErrCode
AutoGetUpStreamChannelFrequency(U32* freq, U32* step, U8* mode)
{
    if((freq == NULL)||(step == NULL)||(mode == NULL)) {
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }

    upStreamChannelInfo[0].freq < upStreamChannelInfo[1].freq ? *mode = 1 : *mode =0;
    //*mode = 1;//default is 1 increase
    *freq = upStreamChannelInfo[0].freq;//first channel freq
    if(*mode == 1)
        *step = upStreamChannelInfo[1].freq - upStreamChannelInfo[0].freq;
    else
        *step = upStreamChannelInfo[0].freq - upStreamChannelInfo[1].freq;
    return BASE_OK;
}

/*************************************************************************/

BASE_ErrCode
SetUpStreamChannelWidth(U8 channelId,U32 channel_width)
{
    if(channelId>BRCM_CMC_MAX_US_CH) {
        dbgstrm << "Invalid parameter!should be 0~4" <<endl;
        return BASE_ERROR_PARAM;
    }

    if(channel_width!=1600000 && channel_width!=3200000 && channel_width!=6400000 ) {
        dbgstrm << "Invalid parameter!should be 1600000|3200000|6400000" <<endl;
        return BASE_ERROR_PARAM;
    }

    upStreamChannelInfo[channelId-1].width= channel_width;
    return BASE_OK;
}

BASE_ErrCode
GetUpStreamChannelWidth(U8 channelId,U32* channel_width)
{
    if((channelId>BRCM_CMC_MAX_US_CH)||(channel_width == NULL)) {
        dbgstrm << "Invalid parameter! " <<endl;
        return BASE_ERROR_PARAM;
    }
    *channel_width = upStreamChannelInfo[channelId-1].width;
    return BASE_OK;
}

/*************************************************************************/

BASE_ErrCode
SetUpStreamChannelProfile(U8 channelId,U8 channel_profile)
{
    if(channelId==0) { //apply to all channel settings.
        for(int i=0; i<BRCM_CMC_MAX_US_CH; i++)
            upStreamChannelInfo[i].profile_type= channel_profile;
    } else {
        upStreamChannelInfo[channelId-1].profile_type = channel_profile;
    }

    return BASE_OK;
}

BASE_ErrCode
GetUpStreamChannelProfile(U8 channelId,U8* channel_profile)
{
    if((channelId>BRCM_CMC_MAX_US_CH)||(channel_profile == NULL)) {
        dbgstrm << "Invalid parameter! " <<endl;
        return BASE_ERROR_PARAM;
    }

    *channel_profile = upStreamChannelInfo[channelId-1].profile_type;
    return BASE_OK;
}


/*************************************************************************/

BASE_ErrCode
SetUpStreamChannelPowerLevel(U8 channelId,U32 power)
{
    if(channelId>BRCM_CMC_MAX_US_CH) {
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }

    if(power > 230 ) {
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }

    if(channelId==0) {
        for(int i=0; i<BRCM_CMC_MAX_US_CH; i++)
            upStreamPowerLevelInfo[i]= power;
    } else {
        upStreamPowerLevelInfo[channelId-1] = power;
    }
    return BASE_OK;
}

BASE_ErrCode
GetUpStreamChannelPowerLevel(U8 channelId, U32* power)
{
    if((channelId>BRCM_CMC_MAX_US_CH)||(power == NULL)) {
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }

    *power = upStreamPowerLevelInfo[channelId-1];
    return BASE_OK;
}

/*************************************************************************/

BASE_ErrCode
SetUpStreamChannelDocsis30Mode(U8 channelId,U8 enable)
{
    if(enable!=0 && enable!=1) {
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }

    if(channelId>BRCM_CMC_MAX_US_CH) {
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }


    upStreamChannelInfo[channelId-1].us_ch_id = channelId;
    upStreamChannelInfo[channelId-1].d30_mode= enable;
    return BASE_OK;
}

BASE_ErrCode
GetUpStreamChannelDocsis30Mode(U8 channelId,U8* enablePoint)
{
    if((channelId>BRCM_CMC_MAX_US_CH)||(enablePoint == NULL)) {
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }
    *enablePoint = upStreamChannelInfo[channelId-1].d30_mode;
    return BASE_OK;
}

/*************************************************************************/

BASE_ErrCode
SetUpStreamChannelType(U8 channelId,U8 mode)
{
    if(channelId>BRCM_CMC_MAX_US_CH) {
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }

    if(mode!=2 && mode!=3) {
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }

    upStreamChannelInfo[channelId-1].mode= mode;
    return BASE_OK;
}

BASE_ErrCode
GetUpStreamChannelType(U8 channelId,U8* mode)
{
    if((channelId>BRCM_CMC_MAX_US_CH)||(mode == NULL)) {
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }
    *mode = upStreamChannelInfo[channelId-1].mode;
    return BASE_OK;
}

/*************************************************************************/

BASE_ErrCode
SetUpStreamChannelFftAveraging(U8 fft_averaging)
{
    fftAveraging = fft_averaging;
    return BASE_OK;
}

BASE_ErrCode
GetUpStreamChannelFftAveraging(U8* fft_averaging)
{
    if(fft_averaging == NULL) {
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }
    *fft_averaging = fftAveraging;
    return BASE_OK;
}

/*************************************************************************/

BASE_ErrCode
GetAllUpStreamChannel(CMCOAM_GetUsCfgRspMsgT * AllUsdata)
{
    U8 i=0;

    if(NULL == AllUsdata)
        return BASE_ERROR_PARAM;

    for(i=0; i<AllUsdata->num_of_channels; i++) {
#if 0
        upStreamChannelInfo[i].us_ch_id = AllUsdata->uscfg[i].us_ch_id;
        upStreamChannelInfo[i].enabled= AllUsdata->uscfg[i].enable;
        upStreamChannelInfo[i].freq = ntohl(AllUsdata->uscfg[i].freq);
        upStreamChannelInfo[i].width = ntohl(AllUsdata->uscfg[i].width);
        upStreamChannelInfo[i].profile_type= AllUsdata->uscfg[i].profile_type;
        upStreamChannelInfo[i].d30_mode= AllUsdata->uscfg[i].d30_mode;
        upStreamChannelInfo[i].minislot_size = ntohl(AllUsdata->uscfg[i].minislot_size);
        upStreamChannelInfo[i].trsmit_time_offset = ntohl(AllUsdata->uscfg[i].trsmit_time_offset);
        upStreamChannelInfo[i].initRNG_win= AllUsdata->uscfg[i].initRNG_win;
        upStreamChannelInfo[i].finRNG_win= AllUsdata->uscfg[i].finRNG_win;
        upStreamChannelInfo[i].initData_win= AllUsdata->uscfg[i].initData_win;
        upStreamChannelInfo[i].finData_win= AllUsdata->uscfg[i].finData_win;
        upStreamChannelInfo[i].scdma_code= AllUsdata->uscfg[i].scdma_code;
        upStreamChannelInfo[i].scdma_code_per_minislot= AllUsdata->uscfg[i].scdma_code_per_minislot;
        upStreamChannelInfo[i].scdma_frm_size= AllUsdata->uscfg[i].scdma_frm_size;
        upStreamChannelInfo[i].scdma_hopping_seed = ntohs(AllUsdata->uscfg[i].scdma_hopping_seed);
        upStreamChannelInfo[i].mode= AllUsdata->uscfg[i].mode;
        upStreamChannelInfo[i].pre_equalization= AllUsdata->uscfg[i].pre_equalization;
        upStreamChannelInfo[i].ifindex = ntohs(AllUsdata->uscfg[i].ifindex);
#else
        AllUsdata->uscfg[i].freq = ntohl(AllUsdata->uscfg[i].freq);
        AllUsdata->uscfg[i].width = ntohl(AllUsdata->uscfg[i].width);
        AllUsdata->uscfg[i].minislot_size = ntohl(AllUsdata->uscfg[i].minislot_size);
        AllUsdata->uscfg[i].trsmit_time_offset = ntohl(AllUsdata->uscfg[i].trsmit_time_offset);
        AllUsdata->uscfg[i].scdma_hopping_seed = ntohs(AllUsdata->uscfg[i].scdma_hopping_seed);
        AllUsdata->uscfg[i].ifindex = ntohs(AllUsdata->uscfg[i].ifindex);

        memcpy(&(upStreamChannelInfo[i]) , &(AllUsdata->uscfg[i]) , sizeof(CMCOAM_UsChannelCfgT));

#endif
    }

    return BASE_OK;
}

BASE_ErrCode
GetFirstUpStreamChannel(UpStreamChannel ** Usdata)
{
    if(Usdata == NULL) {
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }

    *Usdata = &(upStreamChannelInfo[0]);

    return BASE_OK;
}

BASE_ErrCode
GetNextUpStreamChannel(UpStreamChannel * NowUsdata , UpStreamChannel ** NextUsdata)
{
    if(NowUsdata->us_ch_id > BRCM_CMC_MAX_US_CH)
        return BASE_ERROR;

    if((NowUsdata == NULL)||(NextUsdata == NULL)) {
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }

    *NextUsdata = &(upStreamChannelInfo[NowUsdata->us_ch_id]);

    //dbgstrm << "=========Next000 us_ch_id " << (int)NowUsdata->us_ch_id << endl;

    return BASE_OK;
}

BASE_ErrCode
FindUpStreamChannel(UpStreamChannel ** Usdata , U8 us_id)
{
    if(us_id > BRCM_CMC_MAX_US_CH)
        return BASE_ERROR;

    if(Usdata == NULL) {
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }

    *Usdata = &(upStreamChannelInfo[us_id-1]);

    return BASE_OK;
}
/*************************************************************************/

BASE_ErrCode
GetAllDownStreamChannel(CMCOAM_GetDsCfgRspMsgT * AllDsdata)
{
    int i=0;
    if(NULL == AllDsdata)
        return BASE_ERROR_PARAM;

    //Only need to convert the network to host.
    for(i=0; i<AllDsdata->num_of_channels; i++) {
        AllDsdata->dscfg[i].power_level = ntohs(AllDsdata->dscfg[i].power_level);
        AllDsdata->dscfg[i].ifindex = ntohs(AllDsdata->dscfg[i].ifindex);
    }

    return BASE_OK;
}

BASE_ErrCode
GetFirstDownStreamChannel(DownStreamChannel ** Dsdata)
{
    if(NULL == Dsdata)
        return BASE_ERROR_PARAM;
    *Dsdata = &(downStreamChannelInfo[0]);

    return BASE_OK;
}

BASE_ErrCode
GetNextDownStreamChannel(DownStreamChannel * NowDsdata , DownStreamChannel ** NextDsdata)
{
    if(NowDsdata->ds_ch_id >= BRCM_CMC_MAX_DS_CH)
        return BASE_ERROR;

    if((NowDsdata == NULL)||(NextDsdata == NULL)) {
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }


    *NextDsdata = &(downStreamChannelInfo[NowDsdata->ds_ch_id]);

    //dbgstrm << "=========Next000 ds_ch_id " << (int)NowDsdata->ds_ch_id << endl;

    return BASE_OK;
}

BASE_ErrCode
FindDownStreamChannel(DownStreamChannel ** Dsdata , U8 ds_id)
{
    if(ds_id > BRCM_CMC_MAX_DS_CH)
        return BASE_ERROR;

    if(Dsdata == NULL) {
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }

    *Dsdata = &(downStreamChannelInfo[ds_id-1]);

    return BASE_OK;
}

/*************************************************************************/

BASE_ErrCode
GetAllUpStreamChannelSignalQuality(CMCOAM_GetSignalQualityRspT * AllUsSigdata)
{
    U8 i=0;

    if(NULL == AllUsSigdata)
        return BASE_ERROR_PARAM;


    for(i=0; i<AllUsSigdata->num_of_channels; i++) {
#if 0
        AllUsSigdata->channel[i].ch_index = ntohs(AllUsSigdata->channel[i].ch_index);
        AllUsSigdata->channel[i].unerr_code = ntohll(AllUsSigdata->channel[i].unerr_code);
        AllUsSigdata->channel[i].uncorrectted_code = ntohll(AllUsSigdata->channel[i].uncorrectted_code);
        AllUsSigdata->channel[i].corrected_code = ntohll(AllUsSigdata->channel[i].corrected_code);
        AllUsSigdata->channel[i].snr = ntohl(AllUsSigdata->channel[i].snr);
#endif

        memcpy(&(upStreamSignalQualityInfo[i]) , &(AllUsSigdata->channel[i]) , sizeof(CMCOAM_UpstreamIfcQualT));
    }
    return BASE_OK;
}

BASE_ErrCode
GetFirstUpStreamChannelSignalQuality(UpStreamSignalQuality ** UsSigdata)
{
    if(UsSigdata == NULL) {
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }

    *UsSigdata = &(upStreamSignalQualityInfo[0]);

    return BASE_OK;
}

BASE_ErrCode
GetNextUpStreamChannelSignalQuality(UpStreamSignalQuality * NowUsSigdata , UpStreamSignalQuality ** NextUsSigdata)
{
    if((NowUsSigdata->ch_index - 200) >= BRCM_CMC_MAX_US_CH)
        return BASE_ERROR;

    if((NowUsSigdata == NULL)||(NextUsSigdata == NULL)) {
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }

    *NextUsSigdata = &(upStreamSignalQualityInfo[NowUsSigdata->ch_index - 200]);
    //dbgstrm << "=========Next000 us_ch_index " << (int)NowUsSigdata->ch_index << endl;
    return BASE_OK;
}

BASE_ErrCode
FindUpStreamChannelSignalQuality(UpStreamSignalQuality ** UsSigdata , U8 us_id)
{
    if(us_id > BRCM_CMC_MAX_US_CH)
        return BASE_ERROR;

    if(UsSigdata == NULL) {
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }

    *UsSigdata = &(upStreamSignalQualityInfo[us_id-1]);

    return BASE_OK;
}


/*************************************************************************/


BASE_ErrCode
ClearAllChannelConfig()
{
    memset(downStreamChannelInfo, 0, sizeof(DownStreamChannel)*BRCM_CMC_MAX_DS_CH);
    memset(upStreamChannelInfo, 0, sizeof(UpStreamChannel)*BRCM_CMC_MAX_US_CH);
    return BASE_OK;
}

BASE_ErrCode
PrintAllChannelConfig()
{
    int i = 0;
    for( i=0; i<BRCM_CMC_MAX_DS_CH ; ++i) {
        cout << " ds_ch_id " << (int)downStreamChannelInfo[i].ds_ch_id << "   ";
        cout << "enable " << (int)downStreamChannelInfo[i].enabled << "   ";
        cout << "freq " << downStreamChannelInfo[i].freq<< "   ";
        cout << "modulatoin " << (int)downStreamChannelInfo[i].modulatoin << "   ";
        cout << "annex " << (int)downStreamChannelInfo[i].annex<< "   ";
        cout << "interleaver " << (int)downStreamChannelInfo[i].interleaver<< "   ";
        cout << "power_level " << downStreamChannelInfo[i].power_level<< endl;
    }

    for( i=0; i<BRCM_CMC_MAX_US_CH ; ++i) {
        cout << " us_ch_id " << (int)upStreamChannelInfo[i].us_ch_id<< "   ";
        cout << "enable " << (int)upStreamChannelInfo[i].enabled<< "   ";
        cout << "freq " << upStreamChannelInfo[i].freq<< "   ";
        cout << "width " << upStreamChannelInfo[i].width<< "   ";
        cout << "profile_type " << (int)upStreamChannelInfo[i].profile_type<< "   ";
        cout << "power_level " << upStreamPowerLevelInfo[i]<< "   ";
        cout << "d30_mode " << (int)upStreamChannelInfo[i].d30_mode<< "   ";
        cout << "mode " << (int)upStreamChannelInfo[i].mode<< endl;
    }

    return BASE_OK;
}

/*************************************************************************/

BASE_ErrCode
AllChannelInitialize()
{
    int i = 0;
    for( i=0; i<BRCM_CMC_MAX_DS_CH ; ++i) {
        downStreamChannelInfo[i].ds_ch_id = i+1;
        downStreamChannelInfo[i].enabled =CHANNEL_DISENABLE;
        //downStreamChannelInfo[i].freq  = DS_CHANNEL_FREQ_START + i*DS_CHANNEL_FREQ_STEP;
        downStreamChannelInfo[i].freq  = 0;
        downStreamChannelInfo[i].modulatoin = DS_CHANNEL_MODULATION;
        downStreamChannelInfo[i].annex = DS_CHANNEL_ANNEX;
        downStreamChannelInfo[i].interleaver   = DS_CHANNEL_INTERLEAVE;
        downStreamChannelInfo[i].power_level = DS_CHANNEL_POWER;
    }

    for( i=0; i<BRCM_CMC_MAX_US_CH ; ++i) {
        upStreamChannelInfo[i].us_ch_id = i+1;
        upStreamChannelInfo[i].enabled =CHANNEL_ENABLE;
        upStreamChannelInfo[i].freq = US_CHANNEL_FREQ_START + i*US_CHANNEL_FREQ_STEP;
        upStreamChannelInfo[i].width = US_CHANNEL_WIDTH;
        upStreamChannelInfo[i].profile_type  = US_CHANNEL_PROFILE;
        upStreamPowerLevelInfo[i] = US_CHANNEL_POWER;
        upStreamChannelInfo[i].d30_mode = US_CHANNEL_D30;
        upStreamChannelInfo[i].mode = US_CHANNEL_MODE;
    }

    fftAveraging = 0;//disable

    return BASE_OK;
}

