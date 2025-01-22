#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include "database.h"

using namespace std;

// Read table from the database
vector<string> Database::get(string table)
{
    // Check if the table exists
    if (!this->table_exists(table))
    {
        printf("Table does not exist\n");
        return {};
    }

    // Vector to store results
    vector<string> data = this->get_table_data(table);

    return data;
}

// Read data by id
string Database::get_by_id(string table, string id)
{
    // Check if the table exists
    if (!this->table_exists(table))
    {
        printf("Table does not exist\n");
        return "";
    }

    // Open the file in read mode
    string filepath = this->get_filepath(table);
    ifstream file(filepath);
    string line;

    // Read the rest of the file
    while (getline(file, line))
    {
        // If the line starts with the id, return it
        if (line.rfind(id, 0) == 0)
        {
            return line;
        }
    }

    file.close();

    return "";
}

int get_table_idx(string table, vector<string> tables)
{
    for (int i = 0; i < tables.size(); i++)
    {
        if (tables[i] == table)
        {
            return i;
        }
    }

    return -1;
}

string get_value_by_idx(string row, int idx)
{
    string value = "";
    int count = 0;

    for (int i = 0; i < row.size(); i++)
    {
        if (row[i] == '|')
        {
            count++;
        }

        if (count > idx)
        {
            break;
        }

        if (count == idx && row[i] != '|')
        {
            value += row[i];
        }
    }

    return value;
}

// Read data with join
vector<string> Database::get_joined(string table1, string table2, string table1_col, string table2_col)
{
    // Check if both tables exists
    if (!this->table_exists(table1))
    {
        printf("Table 1 does not exist\n");
        return {};
    }

    if (!this->table_exists(table2))
    {
        printf("Table 2 does not exist\n");
        return {};
    }

    vector<string> table1_cols = this->get_table_columns(table1);
    vector<string> table1_data = this->get_table_data(table1);
    vector<string> table2_cols = this->get_table_columns(table2);
    vector<string> table2_data = this->get_table_data(table2);

    int table1_col_idx = get_table_idx(table1_col, table1_cols);

    if (table1_col_idx == -1)
    {
        printf("Table column does not exist on table 1\n");
        return {};
    }

    int table2_col_idx = get_table_idx(table2_col, table2_cols);

    if (table2_col_idx == -1)
    {
        printf("Table column does not exist on table 2\n");
        return {};
    }

    vector<string> result;

    // Join tables
    for (int i = 0; i < table1_data.size(); i++)
    {
        string table1_row = table1_data[i];
        string table1_join_value = get_value_by_idx(table1_row, table1_col_idx);

        for (int j = 0; j < table2_data.size(); j++)
        {
            string table2_row = table2_data[j];
            string table2_join_value = get_value_by_idx(table2_row, table2_col_idx);

            if (table1_join_value.compare(table2_join_value) == 0)
            {
                result.push_back(table1_row + "|" + table2_row);
            }
        }
    }

    return result;
}

// Get table names
vector<string> Database::get_table_names()
{
    vector<string> tables;
    string folder = this->folder;
    string table;

    ifstream file(folder);

    try
    {
        for (const auto &entry : filesystem::directory_iterator(folder))
        {
            table = entry.path().filename().string();
            table = table.substr(0, table.size() - 4);
            tables.push_back(table);
        }
    }
    catch (exception e)
    {
        cerr << "Error: " << e.what() << endl;
    }

    return tables;
}

// Get columns from the table
vector<string> Database::get_table_columns(string table)
{
    // Check if the table exists
    if (!this->table_exists(table))
    {
        printf("Table does not exist\n");
        return {};
    }

    // Open the file in read mode
    string filepath = this->get_filepath(table);
    ifstream file(filepath);
    string line;

    // Read the columns
    getline(file, line);
    vector<string> columns;

    // Split the line by '|'
    size_t pos = 0;
    string column;

    // Read the columns
    while ((pos = line.find("|")) != string::npos)
    {
        column = line.substr(0, pos);
        columns.push_back(column);
        line.erase(0, pos + 1);
    }

    // Add the last column
    columns.push_back(line);

    file.close();

    return columns;
}

vector<string> Database::get_table_data(string table)
{
    vector<string> result;

    // Get file path
    string filepath = this->get_filepath(table);
    ifstream file(filepath);
    string line;

    // Skip first line with metadata
    getline(file, line);

    while (getline(file, line))
    {
        result.push_back(line);
    }

    return result;
}