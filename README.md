# ZcChat

一个有长期记忆、表情动作立绘显示和语音功能的ai桌宠

![QQ20241115-201211](https://github.com/user-attachments/assets/46a80ea9-35dc-4c2d-be6a-4f262a3f3a35)

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

### Step 1 - 安装Letta

[Letta安装教程](https://github.com/letta-ai/letta?tab=readme-ov-file#-quickstart)

### Step 2 - （可选）安装Vits

[Vits安装教程](https://github.com/Artrajz/vits-simple-api/blob/main/README_zh.md)

### Steap 3 - 准备角色数据

在[讨论区](https://github.com/Zao-chen/ZcChat/discussions)找到现成的角色数据，当然也可以自定义角色。

### Steap 4 - 配置Letta

启动了LettaServer后，角色，并填入之前找的现成的角色数据的提示词（核心记忆）。

![QQ20241118-194653](https://github.com/user-attachments/assets/1d269f53-9679-4fb0-9b9f-6dea78c8fa61)

然后创建agent。

Agent名称、LLM模型、嵌入配置 根据自己的需求配置。

![QQ20241118-194935](https://github.com/user-attachments/assets/cf7cca75-5a3f-4add-8b14-b2dc3c3ec268)

之后，将Letta的Url和AgencyID（chat页面右上角可以找到）填入ZcCaht设置。

![QQ20241118-195112](https://github.com/user-attachments/assets/fab1e378-f009-4fbb-b442-ce9627e0c8fe)

### Steap 5 - 配置立绘

将角色数据中的立绘放入"/tachie/{名称}"文件夹，并在ZcChat设置中填写{名称}。

最后的项目结构应该是"/tachie/{名称}/开心.png..."

### Steap 5 - （可选）配置Vits

Url和Id填入ZcChat设置即可。

## F&Q

### 如何使用其他LLM，如GPT-4

[Letta使用LLM文档](https://docs.letta.com/models/openai)

## 相关链接

[LogChat](https://github.com/log159/LogChat)

[Letta](https://github.com/letta-ai/letta)

[vits-simple-api](https://github.com/Artrajz/vits-simple-api)
