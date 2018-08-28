//
// Created by chenh on 2018/7/20.
//

#ifndef AJ_PRJ_QNMQTT_H
#define AJ_PRJ_QNMQTT_H

typedef struct
{
    char *id;
    char *password;
    char *sigHost;
    char *mediaHost;
    char *imHost;
    int timeOut;
    unsigned char init;
    int accountId;
    int callId;
    int sendFlag;
    long long int timeCount;
}QnMqttRegisterContext;

int QnMqttInit();
void QnMqttRelease();
void QnMqttStart();

#endif //AJ_PRJ_QNMQTT_H
