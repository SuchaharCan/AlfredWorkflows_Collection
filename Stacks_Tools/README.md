# Stacks2 Tasks

一个面向 [Stacks 2](https://www.stacksbrowser.com/) 的 Alfred workflow，用于在不保持 Stacks 前台运行的情况下快速创建任务。

当前版本：**1.0.0**

发布标签：**Stacks2**

## 下载与安装

1. 下载 [Stacks2.alfredworkflow](../Stacks2.alfredworkflow)。
2. 双击安装包并导入 Alfred。
3. 在 Alfred 输入 `s reload`，选择包含 Stacks `.project` 文件的目录。
4. 输入 `s`，选择 Project 或 Stack，再输入任务内容。

任务输入格式：

```text
任务标题 | 任务说明
```

## 功能

- 扫描指定目录中的 Stacks Project。
- 索引 Project、Stack 和任务目标。
- 使用 Alfred 统一输入框快速录入任务。
- 无需 Stacks 应用保持打开。
- 安装包已包含 ARM64 可执行文件及 C++ 运行库。

## 环境要求

- Apple Silicon Mac
- Alfred 5 + Powerpack
- Stacks 2

最低 macOS 版本尚未进行完整兼容测试。当前验证环境：

- macOS 27.2
- Stacks 2.7.4 Build 25913
- Alfred 5.8.1
- Homebrew LLVM 23.1.4

## 数据与隐私

发布包中的 `data/Project.json` 和 `data/Stack.json` 均为空白数据。第一次使用必须执行 `s reload`，由工作流在本地生成索引。

工作流不会上传你的 Stacks 项目路径、项目标题或任务内容。

## 从源码构建

构建需要 CMake 4.1+、Ninja 和 Homebrew LLVM。

```bash
cmake -S Stacks_Tools -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build -j 8
```

生成的二进制位于：

```text
build/Stacks_Tools
```

命令行接口：

```bash
Stacks_Tools --version
Stacks_Tools sync "/path/to/Stacks/library"
Stacks_Tools add "<project-or-stack-id>" "任务标题 | 任务说明"
```

## 目录结构

```text
Stacks_Tools/
├── CMakeLists.txt
├── src/main.cpp
├── modules/
│   ├── project_sync/
│   └── task_manager/
├── data/
│   ├── Project.json
│   └── Stack.json
└── json.hpp
```

## 许可证

项目使用仓库根目录中的 MIT License。
