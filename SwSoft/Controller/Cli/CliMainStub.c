#include "SystemInclude.h"
#include "BcmController.h"

#include "cparser.h"
#include "cparser_priv.h"
#include "cparser_token.h"
#include "cparser_tree.h"

//API functions.
BASE_ErrCode cliDoesCmcSelected(void)
{
	CMCOAM_CmcAddrT cmcAddr;

	memset(&cmcAddr, 0, sizeof(cmcAddr));

	memcpy(&cmcAddr, CMCOAM_Platform_GetSelectedCmc(), sizeof(cmcAddr));
	if(!BASE_IsValidCmcAddress((U8*)&cmcAddr))
	{
		printf("\n**** You need to select a CMC first! ***\n");
		return BASE_ERROR;
	}
	else
		return BASE_OK;
}

CMCOAM_CmcAddrT *cliGetCmc(void)
{
	return CMCOAM_Platform_GetSelectedCmc();
}

int cliGetString(char * ch, int len)
{
	int i=0;
	if(NULL == ch)
		return 0;

	memset(ch, 0, sizeof(len));

	ch[i] = getchar();
	putchar(ch[i++]);

	while((ch[i-1] != '\n') && (i < len))
	{
		ch[i] = getchar();
		putchar(ch[i++]);
	}

	ch[i-1] = '\0';

	return (i-1);
}

//command implementation functions.
cparser_result_t
cparser_cmd_show_version(cparser_context_t *context)
{
	assert(context);
	printf("Version: %s\n", CMC_VERSION);

	return CPARSER_OK;
}

/*****************************************************************************
 *
 *  CLI Functions
 *
 ****************************************************************************/
/**
 * @brief Parses a string checking to see if it conforms to a IOS MAC Address
 *
 * MAC Address is of the form xx:xx:xx:xx:xx:xx or Cisco ISO mac: xxxx.xxxx.xxxx
 *
 * @param macAddrStr  Pointer to string containing the MAC Address
 * @param macAddr     Pointer to MAC Address to set based on the string value
 *
 * @return BASE_ErrCode
 * @retval BASE_OK    String was of the proper format
 * @retval BASE_ERROR_PARAM String was not of the proper format
 */
BASE_ErrCode cliParseCmMacAddr(char *macAddrStr, MacAddressT*macAddr)
{
    BASE_ErrCode   retCode = BASE_ERROR_PARAM;

	if(strlen(macAddrStr) == 6)//OUI case,
	{
		if (sscanf(macAddrStr,"%02hhx%02hhx%02hhx",
                   &macAddr->addr[0],
                   &macAddr->addr[1],
                   &macAddr->addr[2]) == 3)
        {
        	macAddr->addr[3] = 0;
			macAddr->addr[4] = 0;
			macAddr->addr[5] = 0;
            retCode = BASE_OK;
        }
	}
    else if ((strlen(macAddrStr) <= CLI_MACADDR_STR_LEN) &&
		(strlen(macAddrStr) > CLI_CISCO_MACADDR_STR_LEN))
    {
        if (sscanf(macAddrStr,"%02hhx:%02hhx:%02hhx:%02hhx:%02hhx:%02hhx",
                   &macAddr->addr[0],
                   &macAddr->addr[1],
                   &macAddr->addr[2],
                   &macAddr->addr[3],
                   &macAddr->addr[4],
                   &macAddr->addr[5]) == 6)
        {
            retCode = BASE_OK;
        }
    }
	else
	{
        if (sscanf(macAddrStr,"%02hhx%02hhx.%02hhx%02hhx.%02hhx%02hhx",
                   &macAddr->addr[0],
                   &macAddr->addr[1],
                   &macAddr->addr[2],
                   &macAddr->addr[3],
                   &macAddr->addr[4],
                   &macAddr->addr[5]) == 6)
        {
            retCode = BASE_OK;
        }

	}

    return retCode;
}
