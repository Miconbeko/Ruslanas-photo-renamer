#pragma once

#include <OpenXLSX.hpp>

#include <string>
#include <filesystem>
#include <set>
#include <vector>
#include <map>

namespace fs = std::filesystem;
namespace xl = OpenXLSX;

class Database {
    private:
        const int EMPTY_CELL_LIMIT = 1000;

        std::string EUROCODES_COL_INDEX;
        std::string ID_COL_INDEX;
        std::string BRAND_COL_INDEX;
        // std::string MODEL_COL_INDEX;
        std::string PROCESSED_CHECK_COL_INDEX;
        std::string RETOUCHED_CHECK_COL_INDEX;
        std::string MISSING_ID_COL_INDEX;

        fs::path filepath;
        std::string sheetname;
        std::string missing_sheetname;
        std::multimap<std::string, int> euro_to_row;
        std::multimap<std::string, std::string> euro_to_id;
        std::map<std::string, std::string> euro_to_brand;
        // std::map<std::string, std::string> euro_to_model;
        std::map<std::string, std::string> id_to_euro;
        std::set<std::string> ids;
        std::set<std::string> brands;
        std::set<std::string> eurocodes;
        // std::set<std::string> models;
        std::set<int> processed_rows;
        std::set<int> retouched_rows;
        std::set<std::string> missing_ids;

        static Database* database;

        void load_db();
        Database();
    public:
        static Database& getInstance();

        std::vector<std::string> get_ids(std::string eurocode);
        std::string get_brand(std::string eurocode);
        // std::string get_model(std::string eurocode);
        std::string get_eurocode(std::string id);

        bool contains_eurocode(std::string eurocode);
        bool contains_id(std::string id);
        bool contains_brand(std::string brand);
        // bool contains_model(std::string model);
        bool contains_missing_id(std::string id);

        void mark_eurocodes(std::vector<std::string> found_eurocodes, std::vector<std::string> sized_eurocodes);
};