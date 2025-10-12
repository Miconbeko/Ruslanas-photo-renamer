#include "copyMissingABFiles.hxx"

#include "utils/utils.hxx"

bool CopyMissingABFiles::is_done(fs::path entity) const {
    return fs::exists(entity);
}

void CopyMissingABFiles::execute() {
    std::string info_msg;
    std::vector<fs::path> files = Utils::get_files(current_dir);

    for (auto const& file : files) {
        std::string prefix = "";
        std::string id;
        std::string filename = Utils::remove_prefix(file, &prefix).stem().string();

        id = prefix == "" ? filename : prefix;

        try {
            fs::path new_file = ((fs::path)dest_dir).append(file.filename().string());

            if (!db.contains_id(id)) {
                info_msg = Utils::handle_info("COPY", "", file, new_file, dye::yellow);
                throw std::runtime_error("The string (" + id + ") is not an AB");
            }

            if (!db.contains_missing_id(id)) {
                return;
            }

            if (is_done(new_file)) {
                info_msg = Utils::handle_info("COPY", "", file, new_file, dye::yellow);
                throw std::runtime_error("File is already copied into destination folder");
            }

            info_msg = Utils::handle_info("COPY", "", file, new_file, dye::yellow);

            fs::copy(file, new_file);
            Utils::handle_success(info_msg);
        } catch (std::runtime_error err) {
            Utils::handle_error(info_msg, err);
        }
    }
}

void CopyMissingABFiles::pre_execute() {
    std::wstring dir_wstr;

    while (true) {
        std::cout << "Enter copy destination folder path: \t";
        getline(std::wcin >> std::ws, dir_wstr);
        dest_dir = dir_wstr;

        try {
            if (!fs::is_directory(dest_dir)) {
                throw std::invalid_argument("Invalid folder path");
            }

            return;
        } catch (std::invalid_argument err) {
            Utils::handle_error("", err.what() + std::string(". Try again"));
        }
    }
}
