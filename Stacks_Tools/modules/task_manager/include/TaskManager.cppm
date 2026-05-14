// modules/task_manager/include/TaskManager.cppm
module;
#include <string>
#include <vector>

export module TaskManager;

namespace TaskManager {
    export struct TaskRequest {
        std::string title;
        std::string description;
        std::string project_id; // 目标 ID (可能是 ProjectID 或 StackID)
    };

    export bool create_task(const TaskRequest& req,
                                const std::string& project_db_path,
                                const std::string& stack_db_path);
    export std::string generate_id();
}
