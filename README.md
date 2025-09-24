# ZcChat

🌟 **一个有长期记忆、表情动作立绘显示、操控电脑和语音功能，更适合Galgame角色的AI桌宠** 🌟

<img width="1070" height="742" alt="422387848-c03ceb8f-b0cc-46a8-bb50-07433c630048" src="https://github.com/user-attachments/assets/d2f117cb-ff65-49cf-8b08-a6ef833e3464" />

[![GitHub Release](https://img.shields.io/github/v/release/Zao-chen/ZcChat?include_prereleases&style=for-the-badge)](https://github.com/Zao-chen/ZcChat/releases)[![GitHub License](https://img.shields.io/github/license/Zao-chen/ZcChat?style=for-the-badge)](LICENSE)[![GitHub commit activity](https://img.shields.io/github/commit-activity/m/Zao-chen/ZcChat?style=for-the-badge)](https://github.com/Zao-chen/ZcChat/commits)
[![GitHub Downloads (all assets, all releases)](https://img.shields.io/github/downloads/Zao-chen/ZcChat/total?style=for-the-badge)](https://github.com/Zao-chen/ZcChat/releases)[![GitHub Repo stars](https://img.shields.io/github/stars/Zao-chen/ZcChat?style=for-the-badge)](https://github.com/Zao-chen/ZcChat/stargazers)[![GitHub Discussions](https://img.shields.io/github/discussions/Zao-chen/ZcChat?style=for-the-badge)](https://github.com/Zao-chen/ZcChat/discussions)[![GitHub Issues or Pull Requests](https://img.shields.io/github/issues/Zao-chen/ZcChat?style=for-the-badge)](https://github.com/Zao-chen/ZcChat/issues)

### 中文 [English](/README_EN.md)

---

## 🎯 项目介绍

[视频演示](https://www.bilibili.com/video/BV1hA9SYYEbb)

### ✨ 特色功能

- **长期记忆**：使用Letta实现AI长期记忆，并且发展拓展性格和个性。
- **表情动作**：采用Galgame立绘的方式而不是Live2d，使得角色可以有更多的动作和表情
- **立绘动画**：采用类似Galgame立绘的动画演出，提升视觉体验。
- **语音交互**：支持语音输入、直接对话、唤醒和回复打断功能。
- **语音合成**：支持Vits等多种语音合成工具，让声音更真实。

### 🛠️ 技术概览

一图看懂ZcChat的工作原理：

![ZcChat原理图](https://github.com/user-attachments/assets/3e729fac-6568-4e3b-9be4-16e429a32f11)

---

## 🚀 快速入门

### 📺 视频教程

[点击这里观看视频教程](https://www.bilibili.com/video/BV1hA9SYYEbb)

### 📋 Step 1 - 准备工作

1. 在[Release页面](https://github.com/Zao-chen/ZcChat/releases)下载并安装ZcChat。**请尽量避免中文路径，以免产生一些列问题！**

> 如果出现`尝试创建文件时出错`请检查是否被杀毒软件拦截，并尝试使用管理员权限启动安装包。

2. [讨论区](https://github.com/Zao-chen/ZcChat/discussions)找到现成的角色数据，或者自定义你的角色。

### 🎭 Step 2 - 配置角色

1. 解压角色数据放入 `{文档}/ZcChat/characters/{名称}` 文件夹。
   
   > 最终的项目结构应为 `{文档}/ZcChat/characters/{名称}/正常.png...`

2. 在 `ZcChat > 通用设置 > 角色选择` 中选中角色。
   
   > Tips：右键立绘可以打开和关闭对话框，在对话框滚轮可以打开和关闭历史。

### 🤖 Step 3- 安装配置LLM模型

#### 方式1：Openai通用接口

1. 将BaseUrl、Key填入 `ZcChat > AI模型配置 > OpenAI API设定`。
   
   > BaseUrl应该类似于为 `https://api.openai.com/v1/completions`

2. 在 `ZcChat > 角色设置> 接口设置` 中选择OpenAI并填入角色提示词。

   （部分提示词可能已经内置在了角色数据中，无需额外配置）

3. 配置格式提示词或者开启[输出格式增强](https://github.com/Zao-chen/ZcChat/wiki/%E8%BE%93%E5%87%BA%E6%A0%BC%E5%BC%8F%E5%A2%9E%E5%BC%BA)

   默认格式提示词：
   ```
   以下是我必须遵循的事项：
   我精通中文和日语，在与用户交谈时，无论他们使用何种语言，我都必须始终提供双语回复。
   我会在每句话之前添加我的心情和动作，我的心情必须选择以下单词之一：{emo}
   我应该严格按照这个格式回答{心情}|{中文}|{日语}
   ```

#### 方式2：Letta
> [Letta](https://www.letta.com/) 是一个可以提供长期记忆和学习功能的AI代理

1. [安装Letta](https://github.com/letta-ai/letta?tab=readme-ov-file#-quickstart) 并启动。

2. 创建agent，并在 `LettaADE > agent > Persona` 中填入角色提示词+格式提示词。

3. 将Letta地址填入 `ZcChat > AI模型配置 > Letta地址`。
   
   > Letta默认地址为 `http://localhost:8283`

4. 在 `ZcChat > 角色设置> 接口设置` 中选择Letta并填入角色提示词。

5. 将agentID填入 `ZcChat > 角色设置 > 接口设置 > agentID`。

**🎉🎉🎉至此，ZcChat已经可以基本使用了，以下是一些进阶功能的使用：🎉🎉🎉**

### 🎙️ （可选）安装配置语音合成

#### 方式1：vits-simple-api

1. [安装vits-simple-api](https://github.com/Artrajz/vits-simple-api/blob/main/README_zh.md) 并启动。

2. 将URL填入 `ZcChat > 语言合成设置 > vits-simple-api地址`。
   
   > vits-simple-api默认地址为 `http://localhost:23456`

3. `ZcChat > 角色配置 > 语音合成 ` 中配置相关项（更多参数可以直接在id里以url的形式添加，比如4&length=1.20）

#### 方式2：其他语音合成工具

1. 开启自定义模式。

2. 填入API调用地址，确保输出为mp3文件，并使用 `{msg}` 作为文本占位符，例如：
   
   ```
   http://127.0.0.1:23456/voice/vits?id=0&text={msg}&format=mp3
   ```

3. `ZcChat > 角色配置 > 语音合成 ` 中配置相关项

### 🎤 （可选）配置语音输入

#### 方式1：whisper-asr-webservice

1. [安装whisper-asr-webservice](https://github.com/ahmetoner/whisper-asr-webservice?tab=readme-ov-file#quick-usage) 并启动。

2. 将URL填入 `ZcChat > 语言输入设置 > whisper-asr-webservice地址`。
   
   > 默认地址为 `http://localhost:9000`

#### 方式2：百度语音识别

1. 在[百度智能云控制台](https://console.bce.baidu.com/ai-engine/old/#/ai/speech/app/list)创建短语音识别应用。
2. 将API Key和Secret Key填入 `ZcChat > 语言输入设置 > API Key/Secret Key`。

### 🔔 （可选）配置语音唤醒和打断

1. 先配置好语音输入

2. 观察 `语言输入设置 > 语音能量赋值`，不说话和说话的能量应该会有较大区别，寻找的适合自己的阈值填入设置。
   
   由于VAD算法较为简单，语音唤醒和打断对环境要求较高，请仔细配置。

### 💻 （可选）给予桌宠操控电脑的接口

1. 将[文档](https://github.com/Zao-chen/ZcChat/wiki/电脑操控)内提示词追加到角色提示词即可。

### 🦊 （可选）制作自己的角色

1. 见[角色制作文档](https://github.com/Zao-chen/ZcChat/wiki/角色制作)

---

## ❓ 常见问题 (F&Q)

### 🤔 如何使用其他LLM，如OpenAI、Gemini、DeepSeek等？

你可以使用Openai通用接口，市面上几乎所有AI都支持这个协议。

如果你使用Letta，可以看 [Letta文档](https://docs.letta.com/models/openai)

### 📝 对话不久后会出现格式错误？

可能是AI忘记了或受到误导性词语的影响。尝试改进提示词，或用命令的语气告诉AI使用正确的格式回复。效果取决于所使用的模型。

或者你可以开启[回复格式增强](https://github.com/Zao-chen/ZcChat/wiki/%E8%BE%93%E5%87%BA%E6%A0%BC%E5%BC%8F%E5%A2%9E%E5%BC%BA)功能

### ⁉️ 为什么不使用Live2d？

本项目的特色就是使用了类Galgame的立绘方式，使得拥有表情和动作变化，并且降低了角色制作成本。如果你在找一个Live2d桌宠，我推荐LogChat。

### 🌏 如何纯本地运行

Letta支持纯本地部署，当然你也可以使用如Ollama等工具并使用OpenAI通用接口。

---

## 🤗 如何贡献

ZcChat是一个开源项目。参与ZcChat项目的方法有很多！

- **为项目做出贡献**：有兴趣做出贡献吗？欢迎[Pull Request](https://github.com/Zao-chen/ZcChat/pulls)！~~（如果能读懂我的垃圾代码的话）~~
- **报告BUG、建议**：有BUG或有功能请求？请通过[Issues](https://github.com/Zao-chen/ZcChat/issues)提交它们。
- **分享自己制作的模板**：制作了自己的角色？不妨在[Discussions](https://github.com/Zao-chen/ZcChat/discussions)分享一下！
- **或者……**：给项目来一个star⭐怎么样？

## 🔗 相关链接

- [log159/LogChat: 一个开源免费的AI聊天客户端！](https://github.com/log159/LogChat)
- [letta-ai/letta: Letta (formerly MemGPT) is a framework for creating LLM services with memory.](https://github.com/letta-ai/letta)
- [Artrajz/vits-simple-api: A simple VITS HTTP API, developed by extending Moegoe with additional features.](https://github.com/Artrajz/vits-simple-api)
- [ahmetoner/whisper-asr-webservice: OpenAI Whisper ASR Webservice API](https://github.com/ahmetoner/whisper-asr-webservice)
- [Liniyous/ElaWidgetTools: Fluent-UI For QT-Widget](https://github.com/Liniyous/ElaWidgetTools)
- [nlohmann/json: JSON for Modern C++](https://github.com/nlohmann/json)

[![Stargazers over time](https://starchart.cc/Zao-chen/ZcChat.svg?variant=adaptive)](https://starchart.cc/Zao-chen/ZcChat)
