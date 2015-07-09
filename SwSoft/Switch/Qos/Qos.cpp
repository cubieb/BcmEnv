#include "SystemInclude.h"

#include "BcmSdk.h"
#include "Debug.h"
#include "Util.h"
#include "Qos.h"

//edit by leonli port stat test
#include <bcm/port.h>
using namespace std;
//need mac to ip and entry id
/*create a map and all need map api*/

extern "C" bcm_field_qualify_t parse_field_qualifier(char *str);
extern "C" bcm_field_action_t parse_field_action(char *str);

typedef map<uint32,data_struct_t> U32toStructMapT;
U32toStructMapT 	AU32toStructMap;		

//map modify api
//add or update map entry
int fp_qos_map_add_or_update(uint32 key , data_struct_t a_data_struct_entry)
{
    pair<std::map<uint32,data_struct_t>::iterator,bool> p;
    p = AU32toStructMap.insert((make_pair(key,a_data_struct_entry)));
    if(p.second == 0)
    {
        errstrm << "insert a data entry to map fail !" << endl;
        return -1;
    }
    else
        dbgcstrm << "insert a data entry to map success !" << endl;	
    return 0;
}

int fp_qos_map_delete(uint32 key)
{
    U32toStructMapT::iterator key2data_iter;
    key2data_iter = AU32toStructMap.find(key);
    if(key2data_iter!=AU32toStructMap.end())
    {
        AU32toStructMap.erase(key2data_iter); 
    }
    else
    {
        return -1;
    }
    return 0;
}

//search map
int fp_qos_map_search(uint32 key,data_struct_t* a_data_struct_entry_point)
{
    U32toStructMapT::iterator key2data_iter;
    key2data_iter=AU32toStructMap.find(key);
    if(key2data_iter!=AU32toStructMap.end())
    {
        memcpy(a_data_struct_entry_point,&(key2data_iter->second),sizeof(data_struct_t));	
    }
    else
    {
        return -1;
    }
    return 0;
}

int fp_qos_map_show_all()
{
    U32toStructMapT::iterator key2data_iter;
    key2data_iter = AU32toStructMap.begin();
    cout << "=======SHOW MAP "<< endl;
    while(key2data_iter!=AU32toStructMap.end())	
    {			
        cout << "group_id = " << key2data_iter->second.group_id << endl;
        cout << "entry_id = " << key2data_iter->second.entry_id << endl;
        cout << "policer_id = " << key2data_iter->second.policer_id << endl;
        cout << "ip_address = " << key2data_iter->second.ip_address << endl;
        key2data_iter++;	
    }	
    return 0;
}

//edit by leonli port stat test
int get_port_stat()
{
    unsigned long long val;
    bcm_port_stat_t stat = bcmPortStatIngressBytes;
    int rev = BCM_E_UNAVAIL;
    
    rev =  bcm_port_stat_get(0, 1, stat, &val);
    
    return 0;    
}

//fp rules modify api
STATIC int ds_qos_rule_cfg_show()
{        
    int unit = 0;
    bcm_field_show(unit, "FP");
    return 0;
}

STATIC int ds_qos_rule_cfg_delete(bcm_ip_t ip)
{
    int unit = 0;
    int bcm_rv=0;

    data_struct_t fp_rules_data_entry;
    memset(&fp_rules_data_entry, 0, sizeof(data_struct_t));        

    bcm_rv = fp_qos_map_search((uint32)ip,&fp_rules_data_entry);
    if(bcm_rv < 0)
    {
        errstrm << "search data entry fail " << endl;
        return bcm_rv;
    }
    else
    {
        dbgcstrm << "=======SEARCH MAP SUCCESS"<< endl;
        dbgcstrm << "group_id = " << fp_rules_data_entry.group_id << endl;
        dbgcstrm << "entry_id = " << fp_rules_data_entry.entry_id << endl;
        dbgcstrm << "policer_id = " << fp_rules_data_entry.policer_id << endl;
        dbgcstrm << "ip_address = " << hex << fp_rules_data_entry.ip_address << endl;            
    }

    /*map or list search entry id*/
    bcm_rv = bcm_field_entry_destroy(unit, fp_rules_data_entry.entry_id);//entry_id
    if (BCM_E_NONE != bcm_rv) 
    {
        errstrm << "Error in bcm_field_entry_destroy" << endl;
        return -1;
    }

    bcm_field_group_destroy(unit, fp_rules_data_entry.group_id);//group_id
    if (BCM_E_NONE != bcm_rv) 
    {
        errstrm << "Error in bcm_field_group_destroy" << endl;
        return -1;
    }

    bcm_rv = bcm_policer_destroy(unit, fp_rules_data_entry.policer_id);//policer_id
    if (bcm_rv != BCM_E_NONE) 
    {
        errstrm << "Error in bcm_policer_destroy with policer_id " << fp_rules_data_entry.policer_id << endl;
        return bcm_rv;
    }

    bcm_rv = fp_qos_map_delete((uint32)ip);
    if(bcm_rv < 0)
    {
        errstrm << "delete data entry fail " << endl;
        return bcm_rv;
    }

    //just for test
    //bcm_field_show(unit, "FP");

    return bcm_rv;
}

STATIC int ds_qos_rule_cfg_create(bcm_ip_t ip, bcm_ip_t mask, uint32 bandwidth)
{
    int unit = 0;
    int bcm_rv=0;
    int pri = 1;	
    int prio = 1;
    int p0 = 0;
    int p1 = 0;
    
    bcm_field_qset_t      qset;
    bcm_field_qualify_t   qual;
    bcm_field_group_t   gid;
    bcm_field_entry_t     eid;
    bcm_policer_t policer_id;
    bcm_policer_config_t     policer_cfg;
    bcm_field_action_t   action;

    data_struct_t fp_rules_data_entry;
    memset(&fp_rules_data_entry, 0, sizeof(data_struct_t));
       
    bcm_rv = fp_qos_map_search((uint32)ip,&fp_rules_data_entry);
    if(bcm_rv >= 0)
    {
        dbgcstrm << "=======SEARCH MAP SUCCESS"<< endl;
        dbgcstrm << "group_id = " << fp_rules_data_entry.group_id << endl;
        dbgcstrm << "entry_id = " << fp_rules_data_entry.entry_id << endl;
        dbgcstrm << "policer_id = " << fp_rules_data_entry.policer_id << endl;
        dbgcstrm << "ip_address = " << hex << fp_rules_data_entry.ip_address << endl;     

        if(ds_qos_rule_cfg_delete(ip) <0 )
        {
            errstrm << "delete old fp rules fail !" << endl;
            return -1;
        }
    }

    bcm_rv = 0;
    memset(&fp_rules_data_entry, 0, sizeof(data_struct_t));

    /*fp qset add*/
    BCM_FIELD_QSET_INIT(qset);
    qual = parse_field_qualifier("DstIp");
    BCM_FIELD_QSET_ADD(qset, qual);
    qual = parse_field_qualifier("Statge");
    BCM_FIELD_QSET_ADD(qset, qual);
    qual = parse_field_qualifier("StageIngress");
    BCM_FIELD_QSET_ADD(qset, qual);

    bcm_rv = bcm_field_group_create(unit, qset, pri, &gid);
    if (BCM_E_NONE != bcm_rv) {
        errstrm << "Error in bcm_field_group_create" << endl;
        return -1;
    }
    else
    {
        fp_rules_data_entry.group_id = gid;
    }   

    /*fp group and entry create*/
    bcm_rv = bcm_field_entry_create(unit, gid, &eid);
    if (BCM_E_NONE != bcm_rv) 
    {
        errstrm << "Error in bcm_field_entry_create" << endl;
        return -1;
    }
    else
    {
        fp_rules_data_entry.entry_id = eid;
    }  
	
    /*fp entry prio set*/
    bcm_rv = bcm_field_entry_prio_set(unit, eid, prio);
    if (BCM_E_NONE != bcm_rv) 
    {
        errstrm << "Error in bcm_field_entry_prio_set" << endl;
        return -1;
    }       

    /*fp qual set*/
    bcm_rv = bcm_field_qualify_DstIp(unit, eid, ip, mask);
    if (BCM_E_NONE != bcm_rv) 
    {
        errstrm << "Error in bcm_field_qualify_DstIp" << endl;
        return -1;
    }          

    /*fp policer create*/
    bcm_policer_config_t_init(&policer_cfg);
    //policer_cfg.flags |= BCM_POLICER_WITH_ID;
    policer_cfg.mode = bcmPolicerModeCommitted;
    policer_cfg.ckbits_sec = bandwidth*1000; /* 15M:15000 */
    policer_cfg.ckbits_burst = 1024;
    policer_cfg.pkbits_burst = 0;
    policer_cfg.max_pkbits_sec = 1024;
    policer_id = -1;
    
    bcm_rv = bcm_policer_create(unit, &policer_cfg, &policer_id);
    if (bcm_rv != BCM_E_NONE)
    {
        printf("Error in bcm_policer_create with policer_id %d\n", policer_id);
        return bcm_rv;
    }
    else
    {
        //cout << "create a policer entry id is: " << policer_id << " !" << endl; 
        fp_rules_data_entry.policer_id = policer_id;
    }          

    /*fp policer attach*/
    bcm_rv = bcm_field_entry_policer_attach(unit, eid, 0, policer_id);
    if (bcm_rv != BCM_E_NONE)
    {
        errstrm << "Error in bcm_field_entry_policer_attach with policer_id " << policer_id << endl;
        return bcm_rv;
    }       
        
    /*fp action set*/
    action = parse_field_action("ypdrop");
    bcm_rv = bcm_field_action_add(unit, eid, action, p0, p1);
    if (BCM_E_NONE != bcm_rv)
    {
        errstrm << "Error in bcm_field_action_add" << endl;
        return -1;
    }           

    action = parse_field_action("rpdrop");
    bcm_rv = bcm_field_action_add(unit, eid, action, p0, p1);
    if (BCM_E_NONE != bcm_rv)
    {
        errstrm << "Error in bcm_field_action_add" << endl;
        return -1;
    }        

    /*fp install*/
    bcm_rv = bcm_field_entry_install(unit, eid);
    if (BCM_E_NONE != bcm_rv) 
    {
        errstrm << "Error in bcm_field_entry_install" << endl;
        return -1;
    }            
    else
    {
        //cout << "create qos rules success!" << endl;
        /*add new data to map*/
        fp_rules_data_entry.ip_address = ip;
        bcm_rv =  fp_qos_map_add_or_update((uint32)ip , fp_rules_data_entry);
        if(bcm_rv < 0)
        {
             errstrm << "add data entry to map fail" << endl;
            /*call delete fp rules apis*/
        }
    }

    //just for test
    //bcm_field_show(unit, "FP");

    return bcm_rv;
}
