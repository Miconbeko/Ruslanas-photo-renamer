#include "utils.hxx"

#include "config/config.hxx"

Utils* Utils::utils = nullptr;

Utils::Utils() {
    Config& config = Config::getInstance();

    legit_filenames = split_filename(config.get("LEGIT_FILENAMES"), ',');
}

Utils& Utils::getInstance() {
    if (utils == nullptr) {
        utils = new Utils();
    }

    return *utils;
}

bool Utils::is_legit(fs::path file, bool do_remove_prefix) {
    if (do_remove_prefix)
        file = remove_prefix(file);

    std::string filename = file.stem().string();

    return find(legit_filenames.begin(), legit_filenames.end(), filename) != legit_filenames.end();
}

std::vector<fs::path> Utils::get_files(fs::path dir) {
    std::vector<fs::path> files;

    for (auto const& file : fs::directory_iterator{dir})
        files.push_back(file);

    return files;
};

void Utils::check_folder(fs::path dir) {
    if (!fs::is_directory(dir)) {
        throw std::invalid_argument("Invalid folder path");
    }
    if (fs::is_empty(dir)) {
        throw std::invalid_argument("This folder is empty");
    }
}

bool Utils::has_folders(fs::path dir) {
    for (auto const& dir : fs::directory_iterator{dir})
        if (fs::is_directory(dir))
            return true;
    return false;
}

bool Utils::has_files(fs::path dir) {
    for (auto const& dir : fs::directory_iterator{dir})
        if (!fs::is_directory(dir))
            return true;
    return false;
}

void Utils::handle_error(std::string info_msg, std::string err, std::string err_type, dye::R<std::string> (*func)(std::string)) {
    Logger& logger = Logger::getInstance();

    std::cout << func(err_type) << dye::black_on_black("*") << "\n";
    std::cerr << dye::light_red(err) << "\n\n";

    logger.error(info_msg + err);
}

void Utils::handle_error(std::string info_msg, std::exception& err, std::string err_type, dye::R<std::string> (*func)(std::string)) {
    handle_error(info_msg, err.what(), err_type, func);
}

std::string Utils::handle_info(std::string info, fs::path initial_dir, fs::path old_entity, fs::path new_entity, dye::R<std::string> (*func)(std::string)) {
    int initial_dir_len = initial_dir.string().length();

    std::cout << func(info) << dye::light_yellow(" | ")  << old_entity.string().substr(initial_dir_len) << dye::light_yellow(" -> ") << new_entity.string().substr(initial_dir_len) << dye::light_yellow(" | ");

    return info + " | " + old_entity.string().substr(initial_dir_len) + " -> " + new_entity.string().substr(initial_dir_len) + " | ";
}

void Utils::handle_success(std::string info_msg) {
    std::cout << dye::on_green("SUCCESS") << dye::black_on_black("*") << "\n";
}

fs::path Utils::append_prefix(fs::path file, std::string prefix, char separator) {
    return file.replace_filename(prefix + separator + file.filename().string());
}

fs::path Utils::remove_prefix(fs::path file, std::string* prefix, char separator) {
    std::string fileName = file.filename().string();
    int separatorIndex = fileName.find(separator) + 1;

    if (separatorIndex == std::string::npos || separatorIndex == 0)
        return file;

    if (prefix != NULL)
        *prefix = fileName.substr(0, separatorIndex - 1);

    return file.replace_filename(fileName.substr(separatorIndex));
}

std::vector<std::string> Utils::split_filename(fs::path file, char separator) {
    std::vector<std::string> words;
    std::string filename = file.stem().string();
    int separatorIndex;

    while ((separatorIndex = filename.find(separator)) != std::string::npos) {
        words.push_back(filename.substr(0, separatorIndex));
        filename = filename.substr(separatorIndex + 1);
    }
    words.push_back(filename);

    return words;
}
