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

cd build.ninja  

rm -rf *  ( rembuild * )
cmake .. -G Ninja 
cmake --build . -j 8

Core Functions 

./Stacks_Tools sync "~/YourName/StacksDir" 

./Stacks_Tools add "id"  "taskTitle | taskDisc"
