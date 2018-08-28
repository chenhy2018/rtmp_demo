#include <stdio.h>
#include <stdlib.h>
#include "devsdk.h"
#include "QnCommon.h"
#include "qnRtmp.h"
#include "ajSdk.h"


#define VIDEO_CHN_ID 0
#define VIDEO_STREAM_ID 0
#define AUDIO_CHN_ID 0
#define AUDIO_STREAM_ID 1

static MediaStreamConfig gAjMediaStreamConfig = {0};

static int GetMediaStreamConfig(IN MediaStreamConfig *_pMediaStreamConfig);

void AjInit()
{
	dev_sdk_init(DEV_SDK_PROCESS_APP);
	GetMediaStreamConfig(&gAjMediaStreamConfig);
}

void AjStart()
{
	static int context = 1;

	dev_sdk_start_video(VIDEO_CHN_ID, VIDEO_STREAM_ID, VideoCallBack, &context);
	if (CheckAudioEnable()) {
		dev_sdk_start_audio(AUDIO_CHN_ID, AUDIO_STREAM_ID, AudioCallBack, &context);
	}
}

void AjStop()
{
	dev_sdk_stop_video(VIDEO_CHN_ID, VIDEO_STREAM_ID);
	if (CheckAudioEnable()) {
		dev_sdk_stop_audio(AUDIO_CHN_ID, AUDIO_STREAM_ID);
	}
}

int AjRelease()
{
	dev_sdk_release();
}

int CheckAudioEnable()
{
	AudioConfig audioConfig;

	dev_sdk_get_AudioConfig(&audioConfig);
	if( audioConfig.audioEncode.enable != 1)
		return 0;
	else 
		return 1 ;
}

static int GetMediaStreamConfig(IN MediaStreamConfig *_pMediaStreamConfig)
{
	if (!_pMediaStreamConfig) {
		QnDemoPrint(DEMO_WARNING, "%s[%d] null pointer.\n", __func__, __LINE__);
		return QN_FAIL;
	}
	if (dev_sdk_get_MediaStreamConfig(_pMediaStreamConfig) == QN_SUCCESS) {
		return QN_SUCCESS;
	}

	return QN_FAIL;
}

void AjGetRtmpUrl(IN char *_pUrl)
{
	if (!_pUrl) {
		QnDemoPrint(DEMO_WARNING, "%s[%d] parameter is null.\n", 
				__func__, __LINE__);
		return;
	}
	memcpy(_pUrl, &gAjMediaStreamConfig.rtmpConfig.appname[0], MAX_RTMP_APP_NAME_LEN);
}

void AjGetMqttID(IN char *_pMqttID, IN int _nIDLen, IN char *_pPss, IN int _nPssLen)
{
	if ((!_pMqttID) || (!_pPss)) {
		QnDemoPrint(DEMO_WARNING, "%s[%d] parameter is null.\n",
					__func__, __LINE__);
		return;
	}

	int nPssIdx = 0;
	int i = 0;

	for (i = 0; i < MAX_IP_NAME_LEN; i++) {
		if (gAjMediaStreamConfig.rtmpConfig.server[i] != '.') {
			nPssIdx++;
		} else{
			break;
		}
	}
	if (i >= MAX_IP_NAME_LEN -1) {
		memcpy(_pMqttID, &gAjMediaStreamConfig.rtmpConfig.server[0], _nIDLen);
		return;
	}
	else {
		if (i < _nIDLen) {
			memcpy(_pMqttID, &gAjMediaStreamConfig.rtmpConfig.server[0], i);
		} else {
			memcpy(_pMqttID, &gAjMediaStreamConfig.rtmpConfig.server[0], _nIDLen);
		}
		if ((MAX_IP_NAME_LEN -1 - i) < _nPssLen) {
			memcpy(_pPss, &gAjMediaStreamConfig.rtmpConfig.server[i + 1],
				   (MAX_IP_NAME_LEN -i));
		} else {
			memcpy(_pPss, &gAjMediaStreamConfig.rtmpConfig.server[i + 1], _nPssLen);
		}
	}
}
