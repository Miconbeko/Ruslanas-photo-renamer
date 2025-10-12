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

    xl::XLCellRange eurocode_range = sheet.range(xl::XLCellReference(2, EUROCODES_COL_INDEX), xl::XLCellReference(xl::MAX_ROWS, EUROCODES_COL_INDEX));
    xl::XLCellRange id_range = sheet.range(xl::XLCellReference(2, ID_COL_INDEX), xl::XLCellReference(xl::MAX_ROWS, ID_COL_INDEX));
    xl::XLCellRange brand_range = sheet.range(xl::XLCellReference(2, BRAND_COL_INDEX), xl::XLCellReference(xl::MAX_ROWS, BRAND_COL_INDEX));
    // xl::XLCellRange model_range = sheet.range(xl::XLCellReference(2, MODEL_COL_INDEX), xl::XLCellReference(xl::MAX_ROWS, MODEL_COL_INDEX));
    xl::XLCellRange processed_range = sheet.range(xl::XLCellReference(2, PROCESSED_CHECK_COL_INDEX), xl::XLCellReference(xl::MAX_ROWS, PROCESSED_CHECK_COL_INDEX));
    xl::XLCellRange retouched_range = sheet.range(xl::XLCellReference(2, RETOUCHED_CHECK_COL_INDEX), xl::XLCellReference(xl::MAX_ROWS, RETOUCHED_CHECK_COL_INDEX));

    xl::XLWorksheet missing_sheet = doc.workbook().worksheet(missing_sheetname);

    xl::XLCellRange missing_id_range = missing_sheet.range(xl::XLCellReference(2, MISSING_ID_COL_INDEX), xl::XLCellReference(xl::MAX_ROWS, MISSING_ID_COL_INDEX));

    std::cout << "Start loading data from excel file . . ." << std::endl;
    int cell_count = 2;
    for (xl::XLCellIterator eurocode = eurocode_range.begin(),
                            id = id_range.begin(),
                            brand = brand_range.begin(),
                            // model = model_range.begin(),
                            processed = processed_range.begin(),
                            retouched = retouched_range.begin(),
                            missing_id = missing_id_range.begin();
                            eurocode != eurocode_range.end();
                            eurocode++,
                            id++,
                            brand++,
                            // model++,
                            cell_count++,
                            processed++,
                            retouched++,
                            missing_id++
                        ) {
        if (empty_cell_count == EMPTY_CELL_LIMIT) {
            break;
        }

        if (eurocode->value() == "") {
            empty_cell_count++;
            continue;
        }
        else {
            empty_cell_count = 0;
        }

        // cout << it->value().getString() << " " << cell_count << endl;
        euro_to_row.insert(std::make_pair(eurocode->value().getString(), cell_count));
        euro_to_id.insert(std::make_pair(eurocode->value().getString(), id->value().getString()));
        euro_to_brand.insert(std::make_pair(eurocode->value().getString(), brand->value().getString()));
        // euro_to_model.insert(std::make_pair(eurocode->value().getString(), model->value().getString()));
        id_to_euro.insert(std::make_pair(id->value().getString(), eurocode->value().getString()));
        eurocodes.insert(eurocode->value().getString());
        ids.insert(id->value().getString());
        brands.insert(brand->value().getString());
        // models.insert(model->value().getString());

        missing_ids.insert(missing_id->value().getString());

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
    // std::cout << "Number of pairs <EC, Model>: " << euro_to_model.size() << std::endl;
    std::cout << "Number of loaded EC at column (" << EUROCODES_COL_INDEX << "): " << eurocodes.size() << std::endl;
    std::cout << "Number of loaded AB at column (" << ID_COL_INDEX << "): " << ids.size() << std::endl;
    std::cout << "Number of loaded Brands at column (" << BRAND_COL_INDEX << "): " << brands.size() << std::endl;
    // std::cout << "Number of loaded Models at column (" << MODEL_COL_INDEX << "): " << models.size() << std::endl;
    std::cout << "Number of processed AB at column (" << PROCESSED_CHECK_COL_INDEX << "): " << processed_rows.size() << std::endl;
    std::cout << "Number of retouched AB at column (" << RETOUCHED_CHECK_COL_INDEX << "): " << retouched_rows.size() << std::endl;
    std::cout << "Number of missing AB on sheet (" << "missing_sheetname" << ") at column (" << MISSING_ID_COL_INDEX << "): " << missing_ids.size() << std::endl;

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

std::string Database::get_brand(std::string eurocode) {
    try {
        return euro_to_brand.at(eurocode);
    } catch(...) {}

    return "";
}

// std::string Database::get_model(std::string eurocode) {
//     try {
//         return euro_to_model.at(eurocode);
//     } catch(...) {}

//     return "";
// }

std::string Database::get_eurocode(std::string id) {
    try {
        return id_to_euro.at(id);
    } catch(...) {}

    return "";
}

bool Database::contains_eurocode(std::string eurocode) {
    return eurocodes.find(eurocode) != eurocodes.end();
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

// bool Database::contains_model(std::string model) {
//     if (model == "")
//         return false;

//     return models.find(model) != models.end();
// }

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

bool Database::contains_missing_id(std::string id) {
    if (id == "")
        return false;

    return missing_ids.find(id) != missing_ids.end();
}