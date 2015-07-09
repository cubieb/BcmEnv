#include "SystemInclude.h"
#include <pthread.h>
#include "BcmController.h"

#include "SaveCfgFile.h"
#include "DhcpRelay.h"
#include "DhcpRelayMgt.h"

using namespace std;

void InitDhcpRelay()
{
    ;
}

void SetDhcpServerIp(struct in_addr server)
{
    DhcpRelay& dhcpRelay = DhcpRelay::GetInstance();
    dhcpRelay.ClearServerIp();
    dhcpRelay.AddServerIp(server);
}

struct in_addr GetDhcpServerIp()
{
    DhcpRelay& dhcpRelay = DhcpRelay::GetInstance();
    DhcpRelay::ServerIterator iter = dhcpRelay.DhcpServerBegin();
    assert(iter != dhcpRelay.DhcpServerEnd());

    return *iter;
}

void DeinitDhcpRelay()
{
    ;
}

static void* DhcpRelayThreadProc(void*)
{
    DhcpRelay& dhcpRelay = DhcpRelay::GetInstance();
    dhcpRelay.Start();
    return NULL;
}

void StartDhcpRealy()
{
    pthread_t threadCtxt;
    pthread_create(&threadCtxt, NULL, DhcpRelayThreadProc, NULL);
}

BASE_ErrCode SaveDhcpRelayConfig()
{
    DhcpRelay& dhcpRelay = DhcpRelay::GetInstance();
    DhcpRelay::ServerIterator iter;
    ostringstream os;

    if (dhcpRelay.DhcpServerBegin() == dhcpRelay.DhcpServerEnd())
    {
        os << "dhcp-relay " << "255.255.255.255" << endl;
        SaveCfg(os.str().c_str(), InsertToEnd);
        return BASE_OK;
    }

    for (iter = dhcpRelay.DhcpServerBegin(); iter != dhcpRelay.DhcpServerEnd(); ++iter)
    {
        os << "dhcp-relay " << inet_ntoa(*iter) << endl;
    }

    SaveCfg(os.str().c_str(), InsertToEnd);
    return BASE_OK;
}
