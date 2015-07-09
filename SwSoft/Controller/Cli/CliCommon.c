
#include "cparser.h"
#include "baseError.h"

#include "CliCommon.h"

cparser_result_t BaseErrCodeToCliResult(BASE_ErrCode errCode)
{
    if (errCode == BASE_OK)
        return CPARSER_OK;

    return CPARSER_NOT_OK;
}
