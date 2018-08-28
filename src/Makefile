#PROJECT_PATH=${shell pwd}
#CC=arm-linux-gnueabihf-gcc

DEMO_PRELIB_PATH = $(PROJECT_PATH)/lib/prelib
PRELIB_SO_PATH = $(DEMO_PRELIB_PATH)/so
PRELIB_A_PATH = $(DEMO_PRELIB_PATH)/a
PRELIB_PATH = $(PRELIB_A_PATH)

DEMO_LIBA = $(PROJECT_PATH)/lib/libua.a $(PROJECT_PATH)/lib/librtmp_sdk.a $(PROJECT_PATH)/lib/librtmp.a $(PROJECT_PATH)/lib/libfdk-aac.a  $(PROJECT_PATH)/lib/libdevsdk.a $(PROJECT_PATH)/lib/libtools.a

DEMO_LIBA += $(PRELIB_PATH)/libssl.a $(PRELIB_PATH)/libcrypto.a

DEMO_LIBS =-lm  -lstdc++  -lpthread
INCLUDE_PATH = -I$(PROJECT_PATH)/inc


CC = $(CROSS_COMPILE)gcc
OBJS= main.o QnRtmp.o AjSdk.o QnCommon.o QnMqtt.o

.c.o:
	$(CC) $(INCLUDE_PATH) -g -c $<


demo_rtmp:$(OBJS)
	#$(CC) -o $@ $^ $(LDFLAGS)
	$(CC) -o $@ $^ $(DEMO_LIBA) $(DEMO_LIBS)

clean:
	rm *.o demo_rtmp

