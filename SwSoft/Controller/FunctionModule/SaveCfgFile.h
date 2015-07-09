#ifndef _SaveCfgFile_h_
#define _SaveCfgFile_h_

extern const char* const ConfigureFileName;

#define START 0
#define CONTINUE 1

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef enum
{
    InsertToStart = 0,
    InsertToEnd
}InsertLineToFileMode;

int SaveCfg(char const* write_string , unsigned char start_type);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
