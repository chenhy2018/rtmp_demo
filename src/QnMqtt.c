//
// Created by chenh on 2018/7/20.
//
#include <pthread.h>
#include <QnMqtt.h>
#include "sdk_interface.h"
#include "QnCommon.h"
#include "QnMqtt.h"

#define HOST "180.97.147.170"

static QnMqttRegisterContext gMqttCtx;
static int gMqttState = 1;

Event *NewEvent()
{
    return (Event *)malloc(sizeof(Event));
}

void DeleteEvent(IN Event *_pEvent)
{
    if (_pEvent) {
        free(_pEvent);
    }
}

int QnMqttInit()
{
    gMqttCtx.id = (char *)malloc(8);
    gMqttCtx.password = (char *)malloc(8);
    gMqttCtx.sigHost = (char *)malloc(16);
    gMqttCtx.mediaHost = (char *)malloc(16);
    gMqttCtx.imHost = (char *)malloc(16);

    AjGetMqttID(gMqttCtx.id, 8, gMqttCtx.password, 8);
    memcpy(gMqttCtx.sigHost, HOST, sizeof(HOST));
    memcpy(gMqttCtx.mediaHost, HOST, sizeof(HOST));
    memcpy(gMqttCtx.imHost, HOST, sizeof(HOST));
    gMqttCtx.timeOut = 10;
    gMqttCtx.init = 1;
    if (InitSDK(NULL, 0) != RET_OK) {
        QnDemoPrint(DEMO_WARNING, "%s[%d] Init Qn SDK failed.\n", __func__, __LINE__);
        return QN_FAIL;
    }

    gMqttCtx.accountId = Register(gMqttCtx.id, gMqttCtx.password, NULL, gMqttCtx.imHost);
    usleep(600);

    return QN_SUCCESS;
}

void QnMqttRelease()
{
    free(gMqttCtx.id);
    free(gMqttCtx.password);
    free(gMqttCtx.sigHost);
    free(gMqttCtx.mediaHost);
    free(gMqttCtx.imHost);
    UninitSDK();
    QnDemoPrint(DEMO_INFO, "QnMqttRelease.......\n");
}

static void *MqttHandler(void *param)
{
    QnMqttRegisterContext *pMqttCtx = (QnMqttRegisterContext *)param;
    ErrorID id = 0;
    EventType type;
    int sendCount = 0;
    int missCount = 0;
    int disCount = 0;
    int timeCount = 0;
    int sendFlag = 0;

    Event *mqttEvent = NewEvent();
    if (!mqttEvent) {
        QnDemoPrint(DEMO_WARNING, "%s[%d] NewEvent failed.\n",
                    __func__, __LINE__);
        return;
    }

    while (gMqttState) {

        if (sendFlag) {
            if (sendCount > 500) {
                missCount = sendCount - timeCount;
                UnRegister(gMqttCtx.accountId);
                gMqttCtx.accountId = Register(gMqttCtx.id,
                         gMqttCtx.password, NULL, gMqttCtx.imHost);
                sendFlag = 0;
                sendCount = 0;
                timeCount = 0;
                continue;
            }
            Report(gMqttCtx.accountId, gMqttCtx.id, "Oooo", 10);
            //QnDemoPrint(DEMO_INFO, "%s[%d] Report :Oooo\n", __func__, __LINE__);
            sendCount++;
            usleep(100000);
        }
        id = PollEvent(gMqttCtx.accountId, &type, &mqttEvent, 100);
        if (id != RET_OK) {
            sleep(1);
            QnDemoPrint(DEMO_INFO, "PollEvent return !RET_OK\n");
            continue;
        }
        switch (type) {
            case EVENT_CALL:
            {
                CallEvent *pCallEvent = &(mqttEvent->body.callEvent);
                if (pCallEvent->status == CALL_STATUS_INCOMING) {
                    AnswerCall(gMqttCtx.accountId, pCallEvent->callID);
                }
                if (pCallEvent->status == CALL_STATUS_ERROR
                    || pCallEvent->status == CALL_STATUS_HANGUP) {
                    QnDemoPrint(DEMO_INFO, "%s[%d] CallEvent :CALL_STATUS_ERROR\n", __func__, __LINE__);
                }
                if (pCallEvent->status == CALL_STATUS_ESTABLISHED) {
                    QnDemoPrint(DEMO_INFO, "%s[%d] CallEvent “:CALL_STATUS_ESTABLISHED\n", __func__, __LINE__);
                    sendFlag = 1;
                }
            }
            break;

            case EVENT_MESSAGE:
            {
                MessageEvent *pMessage = &(mqttEvent->body.messageEvent);
                if (pMessage->status == MESSAGE_STATUS_CONNECT) {
                    sendFlag = 1;
                    Subscribe(gMqttCtx.accountId, gMqttCtx.id);
                    QnDemoPrint(DEMO_INFO, "###MESSAGE_STATUS_CONNECT###\n", __func__, __LINE__);
                } else if (pMessage->status == MESSAGE_STATUS_DATA) {
                    QnDemoPrint(DEMO_INFO, "%s[%d] ###MESSAGE_STATUS_DATA###\n", __func__, __LINE__);
                    if (timeCount % 10 == 0) {
                        QnDemoPrint(DEMO_INFO, "Message %s status id :%d account :%d\n",
                                    pMessage->message, pMessage->status, gMqttCtx.id);
                    }
                    timeCount++;
                } else if (pMessage->status == MESSAGE_STATUS_DISCONNECT) {
                    disCount++;
                    QnDemoPrint(DEMO_INFO, "###MESSAGE_STATUS_DISCONNECT###\n", \
                                gMqttCtx.id, disCount);
                }
            }
                break;
        }
    }
    DeleteEvent(mqttEvent);
}

void QnMqttStart()
{
    pthread_t mqttPth;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    gMqttCtx.sendFlag = 0;
    gMqttCtx.timeCount = 0;
    gMqttCtx.callId = 0;
    pthread_create(&mqttPth, &attr, MqttHandler, (void *)&gMqttCtx);
}




