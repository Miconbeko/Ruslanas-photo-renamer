#pragma once

#include "database/database.hxx"
#include "config/config.hxx"
#include "utils/utils.hxx"

#include <string>
#include <vector>
#include <filesystem>

class Macros {
    protected:
        Config& config;
        Database& db;

        fs::path current_dir;
        fs::path initial_dir;

        bool exec_on_files;
        bool exec_on_dirs;
        bool exec_recursive;
    public:
        Macros(
            fs::path current_dir = "",
            fs::path initial_dir = "",
            bool exec_on_files = true,
            bool exec_on_dirs = false,
            bool exec_recursive = true,
            Config& config = Config::getInstance(),
            Database& db = Database::getInstance()
        ) : config(config),
            db(db),
            current_dir(current_dir),
            initial_dir(initial_dir),
            exec_on_files(exec_on_files),
            exec_on_dirs(exec_on_dirs),
            exec_recursive(exec_recursive)
        {}

        void set_current_dir(fs::path dir);
        void set_initial_dir(fs::path dir);
        void set_exec_on_files(bool value) { this->exec_on_files = value; }
        void set_exec_on_dirs(bool value) { this->exec_on_dirs = value; }

        bool get_exec_on_files() { return exec_on_files; }
        bool get_exec_on_dirs() { return exec_on_dirs; }
        bool get_exec_recursive() { return exec_recursive; }

        virtual std::string get_name() const = 0;
        virtual bool is_exec_on_dirs_set() const = 0;
        virtual bool is_done(fs::path entity) const = 0;
        virtual void execute() = 0;
};
