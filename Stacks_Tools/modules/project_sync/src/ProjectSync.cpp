module;
#include "json.hpp"
#include <fstream>
#include <filesystem>
#include <chrono>
#include <set>
#include <print>

module ProjectSync;
namespace fs = std::filesystem;
using json = nlohmann::json;

// 获取当前时间
std::string get_now_timestamp() {
    return std::format("{:%Y-%m-%d %H:%M:%S}", std::chrono::system_clock::now());
}

namespace ProjectSync {

    void sync_all(const std::string& current_search_path, const Config& config) {
        fs::create_directories(fs::path(config.project_db_path).parent_path());

        // 1. 读取旧数据（找回记忆）
        json db;
        std::set<std::string> lib_paths;

        if (fs::exists(config.project_db_path)) {
            std::ifstream ifs(config.project_db_path);
            if (ifs >> db) {
                if (db.contains("library_paths")) {
                    for (auto& p : db["library_paths"]) lib_paths.insert(p.get<std::string>());
                }
            }
        }

        // 2. 将本次输入的路径加入记忆
        if (!current_search_path.empty()) {
            lib_paths.insert(fs::absolute(current_search_path).string());
        }

        // 3. 重新扫描所有已记录的路径
        json project_nodes = json::array();
        json stack_nodes = json::array();

        for (const auto& path_str : lib_paths) {
            fs::path p(path_str);
            if (!fs::exists(p)) continue;

            for (const auto& entry : fs::directory_iterator(p)) {
                if (entry.path().extension() == ".project") {
                    try {
                        std::ifstream p_file(entry.path());
                        json p_content;
                        p_file >> p_content;

                        std::string p_id = p_content.value("id", "");
                        std::string p_title = p_content.value("title", "未命名项目");

                        // A. 记录项目索引
                        project_nodes.push_back({
                            {"path", entry.path().string()},
                            {"id", p_id},
                            {"title", p_title}
                        });

                        // B. 记录 Stack 节点（项目本身作为第一条记录）
                        stack_nodes.push_back({
                            {"id", p_id},
                            {"projectid", p_id},
                            {"title", p_title}
                        });

                        // C. 展开 Stacks（子节点记录）
                        if (p_content.contains("stacks") && p_content["stacks"].is_array()) {
                            for (auto& stack : p_content["stacks"]) {
                                // 提取子 ID 和 Title
                                std::string s_id = stack.value("id", "");
                                std::string s_title = stack.value("title", "未命名堆栈");

                                if (!s_id.empty()) {
                                    stack_nodes.push_back({
                                        {"id", s_id},
                                        {"projectid", p_id},
                                        {"title", s_title}
                                    });
                                }
                            }
                        }
                    } catch (const std::exception& e) {
                        std::println(std::cerr, "解析项目文件失败 {}: {}", entry.path().string(), e.what());
                        continue;
                    }
                }
            }
        }

        // 4. 构建并保存最终结果
        json final_project_db = {
            {"library_paths", json(lib_paths)},
            {"nodes", project_nodes},
            {"updateTime", get_now_timestamp()}
        };

        json final_stack_db = {
            {"nodes", stack_nodes},
            {"updateTime", get_now_timestamp()}
        };

        // 写入文件
        std::ofstream(config.project_db_path) << final_project_db.dump(4);
        std::ofstream(config.stack_db_path) << final_stack_db.dump(4);

        std::println(std::cerr, "[Memory] 当前库路径总数: {}", lib_paths.size());
    }
}
