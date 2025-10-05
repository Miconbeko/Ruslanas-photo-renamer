#pragma once

#include "color/color.hpp"
#include "logger/logger.hxx"

#include <iostream>
#include <string>
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

class Utils {
    private:
        std::vector<std::string> legit_filenames;

        static Utils* utils;

        Utils();
    public:
        bool is_legit(fs::path file, bool do_remove_prefix = true);
        std::vector<std::string> get_legit_filenames() { return legit_filenames; }

        static Utils& getInstance();
        static std::vector<fs::path> get_files(fs::path dir);
        static void check_folder(fs::path dir);
        static bool has_folders(fs::path dir);
        static bool has_files(fs::path dir);

        static void handle_error(std::string info_msg, std::string err, std::string err_type = "ERROR", dye::R<std::string> (*func)(std::string) = dye::on_red);
        static void handle_error(std::string info_msg, std::exception& err, std::string err_type = "ERROR", dye::R<std::string> (*func)(std::string) = dye::on_red);
        static std::string handle_info(std::string info, fs::path initial_dir, fs::path old_entity, fs::path new_entity, dye::R<std::string> (*func)(std::string));
        static void handle_success(std::string info_msg);

        static fs::path append_prefix(fs::path file, std::string prefix, char separator = '-');
        static fs::path remove_prefix(fs::path file, std::string* prefix = NULL, char separator = '-');
        static std::vector<std::string> split_filename(fs::path file, char separator = '-');
};
