#include "markFoundECAndSizedFiles.hxx"

#include "utils/utils.hxx"
#include "imageinfo/imageinfo.hpp"

bool MarkFoundECAndSizedFiles::is_done(fs::path entity) const {
    throw std::logic_error("Is not implemented"); //TODO: create separate exception for this
}

void MarkFoundECAndSizedFiles::execute() {
    std::string info_msg;
    std::vector<fs::path> files = Utils::get_files(current_dir);

    for (auto const& file : files) {
        std::string prefix;
        std::string number = Utils::remove_prefix(file, &prefix).stem().string();

        try {
            if (number != "1") {
                return;
            }

            info_msg = Utils::handle_info("CHECK", initial_dir, file, file, dye::light_purple);

            if (!db.contains_eurocode(prefix)) {
                throw std::runtime_error("Prefix (" + prefix + ") is not an EC");
            }

            found_eurocodes.insert(prefix);
            auto image_info = imageinfo::parse<imageinfo::FilePathReader>(file.string());

            if (image_info.error()) {
                throw std::runtime_error("Cannot parse image info: " + std::string(image_info.error_msg()));
            }

            auto image_size = image_info.size();

            if (image_size.width != 2400 || image_size.height != 1800) {
                throw std::runtime_error("Wrong image size");
            }

            sized_eurocodes.insert(prefix);
            Utils::handle_success(info_msg);
        } catch (std::runtime_error err) {
            Utils::handle_error(info_msg, err);
        }
    }
}

void MarkFoundECAndSizedFiles::pre_execute() {
    found_eurocodes.clear();
    sized_eurocodes.clear();
}

void MarkFoundECAndSizedFiles::post_execute(bool is_force_stop) {
    if (is_force_stop) {
        std::cout << "\nThe execution is stopped forcefully. Result is not written to Database\n";
        return;
    }

    std::cout << "\nWriting result to Database. Please wait\n";

    db.mark_eurocodes(std::vector<std::string>(found_eurocodes.begin(), found_eurocodes.end()),
                std::vector<std::string>(sized_eurocodes.begin(), sized_eurocodes.end()));
}
