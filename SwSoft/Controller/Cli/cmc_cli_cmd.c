//****************************************************************************
//
//  Copyright (c) 2011,2012  Broadcom Corporation
//  All Rights Reserved
//  No portions of this material may be reproduced in any form without the
//  written permission of: 
//          Broadcom Corporation
//          16251 Laguna Canyon Road
//          Irvine, California 92618
//  All information contained in this document is Broadcom Corporation
//  company private, proprietary, and trade secret.
//
//****************************************************************************
//

/* --- system includes ---*/
#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include "cparser.h"
#include "cparser_token.h"

/* --- project includes ---*/
#include "cli.h"
#include "cmcOam.h"
#include "cmcOamSystem.h"
#include "cmcController.h"

#if NETWORKINGDEV_L2OAM
cparser_result_t
cparser_cmd_register_cmc_mac(cparser_context_t *context, cparser_macaddr_t *mac_ptr)
{
	CMCOAM_CmcAddrT cmcAddr;
	U8 vendor;
	assert(context&&mac_ptr);

	memcpy(&cmcAddr, mac_ptr->octet, sizeof(MacAddressT));

	if (BASE_OK != CMCCTRL_RegisterCmc(&cmcAddr, &vendor, 1))
	{
		BASE_ERR("register error!");
		return BASE_ERROR;
	}
	CMCOAM_PRINT("CMC register OK!\n");
	
	return BASE_OK;
}
#endif

#if NETWORKINGDEV_HIM
cparser_result_t
cparser_cmd_register_cmc_mac_oltip_oltport(cparser_context_t *context, cparser_macaddr_t *mac_ptr, uint32_t *oltip_ptr, uint32_t *oltport_ptr)

{
	CMCOAM_CmcAddrT cmcAddr;
	IpAddressT oltAddr;
	
	assert(context&&mac_ptr);

	memcpy(&cmcAddr, mac_ptr->octet, sizeof(MacAddressT));
	memcpy(&oltAddr, mac_ptr->octet, sizeof(IpAddressT));

	if(oltip_ptr)
		oltAddr.ipv4Addr = *oltip_ptr;
	else
		oltAddr.ipv4Addr = BASE_Ipv4StringToAddr("192.168.1.101");

	if(oltport_ptr)
		oltAddr.port = *oltport_ptr;
	else
		oltAddr.port = 14113;

	if (BASE_OK != CMCCTRL_RegisterCmc(&cmcAddr, &oltAddr, sizeof(oltAddr)))
	{
		BASE_ERR("register error!");
		return BASE_ERROR;
	}
	CMCOAM_PRINT("CMC register OK!\n");
	
	return BASE_OK;
}

#endif

#ifdef NETWORKINGDEV_TCP
cparser_result_t
cparser_cmd_register_cmc_ipv4_port(cparser_context_t *context, uint32_t *ipv4_ptr, uint32_t *port_ptr)
{
	CMCOAM_CmcAddrT cmcAddr;
	IpAddressT * pIpAddr=NULL;
	U8 vendor;
	assert(context&&ipv4_ptr);

	pIpAddr = (IpAddressT*)&cmcAddr;
	
	pIpAddr->ipv4Addr = *ipv4_ptr;
	if(port_ptr)
		pIpAddr->port = *port_ptr;
	else
		pIpAddr->port = BRCM_CONTROLLER_TCP_LISTEN_PORT;

	if (BASE_OK != CMCCTRL_RegisterCmc(&cmcAddr, &vendor, 1))
	{
		BASE_ERR("register CMC %s Failed!", BASE_CmcAddrToString((U8*)pIpAddr));
		return BASE_ERROR;
	}
	CMCOAM_PRINT("CMC %s register OK!\n", (char *)BASE_CmcAddrToString((U8*)pIpAddr));
	
	return BASE_OK;

}

cparser_result_t
cparser_cmd_no_register_cmc_ipv4_port(cparser_context_t *context, uint32_t *ipv4_ptr, uint32_t *port_ptr)
{
	CMCOAM_CmcAddrT cmcAddr;
	IpAddressT * pIpAddr=NULL;
	assert(context&&ipv4_ptr);

	pIpAddr = (IpAddressT*)&cmcAddr;
	
	pIpAddr->ipv4Addr = *ipv4_ptr;
	if(port_ptr)
		pIpAddr->port = *port_ptr;
	else
		pIpAddr->port = BRCM_CONTROLLER_TCP_LISTEN_PORT;

	if (BASE_OK != CMCCTRL_UnRegisterCmc(&cmcAddr))
	{
		BASE_ERR("Un-Register CMC %s Failed!", BASE_CmcAddrToString((U8*)pIpAddr));
		return BASE_ERROR;
	}
	CMCOAM_PRINT("Un-Registered CMC %s OK!\n", (char *)BASE_CmcAddrToString((U8*)pIpAddr));
	
	return BASE_OK;

}

cparser_result_t
cparser_cmd_select_cmc_ipv4_port(cparser_context_t *context, uint32_t *ipv4_ptr, uint32_t *port_ptr)
{
	IpAddressT * pIpAddr=NULL;
	CMCOAM_CmcAddrT cmcAddr;

	assert(context&&ipv4_ptr);

	memset(&cmcAddr, 0, sizeof(cmcAddr));
	
	pIpAddr = (IpAddressT*)&cmcAddr;

	pIpAddr->ipv4Addr = *ipv4_ptr;
	if(port_ptr)
		pIpAddr->port = *port_ptr;
	else
		pIpAddr->port = BRCM_CONTROLLER_TCP_LISTEN_PORT;

	if(BASE_OK == CMCCTRL_SelectCmc(&cmcAddr))
	{
		CMCOAM_PRINT("Select CMC %s OK!\n", (char *)BASE_CmcAddrToString((U8*)pIpAddr));
		return BASE_OK;
	}
	else
	{
		BASE_ERR("Select CMC %s failed!", BASE_CmcAddrToString((U8*)pIpAddr));
		return BASE_ERROR;
	}

}

#else
cparser_result_t cparser_cmd_no_register_cmc_mac(cparser_context_t *context,    cparser_macaddr_t *mac_ptr)
{
	CMCOAM_CmcAddrT cmcAddr;
	U8 vendor;
	assert(context&&mac_ptr);

	memcpy(&cmcAddr, mac_ptr->octet, sizeof(MacAddressT));

	if (BASE_OK != CMCCTRL_UnRegisterCmc(&cmcAddr, &vendor, 1))
	{
		BASE_ERR("Un-Register CMC error!");
		return BASE_ERROR;
	}
	CMCOAM_PRINT("Un-Register Successed!\n");
	
	return BASE_OK;
}

cparser_result_t
cparser_cmd_select_cmc_mac(cparser_context_t *context, cparser_macaddr_t *mac_ptr)
{	
	char cmc_addr[BRCM_MAC_ADDR_STR_LEN];
	CMCOAM_CmcAddrT cmcAddr;
	
	assert(context&&mac_ptr);

	memset(&cmcAddr, 0, sizeof(cmcAddr));
	memcpy(&cmcAddr, mac_ptr->octet, BRCM_MAC_LEN);
	
	CMCOAM_Utility_MacAddrToString((U8*)&cmcAddr,cmc_addr, sizeof(cmc_addr));

	if(BASE_OK == CMCCTRL_SelectCmc(&cmcAddr))
	{
		CMCOAM_PRINT("Select CMC %s OK!\n", cmc_addr);
		return BASE_OK;
	}
	else
	{
		BASE_ERR("Select CMC %s failed, the CMC you select must be registered first!", cmc_addr);
		return BASE_ERROR;
	}
}
#endif

cparser_result_t
cparser_cmd_show_cmc(cparser_context_t *context)
{
	assert(context);
	return CMCCTRL_ShowAllCmc();
}

cparser_result_t cparser_cmd_clear_oam_buf(cparser_context_t *context)
{
	assert(context);
	return CMCOAM_Utility_DeleteAllOamMsgInBuf();
}

cparser_result_t cparser_cmd_show_oam_buf(cparser_context_t *context)
{
	assert(context);
	return CMCOAM_Utility_PrintOamMsgInBuf();
}

	

/**
 * Exit the parser test program.
 */
cparser_result_t
cparser_cmd_quit (cparser_context_t *context)
{
    assert(context);
    return cparser_quit(context->parser);
}

cparser_result_t
cparser_cmd_exit(cparser_context_t *context)
{
    assert(context);
    return cparser_quit(context->parser);
}

cparser_result_t
cparser_cmd_help_filter(cparser_context_t *context, char **filter_ptr)
{
    assert(context);
    return cparser_help_cmd(context->parser, filter_ptr ? *filter_ptr : NULL);
}

cparser_result_t 
cparser_cmd_m_filter(cparser_context_t *context, char **filter_ptr)
{
    assert(context);
    return cparser_help_cmd(context->parser, filter_ptr ? *filter_ptr : NULL);
}

static BASE_ErrCode remoteCLIHelp(void)
{
	fprintf(stderr, "\nSupported command list:\n");
	fprintf(stderr, "     help(h) --- display this message(help).\n");	
	fprintf(stderr, "     menu(m) --- display this message(help).\n");	
	fprintf(stderr, "     cd dir  --- enter \"dir\" directory\n");	
	fprintf(stderr, "      cd ..  --- enter parent directory\n");	
	fprintf(stderr, "       cd /  --- enter root directory\n");	
	fprintf(stderr, "     exit(e) --- exist remote CLI\n");	
	return BASE_OK;
}

static BASE_ErrCode remoteCLIProcssCdCmd(const char * string, char * path)
{
	char * pPos=NULL;
	int offset=2;
	
	if((NULL==path) || (NULL==string))
		return BASE_ERROR;

	//skip the space.....
	pPos = (char *)(string+offset);

	while( (*pPos == ' ') && (offset < strlen(string)))
	{
		pPos = pPos+1;
		offset++;
	}

	if(offset >= strlen(string))
		return BASE_ERROR;

	if(!strncmp(pPos, "/", 1))
	{
		path[0] = '\0';
	}
	else if(!strncmp(pPos, "..", 1))
	{
		pPos = strrchr(path, '/');
		if(pPos != NULL)
			*pPos = '\0';
	}
	else
	{
		strcat(path, "/");
		strcat(path, pPos);
	}

	return BASE_OK;
}


//remote cli
cparser_result_t
cparser_cmd_remote_cli(cparser_context_t *context)
{
	assert(context);

	char path[128],cmdBuf[512],fullCmd[512];

	if(BASE_OK != cliDoesCmcSelected())
		return CPARSER_NOT_OK;
	
	memset(path, 0, sizeof(path));
	remoteCLIHelp();
	while(1)
	{
		memset(cmdBuf, 0, sizeof(cmdBuf));
		memset(fullCmd, 0, sizeof(fullCmd));
		printf("Remote CLI%s>", path);
		//fgets(cmdBuf, sizeof(cmdBuf), stdin);
		cliGetString(cmdBuf, sizeof(cmdBuf));

		//Remove the "\n"
		//cmdBuf[strlen(cmdBuf)-1] = '\0';
		
		if (!strncmp(cmdBuf, "exit", 4) || 
			((cmdBuf[0] == 'e') && (strlen(cmdBuf)==1)))
			return BASE_OK;
		else if(((cmdBuf[0] == 'm') || (cmdBuf[0] == 'h')) &&(strlen(cmdBuf)==1))
		{
			remoteCLIHelp();
		}
		else if(!strncmp(cmdBuf, "cd ", 3))
		{
			remoteCLIProcssCdCmd(cmdBuf, path);
		}
		else if(strlen(cmdBuf) > 0)
		{
			if(path[0] != '\0')
				snprintf(fullCmd, sizeof(fullCmd), "%s/%s", path+1, cmdBuf);
			else
				snprintf(fullCmd, sizeof(fullCmd), "%s", cmdBuf);
			
			fprintf(stderr, "%s\n", fullCmd);
			CMCOAM_System_SetRemoteCmd(fullCmd, strlen(fullCmd), cliGetCmc());
		}
		else
			;
	}
	return BASE_OK;
}

//debug controller [loglevel]
cparser_result_t
cparser_cmd_debug_controller_loglevel(cparser_context_t *context, char **loglevel_ptr)
{
	assert(loglevel_ptr && (*loglevel_ptr));
	
	return CMCOAM_Utility_SetLogLevel(*loglevel_ptr);
}

//enter to config mode.
cparser_result_t cparser_cmd_configure_terminal(cparser_context_t *context)
{
	char prompt[CPARSER_MAX_PROMPT];
	assert(context);
	
	snprintf(prompt, CPARSER_MAX_PROMPT, "%s-%s#", CLI_ROOT_PROMPT, CLI_CONFIG_PROMPT);
	return cparser_submode_enter(context->parser, NULL, prompt);
}

//exit from config mode
cparser_result_t cparser_cmd_config_exit(cparser_context_t *context)
{
    assert(context && context->parser);
    return cparser_submode_exit(context->parser);
}

//help function the config mode.
cparser_result_t
cparser_cmd_config_help_filter(cparser_context_t *context, char **filter_ptr)
{
    assert(context);
    return cparser_help_cmd(context->parser, filter_ptr ? *filter_ptr : NULL);
}


