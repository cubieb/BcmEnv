
#include "SystemInclude.h"
#include "BcmSdk.h"
#include <syslog.h>

#include "Debug.h"
#include "DriverRoute.h"

using namespace std;

int CpuRoute::CreateEgressObject(RouteEntry const& entry)
{
    int unit = 0;
    int id = -1;

    bcm_l3_egress_t egress;
    bcm_l3_egress_t_init(&egress);

    egress.intf = entry.intf;
    egress.flags |= (BCM_L3_COPY_TO_CPU);

    uint32_t flags = 0;
    int rv = bcm_l3_egress_create(unit, flags, &egress, &id);
    if (rv != BCM_E_NONE)
    {
        errstrm << "Error creating egress object entry, interface="
            << entry.intf << bcm_errmsg(rv) << endl;
        assert(false);
    }

    return id;
}

int HostRoute::CreateEgressObject(RouteEntry const& entry)
{
    int unit = 0;
    int id = -1;

    bcm_l3_egress_t egress;
    bcm_l3_egress_t_init(&egress);

    egress.intf = entry.intf;
    egress.port = entry.port;
    memcpy(egress.mac_addr, entry.mac, sizeof(bcm_mac_t));

    uint32_t flags = 0;
    int rv = bcm_l3_egress_create(unit, flags, &egress, &id);
    if (rv != BCM_E_NONE)
    {
        errstrm << "Error creating egress object entry"
            << ", interface=" << entry.intf << ", port = " << entry.port
            << ", bcm msg: " <<bcm_errmsg(rv) << endl;
        assert(false);
    }

    return id;
}

/*  Cpu route example:
    RouteEntry cpuEntries[] =
    {
        {1, inet_addr("20.0.0.1"),      0, {0x0, 0x0, 0x0, 0x0, 0x0, 0x0}},
        {2, inet_addr("192.168.2.254"), 0, {0x0, 0x0, 0x0, 0x0, 0x0, 0x0}}
    };

    errstrm << "Start to create static route to myown's interface." << endl;
    CpuRoute cpuRoute;
    size_t i;
    for (i = 0; i < sizeof(cpuEntries) / sizeof(RouteEntry); ++i)
    {
        cpuRoute.AddRoute(cpuEntries[i].intf, 0, cpuEntries[i].ip,  NULL);
    }
*/
/*  Host route example:
    RouteEntry hostEntries[] =
    {
        {1, inet_addr("20.0.0.3"),      Ge0, {0x00, 0x0c, 0x29, 0x3b, 0x53, 0xc7}},
        {2, inet_addr("192.168.2.101"), Ge2, {0x00, 0x10, 0x18, 0xde, 0xad, 0x0a}}
    };
    errstrm << "Start to create static route to direct connected host." << endl;
    HostRoute hostRoute;
    for (i = 0; i < sizeof(hostEntries) / sizeof(RouteEntry); ++i)
    {
        hostRoute.AddRoute(hostEntries[i].intf,
                           hostEntries[i].port,
                           hostEntries[i].ip,
                           hostEntries[i].mac);
    }
 */
bool Route::Add(bcm_if_t intf, bcm_port_t port, bcm_ip_t ip, bcm_mac_t mac)
{
    RouteEntry entry;
    entry.intf = intf;
    entry.ip   = ip;
    if (mac != NULL)
    {
        entry.port = port;
        memcpy(entry.mac, mac, sizeof(bcm_mac_t));
    }

    int objectId;
    objectId= CreateEgressObject(entry);

    bcm_l3_host_t   host;
    bcm_l3_host_t_init(&host);

    host.l3a_ip_addr = ip;
    host.l3a_intf    = objectId;
    int unit = 0;
    int rv = bcm_l3_host_add(unit, &host);
    if (rv != BCM_E_NONE)
    {
        syslog(LOG_USER, "AddRoute error.");
        errstrm << "Error creating l3table entry, "
            << "intf = " << intf << ", ip = " << ip << ", "
            << "port = " << port << ", objectId = " << objectId <<". "
            << bcm_errmsg(rv) << endl;
        return false;
    }

    return true;
}

bool Route::Remove(bcm_ip_t ip)
{
    return true;
}


