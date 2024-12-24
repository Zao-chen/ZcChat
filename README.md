# ZcChat

一个有长期记忆、表情动作立绘显示和语音功能的ai桌宠

![QQ20241115-201211](https://github.com/user-attachments/assets/46a80ea9-35dc-4c2d-be6a-4f262a3f3a35)

[视频演示](https://www.bilibili.com/video/BV1YUUaYgEgC/)

## 介绍

### 特色

- 拥有长期记忆
- 性格和个性会发展拓展
- 动作和表情显示
- 支持语音输出
- 多种llm支持
- 支持纯本地运行

### 技术介绍

一图看懂本项目的原理：
![1](https://github.com/user-attachments/assets/dd9ef5b4-dbe4-4941-b075-0af233946f48)

## 快速入门

[视频教程](https://www.bilibili.com/video/BV1nYiyYdE9G)

### Step 0 - 准备角色数据

在[讨论区](https://github.com/Zao-chen/ZcChat/discussions)找到现成的角色数据，当然也可以自定义角色。

同时安装ZcChat。

### Step 1 - 安装Letta

[Letta安装教程](https://github.com/letta-ai/letta?tab=readme-ov-file#-quickstart)

### Step 2 - 配置Letta

启动了LettaServer创建agent。

Agent名称、LLM模型、嵌入配置 根据自己的需求配置。

![QQ20241224-201038](https://github.com/user-attachments/assets/a8612f48-cf72-4a8c-a0b7-d98fb6d09e6d)

persona填入之前找的现成的角色数据的提示词（核心记忆）。

![QQ20241224-201405](https://github.com/user-attachments/assets/64cbcdab-e3ee-4f8c-a6e5-116b85d2601c)

### Step 3 - （可选）安装Vits

[Vits安装教程](https://github.com/Artrajz/vits-simple-api/blob/main/README_zh.md)

### Step 4 配置ZcChat

Letta：Url和AgencyID（chat页面右上角可以找到）填入ZcCaht设置。

![QQ20241118-195112](https://github.com/user-attachments/assets/fab1e378-f009-4fbb-b442-ce9627e0c8fe)

Vits：Url和Id填入ZcChat设置即可。

![QQ20241224-201819](https://github.com/user-attachments/assets/d7f583dd-324a-4d53-8ad0-21a7866589b9)

### Step 5 - 配置立绘

将角色数据中的立绘放入"/tachie/{名称}"文件夹，并在ZcChat设置中填写{名称}。

最后的项目结构应该是"/tachie/{名称}/开心.png..."

## F&Q

### 如何使用其他LLM，如GPT-4

[Letta使用LLM文档](https://docs.letta.com/models/openai)

### 更新/降级letta后数据库报错，无法启动

一般是letta版本变化有数据库格式改动，如果你不在意数据的话可以尝试删除用户文件夹下的.letta文件夹后重新启动。

## 相关链接

[LogChat](https://github.com/log159/LogChat)

[Letta](https://github.com/letta-ai/letta)

[vits-simple-api](https://github.com/Artrajz/vits-simple-api)
