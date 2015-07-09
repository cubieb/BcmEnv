#ifndef _ChannelMgt_h_
#define _ChannelMgt_h_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

//Swsoft api
//ds
BASE_ErrCode MgtChannelSetDownstreamSettingsEnabled(unsigned char channel_id,unsigned char enabled);
BASE_ErrCode MgtChannelGetDownstreamSettingsEnabled(unsigned char channel_id,unsigned char* enablePoint);
BASE_ErrCode MgtChannelSaveDownstreamSettingsEnabled();

BASE_ErrCode MgtChannelSetDownstreamSettingsMode(unsigned char channel_id, unsigned char mode);
BASE_ErrCode MgtChannelGetDownstreamSettingsMode(unsigned char channel_id,unsigned char* mode);
BASE_ErrCode MgtChannelSaveDownstreamSettingsMode();

BASE_ErrCode MgtChannelSetDownstreamSettingsAnnexMode(unsigned char channel_id, unsigned char annex);
BASE_ErrCode MgtChannelGetDownstreamSettingsAnnexMode(unsigned char channel_id, unsigned char* annex);
BASE_ErrCode MgtChannelSaveDownstreamSettingsAnnexMode();

BASE_ErrCode MgtChannelSetDownstreamSettingsFreq(unsigned char channel_id, unsigned int freq);
BASE_ErrCode MgtChannelGetDownstreamSettingsFreq(unsigned char channel_id, unsigned int* freq);
BASE_ErrCode MgtChannelSaveDownstreamSettingsFreq();

BASE_ErrCode MgtChannelAutoSetDownstreamSettingsFreq(unsigned int freq, unsigned int step, unsigned char mode);
BASE_ErrCode MgtChannelAutoGetDownstreamSettingsFreq(unsigned int* freq, unsigned int* step, unsigned char* mode);
BASE_ErrCode MgtChannelAutoSaveDownstreamSettingsFreq();

BASE_ErrCode MgtChannelSetDownstreamSettingsInterleaver(unsigned char channel_id, unsigned char Interleaver);
BASE_ErrCode MgtChannelGetDownstreamSettingsInterleaver(unsigned char channel_id, unsigned char* Interleaver);
BASE_ErrCode MgtChannelSaveDownstreamSettingsInterleaver();

BASE_ErrCode MgtChannelSetDownstreamSettingsModulation(unsigned char channel_id, unsigned char modulation);
BASE_ErrCode MgtChannelGetDownstreamSettingsModulation(unsigned char channel_id, unsigned char* modulation);
BASE_ErrCode MgtChannelSaveDownstreamSettingsModulation();

BASE_ErrCode MgtChannelSetDownstreamSettingsPowerLevel(unsigned char channel_id, unsigned int power);
BASE_ErrCode MgtChannelGetDownstreamSettingsPowerLevel(unsigned char channel_id, unsigned int* power);
BASE_ErrCode MgtChannelSaveDownstreamSettingsPowerLevel();

//us
BASE_ErrCode MgtChannelSetUpstreamSettingsEnabled(unsigned char channel_id,unsigned char enabled);
BASE_ErrCode MgtChannelGetUpstreamSettingsEnabled(unsigned char channel_id,unsigned char* enabled);
BASE_ErrCode MgtChannelSaveUpstreamSettingsEnabled();

BASE_ErrCode MgtChannelSetUpstreamSettingsFreq(unsigned char channel_id, unsigned int freq);
BASE_ErrCode MgtChannelGetUpstreamSettingsFreq(unsigned char channel_id, unsigned int* freq);
BASE_ErrCode MgtChannelSaveUpstreamSettingsFreq();

BASE_ErrCode MgtChannelAutoSetUpstreamSettingsFreq(unsigned int freq, unsigned int step, unsigned char mode);
BASE_ErrCode MgtChannelAutoGetUpstreamSettingsFreq(unsigned int* freq, unsigned int* step, unsigned char* mode);
BASE_ErrCode MgtChannelAutoSaveUpstreamSettingsFreq();

BASE_ErrCode MgtChannelSetUpstreamSettingsChannelWidth(unsigned char channel_id, unsigned int channel_width);
BASE_ErrCode MgtChannelGetUpstreamSettingsChannelWidth(unsigned char channel_id, unsigned int* channel_width);
BASE_ErrCode MgtChannelSaveUpstreamSettingsChannelWidth();

BASE_ErrCode MgtChannelSetUpstreamSettingsChannelProfile(unsigned char channel_id, unsigned char channel_profile);
BASE_ErrCode MgtChannelGetUpstreamSettingsChannelProfile(unsigned char channel_id, unsigned char* channel_profile);
BASE_ErrCode MgtChannelSaveUpstreamSettingsChannelProfile();


BASE_ErrCode MgtChannelSetUpstreamSettingsPowerLevel(unsigned char channel_id, unsigned int power);
BASE_ErrCode MgtChannelGetUpstreamSettingsPowerLevel(unsigned char channel_id, unsigned int* power);
BASE_ErrCode MgtChannelSaveUpstreamSettingsPowerLevel();

BASE_ErrCode MgtChannelSetUpstreamSettingsD30Mode(unsigned char channel_id,unsigned char enabled);
BASE_ErrCode MgtChannelGetUpstreamSettingsD30Mode(unsigned char channel_id,unsigned char* enabled);
BASE_ErrCode MgtChannelSaveUpstreamSettingsD30Mode();

BASE_ErrCode MgtChannelSetUpstreamChannelType(unsigned char channel_id, unsigned char ch_type);
BASE_ErrCode MgtChannelGetUpstreamChannelType(unsigned char channel_id, unsigned char* ch_type);
BASE_ErrCode MgtChannelSaveUpstreamChannelType();

BASE_ErrCode MgtChannelGetFirstUsSignalQuality(CMCOAM_UpstreamIfcQualT * FirstUsSigdata);
BASE_ErrCode MgtChannelGetNextUsSignalQuality(CMCOAM_UpstreamIfcQualT * NowUsSigdata ,CMCOAM_UpstreamIfcQualT * NextUsSigdata);
BASE_ErrCode MgtChannelFindUsSignalQuality(CMCOAM_UpstreamIfcQualT * NowUsSigdata , unsigned char us_id);
BASE_ErrCode MgtChannelShowUsSignalQuality(CMCOAM_UpstreamIfcQualT *NowUsSigdata);

BASE_ErrCode MgtChannelSetUsFftConfig(unsigned char fft_averaging);
BASE_ErrCode MgtChannelGetUsFftConfig(unsigned char* fft_averaging);
BASE_ErrCode MgtChannelSaveUsFftConfig();

BASE_ErrCode MgtChannelGetFirstUpstreamSettings(CMCOAM_UsChannelCfgT * FirstUsdata);
BASE_ErrCode MgtChannelGetNextUpstreamSettings(CMCOAM_UsChannelCfgT * NowUsdata ,CMCOAM_UsChannelCfgT * NextUsdata);
BASE_ErrCode MgtChannelFindUpstreamSettings(CMCOAM_UsChannelCfgT * NowUsdata , unsigned char us_id);
BASE_ErrCode MgtChannelShowUpstreamSettings(CMCOAM_UsChannelCfgT *NowUsdata);

BASE_ErrCode MgtChannelGetFirstDownstreamSettings(CMCOAM_DsChannelCfgT * FirstDsdata);
BASE_ErrCode MgtChannelGetNextDownstreamSettings(CMCOAM_DsChannelCfgT * NowDsdata ,CMCOAM_DsChannelCfgT * NextDsdata);
BASE_ErrCode MgtChannelFindDpstreamSettings(CMCOAM_DsChannelCfgT * NowDsdata , unsigned char ds_id);
BASE_ErrCode MgtChannelShowDownstreamSettings(CMCOAM_DsChannelCfgT *NowDsdata);


BASE_ErrCode SaveChannelConfig();
BASE_ErrCode CreatChannelConfig();
BASE_ErrCode ClearChannelConfig();
BASE_ErrCode ShowChannelConfig();

BASE_ErrCode MgtChannelInitialize();
BASE_ErrCode MgtChannelDeInit();
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
