# ZcChat

🌟 **An AI desktop companion with long-term memory, expression and action illustrations, computer control, and voice functionality, better suited for Galgame characters** 🌟  
![ZcChat Preview](https://github.com/user-attachments/assets/c03ceb8f-b0cc-46a8-bb50-07433c630048)

[![GitHub Release](https://img.shields.io/github/v/release/Zao-chen/ZcChat?include_prereleases&style=for-the-badge)](https://github.com/Zao-chen/ZcChat/releases)[![GitHub License](https://img.shields.io/github/license/Zao-chen/ZcChat?style=for-the-badge)](LICENSE)[![GitHub commit activity](https://img.shields.io/github/commit-activity/m/Zao-chen/ZcChat?style=for-the-badge)](https://github.com/Zao-chen/ZcChat/commits)  
[![GitHub Downloads (all assets, all releases)](https://img.shields.io/github/downloads/Zao-chen/ZcChat/total?style=for-the-badge)](https://github.com/Zao-chen/ZcChat/releases)[![GitHub Repo stars](https://img.shields.io/github/stars/Zao-chen/ZcChat?style=for-the-badge)](https://github.com/Zao-chen/ZcChat/stargazers)[![GitHub Discussions](https://img.shields.io/github/discussions/Zao-chen/ZcChat?style=for-the-badge)](https://github.com/Zao-chen/ZcChat/discussions)[![GitHub Issues or Pull Requests](https://img.shields.io/github/issues/Zao-chen/ZcChat?style=for-the-badge)](https://github.com/Zao-chen/ZcChat/issues)  
### [中文](/README.md) English

---

## 🎯 Project Overview  
[Video Demo](https://www.bilibili.com/video/BV1hA9SYYEbb)  
### ✨ Key Features

- **Long-term Memory**: AI long-term memory implemented with Letta, developing and expanding the character's personality.
- **Expression & Actions**: Uses Galgame-style character illustrations rather than Live2D, enabling more actions and expressions.
- **Illustration Animation**: Similar to Galgame illustrations, enhancing visual experience.
- **Voice Interaction**: Supports voice input, direct conversation, wake-up, and interruption response functions.
- **Voice Synthesis**: Supports various speech synthesis tools like Vits, making the voice more realistic.

### 🛠️ Technical Overview

A diagram to understand how ZcChat works:

![ZcChat Diagram](https://github.com/user-attachments/assets/3e729fac-6568-4e3b-9be4-16e429a32f11)

---

## 🚀 Quick Start

### 📺 Video Tutorial

[Click here to watch the tutorial](https://www.bilibili.com/video/BV1hA9SYYEbb)

### 📋 Step 1 - Preparation

1. Download and install ZcChat from the [Release page](https://github.com/Zao-chen/ZcChat/releases). **Avoid using Chinese paths to prevent potential issues!**

> If you encounter "Error while attempting to create a file," please check if it is being blocked by antivirus software, and try running the installer with administrator privileges.

2. Find existing character data or create your own character in the [Discussion area](https://github.com/Zao-chen/ZcChat/discussions).

### 🎭 Step 2 - Configure Character

1. Place character illustrations into the `{Document}/ZcChat/characters/{Name}` folder.  
   > The final project structure should be `{Document}/ZcChat/characters/{Name}/normal.png...`
2. Select the character in `ZcChat > General Settings > Character Selection`.
3. Set the illustration size in `ZcChat > Character Configuration`.
4. Right-click the tray to reset the illustration position.  
   > Tip: Right-click the illustration to open or close the dialogue box. Scroll in the dialogue box to open or close history.

### 🤖 Step 3- Install and Configure LLM Model

#### Method 1: Letta

1. [Install Letta](https://github.com/letta-ai/letta?tab=readme-ov-file#-quickstart) and launch it.
2. Create an agent and fill in the prompt words (core memory) of the role data in `LettaADE > agent > Persona`.
3. Enter the Letta address in `ZcChat > AI Model Configuration > Letta Address`.
   
   > The default Letta address is `http://localhost:8283`
4. Enter the agentID in `ZcChat > Role Configuration > AI Model > agentID`.

#### Method 2: Openai General Interface
1. Enter BaseUrl and Key in `ZcChat > AI Model Configuration > Openai Settings`.
2. Select Openai in `ZcChat > Role Configuration > AI Model` and fill in the prompt words (core memory) of the Persona role data.

**🎉🎉🎉At this point, ZcChat is basically ready for use. Here are some advanced features: 🎉🎉🎉**

### 🎙️ (Optional) Install and Configure Voice Synthesis

#### Method 1: vits-simple-api

1. [Install vits-simple-api](https://github.com/Artrajz/vits-simple-api/blob/main/README_zh.md) and start it.
2. Enter the URL in `ZcChat > Language Synthesis Settings > vits-simple-api Address`.  
   > The default vits-simple-api address is `http://localhost:23456`.
3. Configure related items in `ZcChat > Character Configuration > Speech Synthesis`.

#### Method 2: Other Speech Synthesis Tools

1. Enable custom mode.
2. Enter the API call address, ensuring the output is an MP3 file and use `{msg}` as the text placeholder, for example:  
   ```  
   http://127.0.0.1:23456/voice/vits?id=0&text={msg}&format=mp3
   ```
3. Configure related items in `ZcChat > Character Configuration > Speech Synthesis`.

### 🎤 (Optional) Configure Voice Input

#### Method 1: whisper-asr-webservice

1. [Install whisper-asr-webservice](https://github.com/ahmetoner/whisper-asr-webservice?tab=readme-ov-file#quick-usage) and start it.
2. Enter the URL in `ZcChat > Language Input Settings > whisper-asr-webservice Address`.  
   > The default address is `http://localhost:9000`.

#### Method 2: Baidu Speech Recognition

1. Create a short voice recognition application in the [Baidu Smart Cloud Console](https://console.bce.baidu.com/ai-engine/old/#/ai/speech/app/list).
2. Enter the API Key and Secret Key in `ZcChat > Language Input Settings > API Key/Secret Key`.

### 🔔 (Optional) Configure Voice Wake-up and Interruption

1. First, configure the voice input.
2. Check `Language Input Settings > Voice Energy Value` to find the appropriate threshold where the energy difference between speaking and silence is noticeable.

   Due to the simplicity of the VAD algorithm, voice wake-up and interruption require a good environment for accurate configuration.

### 💻 (Optional) Provide Computer Control Interface for the Desktop Companion

1. Add the prompt from [documentation](https://github.com/Zao-chen/ZcChat/wiki/电脑操控) to the character's prompt.

### 🦊 (Optional) Create Your Own Character

1. Refer to the [Character Creation Documentation](https://github.com/Zao-chen/ZcChat/wiki/角色制作).

---

## ❓ Frequently Asked Questions (F&Q)

### 🤔 How can I use other LLMs like OpenAI, Gemini, etc.?

Refer to the [Letta documentation for using different LLMs](https://docs.letta.com/models/openai) to set the environment variable.

### 🌐 How can I customize the base URL? I want to use third-party content like DeepSeek or GPT_API_free.

Add the environment variable `OPENAI_API_BASE`.

### 🚨 Database error after updating/downgrading Letta, unable to start?

This is often due to database format changes when updating Letta. If data loss isn't an issue, you can delete the `.letta` folder in your user directory and restart.

Refer to: [#1896](https://github.com/letta-ai/letta/issues/1896) [#1501](https://github.com/letta-ai/letta/issues/1501)

### 📝 Dialogue format error after a short period?

It may be because the AI forgot or was misled by certain words. Try improving the prompt or telling the AI to reply in the correct format. The result depends on the model used.

### ⁉️ Why not use Live2D?

The main feature of this project is the use of Galgame-style illustrations, which allow for more expression and action changes and reduce character creation costs. If you're looking for a Live2D desktop companion, I recommend LogChat.

### 🌏 How to run entirely locally?

You need to use Letta Desktop instead of Docker deployment. Letta Desktop is a fully local ADE. Then, connect to a local LLM like ollama.

---

## 🤗 How to Contribute

ZcChat is an open-source project, and there are many ways to contribute!

- **Contribute to the project**: Interested in contributing? Feel free to [Pull Request](https://github.com/Zao-chen/ZcChat/pulls) (if you can understand my messy code)!
- **Report bugs or suggestions**: Found a bug or have a feature request? Submit them through [Issues](https://github.com/Zao-chen/ZcChat/issues).
- **Share your custom templates**: Created your own character? Share it in [Discussions](https://github.com/Zao-chen/ZcChat/discussions)!
- **Or...**: Give the project a star ⭐?

## 🔗 Related Links

- [log159/LogChat: 一个开源免费的AI聊天客户端!](https://github.com/log159/LogChat)
- [letta-ai/letta: Letta (formerly MemGPT) is a framework for creating LLM services with memory.](https://github.com/letta-ai/letta)
- [Artrajz/vits-simple-api: A simple VITS HTTP API, developed by extending Moegoe with additional features.](https://github.com/Artrajz/vits-simple-api)
- [ahmetoner/whisper-asr-webservice: OpenAI Whisper ASR Webservice API](https://github.com/ahmetoner/whisper-asr-webservice)
- [Liniyous/ElaWidgetTools: Fluent-UI For QT-Widget](https://github.com/Liniyous/ElaWidgetTools)
- [nlohmann/json: JSON for Modern C++](https://github.com/nlohmann/json)

[![Stargazers over time](https://starchart.cc/Zao-chen/ZcChat.svg?variant=adaptive)](https://starchart.cc/Zao-chen/ZcChat)