set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # 显式开启编译数据库生成 Clang-tidy使用
# 项目根CMakeLists.txt
cmake_minimum_required(VERSION 4.1...4.3)
project(
 Stacks_Tools
 VERSION 0.0.1
 DESCRIPTION "基于Stacks衍生开发 任务基本管理 的 C++23项目"
 LANGUAGES CXX)


# --- 诊断工具配置选项 ---
option(ENABLE_ASAN "启用 AddressSanitizer 内存检测" OFF)
option(ENABLE_XRAY "启用 LLVM XRay 性能插桩" OFF)

# --- AddressSanitizer (ASan) 配置 ---
if(ENABLE_ASAN)
  message(STATUS "AddressSanitizer 已开启")
  # 必须同时作用于编译器和链接器
  add_compile_options(-fsanitize=address -fno-omit-frame-pointer -g)
  add_link_options(-fsanitize=address)
endif()

# --- LLVM XRay 配置 ---
if(ENABLE_XRAY)
  message(STATUS "LLVM XRay 已开启")
  # 设定插桩阈值，-fxray-instruction-threshold=1 表示即使是很小的函数也记录
  add_compile_options(-fxray-instrument -fxray-instruction-threshold=1)
  add_link_options(-fxray-instrument)
endif()


# C++ 编译工具环境/编译工具链 配置 Apple平台

if(APPLE)

  set(LLVM_ROOT "/opt/homebrew/opt/llvm")
  set(CMAKE_CXX_COMPILER "${LLVM_ROOT}/bin/clang++")
  set(CMAKE_INSTALL_RPATH "${LLMV_ROOT}/lib")
  set(CMAKE_BUILD_WITH_INSTALL_RPATH ON)

  add_compile_options(-stdlib=libc++)
  add_link_options(-stdlib=libc++)

  set(CMAKE_CXX_STANDARD 23)
  set(CMAKE_CXX_EXTENSIONS OFF)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)

  set(CMAKE_CXX_SCAN_FOR_MODULES ON)  #自动分析模块依赖树 DAG
  add_compile_options(-fno-modules)   #禁用clang 旧版本 隐式模块 map,只使用C++20模块
  set(STDPPM_PATH "${LLVM_ROOT}/share/libc++/v1/std.cppm")   #定位标准库模块路径

  execute_process(COMMAND ${LLVM_ROOT}/bin/clang++ -print-resource-dir
          OUTPUT_VARIABLE CLANG_RESOURCE_DIR
      OUTPUT_STRIP_TRAILING_WHITESPACE)  #自动定位 Homebrew 安装LLVM路径

  #编译优化
  set(CMAKE_EXPERIMENTAL_CXX_SCANDEP_SOURCE ON)  #C++模块 实验性并行扫描 减少 模块扫描耗时
  #add_compile_options(-fno-autolink)  #禁用无用冗余传统头文件搜索路径,减少编译器在模块化项目中的I/O开销

  set(CMAKE_CXX_VISIBILITY_PRESET hidden)
  set(CMAKE_VISIBILITY_INLINES_HIDDEN TRUE) # 去除 非必要符号 缩减动态文件大小, 编译器有更多优化空间,降低逆向破解难度

endif()

if(NOT CMAKE_BUILD_TYPE)
  message(STATUS "No build type selected, default to RelWithDebInfo")
  set(CMAKE_BUILD_TYPE "Release")
endif()

# 寻找 clang-tidy 程序
find_program(CLANG_TIDY_EXE NAMES "clang-tidy")







# 1. 定义标准库模块为 OBJECT 类型 macOS 原生的链接器期望的是传统的 BSD 格式或带有特定索引的静态库 改用“对象库（Object Library）”或直接将源文件编译进主程序
add_library(std_module_lib OBJECT) # 改为 OBJECT
target_sources(std_module_lib
    PUBLIC
    FILE_SET cxx_modules TYPE CXX_MODULES
    BASE_DIRS "/"
    FILES "${STDPPM_PATH}"
)
target_compile_definitions(std_module_lib PUBLIC _LIBCPP_ENABLE_CXX20_REMOVED_FEATURES)
set_source_files_properties("${STDPPM_PATH}" PROPERTIES COMPILE_FLAGS "-Wno-reserved-module-identifier")
target_compile_features(std_module_lib PUBLIC cxx_std_23)

# 关键：定义一个包含路径，方便子模块找到根目录下的 json.hpp
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")
# 2. 包含子目录（子目录里的库现在可以 link 这个 std_module_lib）
add_subdirectory(modules/project_sync)
add_subdirectory(modules/task_manager)

# 3. 创建主程序
add_executable(Stacks_Tools src/main.cpp)

# 4. 链接关系：主程序依赖子模块，子模块依赖 std 模块 链接时，CMake 会自动处理 OBJECT 库的依赖
target_link_libraries(Stacks_Tools PRIVATE project_sync task_manager std_module_lib)

# 剩下的辅助配置...
target_include_directories(Stacks_Tools PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}" "${LLVM_ROOT}/include")


if(CLANG_TIDY_EXE)
  message(STATUS "Clang-Tidy 已启用")
  # 设置检查规则：
  # 必须在 add_executable 或 add_library 指令 之前 设置
  # -* 表示关闭默认规则
  # bugprone-* 查找容易出错的代码 潜伏的炸弹
  # modernize-* 提示你使用 C++11/14/17/20 的新特性
  # readability-* 提高代码可读性
  # performance-* 查找性能陷阱 建议修复，关系到代码的健壮性
  # google-*  谷歌规范
  # cppcoreguidelines-* Bjarne Stroustrup 和 C++ 标准委员会主席 Herb Sutter 亲自牵头编写的一套“C++ 最佳实践圣经
  # clang-analyzer-*
  set(CMAKE_CXX_CLANG_TIDY
      ${CLANG_TIDY_EXE};
      #-checks=-*,bugprone-*,modernize-*,performance-*,readability-*,cppcoreguidelines-*,google-*,clang-analyzer-*;
      -header-filter=${CMAKE_SOURCE_DIR}/.*
    )
endif()

# 确保 data 目录存在
file(MAKE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/data")

# 辅助工具 - CCache
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
  set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE_PROGRAM})
  set(CMAKE_C_COMPILER_LAUNCHER ${CCACHE_PROGRAM})
  message(STATUS "ccache: Enabled via COMPILER_LAUNCHER")
endif()
