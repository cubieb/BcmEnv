/*
 * Note: this file originally auto-generated by mib2c using
 *  $
 */
#ifndef GSANCMCVENDORTOVLANTABLE_H
#define GSANCMCVENDORTOVLANTABLE_H

/* function declarations */
void InitGsAnCmcVendorToVlanTable(void);

/* column number definitions for table gsAnCmcVendorToVlanTable */
#define ColVendorItemIndex 1
#define ColVendorToVlanCmMac 2
#define ColVendorToVlanCmState 3
#define ColVendorToVlanVlanID 4
#define ColVendorToVlanVendorID 5

#define InitRng 0
#define InitRC 1
#define InitReg 2
#define Online 3 
#define other 4

#if defined(__cplusplus)
    extern "C"
    {
#endif  //#if defined(__cplusplus)

Netsnmp_Node_Handler CmcVendorToVlanTableHandler;
Netsnmp_First_Data_Point CmcVendorToVlanTableGetFirst;
Netsnmp_Next_Data_Point CmcVendorToVlanTableGetNext;
              
#if defined(__cplusplus)
    }
#endif  //#if defined(__cplusplus)

#endif /* GSANCMCVENDORTOVLANTABLE_H */