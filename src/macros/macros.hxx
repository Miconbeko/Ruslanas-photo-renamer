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
        bool path_required;
    public:
        Macros(
            fs::path current_dir = "",
            fs::path initial_dir = "",
            bool exec_on_files = true,
            bool exec_on_dirs = false,
            bool exec_recursive = true,
            bool path_required = true,
            Config& config = Config::getInstance(),
            Database& db = Database::getInstance()
        ) : config(config),
            db(db),
            current_dir(current_dir),
            initial_dir(initial_dir),
            exec_on_files(exec_on_files),
            exec_on_dirs(exec_on_dirs),
            exec_recursive(exec_recursive),
            path_required(path_required)
        {}

        void set_current_dir(fs::path dir);
        void set_initial_dir(fs::path dir);
        void set_exec_on_files(bool value) { this->exec_on_files = value; }
        void set_exec_on_dirs(bool value) { this->exec_on_dirs = value; }
        void set_exec_recursive(bool value) { this->exec_recursive = value; }
        void set_path_required(bool value) { this->path_required = value; }

        bool get_exec_on_files() { return get_path_required() ? exec_on_files : false; }
        bool get_exec_on_dirs() { return get_path_required() ? exec_on_dirs : false; }
        bool get_exec_recursive() { return get_path_required() ? exec_recursive : false; }
        bool get_path_required() { return path_required; }

        virtual std::string get_name() const = 0;
        virtual bool is_exec_on_dirs_set() const { return true; }
        virtual bool is_done(fs::path entity) const;
        virtual void execute() = 0;

        virtual void pre_execute() {};
        virtual void post_execute(bool is_force_stop) {};
};
