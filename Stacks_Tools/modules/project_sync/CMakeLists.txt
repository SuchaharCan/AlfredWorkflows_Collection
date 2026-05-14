# 定义一个独立的静态库
add_library(project_sync OBJECT)

# 1. 接口文件 (.cppm)
target_sources(project_sync
    PUBLIC
    FILE_SET cxx_modules TYPE CXX_MODULES
    BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include"
    FILES "${CMAKE_CURRENT_SOURCE_DIR}/include/ProjectSync.cppm"
)

# 2. 实现文件 (.cpp)
target_sources(project_sync
    PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src/ProjectSync.cpp"
)

# 3. 链接标准库模块（假设你在根目录定义了 std_module_lib）
target_link_libraries(project_sync PUBLIC std_module_lib)

# 4. 允许主程序访问头文件/接口
# 在最后添加对根目录的引用
target_include_directories(project_sync PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_include_directories(project_sync PRIVATE "${CMAKE_SOURCE_DIR}")
