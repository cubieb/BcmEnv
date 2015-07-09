/*
 * Note: this file originally auto-generated by mib2c using.
 *        $
 */
#ifndef GSCABLEDOCSLOADBALSYSTEM_H
#define GSCABLEDOCSLOADBALSYSTEM_H

#define LoadBalMethodStatic 2
#define LoadBalMethodDisabled 0
#define LoadBalMethodDynamic 1
#define LoadBalRangingOverrideModeEnabled 1
#define LoadBalRangingOverrideModeDisabled 2

/* function declarations */
void InitGsCableDocsLoadBalSystem(void);

#if defined(__cplusplus)
extern "C"
{
#endif  //#if defined(__cplusplus)

Netsnmp_Node_Handler HandlegsCableDocsLoadBalMethod;
Netsnmp_Node_Handler HandlegsCableDocsLoadBalPeriod;
Netsnmp_Node_Handler HandlegsCableDocsLoadBalNumPeriods;
Netsnmp_Node_Handler HandlegsCableDocsLoadBalOverloadThreshold;
Netsnmp_Node_Handler HandlegsCableDocsLoadBalDifferenceThreshold;
Netsnmp_Node_Handler HandlegsCableDocsLoadBalMaxMovesPerPeriod;
Netsnmp_Node_Handler HandlegsCableDocsLoadBalMinHoldTimePerModem;
Netsnmp_Node_Handler HandlegsCableDocsLoadBalRangingOverrideMode;
Netsnmp_Node_Handler HandlegsCableDocsLoadBalDCCInitTechATDMA;
Netsnmp_Node_Handler HandlegsCableDocsLoadBalDCCInitTechSCDMA;
Netsnmp_Node_Handler HandlegsCableDocsLoadBalDBCInitTechATDMA;
Netsnmp_Node_Handler HandlegsCableDocsLoadBalDBCInitTechSCDMA;

#if defined(__cplusplus)
}
#endif  //#if defined(__cplusplus)

#endif /* GSCABLEDOCSLOADBALSYSTEM_H */
