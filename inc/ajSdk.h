#ifndef __AJSDK_H__
#define __AJSDK_H__

#ifdef __cplusplus
extern "C" {
#endif

void AjInit();
void AjStart();
void AjStop();
int AjRelease();
void AjGetRtmpUrl(IN char *pUrl);
void AjGetMqttID(IN char *pMqttID, IN int nIDLen, IN char *pPss, IN int nPssLen);

#ifdef  __cplusplus
}
#endif

#endif
