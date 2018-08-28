#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include "QnCommon.h"
#include "qnRtmp.h"
#include "ajSdk.h"
#include "QnMqtt.h"
#include "main.h"

int main()
{
	InitSigHandler();
	AjInit();
	QnMqttInit();
	RtmpInit();
	QnMqttStart();
	AjStart();
	
	while(1) {
		usleep(1000);
	}
		
	RtmpRelease();
	QnMqttRelease();
	AjStop();
	AjRelease();
	
	return 0;
}
