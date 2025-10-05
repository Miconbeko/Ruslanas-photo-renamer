#include "config/config.hxx"
#include "database/database.hxx"
#include "imageinfo/imageinfo.hpp"
#include "macrosExecutor/macrosExecutor.hxx"
#include "macros/01_name_files_by_date/nameFilesByDate.hxx"
#include "macros/02_append_EC_to_file/appendECToFile.hxx"
#include "macros/03_copy_file_set_prefix_with_AB/copyFileSetPrefixWithAB.hxx"
#include "macros/04_group_files_by_EC/groupPhotosByEC.hxx"
#include "macros/05_group_dirs_by_brand/groupDirsByBrand.hxx"
#include "macros/06_group_dirs_by_model/groupDirsByModel.hxx"
#include "macros/07_convert_to_jpg/convertToJpg.hxx"
#include "macros/08_remove_prefixes_in_files/removePrefixesInFiles.hxx"
#include "macros/09_delete_files_with_EC/deleteFilesWithEC.hxx"
#include "macros/10_rename_AB_to_EC_files/renameABToECFiles.hxx"
#include "macros/11_check_EC_dir_file_match/checkECDirFileMatch.hxx"
#include "macros/12_mark_found_EC_and_sized_files/markFoundECAndSizedFiles.hxx"

#include <iostream>
#include <windows.h>

void SetConsoleSettings() {
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
    setlocale(LC_ALL, "");
    _wsetlocale(LC_ALL, L"");
}

// void process_file(fs::path filepath, std::set<std::string>& found_eurocodes, std::set<std::string>& sized_eurocodes) {
//     Database& db = Database::getInstance();

//     std::string filename = filepath.stem().string();
//     int delimiter_pos = filename.find('-');
//     std::string eurocode = filename.substr(0, delimiter_pos);
//     std::string number = filename.substr(delimiter_pos + 1);

//     // std::cout << filename << std::endl;
//     if (number != "1") {
//         return;
//     }
//     if (!db.contains_eurocode(eurocode)) {
//         return;
//     }
//     found_eurocodes.insert(eurocode);

//     std::cout << "Found " << eurocode << std::endl;

//     auto image_info = imageinfo::parse<imageinfo::FilePathReader>(filepath.string());
//     if (image_info.error()) {
//         std::cerr << image_info.error_msg() << std::endl;
//         return;
//     }

//     auto image_size = image_info.size();
//     if (image_size.width != 2400 || image_size.height != 1800) {
//         return;
//     }
//     sized_eurocodes.insert(eurocode);

//     std::cout << "Correct size " << eurocode << std::endl;
// }

// void process_dir(fs::path dirpath, std::set<std::string>& found_eurocodes, std::set<std::string>& sized_eurocodes) {
//     for (auto const& entry : fs::directory_iterator{dirpath}) {
//         if (fs::is_directory(entry)) {
//             process_dir(entry, found_eurocodes, sized_eurocodes);
//         }
//         if (fs::is_regular_file(entry)) {
//             process_file(entry, found_eurocodes, sized_eurocodes);
//         }
//     }
// }

int main() {
    Config& env = Config::init(L"config.txt");
    Database& db =  Database::getInstance();
    MacrosExecutor macrosExecutor;


    NameFilesByDate* nameFilesByDate = new NameFilesByDate();
    AppendECToFile* appendECToFile = new AppendECToFile();
    CopyFileSetPrefixWithAB* copyFileSetPrefixWithAB = new CopyFileSetPrefixWithAB();
    GroupPhotosByEC* groupPhotosByEC = new GroupPhotosByEC();
    GroupDirsByBrand* groupDirsByBrand = new GroupDirsByBrand();
    GroupDirsByModel* groupDirsByModel = new GroupDirsByModel();
    ConvertToJpg* convertToJpg = new ConvertToJpg();
    RemovePrefixesInFiles* removePrefixesInFiles = new RemovePrefixesInFiles();
    DeleteFilesWithEC* deleteFilesWithEC = new DeleteFilesWithEC();
    RenameABToECFiles* renameABToECFiles = new RenameABToECFiles();
    CheckECDirFileMatch* checkECDirFileMatch = new CheckECDirFileMatch();
    MarkFoundECAndSizedFiles* markFoundECAndSizedFiles = new MarkFoundECAndSizedFiles();

    SetConsoleSettings();

    macrosExecutor.add_macros(nameFilesByDate);
    macrosExecutor.add_macros(appendECToFile);
    macrosExecutor.add_macros(copyFileSetPrefixWithAB);
    macrosExecutor.add_macros(groupPhotosByEC);
    macrosExecutor.add_macros(groupDirsByBrand);
    macrosExecutor.add_macros(groupDirsByModel);
    macrosExecutor.add_macros(convertToJpg);
    macrosExecutor.add_macros(removePrefixesInFiles);
    macrosExecutor.add_macros(deleteFilesWithEC);
    macrosExecutor.add_macros(renameABToECFiles);
    macrosExecutor.add_macros(checkECDirFileMatch);
    macrosExecutor.add_macros(markFoundECAndSizedFiles);

    macrosExecutor.add_separator(3);
    macrosExecutor.add_separator(6);

    macrosExecutor.eventLoop();


    system("pause");

    return 0;
}
