#ifndef _ChannelDB_h_
#define _ChannelDB_h_

#include "BcmController.h"

#define CHANNEL_ENABLE 1
#define CHANNEL_DISENABLE 0
#define DS_CHANNEL_FREQ_START 440000000 //440M
#define DS_CHANNEL_FREQ_STEP    8000000 //8M
#define DS_CHANNEL_MODULATION 0 //64QAM
#define DS_CHANNEL_ANNEX 0 //ANNEX_A
#define DS_CHANNEL_INTERLEAVE 0 //128_1
#define DS_CHANNEL_POWER  0 //0 dBmv

#define US_CHANNEL_FREQ_START 9000000 //9M
#define US_CHANNEL_FREQ_STEP    6400000 //6.4M
#define US_CHANNEL_WIDTH 6400000 //6.4M
#define US_CHANNEL_PROFILE 2 //SCDMA High Noise - 64QAM
#define US_CHANNEL_POWER 60 //6.0 dB
#define US_CHANNEL_D30  0 //disable
#define US_CHANNEL_MODE  3 //scdma


//Downstream settings.
typedef CMCOAM_DsChannelCfgT DownStreamChannel;

//Upstream settings.
typedef CMCOAM_UsChannelCfgT UpStreamChannel;
typedef CMCOAM_UpstreamIfcQualT UpStreamSignalQuality;

//ds
BASE_ErrCode SetDownStreamChannelEnable(unsigned char channel_id,unsigned char enabled);
BASE_ErrCode GetDownStreamChannelEnable(unsigned int channel_id,unsigned char* enablePoint);

BASE_ErrCode SetDownStreamChannelMode(unsigned char channel_id,unsigned char mode);
BASE_ErrCode GetDownStreamChannelMode(unsigned char channel_id,unsigned char* mode);

BASE_ErrCode SetDownstreamChannleAnnexMode(unsigned char channel_id,unsigned char annex);
BASE_ErrCode GetDownstreamChannleAnnexMode(unsigned char channel_id,unsigned char* annex);

BASE_ErrCode SetDownStreamChannelFrequency(unsigned char channel_id,unsigned int freq);
BASE_ErrCode GetDownStreamChannelFrequency(unsigned char channel_id, unsigned int* freq);

BASE_ErrCode AutoSetDownStreamChannelFrequency(unsigned int freq, unsigned int step, unsigned char mode);
BASE_ErrCode AutoGetDownStreamChannelFrequency(unsigned int* freq, unsigned int* step, unsigned char* mode);

BASE_ErrCode SetDownstreamChannelInterleaver(unsigned char channel_id,unsigned char Interleaver);
BASE_ErrCode GetDownstreamChannelInterleaver(unsigned char channel_id, unsigned char* Interleaver);

BASE_ErrCode SetDownStreamChannelModulation(unsigned char channel_id,unsigned char modulation);
BASE_ErrCode GetDownStreamChannelModulation(unsigned char channel_id, unsigned char* modulation);

BASE_ErrCode SetDownStreamChannelPowerLevel(unsigned char channel_id,unsigned int power);
BASE_ErrCode GetDownStreamChannelPowerLevel(unsigned char channel_id, unsigned int* power);

//us
BASE_ErrCode SetUpStreamChannelEnabled(unsigned char channel_id,unsigned char enabled);
BASE_ErrCode GetUpStreamChannelEnabled(unsigned char channel_id,unsigned char* enabled);

BASE_ErrCode SetUpStreamChannelFrequency(unsigned char channel_id,unsigned int freq);
BASE_ErrCode GetUpStreamChannelFrequency(unsigned char channel_id,unsigned int* freq);

BASE_ErrCode AutoSetUpStreamChannelFrequency(unsigned int freq, unsigned int step, unsigned char mode);
BASE_ErrCode AutoGetUpStreamChannelFrequency(unsigned int* freq, unsigned int* step, unsigned char* mode);

BASE_ErrCode SetUpStreamChannelWidth(unsigned char channel_id,unsigned int channel_width);
BASE_ErrCode GetUpStreamChannelWidth(unsigned char channel_id,unsigned int* channel_width);

BASE_ErrCode SetUpStreamChannelProfile(unsigned char channel_id,unsigned char channel_profile);
BASE_ErrCode GetUpStreamChannelProfile(unsigned char channel_id,unsigned char* channel_profile);

BASE_ErrCode SetUpStreamChannelPowerLevel(unsigned char channel_id,unsigned int power);
BASE_ErrCode GetUpStreamChannelPowerLevel(unsigned char channel_id, unsigned int* power);

BASE_ErrCode SetUpStreamChannelDocsis30Mode(unsigned char channel_id,unsigned char enabled);
BASE_ErrCode GetUpStreamChannelDocsis30Mode(unsigned char channel_id,unsigned char* enabled);

BASE_ErrCode SetUpStreamChannelType(unsigned char channel_id,unsigned char mode);
BASE_ErrCode GetUpStreamChannelType(unsigned char channel_id,unsigned char* mode);

BASE_ErrCode SetUpStreamChannelFftAveraging(unsigned char fft_averaging);
BASE_ErrCode GetUpStreamChannelFftAveraging(unsigned char* fft_averaging);

BASE_ErrCode GetAllUpStreamChannel(CMCOAM_GetUsCfgRspMsgT * AllUsdata);
BASE_ErrCode GetFirstUpStreamChannel(UpStreamChannel ** Usdata);
BASE_ErrCode GetNextUpStreamChannel(UpStreamChannel * NowUsdata , UpStreamChannel ** NextUsdata);
BASE_ErrCode FindUpStreamChannel(UpStreamChannel ** Usdata , unsigned char us_id);

BASE_ErrCode GetAllDownStreamChannel(CMCOAM_GetDsCfgRspMsgT * AllDsdata);
BASE_ErrCode GetFirstDownStreamChannel(DownStreamChannel ** Dsdata);
BASE_ErrCode GetNextDownStreamChannel(DownStreamChannel * NowDsdata , DownStreamChannel ** NextDsdata);
BASE_ErrCode FindDownStreamChannel(DownStreamChannel ** Dsdata , unsigned char ds_id);

BASE_ErrCode GetAllUpStreamChannelSignalQuality(CMCOAM_GetSignalQualityRspT * AllUsSigdata);
BASE_ErrCode GetFirstUpStreamChannelSignalQuality(UpStreamSignalQuality ** UsSigdata);
BASE_ErrCode GetNextUpStreamChannelSignalQuality(UpStreamSignalQuality * NowUsSigdata , UpStreamSignalQuality ** NextUsSigdata);
BASE_ErrCode FindUpStreamChannelSignalQuality(UpStreamSignalQuality ** UsSigdata , unsigned char us_id);

BASE_ErrCode ClearAllChannelConfig();
BASE_ErrCode PrintAllChannelConfig();

BASE_ErrCode AllChannelInitialize();

#endif
