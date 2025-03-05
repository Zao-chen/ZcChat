# ZcChat

一个有长期记忆、表情动作立绘显示和语音功能的ai桌宠

![QQ20241115-201211](https://github.com/user-attachments/assets/46a80ea9-35dc-4c2d-be6a-4f262a3f3a35)

视频演示：[基础功能](https://www.bilibili.com/video/BV1YUUaYgEgC/) [立绘动画](https://www.bilibili.com/video/BV1aFCKYJEy4/)

## 介绍

### 特色

- 拥有长期记忆和可以发展拓展的性格个性
- 动作和表情显示
- 立绘动画显示
- 支持语音输入、对话、唤醒和回复打断
- 支持语音合成

### 技术介绍

一图看懂本项目的原理：
![1](https://github.com/user-attachments/assets/dd9ef5b4-dbe4-4941-b075-0af233946f48)

## 快速入门

[视频教程](https://www.bilibili.com/video/BV1nYiyYdE9G)

### Step 0 - 准备工作

在[Release](https://github.com/Zao-chen/ZcChat/releases)下载安装并启动ZcChat

在[讨论区](https://github.com/Zao-chen/ZcChat/discussions)找到现成的角色数据，当然你也可以自定义角色

### Step 1- 配置角色

将角色数据中的立绘放入"/characters/{名称}"文件夹，并在 ZcChat>通用设置>角色选择 中选中角色

> 最后的项目结构应该是"/characters/{名称}/正常.png..."

### Step 2 - 安装配置Letta

[安装](https://github.com/letta-ai/letta?tab=readme-ov-file#-quickstart)并启动Letta，创建agent

在 LettaADE>agent>Persona 中填入之前找的现成的角色数据的提示词（核心记忆）

将Letta地址填入 ZcChat>AI模型配置>Letta地址 

> Letta地址默认为 http://localhost:8283

将agentID填入 ZcChat>角色配置>AI模型>agentID

### Step 3 - （可选）安装配置Vits

#### 方式1：vits-simple-api

[安装](https://github.com/Artrajz/vits-simple-api/blob/main/README_zh.md)并启动vits-simple-api，比将url填入 ZcChat>语言合成设置>vits-simple-api地址

>vits-simple-api默认地址为 http://localhost:23456

#### 方式2：其他语音合成

项目允许使用任意的语音合成工具，首先开启自定义模式

填入合成的API调用地址，但注意必须输出的是mp3文件，并使用{msg}作为文本的占符位，例如：

```
http://127.0.0.1:23456/voice/vits?id=0&text={msg}&format=mp3
```

这个链接访问得到的应该是一个合成出来的mp3文件

### Step 4 -（可选）配置语音输入

#### 方式1：whisper-asr-webservice

[安装](https://github.com/ahmetoner/whisper-asr-webservice?tab=readme-ov-file#quick-usage)并启动whisper-asr-webservice

将Url填入 ZcChat>语言输入设置>whisper-asr-webservice地址

> whisper-asr-webservicei默认地址为 http://localhost:9000

#### 方式2：百度语音识别

在[百度智能云控制台](https://console.bce.baidu.com/ai-engine/old/#/ai/speech/app/list)创建短语音识别应用。

将API Key和Secret Key填入 ZcChat>语言输入设置>API Key/Secret Key

### Step 5 -（可选）配置语音唤醒和打断

观察 语言输入设置>语音能量赋值 ，不说话和说话的能量应该会有较大区别，寻找的适合自己的阈值填入设置。

VAD算法写的比较粗糙，所以说语音唤醒和语音打断对环境的要求较大，请仔细配置此项。

## F&Q

### 如何使用其他LLM，如OpenAI、Gemini等

[Letta使用不同LLM文档](https://docs.letta.com/models/openai)，总的来说就是设置环境变量即可

### 如何自定义base url，我想用一些第三方的转发，比如说GPT_API_free

添加环境变量OPENAI_API_BASE即可

### 更新/降级letta后数据库报错，无法启动

一般是letta版本变化有数据库格式改动，如果你不在意数据的话可以尝试删除用户文件夹下的.letta文件夹后重新启动。

参考[#1896](https://github.com/letta-ai/letta/issues/1896)[#1501](https://github.com/letta-ai/letta/issues/1501)

### 对话不久后会出现格式错误

可能是她忘记了或者是一些误导性的词语篡改了记忆。尝试改进提示词或者用命令的语气告诉她要用正确的格式回复，使其加入记忆。效果如何取决于你使用的模型。我用的是gpt-4o-mini，命令过了一次后就没出现过该问题了。

## 相关链接

[LogChat](https://github.com/log159/LogChat)

[Letta](https://github.com/letta-ai/letta)

[vits-simple-api](https://github.com/Artrajz/vits-simple-api)

[whisper-asr-webservice](https://github.com/ahmetoner/whisper-asr-webservice)
