#include <iostream>
#include <filesystem>
#include <string>
#include <exception>
#include <format> // 确保包含 format

// 导入模块
import ProjectSync;
import TaskManager;

namespace fs = std::filesystem;

// 自动定位根目录（寻找包含 CMakeLists.txt 的父目录）
fs::path find_project_root(fs::path start_path) {
    fs::path p = fs::absolute(start_path);
    while (p.has_relative_path()) {
        if (fs::exists(p / "CMakeLists.txt")) return p;
        p = p.parent_path();
    }
    return fs::current_path();
}

int main(int argc, char* argv[]) {
    // 检查基础参数数量：程序名(0), 命令(1), ...
    if (argc < 3) {
        std::println("Usage:");
        std::println("  Sync:  Stacks_Tools sync <search_path>");
        std::println("  Task:  Stacks_Tools add <project_id> <title_and_desc>");
        std::println("         (Use '`' to separate title and description)");
        return 1;
    }

    try {
        fs::path root_path = find_project_root(fs::current_path());
        ProjectSync::Config cfg;
        cfg.project_db_path = (root_path / "data" / "Project.json").string();
        cfg.stack_db_path   = (root_path / "data" / "Stack.json").string();

        std::string command = argv[1];

        // --- 路由 1: 同步 (Stacks_Tools sync <path>) ---
        if (command == "sync") {
            if (argc < 3) {
                std::println(std::cerr, "Error: 'sync' requires a search path.");
                return 1;
            }
            ProjectSync::sync_all(argv[2], cfg);
            std::println("Sync completed.");
        }

        // --- 路由 2: 添加任务 (Stacks_Tools add <project_id> <title`desc>) ---
        else if (command == "add") {
            if (argc < 4) {
                std::println(std::cerr, "Error: 'add' requires <project_id> and <title`desc>.");
                return 1;
            }

            TaskManager::TaskRequest req;
                        req.project_id = argv[2]; // 这个 ID 可能是 ProjectID 或 StackID

                        std::string raw_input = argv[3];
                        size_t pos = raw_input.find('|');
                        if (pos != std::string::npos) {
                            req.title = raw_input.substr(0, pos);
                            req.description = raw_input.substr(pos + 1);
                        } else {
                            req.title = raw_input;
                            req.description = "";
                        }

                        // 注意：这里需要传入 cfg.stack_db_path 用于 O(1) 索引查找
                        if (TaskManager::create_task(req, cfg.project_db_path, cfg.stack_db_path)) {
                            std::println("成功创建任务: {}", req.title);
                        } else {
                            // 错误信息已经在 create_task 内部通过 std::cerr 输出
                            return 1;
                        }
                    }
        else {
            std::println(std::cerr, "Unknown command: {}", command);
            return 1;
        }

    } catch (const std::exception& e) {
        std::println(std::cerr, "Runtime Error: {}", e.what());
        return 1;
    }

    return 0;
}
