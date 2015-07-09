#include <stdio.h>
#include <time.h>

#include "SystemInclude.h"
#include "BcmController.h"
#include "Debug.h"

#include "AutoLock.h"
#include "SaveCfgFile.h"
#include "ChannelDB.h"
#include "ChannelMgt.h"

using namespace std;

pthread_mutex_t ChannelmgtMutex = PTHREAD_MUTEX_INITIALIZER;

ostringstream os;

//ds
/*************************************************************************/

//cable downstream <UINT:channel> shutdown
BASE_ErrCode
MgtChannelSetDownstreamSettingsEnabled(unsigned char channel_id,unsigned char enabled)
{
    BASE_ErrCode ret = BASE_ERROR;
    AutoLock autoLock(ChannelmgtMutex);

    if(channel_id>BRCM_CMC_MAX_DS_CH) {
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }

    if(enabled!=0 && enabled!=1 && enabled!=2) {
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }

    ret = CMCOAM_Channel_SetDownstreamSettingsEnabled(channel_id, enabled, cliGetCmc());

    if(ret == BASE_OK) {
        return SetDownStreamChannelEnable(channel_id,enabled);
    } else
        return BASE_ERROR;
}

BASE_ErrCode
MgtChannelGetDownstreamSettingsEnabled(unsigned char channel_id,unsigned char* enablePoint)
{
    AutoLock autoLock(ChannelmgtMutex);
    if((channel_id>BRCM_CMC_MAX_DS_CH)||(enablePoint == NULL)) {
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }
    return GetDownStreamChannelEnable(channel_id,enablePoint);
}

BASE_ErrCode
MgtChannelSaveDownstreamSettingsEnabled()
{
    BASE_ErrCode ret;
    unsigned char enabled;
    AutoLock autoLock(ChannelmgtMutex);

    for(int i=1 ; i <= BRCM_CMC_MAX_DS_CH ; ++i) {
        os.str("");
        ret = GetDownStreamChannelEnable( i , &enabled );

        if(enabled ==0) {
            //os << "cable downstream " << (int)i
            //<< " shutdown " << endl;
            continue;
        } else {
            os << "no cable downstream " << (int)i
               << " shutdown " << endl;
        }

        if(SaveCfg(os.str().c_str() ,CONTINUE )<BASE_OK) {
            dbgstrm << "save error"<< endl;
            return BASE_ERROR;
        }
    }
    return BASE_OK;
}

/*************************************************************************/

//cable downstream <UINT:channel> mode <LIST:docsis,ipqam:mode>
BASE_ErrCode
MgtChannelSetDownstreamSettingsMode(unsigned char channel_id, unsigned char mode)
{
    BASE_ErrCode ret = BASE_ERROR;
    AutoLock autoLock(ChannelmgtMutex);
    if(channel_id>BRCM_CMC_MAX_DS_CH) {
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }

    if(mode!=1 && mode!=2) {
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }

    ret = CMCOAM_Channel_SetDownstreamSettingsMode(channel_id, mode, cliGetCmc());
    if(ret == BASE_OK) {
        return SetDownStreamChannelMode(channel_id,mode);
    } else
        return BASE_ERROR;
}

BASE_ErrCode
MgtChannelGetDownstreamSettingsMode(unsigned char channel_id,unsigned char* mode)
{
    AutoLock autoLock(ChannelmgtMutex);
    if((channel_id>BRCM_CMC_MAX_DS_CH)||(mode == NULL)) {
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }
    return GetDownStreamChannelMode(channel_id,mode);
}

BASE_ErrCode
MgtChannelSaveDownstreamSettingsMode()
{
    BASE_ErrCode ret;
    unsigned char i;
    unsigned char mode;
    AutoLock autoLock(ChannelmgtMutex);

    for(i=1 ; i <= BRCM_CMC_MAX_DS_CH ; ++i) {
        os.str("");
        ret = GetDownStreamChannelMode( i , &mode );
        if(ret < BASE_OK) {
            return BASE_ERROR;
        }

        if(mode == 0)
            continue;
        else {
            if(mode == 1) {
                os << "cable downstream " << (int)i
                   << " mode docsis " << endl;
            } else {
                if(mode ==2) {
                    os << "cable downstream " << (int)i
                       << " mode ipqam " << endl;
                } else
                    return BASE_ERROR_PARAM;

                if(SaveCfg(os.str().c_str() ,CONTINUE) < BASE_OK) {
                    dbgstrm << "no cable downstream "<< i <<" shutdown write to cfg file fail " << endl;
                    return BASE_ERROR;
                }
            }
        }
    }
    return BASE_OK;
}

/*************************************************************************/

//cable downstream <UINT:channel> annex <LIST:a,b:annex_mode> { <LIST:6952000,6875000,6900000:symbolrate> }
BASE_ErrCode
MgtChannelSetDownstreamSettingsAnnexMode(unsigned char channel_id, unsigned char annex)
{
    BASE_ErrCode ret = BASE_ERROR;
    AutoLock autoLock(ChannelmgtMutex);
    if(channel_id>BRCM_CMC_MAX_DS_CH) {
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }

    if(annex!=ANNEX_A && annex!=ANNEX_B && annex!=ANNEX_A_6875 && annex!=ANNEX_A_6900) {
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }

    ret = CMCOAM_Channel_SetDownstreamSettingsAnnexMode(channel_id, annex, cliGetCmc());
    if(ret == BASE_OK) {
        return SetDownstreamChannleAnnexMode(channel_id,annex);
    } else
        return BASE_ERROR;
}

BASE_ErrCode
MgtChannelGetDownstreamSettingsAnnexMode(unsigned char channel_id, unsigned char* annex)
{
    AutoLock autoLock(ChannelmgtMutex);
    if((channel_id>BRCM_CMC_MAX_DS_CH)||(annex == NULL)) {
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }
    return GetDownstreamChannleAnnexMode(channel_id,annex);
}

BASE_ErrCode
MgtChannelSaveDownstreamSettingsAnnexMode()
{
    BASE_ErrCode ret;
    unsigned char i;
    unsigned char annex;
    AutoLock autoLock(ChannelmgtMutex);
    //  ANNEX_A=0,
    //  ANNEX_B,
    //  ANNEX_A_6875=254, //Annex A 6.875 symbol rate
    //  ANNEX_A_6900=255, //Annex A 6.900 symbol rate

    for(i=1 ; i <= BRCM_CMC_MAX_DS_CH ; ++i) {
        os.str("");
        ret = GetDownstreamChannleAnnexMode( i , &annex );
        if(ret < BASE_OK) {
            return BASE_ERROR;
        }

        switch(annex) {
            case ANNEX_A:

                //os << "cable downstream " << (int)i
                //<< " annex a 6952000" << endl;
                continue;

                break;

            case ANNEX_A_6875:

                os << "cable downstream " << (int)i
                   << " annex a 6875000" << endl;

                break;

            case ANNEX_A_6900:

                os << "cable downstream " << (int)i
                   << " annex a 6875000" << endl;

                break;

            case ANNEX_B:

                os << "cable downstream " << (int)i
                   << " annex b" << endl;

                break;
        }

        if(SaveCfg(os.str().c_str() ,CONTINUE) < BASE_OK) {
            dbgstrm << "no cable downstream " << i << " shutdown write to cfg file fail " << endl;
            return BASE_ERROR;
        }
    }

    return BASE_OK;
}

/*************************************************************************/

//cable downstream <UINT:channel> freq <UINT:freq>
BASE_ErrCode
MgtChannelSetDownstreamSettingsFreq(unsigned char channel_id, unsigned int freq)
{
    //单个通道间距不超过4倍带宽，整体范围75M ~
    BASE_ErrCode ret = BASE_ERROR;
    AutoLock autoLock(ChannelmgtMutex);
    if(channel_id>BRCM_CMC_MAX_DS_CH) {
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }
    
    ret = CMCOAM_Channel_SetDownstreamSettingsFreq(channel_id, freq, cliGetCmc());
    if(ret == BASE_OK) {
        return SetDownStreamChannelFrequency(channel_id,freq);
    } else
        return BASE_ERROR_PARAM;
}

BASE_ErrCode
MgtChannelGetDownstreamSettingsFreq(unsigned char channel_id, unsigned int* freq)
{
    AutoLock autoLock(ChannelmgtMutex);
    if((channel_id>BRCM_CMC_MAX_DS_CH)||(freq == NULL)) {
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }
    return GetDownStreamChannelFrequency(channel_id,freq);
}

BASE_ErrCode
MgtChannelSaveDownstreamSettingsFreq()
{
    BASE_ErrCode ret;
    unsigned char i;
    unsigned int freq;
    AutoLock autoLock(ChannelmgtMutex);

    for(i=1 ; i <= BRCM_CMC_MAX_DS_CH ; ++i) {
        os.str("");
        ret = GetDownStreamChannelFrequency( i , &freq );
        if(ret < BASE_OK) {
            return BASE_ERROR;
        }

        os << "cable downstream " << (int)i
           << " freq " << (int)freq << endl;

        if(SaveCfg(os.str().c_str() ,CONTINUE) < BASE_OK) {
            dbgstrm << "no cable downstream "<< i <<" shutdown write to cfg file fail " << endl;
            return BASE_ERROR;
        }
    }

    return BASE_OK;
}

/*************************************************************************/

//cable downstream freq auto-assign <UINT:start> <UINT:step> <LIST:increase,decrease:mode>
BASE_ErrCode
MgtChannelAutoSetDownstreamSettingsFreq(unsigned int freq, unsigned int step, unsigned char mode)
{
    BASE_ErrCode ret = BASE_ERROR;
    AutoLock autoLock(ChannelmgtMutex);

    if(mode != 1 && mode != 2) {
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }

    ret = CMCOAM_Channel_AutoAssignDownstreamSettingsFreq(freq, step, mode, cliGetCmc());
    if(ret == BASE_OK) {
        return AutoSetDownStreamChannelFrequency(freq, step, mode);
    } else
        return BASE_ERROR_PARAM;
}

BASE_ErrCode
MgtChannelAutoGetDownstreamSettingsFreq(unsigned int* freq, unsigned int* step, unsigned char* mode)
{
    AutoLock autoLock(ChannelmgtMutex);
    if((freq == NULL)||(step == NULL)||(mode == NULL)) {
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }
    return AutoGetDownStreamChannelFrequency(freq, step, mode);
}

BASE_ErrCode
MgtChannelAutoSaveDownstreamSettingsFreq()
{
    BASE_ErrCode ret = BASE_ERROR;
    unsigned int freq;
    unsigned int step;
    unsigned char mode;
    AutoLock autoLock(ChannelmgtMutex);
    os.str("");
    ret = AutoGetDownStreamChannelFrequency( &freq, &step, &mode );
    if(ret < BASE_OK) {
        return BASE_ERROR;
    }

    if(freq == 0)
        return BASE_OK;//if didn't set once will be go there

    if(mode == 1)
        os << "cable downstream freq auto-assign " << (int)freq <<" " << (int)step << " increase" << endl;
    else
        os << "cable downstream freq auto-assign " << (int)freq << " " << (int)step << " decrease" << endl;

    if(SaveCfg(os.str().c_str() ,CONTINUE) < BASE_OK) {
        dbgstrm << "no cable downstream "<<" shutdown write to cfg file fail " << endl;
        return BASE_ERROR;
    }

    return BASE_OK;
}

/*************************************************************************/

//cable downstream <UINT:channel> interleaver <LIST:128_1,128_2,64_2,128_3,32_4,128_4,16_8,128_5,8_16,128_6,4_32,128_7,2_64,128_8,1_128:interleaver>
BASE_ErrCode
MgtChannelSetDownstreamSettingsInterleaver(unsigned char channel_id, unsigned char Interleaver)
{
    BASE_ErrCode ret = BASE_ERROR;
    AutoLock autoLock(ChannelmgtMutex);
    if(channel_id>BRCM_CMC_MAX_DS_CH) {
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }

    if(Interleaver>14) {
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }

    ret = CMCOAM_Channel_SetDownstreamSettingsInterleaver(channel_id, Interleaver, cliGetCmc());
    if(ret == BASE_OK) {
        return SetDownstreamChannelInterleaver(channel_id,Interleaver);
    } else
        return BASE_ERROR;
}

BASE_ErrCode
MgtChannelGetDownstreamSettingsInterleaver(unsigned char channel_id, unsigned char* Interleaver)
{
    AutoLock autoLock(ChannelmgtMutex);
    if((channel_id<1)&&(channel_id>BRCM_CMC_MAX_DS_CH)||(Interleaver == NULL)) {
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }
    return GetDownstreamChannelInterleaver(channel_id,Interleaver);
}

BASE_ErrCode
MgtChannelSaveDownstreamSettingsInterleaver()
{
    BASE_ErrCode ret;
    unsigned char i;
    unsigned char Interleaver;
    AutoLock autoLock(ChannelmgtMutex);

    //0={128,1}, 1={128,2}, 2={64,2}, 3={128,3],
    //4={32,4}, 5={128,4}, 6={16,8}, 7={128,5},
    //8={8,16}, 9={128,6}, 10={4,32}, 11={128,7},
    //12={2,64}, 13={128,8}, 14={1,128}
    string Interleavers[] = {
        "128_1",
        "128_2",
        "64_2",
        "128_3",
        "32_4",
        "128_4",
        "16_8",
        "128_5",
        "8_16",
        "128_6",
        "4_32",
        "128_7",
        "2_64",
        "128_8",
        "1_128",
    };

    for(i=1 ; i <= BRCM_CMC_MAX_DS_CH ; ++i) {
        os.str("");
        ret = GetDownstreamChannelInterleaver( i , &Interleaver );
        if(ret < BASE_OK) {
            return BASE_ERROR;
        }
        os << "cable downstream " << (int)i
           << " interleaver " << Interleavers[Interleaver] << endl;

        if(SaveCfg(os.str().c_str() ,CONTINUE) < BASE_OK) {
            dbgstrm << "no cable downstream "<< i <<" shutdown write to cfg file fail " << endl;
            return BASE_ERROR;
        }
    }
    return BASE_OK;
}

/*************************************************************************/

//cable downstream <UINT:channel> modulation <LIST:64QAM,256QAM,1024QAM:modulation>
BASE_ErrCode
MgtChannelSetDownstreamSettingsModulation(unsigned char channel_id, unsigned char modulation)
{
    BASE_ErrCode ret = BASE_ERROR;
    AutoLock autoLock(ChannelmgtMutex);
    if(channel_id>BRCM_CMC_MAX_DS_CH) {
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }

    if(modulation>2) { //modulation must be one of 0,1,2
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }

    ret = CMCOAM_Channel_SetDownstreamSettingsModulation(channel_id, modulation,cliGetCmc());
    if(ret == BASE_OK) {
        return SetDownStreamChannelModulation(channel_id,modulation);
    } else
        return BASE_ERROR;
}

BASE_ErrCode
MgtChannelGetDownstreamSettingsModulation(unsigned char channel_id, unsigned char* modulation)
{
    AutoLock autoLock(ChannelmgtMutex);
    if((channel_id>BRCM_CMC_MAX_DS_CH)||(modulation == NULL)) {
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }
    return GetDownStreamChannelModulation(channel_id,modulation);
}

BASE_ErrCode
MgtChannelSaveDownstreamSettingsModulation()
{
    BASE_ErrCode ret;
    unsigned char i;
    unsigned char modulation;
    AutoLock autoLock(ChannelmgtMutex);

    string Modulations[] = {
        "64QAM",
        "256QAM",
        "1024QAM",
    };

    for(i=1 ; i <= BRCM_CMC_MAX_DS_CH ; ++i) {
        os.str("");
        ret = GetDownStreamChannelModulation( i , &modulation );
        if(ret < BASE_OK) {
            return BASE_ERROR;
        }

        if(modulation == 0)//64QAM
            break;

        os << "cable downstream " << (int)i
           << " modulation " << Modulations[modulation] << endl;

        if(SaveCfg(os.str().c_str() ,CONTINUE) < BASE_OK) {
            dbgstrm << "no cable downstream "<< i <<" shutdown write to cfg file fail " << endl;
            return BASE_ERROR;
        }
    }
    return BASE_OK;
}

/*************************************************************************/

//cable downstream <UINT:channel> power <INT:power>
BASE_ErrCode
MgtChannelSetDownstreamSettingsPowerLevel(unsigned char channel_id, unsigned int power)
{
    BASE_ErrCode ret = BASE_ERROR;
    AutoLock autoLock(ChannelmgtMutex);

    if(power > 600 ) {
        dbgstrm << "Invalid power level "<< power <<"(<=600 tenth of dBmv)\n" << endl;
        return BASE_ERROR_PARAM;
    }

    if(channel_id>BRCM_CMC_MAX_DS_CH) {
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }

    ret = CMCOAM_Channel_SetDownstreamSettingsPowerLevel(channel_id, power, cliGetCmc());
    if(ret == BASE_OK) {
        return SetDownStreamChannelPowerLevel(channel_id,power);
    } else
        return BASE_ERROR;
}

BASE_ErrCode
MgtChannelGetDownstreamSettingsPowerLevel(unsigned char channel_id, unsigned int* power)
{
    AutoLock autoLock(ChannelmgtMutex);
    if((channel_id>BRCM_CMC_MAX_DS_CH)||(power == NULL)) {
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }
    return GetDownStreamChannelPowerLevel(channel_id,power);
}

BASE_ErrCode
MgtChannelSaveDownstreamSettingsPowerLevel()
{
    BASE_ErrCode ret;
    unsigned char i;
    unsigned int power;
    AutoLock autoLock(ChannelmgtMutex);

    for(i=1 ; i <= BRCM_CMC_MAX_DS_CH ; ++i) {
        os.str("");
        ret = GetDownStreamChannelPowerLevel( i , &power );
        if(ret < BASE_OK) {
            return BASE_ERROR;
        }

        if(power == 0)//0 dBmV
            break;

        os << "cable downstream " << (int)i
           << " power " << int(power)<< endl;

        if(SaveCfg(os.str().c_str() ,CONTINUE) < BASE_OK) {
            dbgstrm << "no cable downstream "<< i <<" shutdown write to cfg file fail  " << endl;
            return BASE_ERROR;
        }
    }

    return BASE_OK;
}

//us
/*************************************************************************/

//cable upstream <UINT:channel> shutdown
BASE_ErrCode
MgtChannelSetUpstreamSettingsEnabled(unsigned char channel_id,unsigned char enabled)
{
    BASE_ErrCode ret = BASE_ERROR;
    AutoLock autoLock(ChannelmgtMutex);
    if(channel_id>BRCM_CMC_MAX_US_CH) {
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }

    if(enabled!=0 && enabled!=1) {
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }

    ret = CMCOAM_Channel_SetUpstreamSettingsEnabled(channel_id, enabled, cliGetCmc());
    if(ret == BASE_OK) {
        return SetUpStreamChannelEnabled(channel_id,enabled);
    } else
        return BASE_ERROR;
}

BASE_ErrCode
MgtChannelGetUpstreamSettingsEnabled(unsigned char channel_id,unsigned char* enabled)
{
    AutoLock autoLock(ChannelmgtMutex);
    if((channel_id>BRCM_CMC_MAX_US_CH)||(enabled == NULL)) {
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }

    return GetUpStreamChannelEnabled(channel_id,enabled);
}

BASE_ErrCode
MgtChannelSaveUpstreamSettingsEnabled()
{
    BASE_ErrCode ret;
    unsigned char i;
    unsigned char enabled;
    AutoLock autoLock(ChannelmgtMutex);

    for(i=1 ; i <= BRCM_CMC_MAX_US_CH ; ++i) {
        os.str("");
        ret = GetUpStreamChannelEnabled( i , &enabled );
        if(ret < BASE_OK) {
            return BASE_ERROR;
        }

        if(enabled ==0) {
            os << "cable upstream " << (int)i
               << " shutdown " << endl;
        } else {
            //os << "no cable upstream " << (int)i
            //<< " shutdown " << endl;
            break;
        }

        if(SaveCfg(os.str().c_str() ,CONTINUE) < BASE_OK) {
            dbgstrm << "no cable upstream "<< i <<" shutdown write to cfg file fail " << endl;
            return BASE_ERROR;
        }
    }
    return BASE_OK;
}

/*************************************************************************/

//cable upstream <UINT:channel> freq <UINT:freq>
BASE_ErrCode
MgtChannelSetUpstreamSettingsFreq(unsigned char channel_id, unsigned int freq)
{
    BASE_ErrCode ret = BASE_ERROR;
    AutoLock autoLock(ChannelmgtMutex);
    if(channel_id>BRCM_CMC_MAX_US_CH) {
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }

    ret = CMCOAM_Channel_SetUpstreamSettingsFreq(channel_id, freq, cliGetCmc());
    if(ret == BASE_OK) {
        return SetUpStreamChannelFrequency(channel_id,freq);
    } else
        return BASE_ERROR;
}

BASE_ErrCode
MgtChannelGetUpstreamSettingsFreq(unsigned char channel_id, unsigned int* freq)
{
    AutoLock autoLock(ChannelmgtMutex);
    if((channel_id<1)&&(channel_id>BRCM_CMC_MAX_US_CH)||(freq == NULL)) {
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }

    return GetUpStreamChannelFrequency(channel_id,freq);
}

BASE_ErrCode
MgtChannelSaveUpstreamSettingsFreq()
{
    BASE_ErrCode ret;
    unsigned char i;
    unsigned int freq;
    AutoLock autoLock(ChannelmgtMutex);

    for(i=1 ; i <= BRCM_CMC_MAX_US_CH ; ++i) {
        os.str("");
        ret = GetUpStreamChannelFrequency( i , &freq );
        if(ret < BASE_OK) {
            return BASE_ERROR;
        }
        os << "cable downstream " << (int)i
           << " freq " << (int)freq << endl;

        if(SaveCfg(os.str().c_str() ,CONTINUE) < BASE_OK) {
            dbgstrm << "no cable downstream "<< i <<" shutdown write to cfg file fail " << endl;
            return BASE_ERROR;
        }
    }
    return BASE_OK;
}

/*************************************************************************/

//cable downstream freq auto-assign <UINT:start> <UINT:step> <LIST:increase,decrease:mode>
BASE_ErrCode
MgtChannelAutoSetUpstreamSettingsFreq(unsigned int freq, unsigned int step, unsigned char mode)
{
    BASE_ErrCode ret = BASE_ERROR;
    AutoLock autoLock(ChannelmgtMutex);
    ret = CMCOAM_Channel_AutoAssignUpstreamSettingsFreq(freq, step, mode, cliGetCmc());
    if(ret == BASE_OK) {
        return AutoSetUpStreamChannelFrequency(freq, step, mode);
    } else
        return BASE_ERROR_PARAM;
}

BASE_ErrCode
MgtChannelAutoGetUpstreamSettingsFreq(unsigned int* freq, unsigned int* step, unsigned char* mode)
{
    AutoLock autoLock(ChannelmgtMutex);
    if((freq == NULL)||(step == NULL)||(mode == NULL)) {
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }
    return AutoGetUpStreamChannelFrequency(freq, step, mode);
}

BASE_ErrCode
MgtChannelAutoSaveUpstreamSettingsFreq()
{
    BASE_ErrCode ret = BASE_ERROR;
    unsigned int freq;
    unsigned int step;
    unsigned char mode;
    AutoLock autoLock(ChannelmgtMutex);
    os.str("");
    ret = AutoGetUpStreamChannelFrequency( &freq, &step, &mode );
    if(ret < BASE_OK) {
        return BASE_ERROR;
    }

    if(freq == 9000000)
        return BASE_OK;

    if(mode == 1)
        os << "cable upstream freq auto-assign " << (int)freq << " " << (int)step << " increase" << endl;
    else
        os << "cable upstream freq auto-assign " << (int)freq << " " << (int)step << " decrease" << endl;

    if(SaveCfg(os.str().c_str() ,CONTINUE) < BASE_OK) {
        dbgstrm << "no cable downstream "<<" shutdown write to cfg file fail " << endl;
        return BASE_ERROR;
    }
    return BASE_OK;
}

/*************************************************************************/

//cable upstream <UINT:channel> channel-width <LIST:1600000,3200000,6400000:width>
BASE_ErrCode
MgtChannelSetUpstreamSettingsChannelWidth(unsigned char channel_id, unsigned int channel_width)
{
    AutoLock autoLock(ChannelmgtMutex);
    if(channel_id>BRCM_CMC_MAX_US_CH) {
        dbgstrm << "Invalid parameter!should be 0~4" <<endl;
        return BASE_ERROR_PARAM;
    }

    if(channel_width!=1600000 && channel_width!=3200000 && channel_width!=6400000 ) {
        dbgstrm << "Invalid parameter!should be 1600000|3200000|6400000" <<endl;
        return BASE_ERROR_PARAM;
    }

    BASE_ErrCode ret = BASE_ERROR;
    ret = CMCOAM_Channel_SetUpstreamSettingsChannelWidth(channel_id, channel_width, cliGetCmc());
    if(ret == BASE_OK) {
        return SetUpStreamChannelWidth(channel_id,channel_width);
    } else
        return BASE_ERROR;

}

BASE_ErrCode
MgtChannelGetUpstreamSettingsChannelWidth(unsigned char channel_id, unsigned int* channel_width)
{
    AutoLock autoLock(ChannelmgtMutex);
    if((channel_id>BRCM_CMC_MAX_US_CH)||(channel_width ==NULL)) {
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }

    if(channel_id>BRCM_CMC_MAX_US_CH) {
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }
    return GetUpStreamChannelWidth(channel_id,channel_width);

}

BASE_ErrCode
MgtChannelSaveUpstreamSettingsChannelWidth()
{
    BASE_ErrCode ret;
    unsigned char i;
    unsigned int width;
    AutoLock autoLock(ChannelmgtMutex);

    for(i=1 ; i <= BRCM_CMC_MAX_US_CH ; ++i) {
        os.str("");
        ret = GetUpStreamChannelWidth( i , &width );
        if(ret < BASE_OK) {
            return BASE_ERROR;
        }

        if(width == 6400000)
            break;
        else
            os << "cable upstream " << (int)i << " channel-width " << width << endl;

        if(SaveCfg(os.str().c_str() ,CONTINUE) < BASE_OK) {
            dbgstrm << "no cable downstream "<< i <<" shutdown write to cfg file fail " << endl;
            return BASE_ERROR;
        }
    }
    return BASE_OK;
}

/*************************************************************************/

//cable upstream <UINT:channel> profile <LIST:QPSK,16QAM,32QAM,64QAM,256QAM:profile> { <LIST:scdma,atdma:type> } { <LIST:high_noise,medium_noise,low_noise,low_latency:group> }
BASE_ErrCode
MgtChannelSetUpstreamSettingsChannelProfile(unsigned char channel_id, unsigned char channel_profile)
{
    if((channel_id>BRCM_CMC_MAX_US_CH)||(channel_profile > 15)) {
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }

    BASE_ErrCode ret = BASE_ERROR;
    AutoLock autoLock(ChannelmgtMutex);
    ret = CMCOAM_Channel_SetUpstreamSettingsChannelProfile(channel_id, channel_profile, cliGetCmc());

    if(ret == BASE_OK) {
        return SetUpStreamChannelProfile(channel_id,channel_profile);
    } else
        return BASE_ERROR;

}

BASE_ErrCode
MgtChannelGetUpstreamSettingsChannelProfile(unsigned char channel_id, unsigned char* channel_profile)
{
    if((channel_id>BRCM_CMC_MAX_US_CH)||(channel_profile == NULL)) {
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }

    AutoLock autoLock(ChannelmgtMutex);
    return GetUpStreamChannelProfile(channel_id,channel_profile);
}

BASE_ErrCode
MgtChannelSaveUpstreamSettingsChannelProfile()
{
    BASE_ErrCode ret;
    unsigned char i;
    unsigned char channel_profile;
    AutoLock autoLock(ChannelmgtMutex);

    string Profiles[] = {
        "QPSK scdma high_noise",
        "16QAM scdma high_noise",
        "64QAM scdma high_noise",
        "256QAM scdma high_noise",
        "QPSK scdma medium_noise",
        "16QAM scdma medium_noise",
        "64QAM scdma medium_noise",
        "256QAM scdma medium_noise",
        "64QAM scdma low_noise",
        "256QAM scdma low_noise",
        "QPSK atdma medium_noise",
        "16QAM atdma medium_noise",
        "64QAM atdma medium_noise",
        "256QAM atdma medium_noise",
        "64QAM scdma low_latency",
        "256QAM scdma low_latency",
        "32QAM scdma medium_noise",
    };

    for(i=1 ; i <= BRCM_CMC_MAX_US_CH ; ++i) {
        os.str("");
        ret = GetUpStreamChannelProfile( i , &channel_profile );
        if(ret < BASE_OK) {
            return BASE_ERROR;
        }


        if(channel_profile == 2)//Channel Profile Type: SCDMA High Noise - 64QAM
            break;
        else
            os << "cable upstream " << (int)i << " profile " << Profiles[channel_profile] << endl;

        if(SaveCfg(os.str().c_str() ,CONTINUE) < BASE_OK) {
            dbgstrm << "no cable upstream "<< i <<" shutdown write to cfg file fail " << endl;
            return BASE_ERROR;
        }
    }
    return BASE_OK;
}

/*************************************************************************/

//cable upstream <UINT:channel> power-level <INT:power>
BASE_ErrCode
MgtChannelSetUpstreamSettingsPowerLevel(unsigned char channel_id, unsigned int power)
{
    AutoLock autoLock(ChannelmgtMutex);
    if(channel_id>BRCM_CMC_MAX_US_CH) {
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }

    //if(power < -130 || power > 230)
    if(power > 230) {
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }

    BASE_ErrCode ret = BASE_ERROR;
    ret = CMCOAM_Channel_SetUpstreamInputPower(channel_id, power, cliGetCmc());

    if(ret == BASE_OK) {
        return SetUpStreamChannelPowerLevel(channel_id,power);
    } else
        return BASE_ERROR;
}

BASE_ErrCode
MgtChannelGetUpstreamSettingsPowerLevel(unsigned char channel_id, unsigned int* power)
{
    AutoLock autoLock(ChannelmgtMutex);
    if((channel_id<1)&&(channel_id>BRCM_CMC_MAX_US_CH)||(power == NULL)) {
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }

    return GetUpStreamChannelPowerLevel(channel_id,power);
}

BASE_ErrCode
MgtChannelSaveUpstreamSettingsPowerLevel()
{    
    BASE_ErrCode ret;
    unsigned char i;
    unsigned int power;
    AutoLock autoLock(ChannelmgtMutex);

    for(i=1 ; i <= BRCM_CMC_MAX_US_CH ; ++i) {
        os.str("");
        ret = GetUpStreamChannelPowerLevel( i , &power );
        if(ret < BASE_OK) {
            return BASE_ERROR;
        }

        if(power == 60)
            break;
        else
            os << "cable upstream " << (int)i << " power-level " << (int)power << endl;

        if(SaveCfg(os.str().c_str() ,CONTINUE) < BASE_OK) {
            dbgstrm << "no cable upstream "<< i <<" shutdown write to cfg file fail " << endl;
            return BASE_ERROR;
        }
    }
    return BASE_OK;
}

/*************************************************************************/

//cable upstream <UINT:channel> docsis30 shutdown
BASE_ErrCode
MgtChannelSetUpstreamSettingsD30Mode(unsigned char channel_id,unsigned char enabled)
{
    BASE_ErrCode ret = BASE_ERROR;
    AutoLock autoLock(ChannelmgtMutex);

    if(channel_id>BRCM_CMC_MAX_US_CH) {
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }

    if(enabled!=0 && enabled!=1) {
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }

    ret = CMCOAM_Channel_SetUpstreamSettingsD30Mode(channel_id, enabled, cliGetCmc());

    if(ret == BASE_OK) {
        return SetUpStreamChannelDocsis30Mode(channel_id,enabled);
    } else
        return BASE_ERROR;
}

BASE_ErrCode
MgtChannelGetUpstreamSettingsD30Mode(unsigned char channel_id,unsigned char* enablePoint)
{
    AutoLock autoLock(ChannelmgtMutex);
    if((channel_id>BRCM_CMC_MAX_US_CH)||(enablePoint == NULL)) {
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }
    return GetUpStreamChannelDocsis30Mode(channel_id,enablePoint);
}

BASE_ErrCode
MgtChannelSaveUpstreamSettingsD30Mode()
{
    BASE_ErrCode ret;
    unsigned char i;
    unsigned char enabled;
    AutoLock autoLock(ChannelmgtMutex);

    for(i=1 ; i <= BRCM_CMC_MAX_US_CH ; ++i) {
        os.str("");
        ret = GetUpStreamChannelDocsis30Mode( i , &enabled );
        if(ret < BASE_OK) {
            return BASE_ERROR;
        }
        if(enabled == 0)
            //os << "cable upstream " << (int)i<< " docsis30 shutdown " << endl;
            break;
        else
            os << "no cable upstream " << (int)i<< " docsis30 shutdown " << endl;

        if(SaveCfg(os.str().c_str() ,CONTINUE) < BASE_OK) {
            dbgstrm << "no cable upstream "<< i <<" shutdown write to cfg file fail " << endl;
            return BASE_ERROR;
        }
    }
    return BASE_OK;
}

/*************************************************************************/

//cable upstream <UINT:channel> channel-type <LIST:atdma,scdma:type>
BASE_ErrCode
MgtChannelSetUpstreamChannelType(unsigned char channel_id, unsigned char ch_type)
{
    BASE_ErrCode ret = BASE_ERROR;
    AutoLock autoLock(ChannelmgtMutex);
    if(channel_id>BRCM_CMC_MAX_US_CH) {
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }

    if(ch_type!=2 && ch_type!=3) {
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }

    ret = CMCOAM_Channel_SetUpstreamChannelType(channel_id, ch_type, cliGetCmc());
    if(ret == BASE_OK) {
        return SetUpStreamChannelType(channel_id,ch_type);
    } else
        return BASE_ERROR;
}

BASE_ErrCode
MgtChannelGetUpstreamChannelType(unsigned char channel_id,unsigned char* ch_type)
{
    AutoLock autoLock(ChannelmgtMutex);
    if((channel_id>BRCM_CMC_MAX_US_CH)||(ch_type == NULL)) {
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }
    return GetUpStreamChannelType(channel_id,ch_type);
}

BASE_ErrCode
MgtChannelSaveUpstreamChannelType()
{
    BASE_ErrCode ret;
    unsigned char i;
    unsigned char ch_type;
    AutoLock autoLock(ChannelmgtMutex);

    for(i=1 ; i <= BRCM_CMC_MAX_US_CH ; ++i) {
        os.str("");
        ret = GetUpStreamChannelType( i , &ch_type );
        if(ret < BASE_OK) {
            return BASE_ERROR;
        }
        if(ch_type == 2)
            os << "cable upstream " << (int)i<< " channel-type atdma " << endl;
        else if(ch_type == 2)
            //os << "cable upstream " << (int)i<< " channel-type scdma " << endl;
            break;
        else
            return BASE_ERROR_PARAM;

        if(SaveCfg(os.str().c_str() ,CONTINUE) < BASE_OK) {
            dbgstrm << "no cable upstream "<< i <<" shutdown write to cfg file fail " << endl;
            return BASE_ERROR;
        }
    }
    return BASE_OK;
}

/*************************************************************************/
//get cable upstream signal quality
//show cable upstream N signal-quality

BASE_ErrCode
MgtChannelGetFirstUsSignalQuality(CMCOAM_UpstreamIfcQualT * FirstUsSigdata)
{
    assert(FirstUsSigdata);
    unsigned char   channel_id;
    CMCOAM_GetSignalQualityRspT rspmsg;
    memset(&rspmsg, 0, sizeof(rspmsg));

    static time_t last_call_time,now_time;
    double cost_time;

    AutoLock autoLock(ChannelmgtMutex);

    time(&now_time);
    cost_time = difftime(now_time , last_call_time);

    if((cost_time > 2)||(last_call_time == 0)) {
        memset(&rspmsg, 0, sizeof(rspmsg));
        channel_id = 0;//Get All Us Signal Channel
        if(CMCOAM_Channel_GetSignalQuality(channel_id, &rspmsg, cliGetCmc()) != BASE_OK)
            return BASE_ERROR;

        if(GetAllUpStreamChannelSignalQuality(&rspmsg) != BASE_OK)
            return BASE_ERROR;
    }

    time(&last_call_time);
    CMCOAM_UpstreamIfcQualT * UsSigdata;
    if(GetFirstUpStreamChannelSignalQuality(&UsSigdata) != BASE_OK)
        return BASE_ERROR;

    if(UsSigdata != NULL)
        memcpy(FirstUsSigdata , UsSigdata , sizeof(CMCOAM_UpstreamIfcQualT));
    else
        return BASE_ERROR;

    return BASE_OK;
}

BASE_ErrCode
MgtChannelGetNextUsSignalQuality(CMCOAM_UpstreamIfcQualT * NowUsSigdata ,CMCOAM_UpstreamIfcQualT * NextUsSigdata)
{
    assert(NowUsSigdata);
    assert(NextUsSigdata);
    AutoLock autoLock(ChannelmgtMutex);

    UpStreamSignalQuality * UsSigdata;

    if(GetNextUpStreamChannelSignalQuality((UpStreamSignalQuality*)NowUsSigdata , &UsSigdata) != BASE_OK)
        return BASE_ERROR;

    if(UsSigdata != NULL)
        memcpy(NextUsSigdata , (CMCOAM_UpstreamIfcQualT*)UsSigdata , sizeof(UpStreamSignalQuality));
    else
        return BASE_ERROR;

    return BASE_OK;
}

BASE_ErrCode
MgtChannelFindUsSignalQuality(CMCOAM_UpstreamIfcQualT * NowUsSigdata , unsigned char us_id)
{
    assert(NowUsSigdata);
    AutoLock autoLock(ChannelmgtMutex);

    UpStreamSignalQuality * UsSigdata;

    if(FindUpStreamChannelSignalQuality(&UsSigdata , us_id) != BASE_OK)
        return BASE_ERROR;

    if(UsSigdata != NULL)
        memcpy(NowUsSigdata , (CMCOAM_UpstreamIfcQualT*)UsSigdata , sizeof(UpStreamSignalQuality));
    else
        return BASE_ERROR;

    return BASE_OK;
}

BASE_ErrCode
MgtChannelShowUsSignalQuality(CMCOAM_UpstreamIfcQualT *NowUsSigdata)
{
    assert(NowUsSigdata);
    AutoLock autoLock(ChannelmgtMutex);

    CMCOAM_PRINT("         Interface Index = %u\n", NowUsSigdata->ch_index);
    CMCOAM_PRINT("    Contention Intervals = %u\n", NowUsSigdata->cont_interval);
    CMCOAM_PRINT("     Unerrored Codewords = %llu\n",  NowUsSigdata->unerr_code);
    CMCOAM_PRINT("     Corrected Codewords = %llu\n",  NowUsSigdata->uncorrectted_code);
    CMCOAM_PRINT(" Uncorrectable Codewords = %llu\n",  NowUsSigdata->corrected_code);
    CMCOAM_PRINT("            Upstream SNR = %.1f dB\n",  (NowUsSigdata->snr)/10.0);
    CMCOAM_PRINT("       Micro-reflections = %u\n\n",  NowUsSigdata->micro_reflect);

    return BASE_OK;
}

/*************************************************************************/


//disable upstream fft averaging   //enable upstream fft averaging
BASE_ErrCode
MgtChannelSetUsFftConfig(unsigned char fft_averaging)
{
    BASE_ErrCode ret = BASE_ERROR;
    AutoLock autoLock(ChannelmgtMutex);

    CMCOAM_UsFftConfig config;
    memset(&config, 0, sizeof(config));

    config.fft_averaging = fft_averaging;

    ret =  CMCOAM_Channel_SetUsFftConfig(&config, cliGetCmc());

    if(ret == BASE_OK) {
        return SetUpStreamChannelFftAveraging(fft_averaging);
    } else
        return BASE_ERROR;
}

BASE_ErrCode
MgtChannelGetUsFftConfig(unsigned char* fft_averaging)
{
    AutoLock autoLock(ChannelmgtMutex);
    if(fft_averaging == NULL) {
        dbgstrm << "Invalid parameter!" <<endl;
        return BASE_ERROR_PARAM;
    }
    return GetUpStreamChannelFftAveraging(fft_averaging);
}

BASE_ErrCode
MgtChannelSaveUsFftConfig()
{
    BASE_ErrCode ret;
    unsigned char fft_averaging;
    AutoLock autoLock(ChannelmgtMutex);
    os.str("");
    ret = GetUpStreamChannelFftAveraging(&fft_averaging );
    if(ret < BASE_OK) {
        return BASE_ERROR;
    }

    if(fft_averaging == 1)
        os << "cable upstream fft averaging shutdown " << endl;
    else if(fft_averaging == 0) {
        //os << "no cable upstream fft averaging shutdown " << endl;
        ;
    } else
        return BASE_ERROR_PARAM;

    if(SaveCfg(os.str().c_str() ,CONTINUE) < BASE_OK) {
        dbgstrm << "no cable downstream "<<" shutdown write to cfg file fail " << endl;
        return BASE_ERROR;
    }

    return BASE_OK;
}

/*************************************************************************/

//extern "C" void BASE_P_DbgBuf(int dbgLevel, unsigned char *p, unsigned int len, unsigned short width);

BASE_ErrCode
MgtChannelGetFirstUpstreamSettings(CMCOAM_UsChannelCfgT * FirstUsdata)
{
    assert(FirstUsdata);
    unsigned char   channel_id=0;
    CMCOAM_GetUsCfgRspMsgT  rspmsg;
    static time_t last_call_time,now_time;
    double cost_time;

    time(&now_time);

    cost_time = difftime(now_time , last_call_time);

    if((cost_time > 2)||(last_call_time == 0)) {
        //dbgstrm << "-----------------i am here" << endl;
        memset(&rspmsg, 0, sizeof(rspmsg));
        channel_id = 0;//Get All Us Channel
        if(CMCOAM_Channel_GetUpstreamSettings(channel_id, &rspmsg,cliGetCmc()) != BASE_OK)
            return BASE_ERROR;

        //Update DB data
        if(GetAllUpStreamChannel(&rspmsg) != BASE_OK)
            return BASE_ERROR;
    }

    time(&last_call_time);

    UpStreamChannel * Usdata;

    if(GetFirstUpStreamChannel(&Usdata) != BASE_OK)
        return BASE_ERROR;

    if(Usdata != NULL)
        memcpy(FirstUsdata , Usdata , sizeof(CMCOAM_UsChannelCfgT));
    else
        return BASE_ERROR;

#if 0
    CMCOAM_PRINT_BUF((unsigned char*)Usdata,sizeof(CMCOAM_UsChannelCfgT),16);
    cout << "---------------------------" <<endl;
    CMCOAM_PRINT_BUF((unsigned char*)FirstUsdata,sizeof(CMCOAM_UsChannelCfgT),16);


    cout << "=========First1 us_ch_id " << (int)Usdata->us_ch_id<< endl;
    cout << "enabled " << (int)Usdata->enabled<< endl;
    cout << "freq " << Usdata->freq<< endl;
    cout << "width " << Usdata->width<< endl;
    cout << "profile_type " << (int)Usdata->profile_type<< endl;
    //cout << "power_level " << FirstUsdata->power_level<< endl;
    cout << "d30_mode " << (int)Usdata->d30_mode<< endl;
    cout << "mode " << (int)Usdata->mode<< endl;

    cout << "=========First2 us_ch_id " << (int)FirstUsdata->us_ch_id<< endl;
    cout << "enabled " << (int)FirstUsdata->enabled<< endl;
    cout << "freq " << FirstUsdata->freq<< endl;
    cout << "width " << FirstUsdata->width<< endl;
    cout << "profile_type " << (int)FirstUsdata->profile_type<< endl;
    //cout << "power_level " << FirstUsdata->power_level<< endl;
    cout << "d30_mode " << (int)FirstUsdata->d30_mode<< endl;
    cout << "mode " << (int)FirstUsdata->mode<< endl;
#endif

    return BASE_OK;
}

BASE_ErrCode
MgtChannelGetNextUpstreamSettings(CMCOAM_UsChannelCfgT * NowUsdata ,CMCOAM_UsChannelCfgT * NextUsdata)
{
    assert(NowUsdata);
    assert(NextUsdata);
    UpStreamChannel * Usdata;

    if(GetNextUpStreamChannel((UpStreamChannel*)NowUsdata , &Usdata) != BASE_OK)
        return BASE_ERROR;

    if(Usdata != NULL)
        memcpy(NextUsdata , (CMCOAM_UsChannelCfgT*)Usdata , sizeof(UpStreamChannel));
    else
        return BASE_ERROR;

#if 0
    cout << "=========Next us_ch_id " << (int)Usdata->us_ch_id<< endl;
    cout << "enabled " << (int)Usdata->enabled<< endl;
    cout << "freq " << Usdata->freq<< endl;
    cout << "width " << Usdata->width<< endl;
    cout << "profile_type " << (int)Usdata->profile_type<< endl;
    //cout << "power_level " << FirstUsdata->power_level<< endl;
    cout << "d30_mode " << (int)Usdata->d30_mode<< endl;
    cout << "mode " << (int)Usdata->mode<< endl;
#endif

    return BASE_OK;
}

BASE_ErrCode
MgtChannelFindUpstreamSettings(CMCOAM_UsChannelCfgT * NowUsdata , unsigned char us_id)
{
    assert(NowUsdata);
    UpStreamChannel * Usdata;

    if(FindUpStreamChannel(&Usdata , us_id) != BASE_OK)
        return BASE_ERROR;

    if(Usdata != NULL)
        memcpy(NowUsdata , (CMCOAM_UsChannelCfgT*)Usdata , sizeof(UpStreamChannel));
    else
        return BASE_ERROR;

    return BASE_OK;
}

BASE_ErrCode
MgtChannelShowUpstreamSettings(CMCOAM_UsChannelCfgT *NowUsdata)
{
    assert(NowUsdata);
    CMCOAM_PRINT("           Upstream Channel ID: %u\n", NowUsdata->us_ch_id);
    CMCOAM_PRINT("                Enable/Disable: %s\n", NowUsdata->enabled ? "Enabled" : "Disabled");
    CMCOAM_PRINT("              Center Frequency: %u Hz\n", NowUsdata->freq);
    CMCOAM_PRINT("                 Channel Width: %u Hz\n", NowUsdata->width);
    CMCOAM_PRINT("          Channel Profile Type: %u\n", NowUsdata->profile_type);
    CMCOAM_PRINT("        Docsis3.0 Channel Mode: %s\n", NowUsdata->d30_mode ? "Enabled" : "Disabled");
    CMCOAM_PRINT("         Channel Minislot size: %u ticks\n", NowUsdata->minislot_size);
    CMCOAM_PRINT("Channel Transmit Timing Offset: %u\n", NowUsdata->trsmit_time_offset);
    CMCOAM_PRINT("Initial Ranging Backoff Window: %u\n", NowUsdata->initRNG_win);
    CMCOAM_PRINT("  Final Ranging Backoff Window: %u\n", NowUsdata->finRNG_win);
    CMCOAM_PRINT("   Initial Data Backoff Window: %u\n", NowUsdata->initData_win);
    CMCOAM_PRINT("     Final Data Backoff Window: %u\n", NowUsdata->finData_win);
    CMCOAM_PRINT("            Active SCDMA Codes: %u\n", NowUsdata->scdma_code);
    CMCOAM_PRINT("     SCDMA Codes per mini-slot: %u\n", NowUsdata->scdma_code_per_minislot);
    CMCOAM_PRINT("              SCDMA Frame size: %u\n", NowUsdata->scdma_frm_size);
    CMCOAM_PRINT("            SCDMA Hopping Seed: %u\n", NowUsdata->scdma_hopping_seed);
    CMCOAM_PRINT("                 Upstream Mode: %u\n", NowUsdata->mode);
    CMCOAM_PRINT("     Pre-Equalization Settings: %s\n", NowUsdata->pre_equalization ? "True" : "False");
    CMCOAM_PRINT("               Interface index: %u\n\n", NowUsdata->ifindex);

    return BASE_OK;

}

//extern "C" void BASE_P_DbgBuf(int dbgLevel, unsigned char *p, unsigned int len, unsigned short width);

/*************************************************************************/

BASE_ErrCode
MgtChannelGetFirstDownstreamSettings(CMCOAM_DsChannelCfgT * FirstDsdata)
{
    assert(FirstDsdata);
    unsigned char   channel_id=0;
    CMCOAM_GetDsCfgRspMsgT  rspmsg;
    static time_t last_call_time,now_time;
    double cost_time;

    time(&now_time);
    cost_time = difftime(now_time , last_call_time);
    if((cost_time > 2)||(last_call_time == 0)) {
        //dbgstrm << "-----------------i am here" << endl;
        memset(&rspmsg, 0, sizeof(rspmsg));
        channel_id = 0;//Get All Ds Channel
        if(CMCOAM_Channel_GetDownstreamSettings(channel_id, &rspmsg,cliGetCmc()) != BASE_OK)
            return BASE_ERROR;

        if(GetAllDownStreamChannel(&rspmsg) != BASE_OK)
            return BASE_ERROR;
    }

    time(&last_call_time);
    DownStreamChannel * Dsdata;
    if(GetFirstDownStreamChannel(&Dsdata) != BASE_OK)
        return BASE_ERROR;

    if(Dsdata != NULL)
        memcpy(FirstDsdata , Dsdata , sizeof(CMCOAM_DsChannelCfgT));
    else
        return BASE_ERROR;

#if 0
    CMCOAM_PRINT_BUF((unsigned char*)Dsdata,sizeof(CMCOAM_DsChannelCfgT),16);
    cout << "---------------------------" <<endl;
    CMCOAM_PRINT_BUF((unsigned char*)FirstDsdata,sizeof(CMCOAM_DsChannelCfgT),16);

    cout << "========= 1 ds_ch_id " << (int)Dsdata->ds_ch_id<< endl;
    cout << "enabled " << (int)Dsdata->enabled<< endl;
    cout << "freq " << Dsdata->freq<< endl;
    cout << "modulatoin " << (int)Dsdata->modulatoin<< endl;
    cout << "annex " << (int)Dsdata->annex<< endl;
    cout << "interleaver " << (int)Dsdata->interleaver<< endl;
    cout << "power_level " << (int)Dsdata->power_level<< endl;

    cout << "========= 2 ds_ch_id " << (int)FirstDsdata->ds_ch_id<< endl;
    cout << "enabled " << (int)FirstDsdata->enabled<< endl;
    cout << "freq " << FirstDsdata->freq<< endl;
    cout << "modulatoin " << (int)FirstDsdata->modulatoin<< endl;
    cout << "annex " << (int)FirstDsdata->annex<< endl;
    cout << "interleaver " << (int)FirstDsdata->interleaver<< endl;
    cout << "power_level " << (int)FirstDsdata->power_level<< endl;
#endif

    return BASE_OK;
}


BASE_ErrCode
MgtChannelGetNextDownstreamSettings(CMCOAM_DsChannelCfgT * NowDsdata ,CMCOAM_DsChannelCfgT * NextDsdata)
{
    assert(NowDsdata);
    assert(NextDsdata);
    DownStreamChannel * Dsdata;

    if(GetNextDownStreamChannel((DownStreamChannel*)NowDsdata , &Dsdata) != BASE_OK)
        return BASE_ERROR;

    if(Dsdata != NULL)
        memcpy(NextDsdata , (DownStreamChannel*)Dsdata , sizeof(DownStreamChannel));
    else
        return BASE_ERROR;

#if 0
    cout << "========= 000Next ds_ch_id " << (int)NextDsdata->ds_ch_id<< endl;
    cout << "enabled " << (int)NextDsdata->enabled<< endl;
    cout << "freq " << NextDsdata->freq<< endl;
    cout << "modulatoin " << (int)NextDsdata->modulatoin<< endl;
    cout << "annex " << (int)NextDsdata->annex<< endl;
    cout << "interleaver " << (int)NextDsdata->interleaver<< endl;
    cout << "power_level " << (int)NextDsdata->power_level<< endl;
#endif

    return BASE_OK;
}

BASE_ErrCode
MgtChannelFindDpstreamSettings(CMCOAM_DsChannelCfgT * NowDsdata , unsigned char ds_id)
{
    assert(NowDsdata);
    DownStreamChannel * Dsdata;

    if(FindDownStreamChannel(&Dsdata , ds_id) != BASE_OK)
        return BASE_ERROR;

    if(Dsdata != NULL)
        memcpy(NowDsdata , (CMCOAM_UsChannelCfgT*)Dsdata , sizeof(DownStreamChannel));
    else
        return BASE_ERROR;

    return BASE_OK;
}

BASE_ErrCode
MgtChannelShowDownstreamSettings(CMCOAM_DsChannelCfgT *NowDsdata)
{
    assert(NowDsdata);
    CMCOAM_PRINT("           Downstream Channel ID: %u\n", NowDsdata->ds_ch_id);
    CMCOAM_PRINT("                Enable/Disable: %s\n", NowDsdata->enabled ? "Enabled" : "Disabled");
    CMCOAM_PRINT("              Center Frequency: %u Hz\n", NowDsdata->freq);
    CMCOAM_PRINT("                 Channel modulatoin: %u \n", NowDsdata->modulatoin);
    CMCOAM_PRINT("          Channel annex: %u\n", NowDsdata->annex);
    CMCOAM_PRINT("        Channel interleaver: %u\n", NowDsdata->interleaver);
    CMCOAM_PRINT("         Channel power_level: %u dBmv\n", NowDsdata->power_level);
    CMCOAM_PRINT("               Interface index: %u\n\n", NowDsdata->ifindex);

    return BASE_OK;
}

/*************************************************************************/

BASE_ErrCode
SaveChannelConfig()
{
    MgtChannelSaveDownstreamSettingsEnabled();
    MgtChannelSaveDownstreamSettingsMode();
    MgtChannelSaveDownstreamSettingsAnnexMode();
    //MgtChannelSaveDownstreamSettingsFreq();
    MgtChannelAutoSaveDownstreamSettingsFreq();
    //MgtChannelSaveDownstreamSettingsInterleaver();//para changed so miss this aciton
    MgtChannelSaveDownstreamSettingsModulation();
    MgtChannelSaveDownstreamSettingsPowerLevel();
    MgtChannelSaveUpstreamSettingsEnabled();
    //MgtChannelSaveUpstreamSettingsFreq();
    MgtChannelAutoSaveUpstreamSettingsFreq();
    MgtChannelSaveUpstreamSettingsChannelWidth();
    MgtChannelSaveUpstreamSettingsChannelProfile();
    MgtChannelSaveUpstreamSettingsPowerLevel();
    MgtChannelSaveUpstreamSettingsD30Mode();
    MgtChannelSaveUpstreamChannelType();
    MgtChannelSaveUsFftConfig();

    return BASE_OK;
}

BASE_ErrCode
CreatChannelConfig()
{
    //ds
    if(SetDownStreamChannelEnable(0, 1) < BASE_OK)
        return BASE_ERROR;

    if(SetDownStreamChannelMode(0, 1) < BASE_OK)
        return BASE_ERROR;

    if(SetDownstreamChannleAnnexMode(0, 1) < BASE_OK)
        return BASE_ERROR;

    if(SetDownStreamChannelFrequency(1,4400000) < BASE_OK)
        return BASE_ERROR;

    if(AutoSetDownStreamChannelFrequency(440000000, 8000000,  1) < BASE_OK)
        return BASE_ERROR;

    if(SetDownstreamChannelInterleaver(0, 1) < BASE_OK)
        return BASE_ERROR;

    if(SetDownStreamChannelModulation(0, 1) < BASE_OK)
        return BASE_ERROR;

    if(SetDownStreamChannelPowerLevel(0,100) < BASE_OK)
        return BASE_ERROR;

    //us
    if(SetUpStreamChannelEnabled(0, 1) < BASE_OK)
        return BASE_ERROR;

    if(SetUpStreamChannelFrequency(1,24000000) < BASE_OK)
        return BASE_ERROR;

    if(AutoSetUpStreamChannelFrequency(24000000, 6400000,  1) < BASE_OK)
        return BASE_ERROR;

    if(SetUpStreamChannelWidth(0,6400000) < BASE_OK)
        return BASE_ERROR;

    if(SetUpStreamChannelProfile(0, 1) < BASE_OK)
        return BASE_ERROR;

    if(SetUpStreamChannelPowerLevel(0,100) < BASE_OK)
        return BASE_ERROR;

    if(SetUpStreamChannelDocsis30Mode(0, 1) < BASE_OK)
        return BASE_ERROR;

    if(SetUpStreamChannelType(0, 1) < BASE_OK)
        return BASE_ERROR;

    if(SetUpStreamChannelFftAveraging(1) < BASE_OK)
        return BASE_ERROR;

    return BASE_OK ;
}

BASE_ErrCode
ClearChannelConfig()
{
    return ClearAllChannelConfig();

}

BASE_ErrCode
ShowChannelConfig()
{
    return PrintAllChannelConfig();
}

BASE_ErrCode
MgtChannelInitialize()
{
    return AllChannelInitialize();
}

BASE_ErrCode
MgtChannelDeInit()
{
    unsigned char enable = 0;
    //unsigned char mode = 0;
    //unsigned char annex = 0;
    unsigned int freq = 0;
    //unsigned char Interleaver = 0;
    //unsigned char modulation = 0;
    unsigned int power = 0;
    unsigned int width = 0;
    unsigned char profile = 0;
    unsigned char type = 0;
    unsigned char fft_averaging = 0;
    int i = 0;
    BASE_ErrCode ret = BASE_ERROR;

    AutoLock autoLock(ChannelmgtMutex);

    //if ds channel shutdown all para reset
    ret = CMCOAM_Channel_SetDownstreamSettingsEnabled(0, CHANNEL_DISENABLE, cliGetCmc());
    if(ret != BASE_OK) {
        cerr << "MgtChannelDeInit SetDownstream enable error" << endl ;
        return BASE_ERROR;
    }

    for( i=0; i<BRCM_CMC_MAX_US_CH ; ++i) {
        if(GetUpStreamChannelEnabled(i+1 , &enable) < BASE_OK)
            return BASE_ERROR;
        if(enable != CHANNEL_ENABLE) {
            ret = CMCOAM_Channel_SetUpstreamSettingsEnabled(i+1, CHANNEL_ENABLE, cliGetCmc());
            if(ret != BASE_OK) {
                cerr << "MgtChannelDeInit SetUpstream Enabled error" << endl ;
                return BASE_ERROR;
            }
            //cout << "leon test in MgtChannelDeInit at 016" << " enable =" << (int)enable<< endl ;
        }

        if(GetUpStreamChannelWidth(i+1, &width) < BASE_OK)
            return BASE_ERROR;
        if(width != US_CHANNEL_FREQ_STEP) {
            ret = CMCOAM_Channel_SetUpstreamSettingsChannelWidth(i+1, US_CHANNEL_FREQ_STEP, cliGetCmc());
            if(ret != BASE_OK) {
                cerr << "MgtChannelDeInit SetUpstream Width error" << endl ;
                return BASE_ERROR;
            }
            //cout << "leon test in MgtChannelDeInit at 018" << " width =" << (int)width<< endl ;
        }

        if(GetUpStreamChannelProfile(i+1, &profile) < BASE_OK)
            return BASE_ERROR;
        if(profile != US_CHANNEL_PROFILE) {
            ret = CMCOAM_Channel_SetUpstreamSettingsChannelProfile(i+1, US_CHANNEL_PROFILE, cliGetCmc());
            if(ret != BASE_OK) {
                cerr << "MgtChannelDeInit SetUpstream Profile error" << endl ;
                return BASE_ERROR;
            }
            //cout << "leon test in MgtChannelDeInit at 020" << " profile =" << (int)profile<< endl ;
        }

        if(GetUpStreamChannelPowerLevel(i+1, &power) < BASE_OK)
            return BASE_ERROR;
        if(power != US_CHANNEL_POWER) {
            ret = CMCOAM_Channel_SetUpstreamInputPower(i+1, US_CHANNEL_POWER, cliGetCmc());
            if(ret != BASE_OK) {
                cerr << "MgtChannelDeInit SetUpstream PowerLevel error" << endl ;
                return BASE_ERROR;
            }
            //cout << "leon test in MgtChannelDeInit at 022" << " power =" << (int)power<< endl ;
        }

        if(GetUpStreamChannelDocsis30Mode(i+1,&enable) < BASE_OK)
            return BASE_ERROR;
        if(enable != CHANNEL_DISENABLE) {
            ret = CMCOAM_Channel_SetUpstreamSettingsD30Mode(i+1, CHANNEL_DISENABLE, cliGetCmc());
            if(ret != BASE_OK) {
                cerr << "MgtChannelDeInit SetUpstream D30Mode error" << endl ;
                return BASE_ERROR;
            }
            //cout << "leon test in MgtChannelDeInit at 024" << " d30mode =" << (int)enable<< endl ;
        }

        if(GetUpStreamChannelType(i+1,&type) < BASE_OK)
            return BASE_ERROR;
        if(type != US_CHANNEL_MODE) {
            ret = CMCOAM_Channel_SetUpstreamChannelType(i+1, US_CHANNEL_MODE, cliGetCmc());
            if(ret != BASE_OK) {
                cerr << "MgtChannelDeInit SetUpstream Type error" << endl ;
                return BASE_ERROR;
            }
            //cout << "leon test in MgtChannelDeInit at 026" << " type =" << (int)type<< endl ;
        }
    }
    if(GetUpStreamChannelFrequency(1, &freq) < BASE_OK)
        return BASE_ERROR;
    if(freq != US_CHANNEL_FREQ_START) {
        ret = CMCOAM_Channel_AutoAssignUpstreamSettingsFreq(US_CHANNEL_FREQ_START, US_CHANNEL_FREQ_STEP, 1 ,cliGetCmc());
        if(ret != BASE_OK) {
            cerr << "MgtChannelDeInit SetUpstream Freq error" << endl ;
            return BASE_ERROR;
        }
        //cout << "leon test in MgtChannelDeInit at 028" << " freq =" << (int)freq<< endl ;
    }

    if(GetUpStreamChannelFftAveraging(&fft_averaging) < BASE_OK)
        return BASE_ERROR;
    if(fft_averaging != 0) {
        ret = CMCOAM_Channel_SetUsFftConfig(0, cliGetCmc());
        if(ret != BASE_OK) {
            cerr << "MgtChannelDeInit SetUpstream Type error" << endl ;
            return BASE_ERROR;
        }
        //cout << "leon test in MgtChannelDeInit at 030" << " fft_averaging =" << (int)fft_averaging<< endl ;
    }
    return BASE_OK;
}


