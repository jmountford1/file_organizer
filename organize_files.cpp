#include <iostream>
#include <algorithm>
#include <filesystem>
#include <unordered_map>
#include <vector>
namespace fs = std::filesystem;

int main() {
    fs::path misc_dir = "misc";

    if (!fs::exists(misc_dir)) {
        std::cout << "\"misc\" directory not found. Creating it..." << std::endl;
        fs::create_directory(misc_dir);
    } else {
        std::cout << "\"misc\" directory already exists. Skipping creation..." << std::endl;
    }

    std::unordered_map<std::string, std::vector<std::string>> categories = {
        {"misc docs", {".doc", ".docx", ".odt"}},
        {"misc excel", {".xls", ".csv", ".xml", ".xlsx", ".xlsb", ".ods"}},
        {"misc pdfs", {".pdf"}},
        {"misc txt", {".txt", ".log"}},
        {"misc img", {".jpg", ".jpeg", ".png", ".gif", ".bmp", ".webp"}},
        {"misc html", {".html"}},
        {"misc json", {".json", ".xml"}},
        {"misc code", {".sql", ".css", ".js", ".php", ".py", ".r"}},
        {"misc zip", {".zip"}}
    };

    for (const auto& category : categories) {
        const std::string& subfolder = category.first;
        fs::path subfolder_path = misc_dir / subfolder;
        if (!fs::exists(subfolder_path)) {
            std::cout << "Creating subfolder: " << subfolder_path << std::endl;
            fs::create_directory(subfolder_path);
        } else {
            std::cout << "Subfolder already exists: " << subfolder_path << std::endl;
        }
    }

    for (const auto& entry : fs::directory_iterator(fs::current_path())) {
        if (entry.is_regular_file()) {
            fs::path file_path = entry.path();
            std::string filename = file_path.filename().string();
            std::size_t dot_pos = filename.find_last_of('.');

            std::string extension;
            if (dot_pos != std::string::npos) {
                extension = filename.substr(dot_pos);
            } else {
                extension = ""; // No extension found
            }

            bool moved = false;
            for (const auto& category : categories) {
                const std::string& subfolder = category.first;
                const std::vector<std::string>& exts = category.second;

                if (std::find(exts.begin(), exts.end(), extension) != exts.end()) {
                    fs::path destination = misc_dir / subfolder / file_path.filename();

                    std::cout << "Moving file: " << file_path << " -> " << destination << std::endl;
                    fs::rename(file_path, destination);
                    moved = true;
                    break;
                }
            }
            if (!moved) {
                std::cout << "File not categorized: " << file_path.filename() << std::endl;
            }
        }
    }

    std::cout << "File organization completed. Press enter to exit..." << std::endl;
    std::cin.get();

    return 0;
}
