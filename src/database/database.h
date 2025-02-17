#pragma once

#include <string>
#include <vector>

using namespace std;

class Database
{
public:
    string folder;

    void init(string folder);
    vector<string> get(string table);
    string get_by_id(string table, string id);
    vector<string> get_joined(string table1, string table2, string table1_col, string table2_col);
    string insert(string table, vector<string> data);
    int remove(string table, string id);
    int update(string table, string id, vector<string> data);
    int clear_table(string table);
    int create_table(string table, vector<string> columns);
    int drop_table(string table);
    vector<string> get_table_names();
    vector<string> get_table_column_names(string table);

    void print(vector<string> data);

private:
    string get_filepath(string table);
    string parse_data(string table, string id, vector<string> data);
    int table_exists(string table);
    vector<string> get_table_data(string table);
};
