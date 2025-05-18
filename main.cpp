#include <iostream>
#include <conio.h>
#include <filesystem>
#include <string>
#include <stdio.h>
#include <windows.h>
#include <vector>
#include <map>
#include <utility>
#include <numeric>
#include <algorithm>
#include <fstream>
#include <locale>
#include <set>
#include <windows.h>
#include <OpenXLSX.hpp>
#include "include/color/color.hpp"

using namespace std;
namespace fs = std::filesystem;
namespace xl = OpenXLSX;


class Database {
    private:
        string EUROCODES_COL_INDEX;
        string IDENTIFIERS_COL_INDEX;
        string MODELS_COL_INDEX;
        const int EMPTY_CELLS_LIMIT = 1000;
        fs::path filePath;
        wstring sheetName;
        multimap<string, string> euroToId;
        map<string, string> euroToModel;
        map<string, string> idToEuro;
        set<string> ids;
        set<string> models;
        set<string> eurocodes;

        void load_db();
    public:
        Database(fs::path filePath, wstring sheetName, string euroCol, string idCol, string modelCol);
        vector<string> get_ids(string eurocode);
        string get_model(string eurocode);
        string get_eurocode(string id);
        bool contains_eurocode(string eurocode);
        bool contains_id(string id);
        bool contains_model(string model);
};

class File {
    private:
        vector<string> legit_filenames;
    public:
        File(map<wstring, wstring> *env);   // FIXME: use singleton
        bool is_legit(fs::path file, bool do_remove_prefix = true);
        vector<string> get_legit_filenames();

        static fs::path append_prefix(fs::path file, string prefix, char separator = '-');
        static fs::path remove_prefix(fs::path file, string* prefix = NULL, char separator = '-');
        static vector<string> split_filename(fs::path file, char separator = '-');
};

class Macros {
    protected:
        fs::path dir;
        fs::path input_dir;
        Database* db;
        map<wstring, wstring>* env;

        vector<fs::path> get_files();
        void handle_error(exception& err, string warning_str = "ERROR", dye::R<string> (*func)(string) = dye::on_red);
        void handle_success();
    public:
        Macros(fs::path dir, fs::path input_dir, Database* db, map<wstring, wstring>* env);
        virtual bool is_done(fs::path file, char separator = '-') = 0;
        virtual void execute() = 0;
};

class AppendDirAsPrefixMacros : private Macros {
    public:
        AppendDirAsPrefixMacros(fs::path dir, fs::path input_dir, Database* db, map<wstring, wstring>* env) : Macros::Macros(dir, input_dir, db, env) {}
        virtual bool is_done(fs::path file, char separator = '-');
        virtual void execute();
};

class CopyFilesAndSetPrefixAsIdMacros : private Macros {
    public:
        CopyFilesAndSetPrefixAsIdMacros(fs::path dir, fs::path input_dir, Database* db, map<wstring, wstring>* env) : Macros::Macros(dir, input_dir, db, env) {}
        virtual bool is_done(fs::path file, char separator = '-');
        virtual void execute();
};

class DeleteFilesWithEurocodeMacros : private Macros {
    private:
        set<string> get_filenames(vector<fs::path> files);
        bool is_ready(fs::path file, set<string> filenames);
    public:
        DeleteFilesWithEurocodeMacros(fs::path dir, fs::path input_dir, Database* db, map<wstring, wstring>* env) : Macros(dir, input_dir, db, env) {}
        virtual bool is_done(fs::path file, char separator = '-');
        virtual void execute();
};

class RemoveIdsFromFolderMacros : private Macros {
    public:
        RemoveIdsFromFolderMacros(fs::path dir, fs::path input_dir, Database* db, map<wstring, wstring>* env) : Macros(dir, input_dir, db, env) {}
        virtual bool is_done(fs::path dir, char separator = ' ');
        virtual void execute();
};

class ConvertToJpgMacros : private Macros {
    private:
        bool is_image(fs::path file);
    public:
        ConvertToJpgMacros(fs::path dir, fs::path input_dir, Database* db, map<wstring, wstring>* env) : Macros(dir, input_dir, db, env) {}
        virtual bool is_done(fs::path dir, char separator = ' ');
        virtual void execute();
};

class NameFilesByDateMacros : private Macros {
    private:
        void sort_files_by_update_time(vector<fs::path>& files);
    public:
        NameFilesByDateMacros(fs::path dir, fs::path input_dir, Database* db, map<wstring, wstring>* env) : Macros(dir, input_dir, db, env) {}
        virtual bool is_done(fs::path dir, char separator = ' ');
        virtual void execute();
};

class GroupPhotosByEurocode : private Macros {
    public:
        GroupPhotosByEurocode(fs::path dir, fs::path input_dir, Database* db, map<wstring, wstring>* env) : Macros(dir, input_dir, db, env) {}
        virtual bool is_done(fs::path dir, char separator = ' ');
        virtual void execute();
};

class GroupFoldersByModel : private Macros {
    public:
        GroupFoldersByModel(fs::path dir, fs::path input_dir, Database* db, map<wstring, wstring>* env) : Macros(dir, input_dir, db, env) {}
        virtual bool is_done(fs::path dir, char separator = ' ');
        virtual void execute();
};

class RenameIdsToEurocodeFiles : private Macros {
    public:
        RenameIdsToEurocodeFiles(fs::path dir, fs::path input_dir, Database* db, map<wstring, wstring>* env) : Macros(dir, input_dir, db, env) {}
        virtual bool is_done(fs::path file, char separator = '-');
        virtual void execute();
};

class MacrosExecutor {
    private:
        int macros_number;
        fs::path input_dir;
        bool allow_folders;
        bool allow_files;
        Database* db;
        map<wstring, wstring>* env;

        void check_folder(fs::path dir);
        bool has_folders(fs::path dir);
        bool has_files(fs::path dir);
        void handle_error(exception& err, string warning_str = "FOLDER ERROR", dye::R<string> (*func)(string) = dye::on_red);

        void exec_recursive(fs::path current_dir, char& btn);
        void exec(fs::path current_dir, char& btn);
    public:
        MacrosExecutor(Database* db, map<wstring, wstring>* env);
        bool ask_macros(bool until_correct = true);
        bool ask_path(bool until_correct = true);
        bool ask_allow_folders(bool until_correct = true);
        bool ask_all();

        void exec_recursive(char& btn);
        void exec(char& btn);
};



Database::Database(fs::path filePath, wstring sheetName, string euroCol, string idCol, string modelCol) {
    this->filePath = filePath;
    this->sheetName = sheetName;
    this->EUROCODES_COL_INDEX = euroCol;
    this->IDENTIFIERS_COL_INDEX = idCol;
    this->MODELS_COL_INDEX = modelCol;

    load_db();
}

void Database::load_db() {
    xl::XLDocument doc;
    string sheetNameShort = string(sheetName.begin(), sheetName.end());
    int emptyCellCount = 0;

    doc.open(filePath.string());
    xl::XLWorksheet sheet = doc.workbook().worksheet(sheetNameShort);

    xl::XLCellRange eurocodeRange = sheet.range(xl::XLCellReference(2, EUROCODES_COL_INDEX), xl::XLCellReference(xl::MAX_ROWS, EUROCODES_COL_INDEX));
    xl::XLCellRange idRange = sheet.range(xl::XLCellReference(2, IDENTIFIERS_COL_INDEX), xl::XLCellReference(xl::MAX_ROWS, IDENTIFIERS_COL_INDEX));
    xl::XLCellRange ModelRange = sheet.range(xl::XLCellReference(2, MODELS_COL_INDEX), xl::XLCellReference(xl::MAX_ROWS, MODELS_COL_INDEX));

    cout << "Start loading data from excel file . . ." << endl;
    for (xl::XLCellIterator eurocode = eurocodeRange.begin(),
                            id = idRange.begin(),
                            model = ModelRange.begin();
                            eurocode != eurocodeRange.end();
                            eurocode++, id++, model++) {
        if (eurocode->value() == "") {
            emptyCellCount++;
            // TODO: maybe delete empty record
        }
        else
            emptyCellCount = 0;

        if (emptyCellCount == EMPTY_CELLS_LIMIT)
            break;

        // cout << eurocode->value().getString() << " " << id->value().getString() << endl;
        euroToId.insert(make_pair<string, string>(eurocode->value().getString(), id->value().getString()));
        euroToModel.insert(make_pair<string, string>(eurocode->value().getString(), model->value().getString()));
        idToEuro.insert(make_pair<string, string>(id->value().getString(), eurocode->value().getString()));
        eurocodes.insert(eurocode->value().getString());
        ids.insert(id->value().getString());
        models.insert(model->value().getString());
    }

    cout << "Number of pairs <Eurocode, AB Nomenclature>: " << euroToId.size() << endl;
    cout << "Number of pairs <Eurocode, Model>: " << euroToModel.size() << endl;
    cout << "Number of loaded Eurocodes at column (" << EUROCODES_COL_INDEX << "): " << eurocodes.size() << endl;
    cout << "Number of loaded AB Nomenclatures at column (" << IDENTIFIERS_COL_INDEX << "): " << ids.size() << endl;
    cout << "Number of loaded Models at column (" << MODELS_COL_INDEX << "): " << models.size() << endl;
}

vector<string> Database::get_ids(string eurocode) {
    auto range = euroToId.equal_range(eurocode);
    vector<string> res;

    for (auto it = range.first; it != range.second; ++it) {
        if (it->second == "")
            continue;

        res.push_back(it->second);
    }

    return res;
}

string Database::get_model(string eurocode) {
    try {
        return euroToModel.at(eurocode);
    } catch(...) {}

    return "";
}

string Database::get_eurocode(string id) {
    try {
        return idToEuro.at(id);
    } catch(...) {}

    return "";
}

bool Database::contains_eurocode(string eurocode) {
    return euroToId.find(eurocode) != euroToId.end();
}

bool Database::contains_id(string id) {
    if (id == "")
        return false;

    return ids.find(id) != ids.end();
}

bool Database::contains_model(string model) {
    if (model == "")
        return false;

    return models.find(model) != models.end();
}



File::File(map<wstring, wstring> *env) {
    this->legit_filenames = split_filename(env->at(L"LEGIT_FILENAMES"), ',');
}

fs::path File::append_prefix(fs::path file, string prefix, char separator) {
    return ((fs::path)file.string()).replace_filename(prefix + separator + file.filename().string());
}

fs::path File::remove_prefix(fs::path file, string* prefix, char separator) {
    string fileName = file.filename().string();
    int separatorIndex = fileName.find(separator) + 1;

    if (separatorIndex < 0)
        return file;

    if (prefix != NULL)
        *prefix = fileName.substr(0, separatorIndex - 1);

    return ((fs::path)file.string()).replace_filename(fileName.substr(separatorIndex));
}

vector<string> File::get_legit_filenames() {
    return legit_filenames;
}

bool File::is_legit(fs::path file, bool do_remove_prefix) {
    if (do_remove_prefix)
        file = remove_prefix(file);

    string fileName = ((fs::path)file.filename().string()).replace_extension("").string();

    return find(legit_filenames.begin(), legit_filenames.end(), fileName) != legit_filenames.end();
}

vector<string> File::split_filename(fs::path file, char separator) {
    vector<string> words;
    string filename = file.filename().string();
    int separatorIndex;

    while ((separatorIndex = filename.find(separator)) != string::npos) {
        words.push_back(filename.substr(0, separatorIndex));
        filename = filename.substr(separatorIndex + 1);
    }
    words.push_back(filename);

    return words;
}



Macros::Macros(fs::path dir, fs::path input_dir, Database* db, map<wstring, wstring>* env) {
    this->dir = dir;
    this->input_dir = input_dir;
    this->db = db;
    this->env = env;
}

vector<fs::path> Macros::get_files() {
    vector<fs::path> files;

    for (auto const& file : fs::directory_iterator{dir})
        files.push_back(file);

    return files;
};

void Macros::handle_error(exception& err, string warning_str, dye::R<string> (*func)(string)) {
    cout << func(warning_str) << dye::black_on_black("*") << endl;
    cerr << dye::light_red(err.what()) << endl << endl;
    // system("pause");
}

void Macros::handle_success() {
    cout << dye::on_green("SUCCESS") << dye::black_on_black("*") << endl;
}



bool AppendDirAsPrefixMacros::is_done(fs::path file, char separator) {
    string file_name = file.filename().string();
    string dirName = dir.filename().string();

    if (file_name.find(separator) != string::npos && file_name.substr(0, file_name.find(separator)) == dirName)
        return true;
    return false;
}

void AppendDirAsPrefixMacros::execute() {
    vector<fs::path> files = get_files();
    string dir_name = dir.filename().string();
    int input_dir_len = input_dir.string().length();

    CopyFilesAndSetPrefixAsIdMacros copy_files_macros(dir, input_dir, db, env);
    File file_utils(env);

    for (auto const& file : files) {
        fs::path new_file = File::append_prefix(file, dir_name);

        try {
            if (AppendDirAsPrefixMacros::is_done(file))
                continue;
                // throw runtime_error("This file is already have prefix in its name");
            if (copy_files_macros.is_done(file))
                continue;
                // throw runtime_error("This file was processed with 2-nd macros");

            cout << dye::yellow("RENAME") << dye::light_yellow(" | ")  << file.string().substr(input_dir_len) << dye::light_yellow(" -> ") << new_file.string().substr(input_dir_len) << dye::light_yellow(" | ");

            if (!file_utils.is_legit(file, false))
                throw logic_error("This file has wrong name");
            if (!db->contains_eurocode(dir_name))
                throw logic_error("This Eurocode(`" + dir_name + "`) doesn't exist");

            fs::rename(file, new_file);
            handle_success();
        } catch (fs::filesystem_error err) {
            handle_error(err);
        } catch (runtime_error err) {
            handle_error(err);
        } catch (logic_error err) {
            handle_error(err, "NAME ERROR", dye::on_yellow);
        }
    }
}



bool CopyFilesAndSetPrefixAsIdMacros::is_done(fs::path file, char separator) {
    string fileName = file.filename().string();
    string prefix = fileName.substr(0, fileName.find(separator));

    return db->contains_id(prefix);
}

void CopyFilesAndSetPrefixAsIdMacros::execute() {
    vector<fs::path> files = get_files();
    string dir_name = dir.filename().string();
    vector<string> ids = db->get_ids(dir_name);
    int input_dir_len = input_dir.string().length();

    AppendDirAsPrefixMacros append_dir_macros(dir, input_dir, db, env);

    try {
        if (ids.size() == 0)
            throw logic_error("This Eurocode (" + dir_name + ") is invalid or has no identifiers");
    } catch (logic_error err) {
        handle_error(err, "NAME ERROR", dye::on_yellow);
    }


    for (auto const& file : files) {
        if (is_done(file))
            continue;

        for (const string& id : ids) {
            fs::path new_file = File::remove_prefix(file);
            new_file = File::append_prefix(new_file, id);

            try {
                cout << dye::aqua("COPY") << dye::light_yellow(" | ") << file.string().substr(input_dir_len) << dye::light_yellow(" -> ") << new_file.string().substr(input_dir_len) << dye::light_yellow(" | ");

                if (!append_dir_macros.is_done(file))
                    throw runtime_error("This file doesn't have Eurocode as prefix");

                fs::copy(file, new_file);
                handle_success();
            } catch (fs::filesystem_error err) {
                handle_error(err);
            } catch (runtime_error err) {
                handle_error(err);
            }
        }
    }
}



set<string> DeleteFilesWithEurocodeMacros::get_filenames(vector<fs::path> files) {
    set<string> filenames;

    for (const auto& file : files)
        filenames.insert(file.filename().string());

    return filenames;
}

bool DeleteFilesWithEurocodeMacros::is_ready(fs::path file, set<string> filenames) {
    string prefix;
    fs::path file_no_prefix = File::remove_prefix(file, &prefix);
    vector<string> ids = db->get_ids(prefix);

    for (auto const& id : ids) {
        string filename_to_find = File::append_prefix(file_no_prefix, id).filename().string();

        if (filenames.count(filename_to_find) == 0)
            return false;
    }

    return true;
}

bool DeleteFilesWithEurocodeMacros::is_done(fs::path file, char separator) {
    string fileName = file.filename().string();
    string prefix = fileName.substr(0, fileName.find(separator));

    return !db->contains_id(prefix);
}

void DeleteFilesWithEurocodeMacros::execute() {
    vector<fs::path> files = get_files();
    set<string> filenames = get_filenames(files);
    string dir_name = dir.filename().string();
    int input_dir_len = input_dir.string().length();

    for (const auto& file : files) {
        string prefix;
        File::remove_prefix(file, &prefix);

        if (!db->contains_eurocode(prefix))
            continue;

        try {
            cout << dye::blue("DELETE") << dye::light_yellow(" | ") << file.string().substr(input_dir_len) << dye::light_yellow(" | ") ;

            if (!is_ready(file, filenames))
                throw runtime_error("This file has no copies or the error while copying was occurred. Stop deletion");

            fs::remove(file);
            handle_success();
        } catch (fs::filesystem_error err) {
            handle_error(err);
        } catch (runtime_error err) {
            handle_error(err);
        }
    }
}



bool RemoveIdsFromFolderMacros::is_done(fs::path dir, char separator) {
    vector<string> words = File::split_filename(dir, separator);

    if (words.size() == 1)
        return true;
    return false;
}

void RemoveIdsFromFolderMacros::execute() {
    string dir_name = dir.filename().string();
    int input_dir_len = input_dir.string().length();
    fs::path newDir;
    string eurocode;

    File::remove_prefix(dir, &eurocode, ' ');
    newDir = ((fs::path)dir.string()).replace_filename(eurocode);

    try {
        if (is_done(dir))
            return;

        cout << dye::yellow("RENAME FOLDER") << dye::light_yellow(" | ") << dir.string().substr(input_dir_len) << dye::light_yellow(" -> ") << newDir.string().substr(input_dir_len) << dye::light_yellow(" | ") ;

        if (!db->contains_eurocode(eurocode))
            throw logic_error("First word in folder's name (" + eurocode + ") isn't valid eurocode");

        fs::rename(dir, newDir);
        handle_success();
    } catch (fs::filesystem_error err) {
        handle_error(err);
    } catch (logic_error err) {
        handle_error(err, "NAME ERROR", dye::on_yellow);
    }
}



bool ConvertToJpgMacros::is_done(fs::path dir, char separator) {
    return dir.has_extension() && dir.extension() == "jpg";
}

bool ConvertToJpgMacros::is_image(fs::path file) {
    set<string> imgExtensions = {".jpeg", ".jfif", ".png"};

    if (file.has_extension() && imgExtensions.find(file.extension().string()) != imgExtensions.end())
        return true;
    return false;
}

void ConvertToJpgMacros::execute() {
    const string JPG_POSTFIX = "jpg";
    const char SEPARATOR = '.';

    vector<fs::path> files = get_files();
    string dir_name = dir.filename().string();
    int input_dir_len = input_dir.string().length();

    for (auto const& file : files) {
        vector<string> words = File::split_filename(file, SEPARATOR); // TODO: use .extension()
        fs::path newFile = ((fs::path)file.string()).replace_filename(words[0] + SEPARATOR + JPG_POSTFIX);

        try {
            if (words[1] == JPG_POSTFIX)
                continue;
            if (!is_image(file))
                continue;

            cout << dye::light_purple("CONVERT") << dye::light_yellow(" | ") << file.string().substr(input_dir_len) << dye::light_yellow(" -> ") << newFile.string().substr(input_dir_len) << dye::light_yellow(" | ") ;

            fs::rename(file, newFile);
            handle_success();
        } catch (fs::filesystem_error err) {
            handle_error(err);
        } catch (runtime_error err) {
            handle_error(err);
        }
    }
}



void NameFilesByDateMacros::sort_files_by_update_time(vector<fs::path>& files) {
    sort(files.begin(), files.end(), [] (fs::path& e1, fs::path& e2) { return fs::last_write_time(e1) < fs::last_write_time(e2); });
}

bool NameFilesByDateMacros::is_done(fs::path dir, char separator) {
    return true; //TODO: create logic for NameFilesByDateMacros is_done
}

void NameFilesByDateMacros::execute() {
    vector<fs::path> files = get_files();
    vector<string> legit_filenames;
    string dir_name = dir.filename().string();
    int input_dir_len = input_dir.string().length();
    int file_count = 0;

    File file_utils(env);

    sort_files_by_update_time(files);
    legit_filenames = file_utils.get_legit_filenames();

    for (auto const& file : files) {
        try {
            if (file_count >= legit_filenames.size())
                throw logic_error("Not enough legit filenames");

            fs::path new_file = ((fs::path)file.string()).replace_filename(legit_filenames[file_count] + ".jpg");
            file_count++;

            if (file == new_file)
                continue;

            cout << dye::yellow("RENAME") << dye::light_yellow(" | ")  << file.string().substr(input_dir_len) << dye::light_yellow(" -> ") << new_file.string().substr(input_dir_len) << dye::light_yellow(" | ");

            if (fs::exists(new_file))
                throw runtime_error("File with such new name already exists");

            fs::rename(file, new_file);
            handle_success();
        } catch (fs::filesystem_error err) {
            handle_error(err);
        } catch (runtime_error err) {
            handle_error(err);
        } catch (logic_error err) {
            handle_error(err);
            break;
        }
    }
}



bool GroupPhotosByEurocode::is_done(fs::path dir, char separator) {
    string eurocode;
    File::remove_prefix(dir, &eurocode);

    if (db->contains_id(eurocode)) {
        eurocode = db->get_eurocode(eurocode);
    }

    return db->contains_eurocode(eurocode) && dir.parent_path().filename() == eurocode;
}

void GroupPhotosByEurocode::execute() {
    vector<fs::path> files = get_files();
    string dir_name = dir.filename().string();
    int input_dir_len = input_dir.string().length();

    for (auto const& file : files) {
        string eurocode;
        File::remove_prefix(file, &eurocode);

        if (db->contains_id(eurocode)) {
            eurocode = db->get_eurocode(eurocode);
        }

        fs::path new_dir = file.parent_path() / eurocode;
        fs::path new_file = (fs::path)new_dir.string() / file.filename();

        try {
            if (fs::is_directory(file))
                continue;
            if (is_done(file))
                continue;
                // throw runtime_error("This file is already where it should be");

            cout << dye::light_purple("GROUP") << dye::light_yellow(" | ") << file.string().substr(input_dir_len) << dye::light_yellow(" -> ") << new_file.string().substr(input_dir_len) << dye::light_yellow(" | ") ;

            if (!db->contains_eurocode(eurocode))
                throw logic_error("This Eurocode(`" + eurocode + "`) doesn't exist");
            if (fs::exists(new_file))
                throw runtime_error("This file already exists in the Eurocode folder");

            fs::create_directory(new_dir);
            fs::rename(file, new_file);

            handle_success();
        } catch (fs::filesystem_error err) {
            handle_error(err);
        } catch (runtime_error err) {
            handle_error(err);
        } catch (logic_error err) {
            handle_error(err, "NAME ERROR", dye::on_yellow);
        }
    }
}



bool GroupFoldersByModel::is_done(fs::path dir, char separator) {
    string eurocode = dir.filename().string();
    string model = db->get_model(eurocode);

    return db->contains_eurocode(eurocode) && dir.parent_path().filename() == model;
}

void GroupFoldersByModel::execute() {
    vector<fs::path> files = get_files();
    string dir_name = dir.filename().string();
    int input_dir_len = input_dir.string().length();

    for (auto const& file : files) {
        string eurocode = file.filename().string();
        string model = db->get_model(eurocode);
        fs::path new_dir = file.parent_path() / model;
        fs::path new_file = (fs::path)new_dir.string() / file.filename();

        try {
            if (!fs::is_directory(file))
                continue;
            if (is_done(file))
                continue;
                // throw runtime_error("This Eurocode folder is already where it should be");

            cout << dye::light_purple("GROUP") << dye::light_yellow(" | ") << file.string().substr(input_dir_len) << dye::light_yellow(" -> ") << new_file.string().substr(input_dir_len) << dye::light_yellow(" | ") ;

            if (!db->contains_eurocode(eurocode))
                throw logic_error("This Eurocode(`" + eurocode + "`) doesn't exist");
            if (fs::exists(new_file))
                throw runtime_error("This Eurocode folder already exists in the Model folder");

            fs::create_directory(new_dir);
            fs::rename(file, new_file);

            handle_success();
        } catch (fs::filesystem_error err) {
            handle_error(err);
        } catch (runtime_error err) {
            handle_error(err);
        } catch (logic_error err) {
            handle_error(err, "NAME ERROR", dye::on_yellow);
        }
    }
}


bool RenameIdsToEurocodeFiles::is_done(fs::path file, char separator) {
    string fileName = file.stem().string();

    return db->contains_eurocode(fileName);
}

void RenameIdsToEurocodeFiles::execute() {
    vector<fs::path> files = get_files();
    string dir_name = dir.filename().string();
    int input_dir_len = input_dir.string().length();

    for (auto const& file : files) {
        string file_name = file.stem().string();
        fs::path new_file;
        string new_file_name;

        try {
            if (RenameIdsToEurocodeFiles::is_done(file))
                continue;

            new_file_name = db->get_eurocode(file_name) + ".jpg";
            new_file = ((fs::path)file.string()).replace_filename(new_file_name);

            cout << dye::yellow("RENAME") << dye::light_yellow(" | ")  << file.string().substr(input_dir_len) << dye::light_yellow(" -> ") << new_file.string().substr(input_dir_len) << dye::light_yellow(" | ");

            if (!db->contains_id(file_name))
                throw runtime_error("This filename is not an AB nomenclatuer");
            if (fs::exists(new_file))
                throw logic_error("File with name (" + new_file_name + ") is already exit");

            fs::rename(file, new_file);
            handle_success();
        } catch (fs::filesystem_error err) {
            handle_error(err);
        } catch (runtime_error err) {
            handle_error(err);
        } catch (logic_error err) {
            handle_error(err, "NAME ERROR", dye::on_yellow);
        }
    }
}


MacrosExecutor::MacrosExecutor(Database* db, map<wstring, wstring>* env) {
    this->db = db;
    this->env = env;
    this->allow_folders = false;
    this->allow_files = true;
    ask_all();
}

bool MacrosExecutor::ask_macros(bool until_correct) {
        string macrosNumStr;

        cout << "Choose macros:" << endl;
        cout << "1. Name files with numerals in order of download" << endl;
        cout << "2. Add Eurocode to files as prefix" << endl;
        cout << "3. Copy files and set their prefixes as AB nomenclature" << endl;
        cout << "4. Delete files that have Eurocode as prefix" << endl;
        cout << "5. Remove AB nomenclature from folder names" << endl;
        cout << "6. Convert image files to .jpg" << endl;
        cout << "7. Group image files by Eurocode" << endl;
        cout << "8. Group Eurocode folders by Model" << endl;
        cout << "9. Rename AB nomenclatures to their Eurocode in files" << endl;

        while (true) {
            cout << "Enter macro's number: \t";
            cin >> macrosNumStr;
            try {
                macros_number = stoi(macrosNumStr);
                if (macros_number < 1 || macros_number > 9)
                    throw runtime_error("");
                else
                    return true;
            } catch (...) {
                cerr << endl << dye::red("Wrong input. Try again") << endl << endl;
                if (!until_correct)
                    return false;
            }
        }
}

bool MacrosExecutor::ask_path(bool until_correct) {
    wstring dirStr;

    while (true) {
        cout << "Enter folder's path: \t";
        getline(wcin >> ws, dirStr);
        input_dir = dirStr;

        try {
            check_folder(input_dir);
            return true;
        } catch (invalid_argument err) {
            cout << endl << dye::red(err.what()) << dye::red(". Try again") << " " << endl << endl;
            if (!until_correct)
                return false;
        }
    }
}

bool MacrosExecutor::ask_allow_folders(bool until_correct) {
    string dirStr;

    while (true) {
        cout << dye::yellow("(WARNING) ") << "Should the macros be executed in folders that contains both folders and files? (y/n): \t";
        getline(cin >> ws, dirStr);

        try {
            if (dirStr == "y" || dirStr == "Y")
                this->allow_folders = true;
            else if (dirStr == "n" || dirStr == "N")
                this->allow_folders = false;
            else
                throw runtime_error("");

            return true;
        } catch (...) {
            cerr << endl << dye::red("Wrong input. Try again") << endl << endl;
            if (!until_correct)
                return false;
        }
    }
}

bool MacrosExecutor::ask_all() {
    while (true) {
        ask_macros(true);
        switch(this->macros_number) {
            case 7:
                this->ask_allow_folders(true);
                break;
            case 8:
                this->allow_folders = true;
                this->allow_files = false;
                break;
        }
        if(ask_path(false))
            return true;
    }
}

void MacrosExecutor::check_folder(fs::path dir) {
    if (!fs::is_directory(dir)) {
        throw invalid_argument("Invalid folder path");
    }
    if (fs::is_empty(dir)) {
        throw invalid_argument("This folder is empty");
    }
}

bool MacrosExecutor::has_folders(fs::path dir) {
    for (auto const& dir : fs::directory_iterator{dir})
        if (fs::is_directory(dir))
            return true;
    return false;
}

bool MacrosExecutor::has_files(fs::path dir) {
    for (auto const& dir : fs::directory_iterator{dir})
        if (!fs::is_directory(dir))
            return true;
    return false;
}

void MacrosExecutor::handle_error(exception& err, string warning_str, dye::R<string> (*func)(string)) {
    cout << func(warning_str) << dye::black_on_black("*") << endl;
    cerr << dye::light_red(err.what()) << endl << endl;
    // system("pause");
}

void MacrosExecutor::exec(fs::path current_dir, char& btn) {
    cout << endl << dye::on_white("**********Processing**********") << dye::black_on_black("*") << endl;
    wcout << "Folder: " << current_dir.wstring() << endl << endl;

    try {
        check_folder(current_dir);

        switch(macros_number) {
        case 1: {
            NameFilesByDateMacros name_files_macros(current_dir, input_dir, db, env);
            name_files_macros.execute();
            break;
        }
        case 2: {
            AppendDirAsPrefixMacros append_dir_macros(current_dir, input_dir, db, env);
            append_dir_macros.execute();
            break;
        }
        case 3: {
            CopyFilesAndSetPrefixAsIdMacros copy_files_macros(current_dir, input_dir, db, env);
            copy_files_macros.execute();
            break;
        }
        case 4: {
            DeleteFilesWithEurocodeMacros delete_files_macros(current_dir, input_dir, db, env);
            delete_files_macros.execute();
            break;
        }
        case 5: {
            RemoveIdsFromFolderMacros remove_ids_macros(current_dir, input_dir, db, env);
            remove_ids_macros.execute();
            break;
        }
        case 6: {
            ConvertToJpgMacros convert_macros(current_dir, input_dir, db, env);
            convert_macros.execute();
            break;
        }
        case 7: {
            GroupPhotosByEurocode group_macros(current_dir, input_dir, db, env);
            group_macros.execute();
            break;
        }
        case 8: {
            GroupFoldersByModel group_macros(current_dir, input_dir, db, env);
            group_macros.execute();
            break;
        }
        case 9: {
            RenameIdsToEurocodeFiles rename_macros(current_dir, input_dir, db, env);
            rename_macros.execute();
            break;
        }
    }
    } catch (invalid_argument err) {
        handle_error(err);
    }

    // system("pause");
    cout << endl << "Press any key to continue (ESC - stop execution) . . ." << endl;
    btn = getch();
}

void MacrosExecutor::exec_recursive(fs::path current_dir, char& btn) {
    if (has_folders(current_dir)) {
        for (auto const& dir : fs::directory_iterator{current_dir}) {
            if (dir.is_directory())
                exec_recursive(dir, btn);
        }
        if (!this->allow_folders)
            return;
    }

    if (btn == 27)
        return;

    if (!allow_files && has_files(current_dir))
        return;

    exec(current_dir, btn);
}

void MacrosExecutor::exec(char& btn) {
    exec(input_dir, btn);
}

void MacrosExecutor::exec_recursive(char& btn) {
    exec_recursive(input_dir, btn);
}



void SetConsoleSettings() {
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
    setlocale(LC_ALL, "");
    _wsetlocale(LC_ALL, L"");
}

wstring s2ws(const string& str)
{
    using convert_typeX = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_typeX, wchar_t> converterX;

    return converterX.from_bytes(str);
}

string ws2s(const wstring& wstr)
{
    using convert_typeX = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_typeX, wchar_t> converterX;

    return converterX.to_bytes(wstr);
}

size_t GetSizeOfFile(const wstring& path)
{
    struct _stat fileinfo;
    _wstat(path.c_str(), &fileinfo);
    return fileinfo.st_size;
}

wstring LoadUtf8FileToString(const wstring& filename)
{
    wstring buffer;            // stores file contents
    FILE* f = _wfopen(filename.c_str(), L"rtS, ccs=UTF-8");

    // Failed to open file
    if (f == NULL)
    {
        cerr << "Failed to open config.txt" << endl;
        return buffer;
    }

    size_t filesize = GetSizeOfFile(filename);

    // Read entire file contents in to memory
    if (filesize > 0)
    {
        buffer.resize(filesize);
        size_t wchars_read = fread(&(buffer.front()), sizeof(wchar_t), filesize, f);
        buffer.resize(wchars_read);
        buffer.shrink_to_fit();
    }

    fclose(f);

    return buffer;
}

map<wstring, wstring> ConvertEnvStringToMap(wstring file) {
    map<wstring, wstring> env;
    wstring line;
    size_t newline = 0;

    while(true) {
        newline = file.find('\n');
        line = file.substr(0, newline);

        if (line.find('=') == string::npos)
            break;

        // wcout << newline << " " << line.substr(0, line.find('=')) << " " << line.substr(line.find('=') + 1) << endl;
        env.insert(make_pair<wstring, wstring>(line.substr(0, line.find('=')), line.substr(line.find('=') + 1)));

        if (newline == string::npos)
            break;

        file = file.substr(newline + 1);
    }

    return env;
}


int main() {
    map<wstring, wstring> env = ConvertEnvStringToMap(LoadUtf8FileToString(L"config.txt"));
    Database db(env.at(L"EXCEL_PATH"),
                env.at(L"EXCEL_SHEET"),
                ws2s(env.at(L"EXCEL_EUROCODE_COL")),
                ws2s(env.at(L"EXCEL_AB_NOMENCLATURE_COL")),
                ws2s(env.at(L"EXCEL_MODEL_COL")));
    SetConsoleSettings();

    while (true) {
        char btn = 1;
        string a;
        cout << "______________________________Image utilities______________________________" << endl << endl;

        // cin >> a;
        // cout << db.contains_eurocode(a) << endl;

        MacrosExecutor macro_exec(&db, &env);
        macro_exec.exec_recursive(btn);

        cout << endl << endl << endl;
    }

    return 0;
}