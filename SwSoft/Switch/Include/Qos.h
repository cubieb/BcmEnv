#ifndef _Qos_h_
#define _Qos_h_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

//map data struct
typedef struct data_struct_s{
	uint32  		group_id;
	uint32 		entry_id;
	uint32 		policer_id;
	uint32		ip_address;
}data_struct_t;

int fp_qos_map_add_or_update(uint32 key , data_struct_t a_data_struct_entry);
int fp_qos_map_delete(uint32 key);
int fp_qos_map_search(uint32 key,data_struct_t* a_data_struct_entry_point);
int fp_qos_map_show_all();

STATIC int ds_qos_rule_cfg_show();
STATIC int ds_qos_rule_cfg_create(bcm_ip_t ip, bcm_ip_t mask, uint32 bandwidth);
STATIC int ds_qos_rule_cfg_delete(bcm_ip_t ip);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _Qos_h_ */
