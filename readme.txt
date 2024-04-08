# 为ffmpeg添加临时环境变量
path_command = "export PATH=/usr/local/ffmpeg/bin:$PATH"
print(f'step 1 path_command:{path_command}')
os.system(f"{path_command}")

1 通过ffmpeg将输入得wav音频转为 8k 16bit 单通道 wav音频
ffmpeg -y -i /home/user/G711/example_files/input.wav -ac 1 -ar 8000 -b:a 16 /home/user/G711/example_files/ffmpeg_8000_16.wav

2 通过调用/home/user/G711/example_files/switch.py 将8k 16bit 单通道 wav音频转化为pcm文件 文件名
python /home/user/G711/example_files/switch.py

3 将pcm文件g711编码
./example example_files/ffmpeg_8000_16.pcm pcm_alaw /home/user/hik/ffmpeg_8000_16_pcm_to_alaw.audio

4 运行./hik文件
cd /home/user/hik
./hik