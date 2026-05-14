// modules/project_sync/include/ProjectSync.cppm
export module ProjectSync;

import std;

export namespace ProjectSync {
    struct Config {
        std::string project_db_path = "data/Project.json";
        std::string stack_db_path = "data/Stack.json";
    };

    void sync_all(const std::string& search_path, const Config& config = {});
}
