import os
# 为ffmpeg添加临时环境变量
path_command = "export PATH=/usr/local/ffmpeg/bin:$PATH"
print(f'step 1 path_command:{path_command}')
os.system(f"{path_command}")


# Step 1: 使用ffmpeg将输入wav音频转为8k 16bit单通道wav音频
input_wav = "/home/user/G711/example_files/input.wav"
output_wav = "/home/user/G711/example_files/ffmpeg_8000_16.wav"
ffmpeg_command = f"ffmpeg -y -i {input_wav} -ac 1 -ar 8000 -b:a 16 {output_wav}"
print(f'step 2 ffmpeg_command:{ffmpeg_command}')
os.system(ffmpeg_command)

# Step 2: 调用switch.py将8k 16bit单通道wav音频转化为pcm文件
switch_script = "/home/user/G711/example_files/switch.py"
print(f'step 3 switch_script:{switch_script}')
os.system(f"python {switch_script}")

# Step 3: 将pcm文件g711编码
pcm_file = "/home/user/G711/example_files/ffmpeg_8000_16.pcm"
output_audio = "/home/user/hik/ffmpeg_8000_16_pcm_to_alaw.audio"
g711_encoder = "./example"
g711_codec = "pcm_alaw"
os.chdir("/home/user/G711")
print(f'step 4 g711_encoder:{g711_encoder} {pcm_file} {g711_codec} {output_audio}')
os.system(f"{g711_encoder} {pcm_file} {g711_codec} {output_audio}")

# Step 4: 运行hik文件
os.chdir("/home/user/hik")
print(f'step 5 ./hik')
os.system("./hik")