# ZcChat

🌟 **An AI desktop pet with long-term memory, expressive character sprites, computer control, and voice features—perfect for Galgame-style characters** 🌟

<img width="1070" height="742" alt="422387848-c03ceb8f-b0cc-46a8-bb50-07433c630048" src="https://github.com/user-attachments/assets/d2f117cb-ff65-49cf-8b08-a6ef833e3464" />

[![GitHub Release](https://img.shields.io/github/v/release/Zao-chen/ZcChat?include_prereleases\&style=for-the-badge)](https://github.com/Zao-chen/ZcChat/releases)[![GitHub License](https://img.shields.io/github/license/Zao-chen/ZcChat?style=for-the-badge)](LICENSE)[![GitHub commit activity](https://img.shields.io/github/commit-activity/m/Zao-chen/ZcChat?style=for-the-badge)](https://github.com/Zao-chen/ZcChat/commits)
[![GitHub Downloads (all assets, all releases)](https://img.shields.io/github/downloads/Zao-chen/ZcChat/total?style=for-the-badge)](https://github.com/Zao-chen/ZcChat/releases)[![GitHub Repo stars](https://img.shields.io/github/stars/Zao-chen/ZcChat?style=for-the-badge)](https://github.com/Zao-chen/ZcChat/stargazers)[![GitHub Discussions](https://img.shields.io/github/discussions/Zao-chen/ZcChat?style=for-the-badge)](https://github.com/Zao-chen/ZcChat/discussions)[![GitHub Issues or Pull Requests](https://img.shields.io/github/issues/Zao-chen/ZcChat?style=for-the-badge)](https://github.com/Zao-chen/ZcChat/issues)

### [中文](/README.md) English

---

## 🎯 Project Introduction

[Video Demo](https://www.bilibili.com/video/BV1hA9SYYEbb)

### ✨ Key Features

* **Long-term Memory**: Uses Letta to implement AI long-term memory while developing and expanding character traits and personality.
* **Expressions & Actions**: Uses Galgame-style character sprites instead of Live2D, allowing more actions and expressions.
* **Sprite Animations**: Galgame-style animated sprite performances enhance visual experience.
* **Voice Interaction**: Supports voice input, direct dialogue, wake-up, and reply interruption.
* **Speech Synthesis**: Supports multiple TTS tools like Vits for more realistic voices.

### 🛠️ Technical Overview

One diagram to understand how ZcChat works:

![ZcChat Architecture](https://github.com/user-attachments/assets/3e729fac-6568-4e3b-9be4-16e429a32f11)

---

## 🚀 Quick Start

### 📺 Video Tutorial

[Click here to watch the video tutorial](https://www.bilibili.com/video/BV1hA9SYYEbb)

### 📋 Step 1 - Preparation

1. Download and install ZcChat from the [Release page](https://github.com/Zao-chen/ZcChat/releases). **Avoid using Chinese paths to prevent potential issues!**

> If you encounter `Error creating file`, check if your antivirus is blocking it and try running the installer as administrator.

2. Find ready-made character data in the [Discussion section](https://github.com/Zao-chen/ZcChat/discussions) or create your own character.

### 🎭 Step 2 - Configure Character

1. Extract the character data into `{Documents}/ZcChat/characters/{Name}`.

   > The final project structure should look like `{Documents}/ZcChat/characters/{Name}/Normal.png...`

2. Select the character in `ZcChat > General Settings > Character Selection`.

   > Tips: Right-click the sprite to open/close the dialogue box; use the scroll wheel in the dialogue box to open/close history.

### 🤖 Step 3 - Install & Configure LLM

#### Option 1: OpenAI API

1. Fill in BaseUrl and Key in `ZcChat > AI Model Settings > OpenAI API Settings`.

   > BaseUrl should be something like `https://api.openai.com/v1/completions`

2. Select OpenAI in `ZcChat > Character Settings > API Settings` and fill in the character prompt.

   (Some prompts may already be included in the character data.)

3. Configure formatting prompts or enable [Output Format Enhancement](https://github.com/Zao-chen/ZcChat/wiki/%E8%BE%93%E5%87%BA%E6%A0%BC%E5%BC%8F%E5%A2%9E%E5%BC%BA)

   Default format prompt:

   ```text
   The following are the rules I must follow:
   I am fluent in Chinese and Japanese, and when conversing with users, I must always reply in both languages regardless of the language they use.
   I will prepend my mood and action to every sentence. My mood must be chosen from the following words: {emo}
   I must strictly reply in this format: {Mood}|{Chinese}|{Japanese}
   ```

#### Option 2: Letta

> [Letta](https://www.letta.com/) is an AI agent providing long-term memory and learning capabilities.

1. [Install Letta](https://github.com/letta-ai/letta?tab=readme-ov-file#-quickstart) and start it.

2. Create an agent and fill in the character prompt + format prompt in `LettaADE > agent > Persona`.

3. Enter the Letta address in `ZcChat > AI Model Settings > Letta Address`.

   > Default address: `http://localhost:8283`

4. Select Letta in `ZcChat > Character Settings > API Settings` and fill in the character prompt.

5. Enter the agentID in `ZcChat > Character Settings > API Settings > agentID`.

**🎉🎉🎉 Now ZcChat is basically ready to use! Here are some advanced features: 🎉🎉🎉**

### 🎙️ (Optional) Configure Speech Synthesis

#### Option 1: vits-simple-api

1. [Install vits-simple-api](https://github.com/Artrajz/vits-simple-api/blob/main/README_zh.md) and start it.

2. Enter the URL in `ZcChat > Speech Synthesis Settings > vits-simple-api Address`.

   > Default: `http://localhost:23456`

3. Configure options in `ZcChat > Character Settings > Speech Synthesis` (additional parameters can be added in the URL, e.g., `4&length=1.20`)

#### Option 2: Other TTS Tools

1. Enable custom mode.
2. Fill in the API URL (ensure output is mp3) using `{msg}` as the text placeholder, for example:

   ```text
   http://127.0.0.1:23456/voice/vits?id=0&text={msg}&format=mp3
   ```
3. Configure options in `ZcChat > Character Settings > Speech Synthesis`.

### 🎤 (Optional) Configure Voice Input

#### Option 1: whisper-asr-webservice

1. [Install whisper-asr-webservice](https://github.com/ahmetoner/whisper-asr-webservice?tab=readme-ov-file#quick-usage) and start it.
2. Enter the URL in `ZcChat > Voice Input Settings > whisper-asr-webservice Address`.

   > Default: `http://localhost:9000`

#### Option 2: Baidu Speech Recognition

1. Create a short-phrase recognition app in the [Baidu AI Console](https://console.bce.baidu.com/ai-engine/old/#/ai/speech/app/list).
2. Fill in the API Key and Secret Key in `ZcChat > Voice Input Settings > API Key/Secret Key`.

### 🔔 (Optional) Configure Voice Wake & Interruption

1. Configure voice input first.
2. Observe `Voice Input Settings > Voice Energy Levels`; adjust thresholds for speaking vs. silence.

   > Note: Due to the simple VAD algorithm, wake-up and interruption require careful calibration in a suitable environment.

### 💻 (Optional) Give Desktop Pet Computer Control

1. Append the prompts from the [Computer Control Guide](https://github.com/Zao-chen/ZcChat/wiki/电脑操控) to your character prompt.

### 🦊 (Optional) Create Your Own Character

1. See [Character Creation Guide](https://github.com/Zao-chen/ZcChat/wiki/角色制作)

---

## ❓ FAQ

### 🤔 How to use other LLMs like OpenAI, Gemini, DeepSeek, etc.?

You can use the OpenAI API standard interface, which is supported by almost all AI models.

If using Letta, refer to [Letta Documentation](https://docs.letta.com/models/openai)

### 📝 Dialog format errors appear after a short while?

The AI may forget or be misled by misleading words. Try improving prompts or instruct the AI explicitly to use the correct format. Results depend on the model used.

You can also enable [Output Format Enhancement](https://github.com/Zao-chen/ZcChat/wiki/%E8%BE%93%E5%87%BA%E6%A0%BC%E5%BC%8F%E5%A2%9E%E5%BC%BA)

### ⁉️ Why not use Live2D?

This project emphasizes Galgame-style sprites, allowing expressions and actions while reducing character creation cost. If you want a Live2D desktop pet, consider LogChat.

### 🌏 How to run fully locally?

Letta supports fully local deployment. You can also use tools like Ollama with OpenAI API.

---

## 🤗 How to Contribute

ZcChat is open-source. Here are ways to contribute:

* **Contribute to the project**: Interested? Welcome [Pull Requests](https://github.com/Zao-chen/ZcChat/pulls)! ~~(If you can read my messy code)~~
* **Report bugs or suggestions**: Submit via [Issues](https://github.com/Zao-chen/ZcChat/issues).
* **Share your templates**: Made your own character? Share on [Discussions](https://github.com/Zao-chen/ZcChat/discussions)!
* **Or…**: Give the project a ⭐ star!

## 🔗 Related Links

* [log159/LogChat: An open-source AI chat client](https://github.com/log159/LogChat)
* [letta-ai/letta: Letta (formerly MemGPT), framework for LLM services with memory](https://github.com/letta-ai/letta)
* [Artrajz/vits-simple-api: Simple VITS HTTP API for voice synthesis](https://github.com/Artrajz/vits-simple-api)
* [ahmetoner/whisper-asr-webservice: OpenAI Whisper ASR Webservice API](https://github.com/ahmetoner/whisper-asr-webservice)
* [Liniyous/ElaWidgetTools: Fluent-UI for Qt Widgets](https://github.com/Liniyous/ElaWidgetTools)
* [nlohmann/json: JSON for Modern C++](https://github.com/nlohmann/json)

[![Stargazers over time](https://starchart.cc/Zao-chen/ZcChat.svg?variant=adaptive)](https://starchart.cc/Zao-chen/ZcChat)
