/*
 * Note: this file originally auto-generated by mib2c using
 *  $
 */
#ifndef GSANCMCDOWNSTREAMCHANNELTABLE_H
#define GSANCMCDOWNSTREAMCHANNELTABLE_H

/* function declarations */
void InitGsAnCmcDownstreamChannelTable(void);

/* column number definitions for table gsAnCmcDownstreamChannelTable */
/* column number definitions for table gsAnCmcUpstreamChannelTable */
#define ColCmcDoChanId 1
#define ColCmcDoChanName 2
#define ColCmcDoChanEnable 3
#define ColCmcDoChanStatus 4
#define ColCmcDoChanActiveCmCount 5
#define ColCmcDoChanRegisterCmCount 6
#define ColCmcDoChanUnregisterCmCount 7
#define ColCmcDoChanMtu 8

Netsnmp_Node_Handler CmcDsChannelTableHandler;
Netsnmp_First_Data_Point CmcDsChannelTableGetFirst;
Netsnmp_Next_Data_Point CmcDsChannelTableGetNext;


#endif /* GSANCMCDOWNSTREAMCHANNELTABLE_H */
