#include "SystemInclude.h"
#include "BcmSdk.h"
#include "Debug.h"

#include <bcm/stat.h>
#include "Statistics.h"

using namespace std;

int Stat_get(uint32 stat_type, uint64* stat_val)
{
    //assert(stat_type >= snmpIfInOctets && stat_type <= snmpValCount);
    assert(stat_val);
    
    int unit = 0;
    soc_port_t port = 0x02; //ge0
    bcm_stat_sync(unit);
    bcm_stat_val_e s = (bcm_stat_val_e)stat_type;

   if(bcm_stat_get(unit, port, s, stat_val) < 0 )
   {
        errstrm << "Get stat info fail !" << endl;
        return -1;
   }
   //cout << "at snmp point : " << s << " val is : " << *stat_val << endl;

   return 0;
}

