CC = gcc
CXX = g++

CORSS_FLAGS = #-D__LINUX__

TARGET = hik

CPP_FILES = main.cpp

LIB_INC = -Ilibs/hik/include

LIBS = -Llibs/hik/lib -Llibs/hik/lib/HCNetSDKCom \
-lanalyzedata \
-lAudioIntercom \
-lHCAlarm \
-lHCCoreDevCfg \
-lHCDisplay \
-lHCGeneralCfgMgr \
-lHCIndustry \
-lHCPlayBack \
-lHCPreview \
-lHCVoiceTalk \
-lStreamTransClient \
-lSystemTransform \
libs/hik/lib/HCNetSDKCom/libiconv.so.2 \
-lAudioRender \
-lcrypto \
-lcrypto \
-lHCCore \
libs/hik/lib/libhcnetsdk.so \
-lhpr \
-lPlayCtrl \
-lssl \
-lssl \
-lSuperRender \
-lz

OBJCPP = $(CPP_FILES:%.cpp=%.ocpp)
OBJC   = $(C_FILES:%.c=%.o)

$(TARGET): $(OBJCPP) $(OBJC)
	$(CXX) $(CORSS_FLAGS) $(LDFLAGS) $(OBJCPP) $(OBJC) -o $@ $(LIBS)
 
$(OBJCPP): %.ocpp: %.cpp
	$(CXX) -c $< $(CORSS_FLAGS) $(LIB_INC) $(INCLUDE) $(CXXFLAGS) -o $@ 

$(OBJC): %.o: %.c
	$(CC) -c $< $(CORSS_FLAGS) $(LIB_INC) $(INCLUDE) $(CFLAGS) -o $@ 

clean:
	rm -rf $(OBJCPP) $(OBJC) $(TARGET)