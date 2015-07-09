#include "SystemInclude.h"
#include <arpa/inet.h>

#include "SnmpCfgMgt.h"

using namespace std;

const char* SnmpCfgFile = "/etc/snmp/snmpd.conf";
const char* TrapV1 = "trapsink";
const char* TrapV2 = "trap2sink";
list<TrapCfgEntry> trapCfgTable;

void InitSnmpCfg()
{
    ifstream ifs(SnmpCfgFile);
    if (!ifs)
        return;

    string line;
    while(getline(ifs, line))
    {
        TrapCfgEntry entry;
        string::size_type begin, end;
        if (line.find(TrapV1) != string::npos)
        {
            begin = line.find_first_not_of(" ", strlen(TrapV1) + 1);
            end = line.find(":");
            string substr = line.substr(begin, end - begin);
            entry.trapHostIpAddr = inet_addr(substr.c_str());
            entry.trapHostSnmpVer = 1;
        }
        else if (line.find(TrapV2) != string::npos)
        {
            begin = line.find_first_not_of(" ", strlen(TrapV2) + 1);
            end = line.find(":");
            string substr = line.substr(begin, end - begin);
            entry.trapHostIpAddr = inet_addr(substr.c_str());
            entry.trapHostSnmpVer = 2;
        }
        else
            continue;

        entry.trapHostEventType = 1;
        entry.trapHostIsGosNmsSever = 1;
        strncpy(entry.trapHostCommunity, "public", MaxHostCommunitySize);
        entry.trapHostMinEventLevel = entry.trapHostSnmpVer;
        trapCfgTable.push_back(entry);
    }
}

bool GetFirstTrapHost(TrapCfgEntry* entry)
{
    list<TrapCfgEntry>::iterator iter = trapCfgTable.begin();
    if (iter != trapCfgTable.end())
    {
        *entry = *iter;
        return true;
    }

    return false;
}

bool GetNextTrapHost(uint32_t curIpAddr, TrapCfgEntry* next)
{
    list<TrapCfgEntry>::iterator iter;
    for (iter = trapCfgTable.begin(); iter != trapCfgTable.end(); ++iter)
    {
        if (iter->trapHostIpAddr == curIpAddr)
        {
            ++iter;
            break;
        }
    }

    if (iter != trapCfgTable.end())
    {
        *next = *iter;
        return true;
    }

    return false;
}

bool FindTrapHost(uint32_t ip, TrapCfgEntry* entry)
{
    list<TrapCfgEntry>::iterator iter;
    for (iter = trapCfgTable.begin(); iter != trapCfgTable.end(); ++iter)
    {
        if (iter->trapHostIpAddr == ip)
        {
            if (entry != NULL)
            {
                *entry = *iter;
            }
            return true;
        }
    }

    return false;
}

bool InsertTrapHost(TrapCfgEntry* entry)
{
    list<TrapCfgEntry>::iterator iter;
    for (iter = trapCfgTable.begin(); iter != trapCfgTable.end(); ++iter)
    {
        if (iter->trapHostIpAddr == entry->trapHostIpAddr)
        {
            return false;
        }
    }
    trapCfgTable.push_back(*entry);
    return true;
}

void DeleteTrapHost(uint32_t hostIp)
{
    list<TrapCfgEntry>::iterator iter;
    for (iter = trapCfgTable.begin(); iter != trapCfgTable.end(); ++iter)
    {
        if (iter->trapHostIpAddr == hostIp)
        {
            trapCfgTable.erase(iter);
            break;
        }
    }
}

bool UpdateTrapHost(TrapCfgEntry* entry)
{
    list<TrapCfgEntry>::iterator iter;
    for (iter = trapCfgTable.begin(); iter != trapCfgTable.end(); ++iter)
    {
        if (iter->trapHostIpAddr == entry->trapHostIpAddr)
        {
            *iter = *entry;
            return true;
        }
    }
    return false;
}

bool SaveSnmpCfg()
{
    const char* cfg= "###########################################################################\n"
        "#\n"
        "#snmpd.conf\n"
        "#\n"
        "#   - created by the snmpconf configuration program\n"
        "#\n"
        "###########################################################################\n"
        "# SECTION: Access Control Setup\n"
        "#\n"
        "#   This section defines who is allowed to talk to your running\n"
        "#   snmp agent.\n"
        "\n"
        "# rocommunity: a SNMPv1/SNMPv2c read-only access community name\n"
        "# arguments:  community [default|hostname|network/bits] [oid]\n"
        "\n"
        "#master agentx\n"
        "\n"
        "rocommunity public\n"
        "rwcommunity private";

    ofstream  ofs(SnmpCfgFile);
    if (!ofs)
        return false;

    ofs << cfg << endl;

    list<TrapCfgEntry>::iterator iter;
    for (iter = trapCfgTable.begin(); iter != trapCfgTable.end(); ++iter)
    {
        char buf[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, (void*)&iter->trapHostIpAddr, buf, INET_ADDRSTRLEN);

        switch (iter->trapHostSnmpVer)
        {
        case 1:
            ofs << "trapsink   " << buf << ":162" << endl;
            break;
        case 2:
            ofs << "trap2sink   " << buf << ":162" << endl;
            ofs << "informsink   " << buf << ":162" << endl;
            break;
        }
    }

    return true;
}

