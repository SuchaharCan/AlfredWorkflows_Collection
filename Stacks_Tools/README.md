用户痛点：

> 新建   `Task`   需手动打开软件 ，当前 版本和支持 无法融入到 Alfred  统一工作流中  

用户场景  工作流 ：

> **阐述**
>
> omnifocus  和 Stacks 二者 均会搭配使用 ，优先 摸索 Stacks  均涉及到 收件箱 和对收件箱中任务 状态管理 
>
> 以 Stacks 作为练手项目  

核心需求： 

1. 快速录入 task 到指定层级： project ->file -> list -> task 
2. 查看 特定层级的代办信息 
3. task推送到 omnifocu 或 apple 日历等 ; 



进阶需求： 

1. 后续 优化自托管数据 



开发/使用环境： 最低版本没有验证过，可反馈调整

> macOS  Version 26.5  Build 25F71
> Stacks Version  2.7.4 Build 25913
>
>  Alfred 5.7.3 [2320] 



> LLVM Homebrew clang version 22.1.4 arm64-apple-darwin25.5.0





## 产品 视角 描述

基础功能：解析 project  生成json文件 供后续功能开发  





上述 是Alfred  workflow 上针对 Stack2   针对 写死的project进行创建新task中的第一步 ； 





现在 进行 后续维护升级改造 ：  



新生成的workflow名字叫做  Stacks-Tools.workflow  
功能清单：

1. ST(Stacks-Tools 简写)  字典表
   字典一：更新维护的project 和 指代数字 

1. 核心且唯一的功能是针对 已存在的project 创建task ,  project id 作为参数传递进来  

workflow 中参数 均 在Alfred 相关的script 中获取， 核心逻辑 均在C++ 项目中完成 ； 



Alfred  文本框中 录入的内容格式 



sc 测试标题 `测试内容 `  



菜单功能：  

1. 点击同步功能  获取  project  - > file -> list -> task;   
   project 存储路径， 指代值 等基础信息， 后续功能开发维护更近需要 
   为何不直接读取？  涉及到 耦合性问题吧 ？   >根据路径查看 文件是否存在， 若存在 继续执行，否则 跳出 》 创建的前提 ； 

   

   2  核心是创建任务 ， 后续可 添加   查看 代办任务 ，进行将 特定任务进行 推送给 苹果进入日程中 ； 

   

   

   











创建 task  功能描述： 

1. 第一步 新建 task文件  

 
