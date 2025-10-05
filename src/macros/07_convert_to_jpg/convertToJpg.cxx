#include "convertToJpg.hxx"

#include "utils/utils.hxx"

#include <set>
#include <string>

bool ConvertToJpg::is_image(fs::path file) {
    std::set<std::string> img_extensions = {".jpeg", ".jfif", ".png"};

    return file.has_extension() && img_extensions.find(file.extension().string()) != img_extensions.end();
}

bool ConvertToJpg::is_done(fs::path entity) const {
    return entity.has_extension() && entity.extension() == ".jpg";
}

void ConvertToJpg::execute() {
    const std::string JPG_POSTFIX = ".jpg";

    std::vector<fs::path> files = Utils::get_files(current_dir);

    for (auto const& file : files) {
        std::string stem = file.stem().string();
        std::string ext = file.extension().string();
        fs::path new_file = ((fs::path)file.string()).replace_extension(JPG_POSTFIX);

        try {
            if (is_done(file) || !is_image(file)) {
                continue;
            }

            Utils::handle_info("CONVERT", initial_dir, file, new_file, dye::light_purple);

            fs::rename(file, new_file);
            Utils::handle_success();
        } catch (fs::filesystem_error err) {
            Utils::handle_error(err);
        } catch (std::runtime_error err) {
            Utils::handle_error(err);
        }
    }
}
