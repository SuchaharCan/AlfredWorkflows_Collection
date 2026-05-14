add_library(task_manager OBJECT)

target_sources(task_manager
    PUBLIC
    FILE_SET cxx_modules TYPE CXX_MODULES
    BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include"
    FILES "${CMAKE_CURRENT_SOURCE_DIR}/include/TaskManager.cppm"
)

target_sources(task_manager
    PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src/TaskManager.cpp"
)

target_link_libraries(task_manager PUBLIC std_module_lib)
target_include_directories(task_manager PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_include_directories(task_manager PRIVATE "${CMAKE_SOURCE_DIR}")
