#include "copyTests.hxx"

#include "utils/utils.hxx"
#include "config/config.hxx"

#include <vector>

CopyTests::CopyTests(std::string current_dir, std::string initial_dir) : Macros::Macros(current_dir, initial_dir, false, true, false) {
	copy_dest = this->config.get_ws("TESTS_PATH");
}

bool CopyTests::is_done(fs::path entity) const {
  	throw std::logic_error("Is not implemented"); //TODO: create separate exception for this
}

void CopyTests::execute() {
	std::vector<fs::path> dirs = Utils::get_files(current_dir);
    std::string info_msg;

	try {
		fs::remove_all(copy_dest);

		info_msg = Utils::handle_info("DELETE", "", copy_dest, copy_dest, dye::yellow);
		Utils::handle_success(info_msg);
	} catch (fs::filesystem_error err) {
		Utils::handle_error(info_msg, err);
	}

	try {
		fs::create_directories(copy_dest);
		info_msg = Utils::handle_info("CREATE", "", copy_dest, copy_dest, dye::green);
		Utils::handle_success(info_msg);
	} catch (fs::filesystem_error err) {
		Utils::handle_error(info_msg, err);
		return;
	}

	for (auto& dir : dirs) {
		try {
            fs::path new_dir = ((fs::path)copy_dest.string()).append(dir.filename().string());

            info_msg = Utils::handle_info("COPY", "", dir, new_dir, dye::yellow);

            fs::copy(dir, new_dir, fs::copy_options::recursive
									| fs::copy_options::overwrite_existing);
            Utils::handle_success(info_msg);
        } catch (fs::filesystem_error err) {
           	Utils::handle_error(info_msg, err);
        } catch (std::runtime_error err) {
            Utils::handle_error(info_msg, err);
        } catch (std::logic_error err) {
            Utils::handle_error(info_msg, err);
            break;
        }
	}
}
