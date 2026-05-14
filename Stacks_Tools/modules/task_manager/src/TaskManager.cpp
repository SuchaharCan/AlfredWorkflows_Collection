// modules/task_manager/src/TaskManager.cpp
module;
#include "json.hpp"
#include <fstream>
#include <filesystem>
#include <chrono>
#include <format>
#include <unordered_map>
#include <iostream>
#include <random>

module TaskManager;

namespace fs = std::filesystem;
using json = nlohmann::json;

// 内部索引结构
struct TargetLink {
    std::string project_id; // 所属项目的 ID (即文件名)
    bool is_project;        // 是否是项目本身
};

namespace TaskManager {

    // 只需要读取 Stacks.json 即可完成所有逻辑匹配
    static std::unordered_map<std::string, TargetLink> load_quick_index(const std::string& stack_db_path) {
            std::unordered_map<std::string, TargetLink> index_map;
            std::ifstream file(stack_db_path);
            if (!file.is_open()) {
                std::println(std::cerr, "无法打开索引文件: {}", stack_db_path);
                return index_map;
            }

            json data;
            try {
                file >> data;
                // 修正：根据你的 JSON 结构，数据在 "nodes" 数组中
                if (data.contains("nodes") && data["nodes"].is_array()) {
                    for (const auto& item : data["nodes"]) {
                        std::string id = item.value("id", "");
                        // 注意：根据你之前提供的格式，Project 节点可能没有 projectid，或者 projectid 等于 id
                        // 我们使用 value 的默认值逻辑来处理
                        std::string pid = item.value("projectid", id);

                        if (id.empty()) continue;

                        if (id == pid) {
                            index_map[id] = { pid, true };  // 是项目 ID
                        } else {
                            index_map[id] = { pid, false }; // 是清单 ID
                        }
                    }
                } else {
                    std::println(std::cerr, "Stacks.json 格式错误：未找到 'nodes' 数组");
                }
            } catch (const std::exception& e) {
                std::println(std::cerr, "解析 Stacks.json 失败: {}", e.what());
            }
            return index_map;
        }

    std::string generate_id() {
        static constexpr std::string_view chars = "-0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz";
        auto now_ns = std::chrono::system_clock::now().time_since_epoch().count();
        std::string id(20, ' ');
        std::random_device rd;
        std::mt19937_64 gen(rd() ^ now_ns);
        std::uniform_int_distribution<int> dist(0, 63);
        auto temp_now = now_ns;
        for (int i = 0; i < 8; ++i) { id[i] = chars[temp_now & 63]; temp_now >>= 6; }
        for (int i = 8; i < 20; ++i) { id[i] = chars[dist(gen)]; }
        return id;
    }

    bool create_task(const TaskRequest& req, const std::string& project_db_path, const std::string& stack_db_path) {
        try {
            // 1. 获取索引映射
            auto index = load_quick_index(stack_db_path);

            // 匹配校验
            if (!index.contains(req.project_id)) {
                std::println(std::cerr, "错误：ID [{}] 在数据库中不存在，请手动同步。", req.project_id);
                return false;
            }

            const auto& link = index[req.project_id];

            // 2. 构造物理路径 (基于你的文件结构：ID.project)
            // 需要从 project_db_path（Project.json）所在的目录获取 base 路径
            fs::path base_dir = fs::path(project_db_path).parent_path().parent_path();
            // 注意：这里可能需要根据你的实际 library_paths 动态获取，
            // 暂且假设我们能通过 link.project_id 拼出路径，或者从 Project.json 辅助获取一次路径

            // 为了稳妥，我们还是从 Project.json 读取一次 Base 路径映射
            std::string actual_file_path;
            {
                std::ifstream p_file(project_db_path);
                json p_data; p_file >> p_data;
                for(const auto& node : p_data["nodes"]) {
                    if(node["id"] == link.project_id) {
                        actual_file_path = node["path"];
                        break;
                    }
                }
            }

            if (actual_file_path.empty() || !fs::exists(actual_file_path)) {
                std::println(std::cerr, "错误：无法定位物理文件，请检查同步状态。");
                return false;
            }

            // 3. 读取并修改 Project 文件
            json project_json;
            {
                std::ifstream f_in(actual_file_path);
                f_in >> project_json;
            }

            std::string task_id = generate_id();
            bool inserted = false;

            if (link.is_project) {
                // 情况 A: 是项目 ID -> 直接仍 backlog
                if (!project_json.contains("backlog")) project_json["backlog"] = json::array();
                project_json["backlog"].insert(project_json["backlog"].begin(), task_id);
                inserted = true;
            } else {
                // 情况 B: 是清单 ID -> 在 stacks 节点查找并插入
                if (project_json.contains("stacks")) {
                    for (auto& s : project_json["stacks"]) {
                        if (s["id"] == req.project_id) {
                            if (!s.contains("tasks")) s["tasks"] = json::array();
                            s["tasks"].insert(s["tasks"].begin(), task_id);
                            inserted = true;
                            break;
                        }
                    }
                }
            }

            if (!inserted) {
                // 兜底：如果清单 ID 在文件中没找到，依然放 backlog 防止数据丢失
                project_json["backlog"].insert(project_json["backlog"].begin(), task_id);
            }

            // 4. 保存
            auto now = std::chrono::system_clock::now();
            project_json["updated"] = std::format("{:%Y-%m-%d %H:%M:%S}", now);

            fs::path task_dir = fs::path(actual_file_path).parent_path() / "tasks";
            fs::create_directories(task_dir);

            json task_json = {
                {"title", req.title}, {"description", req.description},
                {"id", task_id}, {"project", link.project_id},
                {"created", std::format("{:%Y-%m-%dT%H:%M:%S.000Z}", now)},
                {"updated", std::format("{:%Y-%m-%dT%H:%M:%S.000Z}", now)},
                {"archived", ""}, {"public", true}, {"owner", ""},
                {"visibleUsers", json::array()}, {"visibleRoles", json::array()},
                {"tags", json::array()}, {"assignees", json::array()}
            };

            std::ofstream(task_dir / (task_id + ".task")) << task_json.dump(4);
            std::ofstream(actual_file_path) << project_json.dump(4);

            return true;
        } catch (const std::exception& e) {
            std::println(std::cerr, "Runtime Error: {}", e.what());
            return false;
        }
    }
}
