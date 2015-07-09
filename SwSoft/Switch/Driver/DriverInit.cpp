#include "SystemInclude.h"

#include "BcmSdk.h"
#include <arpa/inet.h>
#include <syslog.h>

#include "LinuxCmd.h"
#include "Debug.h"
#include "Port.h"
#include "DriverRx.h"
#include "DriverRoute.h"
#include "DriverIfconfig.h"
#include "DriverInit.h"

using namespace std;

extern ibde_t *bde;
DriverInit::DriverInit()
{
    dbgstrm << "Start." << endl;
    bde = NULL;
}

DriverInit::~DriverInit()
{
    if (bde != NULL)
    {
        linux_bde_destroy(bde);
        bde = NULL;
    }
}

void* BcmuserThreadProc(void*)
{
    dbgstrm << "Start initialize." << endl;
    if (sal_core_init() < 0 || sal_appl_init() < 0)
    {
        errstrm << "SAL Initialization failed" << endl;
        return false;
    }

    knet_kcom_config();

    dbgstrm << "diag_shell() start." << endl;
    diag_shell();
    errstrm << "fatal error, diag_shell() stopped." << endl;

    return NULL;
}

/*
 *  Reffer to socdiag.c  "int main(int argc, char *argv[])"
 */
bool DriverInit::InitStep1()
{
    void *para = NULL;
    pthread_t thread;
    remove("/var/tmp.txt");
    pthread_create(&thread, NULL, BcmuserThreadProc, para);

    /* waiting for daig_shell() to finish reading of rc.soc.
     * 1) delete /var/tmp.txt
     * 2) while(ture) to check /var/tmp.txt.  bcm.user will create /var/tmp.txt.
     */
    while (true)
    {
        if (access("/var/tmp.txt", F_OK) == 0)
        {
            break;
        }
        sleep(1);
    }
    cout << "diag_shell() initialization done!" << endl;
    return true;
}

bool DriverInit::InitStep2()
{
    int unit = 0, rv;
    if(bcm_rx_active(unit))
    {
        dbgstrm << "rx is active before." << endl;
    }
    else
    {
        rv = bcm_rx_start(unit, NULL);
        if (rv < 0)
        {
            errstrm << "Packet receiver not started: %s"
                << bcm_errmsg(rv) << endl;

            return false;
        }
    }

    rv = bcm_rx_register(unit, "l3switch",
                         ProcessRxPacket, BCM_RX_PRIO_MAX - 1,
                         (void *) 0,
                         BCM_RCO_F_ALL_COS);
    if (rv < 0)
    {
        errstrm << "Packet monitor registration failed: %s"
            << bcm_errmsg(rv) << endl;

        return false;
    }

    return true;
}

bool DriverInit::InitStep3()
{
    /*
    ifconfig vlan1 20.0.0.1 netmask 255.255.255.0
    ifconfig vlan2 192.168.2.254 netmask 255.255.255.0
    ifconfig vlan2049 192.168.3.1 netmask 255.255.255.0
    ifconfig vlan2050 192.168.4.1 netmask 255.255.255.0
    ifconfig vlan2051 192.168.5.1 netmask 255.255.255.0
    ifconfig vlan2052 192.168.6.1 netmask 255.255.255.0
     */

    SwitchIfconfig();
    cout << "Switch Init Done!" << endl;
    return true;
}
