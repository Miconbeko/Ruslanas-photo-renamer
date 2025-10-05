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

	try {
		fs::remove_all(copy_dest);

		Utils::handle_info("DELETE", "", copy_dest, copy_dest, dye::yellow);
		Utils::handle_success();
	} catch (fs::filesystem_error err) {
		Utils::handle_error(err);
	}

	try {
		fs::create_directories(copy_dest);
		Utils::handle_info("CREATE", "", copy_dest, copy_dest, dye::green);
		Utils::handle_success();
	} catch (fs::filesystem_error err) {
		Utils::handle_error(err);
		return;
	}

	for (auto& dir : dirs) {
		try {
            fs::path new_dir = ((fs::path)copy_dest.string()).append(dir.filename().string());

            Utils::handle_info("COPY", "", dir, new_dir, dye::yellow);

            fs::copy(dir, new_dir, fs::copy_options::recursive
									| fs::copy_options::overwrite_existing);
            Utils::handle_success();
        } catch (fs::filesystem_error err) {
           	Utils::handle_error(err);
        } catch (std::runtime_error err) {
            Utils::handle_error(err);
        } catch (std::logic_error err) {
            Utils::handle_error(err);
            break;
        }
	}
}
