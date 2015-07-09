#ifndef _CliMain_H_
#define _CliMain_H_

/*** Bcm header. ***/
#if defined(__cplusplus)
extern "C"
{
#endif  //#if defined(__cplusplus)

void InitCli();
void ReadCliConfig();
BASE_ErrCode SaveToCliConfig();
void RestoreDefaultConfig();

void StartCli();
void StopCli();

#if defined(__cplusplus)
}
#endif  //#if defined(__cplusplus)

#endif
