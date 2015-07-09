#include <stdio.h>

#include <iterator>
#include <fstream>

#include "SystemInclude.h"
#include "BcmController.h"
#include "Debug.h"

#include "SaveCfgFile.h"

using namespace std;

//const char* const ConfigureFileName = "/home/liyong/liyong_test_dir/cmc_cfg_file.cli"
const char* const ConfigureFileName = "/etc/cmc_cfg_file.cli";

using namespace std;

BASE_ErrCode 
SaveCfg(char const* write_string , unsigned char start_type)
{
    if(start_type == 0)//at start
    {
        ofstream  f_start(ConfigureFileName, ios::out);
        if(!f_start)
        {
            dbgcstrm << "file open faileds" << endl;
            return BASE_ERROR;
        }
        f_start << write_string << endl;
        f_start.close();
    }
    else
    {
        ofstream  f_continue(ConfigureFileName,ios::app);
        f_continue << write_string << endl;
        f_continue.close();
    }

    return BASE_OK;
}

