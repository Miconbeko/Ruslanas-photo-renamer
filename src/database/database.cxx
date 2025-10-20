#include "database.hxx"
#include "config/config.hxx"

#include <utility>

namespace fs = std::filesystem;
namespace xl = OpenXLSX;

Database* Database::database = nullptr;

Database::Database() {
    Config& env = Config::getInstance();

    this->filepath = env.get_ws("EXCEL_PATH");
    this->sheetname = env.get("EXCEL_SHEET");
    this->EUROCODES_COL_INDEX = env.get("EXCEL_EUROCODE_COL");
    this->ID_COL_INDEX = env.get("EXCEL_AB_COL");
    this->BRAND_COL_INDEX = env.get("EXCEL_BRAND_COL");
    // this->MODEL_COL_INDEX = env.get("EXCEL_MODEL_COL");
    this->PROCESSED_CHECK_COL_INDEX = env.get("EXCEL_PROCESSED_CHECK_COL");
    this->RETOUCHED_CHECK_COL_INDEX = env.get("EXCEL_RETOUCHED_CHECK_COL");

    this->missing_sheetname = env.get("EXCEL_MISSING_SHEET");
    this->MISSING_ID_COL_INDEX = env.get("EXCEL_MISSING_AB_COL");

    this->old_sheetname = env.get("EXCEL_OLD_SHEET");
    this->OLD_EUROCODES_COL_INDEX = env.get("EXCEL_OLD_EUROCODE_COL");
    this->OLD_ID_COL_INDEX = env.get("EXCEL_OLD_AB_COL");
    this->OLD_EUROCODE_WRITE_COL_INDEX = env.get("EXCEL_OLD_EUROCODE_WRITE_COL");

    load_db();
}

Database& Database::getInstance() {
    if (database == nullptr) {
        database = new Database();
    }

    return *database;
}

void Database::load_db() {
    xl::XLDocument doc;
    int empty_cell_count = 0;

    doc.open(filepath.string());
    xl::XLWorksheet sheet = doc.workbook().worksheet(sheetname);
    xl::XLWorksheet missing_sheet = doc.workbook().worksheet(missing_sheetname);
    xl::XLWorksheet old_sheet = doc.workbook().worksheet(old_sheetname);

    xl::XLCellRange eurocode_range = sheet.range(xl::XLCellReference(2, EUROCODES_COL_INDEX), xl::XLCellReference(xl::MAX_ROWS, EUROCODES_COL_INDEX));
    xl::XLCellRange id_range = sheet.range(xl::XLCellReference(2, ID_COL_INDEX), xl::XLCellReference(xl::MAX_ROWS, ID_COL_INDEX));
    xl::XLCellRange brand_range = sheet.range(xl::XLCellReference(2, BRAND_COL_INDEX), xl::XLCellReference(xl::MAX_ROWS, BRAND_COL_INDEX));
    xl::XLCellRange processed_range = sheet.range(xl::XLCellReference(2, PROCESSED_CHECK_COL_INDEX), xl::XLCellReference(xl::MAX_ROWS, PROCESSED_CHECK_COL_INDEX));
    xl::XLCellRange retouched_range = sheet.range(xl::XLCellReference(2, RETOUCHED_CHECK_COL_INDEX), xl::XLCellReference(xl::MAX_ROWS, RETOUCHED_CHECK_COL_INDEX));
    xl::XLCellRange missing_id_range = missing_sheet.range(xl::XLCellReference(2, MISSING_ID_COL_INDEX), xl::XLCellReference(xl::MAX_ROWS, MISSING_ID_COL_INDEX));
    xl::XLCellRange old_eurocode_range = old_sheet.range(xl::XLCellReference(2, EUROCODES_COL_INDEX), xl::XLCellReference(xl::MAX_ROWS, EUROCODES_COL_INDEX));
    xl::XLCellRange old_id_range = old_sheet.range(xl::XLCellReference(2, OLD_ID_COL_INDEX), xl::XLCellReference(xl::MAX_ROWS, OLD_ID_COL_INDEX));

    std::cout << "Start loading data from excel file . . ." << std::endl;
    int cell_count = 2;
    for (xl::XLCellIterator eurocode = eurocode_range.begin(),
                            id = id_range.begin(),
                            brand = brand_range.begin(),
                            processed = processed_range.begin(),
                            retouched = retouched_range.begin(),
                            missing_id = missing_id_range.begin(),
                            old_eurocode = old_eurocode_range.begin(),
                            old_id = old_id_range.begin();
                            eurocode != eurocode_range.end();
                            eurocode++,
                            id++,
                            brand++,
                            cell_count++,
                            processed++,
                            retouched++,
                            missing_id++,
                            old_eurocode++,
                            old_id++
                        ) {
        if (empty_cell_count == EMPTY_CELL_LIMIT) {
            break;
        }

        if (eurocode->value() == "") {
            empty_cell_count++;
        }
        else {
            empty_cell_count = 0;
        }

        if (eurocode->value().getString() != "") {
            euro_to_row.insert(std::make_pair(eurocode->value().getString(), cell_count));
            euro_to_id.insert(std::make_pair(eurocode->value().getString(), id->value().getString()));
            euro_to_brand.insert(std::make_pair(eurocode->value().getString(), brand->value().getString()));
            eurocodes.insert(eurocode->value().getString());
        }

        if (id->value().getString() != "") {
            ids.insert(id->value().getString());
            id_to_euro.insert(std::make_pair(id->value().getString(), eurocode->value().getString()));
            id_to_row.insert(std::make_pair(id->value().getString(), cell_count));
        }

        if (brand->value().getString() != "") {
            brands.insert(brand->value().getString());
        }

        if (missing_id->value().getString() != "") {
            missing_ids.insert(missing_id->value().getString());
        }

        if (old_id->value().getString() != "") {
            id_to_old_euro.insert(std::make_pair(old_id->value().getString(), old_eurocode->value().getString()));
        }

        if (old_eurocode->value().getString() != "") {
            old_euro_to_id.insert(std::make_pair(old_eurocode->value().getString(), old_id->value().getString()));
            old_eurocodes.insert(old_eurocode->value().getString());
        }

        if (processed->value().getString() == "yes") {
            processed_rows.insert(cell_count);
        }
        if (retouched->value().getString() == "R") {
            retouched_rows.insert(cell_count);
        }
    }

    std::cout << "Number of pairs <EC, AB>: " << euro_to_id.size() << std::endl;
    std::cout << "Number of pairs <EC, Row number>: " << euro_to_row.size() << std::endl;
    std::cout << "Number of pairs <EC, Brand>: " << euro_to_brand.size() << std::endl;
    std::cout << "Number of loaded EC at column (" << EUROCODES_COL_INDEX << "): " << eurocodes.size() << std::endl;
    std::cout << "Number of loaded AB at column (" << ID_COL_INDEX << "): " << ids.size() << std::endl;
    std::cout << "Number of loaded Brands at column (" << BRAND_COL_INDEX << "): " << brands.size() << std::endl;
    std::cout << "Number of processed AB at column (" << PROCESSED_CHECK_COL_INDEX << "): " << processed_rows.size() << std::endl;
    std::cout << "Number of retouched AB at column (" << RETOUCHED_CHECK_COL_INDEX << "): " << retouched_rows.size() << std::endl;
    std::cout << "Number of missing AB on sheet (" << missing_sheetname << ") at column (" << MISSING_ID_COL_INDEX << "): " << missing_ids.size() << std::endl;
    std::cout << "Number of pairs <AB, old EC> on sheet (" << old_sheetname << "): " << id_to_old_euro.size() << std::endl;

    doc.close();
}

std::vector<std::string> Database::get_ids(std::string eurocode) {
    auto range = euro_to_id.equal_range(eurocode);
    std::vector<std::string> res;

    for (auto it = range.first; it != range.second; ++it) {
        if (it->second == "")
            continue;

        res.push_back(it->second);
    }

    return res;
}

std::vector<std::string> Database::get_old_eurocode_ids(std::string eurocode) {
    auto range = old_euro_to_id.equal_range(eurocode);
    std::vector<std::string> res;

    for (auto it = range.first; it != range.second; ++it) {
        if (it->second == "")
            continue;

        res.push_back(it->second);
    }

    return res;
}

std::string Database::get_brand(std::string eurocode) {
    try {
        return euro_to_brand.at(eurocode);
    } catch(...) {}

    return "";
}

std::string Database::get_eurocode(std::string id) {
    try {
        return id_to_euro.at(id);
    } catch(...) {}

    return "";
}

std::string Database::get_old_eurocode_by_id(std::string id) {
    try {
        return id_to_old_euro.at(id);
    } catch(...) {}

    return "";
}

int Database::get_row_by_id(std::string id) {
    try {
        return id_to_row.at(id);
    } catch(...) {}

    return 0;
}

bool Database::contains_eurocode(std::string eurocode) {
    return eurocodes.find(eurocode) != eurocodes.end();
}

bool Database::contains_old_eurocode(std::string eurocode) {
    return old_eurocodes.find(eurocode) != old_eurocodes.end();
}

bool Database::contains_id(std::string id) {
    if (id == "")
        return false;

    return ids.find(id) != ids.end();
}

bool Database::contains_brand(std::string brand) {
    if (brand == "")
        return false;

    return brands.find(brand) != brands.end();
}

void Database::mark_eurocodes(std::vector<std::string> found_eurocodes, std::vector<std::string> sized_eurocodes) {
    xl::XLDocument doc;

    doc.open(filepath.string());
    xl::XLWorksheet sheet = doc.workbook().worksheet(sheetname);

    for (std::string eurocode : found_eurocodes) {
        auto it_lower = euro_to_row.lower_bound(eurocode);
        auto it_upper = euro_to_row.upper_bound(eurocode);

        while (it_lower != it_upper) {
            if (it_lower->first == eurocode) {
                sheet.cell(xl::XLCellReference(it_lower->second, PROCESSED_CHECK_COL_INDEX)).value() = "yes";
                // std::cout << it_lower->second << " " << PROCESSED_CHECK_COL_INDEX << " yes" << std::endl;
            }
            it_lower++;
        }
    }

    for (std::string eurocode : sized_eurocodes) {
        auto it_lower = euro_to_row.lower_bound(eurocode);
        auto it_upper = euro_to_row.upper_bound(eurocode);

        while (it_lower != it_upper) {
            if (it_lower->first == eurocode) {
                sheet.cell(xl::XLCellReference(it_lower->second, RETOUCHED_CHECK_COL_INDEX)).value() = "R";
                // std::cout << it_lower->second << " " << RETOUCHED_CHECK_COL_INDEX << " R" << std::endl;
            }
            it_lower++;
        }
    }

    doc.save();
    doc.close();
}

void Database::write_old_eurocodes(std::map<int, std::string> map_eurocodes) {
    xl::XLDocument doc;

    doc.open(filepath.string());
    xl::XLWorksheet sheet = doc.workbook().worksheet(sheetname);

    for (auto row_to_old_eurocode : map_eurocodes) {
        int row = row_to_old_eurocode.first;
        std::string old_eurocode = row_to_old_eurocode.second;

        sheet.cell(xl::XLCellReference(row, OLD_EUROCODE_WRITE_COL_INDEX)).value() = old_eurocode;
    }

    doc.save();
    doc.close();
}

bool Database::contains_missing_id(std::string id) {
    if (id == "")
        return false;

    return missing_ids.find(id) != missing_ids.end();
}