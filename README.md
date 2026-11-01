# Alfred Workflows Collection

收集整理常用的 Alfred Workflows。

## Omni-Stacks

Omni-Stacks 是面向 [Stacks 2](https://stacksapp.io/) 的 Alfred 任务中心。Alfred 负责
交互与轻量参数预处理，单二进制 C++ 核心负责路径索引、任务持久化、计时和提醒同步。

### 安装与开始使用

1. 下载 [Omni-Stacks.alfredworkflow](https://github.com/SuchaharCan/AlfredWorkflows_Collection/releases/download/Stacks2/Omni-Stacks.alfredworkflow)。
2. 双击安装包，在 Alfred 中确认导入。
3. 首次使用输入 `s ap`，选择已导入 Stacks 2 的工作区目录。
4. 输入 `s` 显示全部操作和任务入口。

发布包不包含用户数据和日志。首次运行会自动创建 `data/` 与 `log/`；日志可在
Workflow Configuration 的 `STACKS_LOGGING` 中启停，默认仅保留最近三个月。

### 主要功能

- 管理多个 Stacks 工作区，并分别重载项目、清单和任务索引。
- 使用 `标题|描述|开始时间|截止时间|优先级` 快速录入任务。
- 按工作区、项目、清单和 Inbox 浏览、模糊搜索、编辑或归档任务。
- 设置截止日期，集中列出所有工作区本周到期的任务。
- 开始、停止任务计时，并将计时记录写回 Stacks 数据。
- 将带截止时间的任务同步到 Apple 提醒事项、OmniFocus 或两者。
- 支持成功/失败音效、大字号反馈、通知、完整 Debug trace 和自定义命令别名。

### 常用命令

| 命令 | 功能 |
| --- | --- |
| `s` | 显示全部操作，或按标题参数开始录入任务 |
| `s ap` | 添加新的 Stacks 工作区路径 |
| `s rl` | 重载全部已保存路径 |
| `s cf` | 查看路径配置，添加或重载路径 |
| `s v` | 浏览项目、清单和任务，并进入编辑 |
| `s t 关键词` | 在所有工作区、项目和清单中搜索任务 |
| `s w` | 查看本周到期任务并同步提醒 |
| `s tm` | 查看运行中的计时器并停止计时 |
| `s i` | 选择工作区并进入 Inbox |

更多细节、调试方法与注意事项请查看
[Omni-Stacks Release 说明](https://github.com/SuchaharCan/AlfredWorkflows_Collection/releases/tag/Stacks2)。

核心源码：[CXX-CMake-template](https://github.com/SuchaharCan/CXX-CMake-template)

开发者个人网站：[https://suchaharcan.github.io/](https://suchaharcan.github.io/)

> 拉取核心源码后执行 `just docs`，查看架构设计、接口说明、持久化字段和测试说明。

## Yabai Control

Yabai Control v1.1.0 是一个用于快速管理 yabai 的 Alfred workflow。

- 启动、重启、停止及检查 yabai 服务
- 查看 yabai 版本
- 列出当前显示器和空间中的窗口进程
- 忽略或解除忽略当前应用，并将规则持久化到 `~/.yabairc`
- 同时兼容官方 yabai 与支持 macOS 27 的 yabai 分支

下载：[Yabai Control.alfredworkflow](Yabai%20Control.alfredworkflow)

标签：`Yabai`
