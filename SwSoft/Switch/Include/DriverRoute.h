#ifndef _L3Route_h_
#define _L3Route_h_
#include "SystemInclude.h"

struct RouteEntry
{
    bcm_if_t   intf;
    bcm_ip_t   ip;
    bcm_port_t port;   /* only used by host route */
    bcm_mac_t  mac;    /* only used by host route */
};

class Route
{
public:
    virtual ~Route() {}

    virtual bool Add(bcm_if_t, bcm_port_t, bcm_ip_t, bcm_mac_t);
    virtual bool Remove(bcm_ip_t);
    virtual int CreateEgressObject(RouteEntry const&) = 0;

protected:
    /* Use EgressObjectId as map key. */
    static std::map<int, RouteEntry> routes;
};

class CpuRoute: public Route
{
public:
    virtual int CreateEgressObject(RouteEntry const&);
};

class HostRoute: public Route
{
public:
    virtual int CreateEgressObject(RouteEntry const&);
};

#endif

