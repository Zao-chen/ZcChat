# ZcChat

🌟 **一个有长期记忆、表情动作立绘显示、操控电脑和语音功能，更适合Galgame角色的AI桌宠** 🌟
![ZcChat预览](https://github.com/user-attachments/assets/c03ceb8f-b0cc-46a8-bb50-07433c630048)

[![GitHub Release](https://img.shields.io/github/v/release/Zao-chen/ZcChat?include_prereleases&style=for-the-badge)](https://github.com/Zao-chen/ZcChat/releases)![GitHub Downloads (all assets, all releases)](https://img.shields.io/github/downloads/Zao-chen/ZcChat/total?style=for-the-badge)[![GitHub Repo stars](https://img.shields.io/github/stars/Zao-chen/ZcChat?style=for-the-badge)](https://github.com/Zao-chen/ZcChat/stargazers)
[![GitHub commit activity](https://img.shields.io/github/commit-activity/m/Zao-chen/ZcChat?style=for-the-badge)](https://github.com/Zao-chen/ZcChat/commits)[![GitHub Discussions](https://img.shields.io/github/discussions/Zao-chen/ZcChat?style=for-the-badge)](https://github.com/Zao-chen/ZcChat/discussions)[![GitHub License](https://img.shields.io/github/license/Zao-chen/ZcChat?style=for-the-badge)](LICENSE)

**视频演示**：

- [3.0 操控电脑功能演示](https://www.bilibili.com/video/BV1hA9SYYEbb)
- [2.0 语音输入功能演示](https://www.bilibili.com/video/BV1KWccegEkv)
- [1.0 立绘动画功能演示](https://www.bilibili.com/video/BV1aFCKYJEy4/)
- [0.0 基础功能演示](https://www.bilibili.com/video/BV1YUUaYgEgC/)

---

## 🎯 项目介绍

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
2. 在[讨论区](https://github.com/Zao-chen/ZcChat/discussions)找到现成的角色数据，或者自定义你的角色。

### 🎭 Step 2 - 配置角色

1. 将角色数据中的立绘放入 `/characters/{名称}` 文件夹。
   > 最终的项目结构应为 `/characters/{名称}/正常.png...`
2. 在 `ZcChat > 通用设置 > 角色选择` 中选中角色。
3. 在 `ZcChat > 角色配置` 设置立绘大小。
4. 右键托盘重置立绘位置。
   > Tips：右键立绘可以打开和关闭对话框，在对话框滚轮可以打开和关闭历史。

### 🤖 Step 3- 安装配置Letta

1. [安装Letta](https://github.com/letta-ai/letta?tab=readme-ov-file#-quickstart) 并启动。
2. 创建agent，并在 `LettaADE > agent > Persona` 中填入角色数据的提示词（核心记忆）。
3. 将Letta地址填入 `ZcChat > AI模型配置 > Letta地址`。
   > Letta默认地址为 `http://localhost:8283`
4. 将agentID填入 `ZcChat > 角色配置 > AI模型 > agentID`。

**🎉🎉🎉至此，ZcChat已经可以基本使用了，以下是一些进阶功能的使用：🎉🎉🎉**

### 🎙️ （可选）安装配置语音合成

#### 方式1：vits-simple-api

1. [安装vits-simple-api](https://github.com/Artrajz/vits-simple-api/blob/main/README_zh.md) 并启动。
2. 将URL填入 `ZcChat > 语言合成设置 > vits-simple-api地址`。
   > vits-simple-api默认地址为 `http://localhost:23456`
3. `ZcChat > 角色配置 > 语音合成 ` 中配置相关项

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

1.  将[文档](https://github.com/Zao-chen/ZcChat/wiki/电脑操控)内提示词追加到角色提示词即可。

### 🦊 （可选）制作自己的角色

1.  见[角色制作文档](https://github.com/Zao-chen/ZcChat/wiki/角色制作)

---

## ❓ 常见问题 (F&Q)

### 🤔 如何使用其他LLM，如OpenAI、Gemini等？

[Letta使用不同LLM文档](https://docs.letta.com/models/openai)，设置环境变量即可。

### 🌐 如何自定义base url？我想用一些第三方内容，比如说DeepSeek、GPT_API_free。

添加环境变量 `OPENAI_API_BASE` 即可。

### 🚨 更新/降级Letta后数据库报错，无法启动？

通常是由于Letta版本更新导致数据库格式变化。如果不介意数据丢失，可以删除用户文件夹下的 `.letta` 文件夹后重新启动。

参考：[#1896](https://github.com/letta-ai/letta/issues/1896) [#1501](https://github.com/letta-ai/letta/issues/1501)

### 📝 对话不久后会出现格式错误？

可能是AI忘记了或受到误导性词语的影响。尝试改进提示词，或用命令的语气告诉AI使用正确的格式回复。效果取决于所使用的模型。

### ⁉️ 为什么不使用Live2d？

本项目的特色就是使用了类Galgame的立绘方式，使得拥有表情和动作变化，并且降低了角色制作成本。如果你在找一个Live2d桌宠，我推荐LogChat。

### 🌏 如何纯本地运行

你需要使用Letta Desktop而不是Docker部署，Letta Desktop是一个纯本地运行的ADE。然后连接如ollama等本地LLM。

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