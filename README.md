# ZcChat

一个有长期记忆、表情动作立绘显示和语音功能的ai桌宠

![QQ20241115-201211](https://github.com/user-attachments/assets/46a80ea9-35dc-4c2d-be6a-4f262a3f3a35)

## 介绍

### 特色

- 拥有长期记忆
- 性格和个性会发展拓展
- 动作和表情显示
- 支持语音输出

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

启动了LettaServer后，在网页端创建Agent。

Agent名称、LLM模型、嵌入配置 根据自己的需求配置。

而核心记忆，或者说是提示词根据前面准备的角色数据填写。

之后，将Letta的Url和AgencyID（chat页面右上角可以找到）填入ZcCaht设置。

### Steap 5 - 配置立绘

将角色数据中的立绘放入"\tachie\{名称}"文件夹，并在ZcChat设置中填写{名称}。

### Steap 5 - （可选）配置Vits

Url和Id填入ZcChat设置即可。

## 相关链接

[LogChat](https://github.com/log159/LogChat)

[Letta](https://github.com/letta-ai/letta)

[vits-simple-api](https://github.com/Artrajz/vits-simple-api)
