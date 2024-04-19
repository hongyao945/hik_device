#include <stdio.h>

#include <cstdlib>
#include <cstring>
#include <cstdio> // 包含字符串格式化函数的头文件
#include <thread>
#include <HCNetSDK.h>
//#include <LinuxPlayM4.h>

#include <iostream>

// 音频数据回调函数
void CALLBACK VoiceDataCallBack(LONG lVoiceComHandle, char *pRecvDataBuffer, DWORD dwBufSize, BYTE byAudioFlag, void *pUser) {
    // 在这里处理接收到的音频数据
}

int main(int argc, char* argv[]) {

    if (argc != 6){
        printf("arg failed, error code: %d\n", argc);
        NET_DVR_Cleanup();
        return 0;
    }

    // 解析参数
    char* address = argv[1];
    int port = atoi(argv[2]); // 将字符串转换为整数
    char* name = argv[3];
    char* password = argv[4];
    char* output_audio = argv[5];

    // 打印参数
    printf("地址：%s\n", address);
    printf("端口：%d\n", port);
    printf("用户名：%s\n", name);
    printf("密码：%s\n", password);
    printf("转码后音频文件名：%s\n", output_audio);

    NET_DVR_Init();

    //---------------------------------------
    // 注册设备
    LONG lUserID;

    //登录参数，包括设备地址、登录用户、密码等
    NET_DVR_USER_LOGIN_INFO struLoginInfo = {0};
    struLoginInfo.bUseAsynLogin = 0; //同步登录方式
    strcpy(struLoginInfo.sDeviceAddress, address); //设备IP地址
    struLoginInfo.wPort = port; //设备服务端口
    strcpy(struLoginInfo.sUserName, name); //设备登录用户名
    strcpy(struLoginInfo.sPassword, password); //设备登录密码

        //设备信息, 输出参数
    NET_DVR_DEVICEINFO_V40 struDeviceInfoV40 = {0};

    lUserID = NET_DVR_Login_V40(&struLoginInfo, &struDeviceInfoV40);
    printf("byAudioChanNum:%d \n",struDeviceInfoV40.bySingleStartDTalkChan);
    printf("bySingleDTalkChanNums:%d \n",struDeviceInfoV40.bySingleDTalkChanNums);
    if (lUserID < 0)
    {
        printf("Login failed, error code: %d\n", NET_DVR_GetLastError());
        NET_DVR_Cleanup();
        return 0;
    }

    // 准备一个 NET_DVR_COMPRESSION_AUDIO 结构体实例
    NET_DVR_COMPRESSION_AUDIO compressAudio;
    memset(&compressAudio, 0, sizeof(NET_DVR_COMPRESSION_AUDIO));

    // 获取当前设备的音频压缩参数
    BOOL bRet = NET_DVR_GetCurrentAudioCompress(lUserID, &compressAudio);
    if (bRet) {
        printf("Audio encoding type: %d\n", compressAudio.byAudioEncType);
    } else {
        printf("Audio encoding fail type: %d\n", NET_DVR_GetLastError());
    }

    // 设置语音转发通道号
    DWORD dwVoiceChan = 1;

    // 启动语音转发
    LONG lVoiceComHandle = NET_DVR_StartVoiceCom_MR_V30(lUserID, dwVoiceChan, VoiceDataCallBack, NULL);
    if (lVoiceComHandle < 0) {
        printf("NET_DVR_StartVoiceCom_MR_V30 failed, error code: %d\n", NET_DVR_GetLastError());
        NET_DVR_Cleanup();
        return -1;
    } else {
        printf("NET_DVR_StartVoiceCom_MR_V30 successful: %d\n", lVoiceComHandle);
    }
    // 打开文件
    FILE *in_file = fopen(output_audio, "rb");
    if (in_file == NULL) {
        printf("Error opening file\n");
        exit(1);
    }

    // 获取文件大小
    fseek(in_file, 0, SEEK_END);
    long file_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);

    // 分配内存以存储整个文件
    char *pSendBuf = (char *)malloc(file_size);
    if (pSendBuf == NULL) {
        printf("Memory allocation failed\n");
        fclose(in_file);
        exit(2);
    }

    // 读取文件内容到缓冲区
    size_t read_size = fread(pSendBuf, 1, file_size, in_file);
    if (read_size != file_size) {
        printf("Error reading file\n");
        free(pSendBuf);
        fclose(in_file);
        exit(3);
    }

    printf("file_size:%ld\n",file_size);
    // 关闭文件
    fclose(in_file);

    // 使用 NET_DVR_VoiceComSendData 函数发送语音数据
    size_t offset = 0;
    const size_t chunkSize = 160; // 每次发送160字节
    while (offset < file_size) {
        size_t bytesToSend = (file_size - offset) < chunkSize ? (file_size - offset) : chunkSize;
        if (bytesToSend < 160){
            break;
        }
        BOOL bSentRet = NET_DVR_VoiceComSendData(lVoiceComHandle, pSendBuf + offset, bytesToSend);
        if (bSentRet) {
            // printf("语音数据发送成功！:%zu\n", offset);
            // printf("file_size:%ld\n",file_size);
            // printf("bytesToSend:%ld\n",bytesToSend);
        } else {
            printf("语音数据发送失败！:%d\n",NET_DVR_GetLastError());
        }
        std::chrono::milliseconds duration(16);
        std::this_thread::sleep_for(duration);
        offset += bytesToSend;
    }

    // 释放缓冲区内存
    free(pSendBuf);
    
    if (!NET_DVR_StopVoiceCom(lVoiceComHandle))
    {
        printf("NET_DVR_StopVoiceCom error, %d!\n", NET_DVR_GetLastError());
        NET_DVR_Logout(lUserID);
        NET_DVR_Cleanup();
        return 0;
    }
    //注销用户
    NET_DVR_Logout(lUserID);
    //释放SDK资源
    NET_DVR_Cleanup();
    return 0;
    
}
