#include "SystemInclude.h"

#include "BcmSdk.h"
#include <arpa/inet.h>
#include <syslog.h>

#include "Util.h"
#include "LinuxCmd.h"
#include "Debug.h"
#include "Port.h"

using namespace std;

/* Ifconfig based on "l3 egress table", "l3 l3table" and "vlan table"
 * We configure l3 table and l3 route by rc.soc.   At the same time,
 * we use linux command "ifconfig" to configure ip address for every interface.
 * reffer to ()   <-->  "l3 l3table show"
 *           _l3_cmd_egress_object_show() <-->  "l3 egress show"
 *
 */
extern "C" int TraverseEgressObj(int unit, int index, bcm_l3_egress_t *info, void *cookie);
extern "C" int TraverseL3Host(int unit, int index, bcm_l3_host_t *info, void *cookie);

typedef pair<int, bcm_l3_egress_t> IndexEgressObjPair;
int TraverseEgressObj(int unit, int index, bcm_l3_egress_t *info, void *cookie)
{
    map<int, bcm_l3_egress_t>* egressObjs = (map<int, bcm_l3_egress_t>*)cookie;
    egressObjs->insert(IndexEgressObjPair(index, *info));

    return BCM_E_NONE;
}

typedef pair<int, bcm_l3_host_t> IndexL3TablePair;
int TraverseL3Table(int unit, int index, bcm_l3_host_t *info, void *cookie)
{
    map<int, bcm_l3_host_t>* l3Table = (map<int, bcm_l3_host_t>*)cookie;
    l3Table->insert(IndexL3TablePair(index, *info));
    return BCM_E_NONE;
}

bool SwitchIfconfig()
{
    int    rv, unit = 0;

    bcm_l3_info_t   l3HwStatus;
    map<int, bcm_l3_host_t> l3Table;
    map<int, bcm_l3_host_t>::iterator l3TableIter;
    rv = bcm_l3_info(unit, &l3HwStatus);
    if (rv < 0)
    {
        errstrm << "bcm_l3_info() failed!" << endl;
    }
    int fistEntry = 0;
    int lastEntry = l3HwStatus.l3info_max_host;
    bcm_l3_host_traverse(unit, 0, fistEntry, lastEntry, TraverseL3Table, &l3Table);

    map<int, bcm_l3_egress_t> egressObjs;
    map<int, bcm_l3_egress_t>::iterator objIter;
    rv = bcm_l3_egress_traverse(unit, TraverseEgressObj, (void*)&egressObjs);
    if (rv < 0)
    {
        errstrm << "bcm_l3_egress_traverse() failed!" << endl;
    }

    for (l3TableIter = l3Table.begin(); l3TableIter != l3Table.end(); ++l3TableIter)
    {

        objIter = egressObjs.find(l3TableIter->second.l3a_intf);
        if (objIter == egressObjs.end())
            continue;

        if (objIter->second.flags & BCM_L3_COPY_TO_CPU == 0)
            continue;

        bcm_l3_intf_t intf;
        bcm_l3_intf_t_init(&intf);
        intf.l3a_intf_id = objIter->second.intf;
        if (bcm_l3_intf_get(unit, &intf) < 0)
        {
            errstrm << "bcm_l3_intf_get() failed!" << endl;
            continue;
        }
        ostringstream os;
        os << "ifconfig" << " vlan" << objIter->second.vlan
            << " hw ether " << MacToString(intf.l3a_mac_addr);
        system(os.str().c_str());

        os.str("");
        os << "vlan" << objIter->second.vlan;
        struct in_addr intfIp;
        intfIp.s_addr = l3TableIter->second.l3a_ip_addr;
        ExeLinuxCommand("/sbin/ifconfig", "ifconfig", os.str().c_str(),
                        inet_ntoa(intfIp), "netmask",  "255.255.255.0");
    }

    return true;
}
