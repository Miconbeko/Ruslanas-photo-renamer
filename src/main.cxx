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
#include "macros/13_copy_missing_AB_files/copyMissingABFiles.hxx"
#include "macros/14_correlate_AB_with_old_EC/correlateABWithOldEC.hxx"
#include "macros/15_rename_old_EC_folders_to_new/renameOldECFoldersToNew.hxx"

#include <iostream>
#include <windows.h>

void SetConsoleSettings() {
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
    setlocale(LC_ALL, "");
    _wsetlocale(LC_ALL, L"");
}

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
    CopyMissingABFiles* copyMissingABFiles = new CopyMissingABFiles();
    CorrelateABWithOldEC* correlateABWithOldEC = new CorrelateABWithOldEC();
    RenameOldECFoldersToNew* renameOldECFoldersToNew = new RenameOldECFoldersToNew();

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
    macrosExecutor.add_macros(copyMissingABFiles);
    macrosExecutor.add_macros(correlateABWithOldEC);
    macrosExecutor.add_macros(renameOldECFoldersToNew);

    macrosExecutor.add_separator(3);
    macrosExecutor.add_separator(6);

    macrosExecutor.eventLoop();


    system("pause");

    return 0;
}
