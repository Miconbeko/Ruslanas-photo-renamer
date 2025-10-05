#include "macrosExecutor.hxx"

#include <iostream>
#include <string>
#include <conio.h>

void MacrosExecutor::draw_separator() {
    std::cout << "______________________________________________________________________\n";
}

bool MacrosExecutor::ask_macros(bool loop_question) {
    std::string i_macros_str;
    int i = 1 - COUNT_FROM_ZERO;
    int min_i_macros = 1 - COUNT_FROM_ZERO;
    int max_i_macros = macros.size() - COUNT_FROM_ZERO;

    std::cout << "Choose macros:\n";
    draw_separator();
    for (auto macros_it : macros) {
        std::cout << i << ". " << macros_it->get_name() << "\n";
        if (separators.find(i) != separators.end()) {
            draw_separator();
        }
        i++;
    }
    draw_separator();

    while (true) {
        std::cout << "Enter macro's number: \t";
        std::cin >> i_macros_str;

        i_macros = stoi(i_macros_str);
        if (i_macros < min_i_macros || i_macros > max_i_macros) {
            Utils::handle_error("", "Wrong input. Try again");
            if (!loop_question) {
                return false;
            }
            continue;
        }
        i_macros--;
        current_macros = macros[i_macros + COUNT_FROM_ZERO];

        return true;
    }
}

bool MacrosExecutor::ask_path(bool loop_question) {
    std::wstring dir_wstr;

    while (true) {
        std::cout << "Enter folder path: \t";
        getline(std::wcin >> std::ws, dir_wstr);
        initial_dir = dir_wstr;

        try {
            Utils::check_folder(initial_dir);
            current_macros->set_initial_dir(initial_dir);

            return true;
        } catch (std::invalid_argument err) {
            Utils::handle_error("", err.what() + std::string(". Try again"));
            if (!loop_question) {
                return false;
            }
        }
    }
}

bool MacrosExecutor::ask_exec_on_dirs(bool loop_question) {
    std::string answer;

    while (true) {
        std::cout << dye::yellow("(WARNING) ") << "Should the macros be executed in folders that contains both folders and files? (y/n): \t";
        getline(std::cin >> std::ws, answer);

        if (answer == "y" || answer == "Y") {
            current_macros->set_exec_on_dirs(true);
        }
        else if (answer == "n" || answer == "N") {
            current_macros->set_exec_on_dirs(false);
        }
        else {
            Utils::handle_error("", "Wrong input. Try again");
            if (!loop_question) {
                return false;
            }
            continue;
        }

        return true;
    }
}

void MacrosExecutor::reset() {
    i_macros = -1;
    do_execute = true;
    current_macros = nullptr;
    initial_dir = "";
}

void MacrosExecutor::exec_recursive(fs::path current_dir) {
    if (Utils::has_folders(current_dir)) {
        for (auto const& dir : fs::directory_iterator{current_dir}) {
            if (dir.is_directory()) {
                exec_recursive(dir);
            }
        }
        if (!current_macros->get_exec_on_dirs()) {
            return;
        }
    }

    if (!do_execute) {
        return;
    }

    if (!current_macros->get_exec_on_files() && Utils::has_files(current_dir)) {
        return;
    }

    exec(current_dir);
}

void MacrosExecutor::exec(fs::path current_dir) {
    Logger& logger = Logger::getInstance();
    char btn;

    std::cout << dye::on_white("\n**********Processing**********") << dye::black_on_black("*\n");
    std::wcout << "Folder: " << current_dir.wstring() << "\n";
    std::cout << "Macros: " << i_macros + 1 << ". " << current_macros->get_name() << "\n\n";

    logger.info("Executing macros (" + std::to_string(i_macros + 1) + ". " + current_macros->get_name() + ") on folder (" + current_dir.string() + ")");

    try {
        Utils::check_folder(current_dir);

        current_macros->set_current_dir(current_dir);
        current_macros->execute();
    } catch (std::invalid_argument err) {
        Utils::handle_error("", err);
    }

    std::cout << "\nPress any key to continue (ESC or 'q' - stop execution) . . .\n";
    btn = getch();

    if ((int)btn == 27 || btn == 113) {
        do_execute = false;
    }
}

void MacrosExecutor::ask_all() {
    while (true) {
        ask_macros();
        if (!current_macros->is_exec_on_dirs_set()) {
            ask_exec_on_dirs();
        }
        if (ask_path(false)) {
            break;
        }
    }
}

void MacrosExecutor::eventLoop() {
    while (true) {
        std::cout << "\n\n\n______________________________Image utilities______________________________\n\n";

        reset();
        ask_all();

        current_macros->pre_execute();
        if (current_macros->get_exec_recursive()) {
            exec_recursive(initial_dir);
        } else {
            exec(initial_dir);
        }
        current_macros->post_execute(!do_execute);
    }
}
