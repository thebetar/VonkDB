#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include "database.h"

string generate_id()
{
    string id = "";
    string chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < 32; i++)
    {
        id += chars[rand() % chars.size()];
    }

    return id;
}

string Database::get_filepath(string table)
{
    return this->folder + "/" + table + ".csv";
}

int Database::table_exists(string table)
{
    string filepath = this->get_filepath(table);

    if (filesystem::exists(filepath))
    {
        return 1;
    }

    return 0;
}

string Database::parse_data(string table, string id, vector<string> data)
{
    vector<string> columns = this->get_table_column_names(table);

    string row = id + "|";

    // Iterate over columns and get values (first column is ID)
    for (int i = 0; i < columns.size() - 1; i++)
    {
        row += data[i] + "|";
    }

    row = row.substr(0, row.size() - 1) + "\n";

    return row;
}

// Initialise the database
void Database::init(string folder)
{
    this->folder = folder;
}

// Write to table in the database
string Database::insert(string table, vector<string> data)
{
    try
    {
        // Check if the table exists
        if (!this->table_exists(table))
        {
            printf("Table does not exist\n");
            return "";
        }

        // Open the file in append mode
        string filepath = this->get_filepath(table);
        ofstream file(filepath, ios::app);

        // Generate a unique id
        string row_id = generate_id();
        // Write the data to the file
        file << this->parse_data(table, row_id, data);

        file.close();

        return row_id;
    }
    catch (exception e)
    {
        printf("Error: %s\n", e.what());
        return "";
    }
}

// Remove data from the database
int Database::remove(string table, string id)
{
    try
    {
        // Check if the table exists
        if (!this->table_exists(table))
        {
            printf("Table does not exist\n");
            return 0;
        }

        // Open the file in read mode
        string filepath = this->get_filepath(table);
        ifstream file(filepath);
        string line;

        // New data to be written
        string new_data = "";

        while (getline(file, line))
        {
            // If the line starts with the id, skip it (delete it)
            if (line.rfind(id, 0) == 0)
            {
                continue;
            }

            // Otherwise, add it to the new data
            new_data += line + "\n";
        }

        file.close();

        // Write new content to file
        ofstream new_file(filepath);
        new_file << new_data;
        new_file.close();

        return 1;
    }
    catch (exception e)
    {
        printf("Error: %s\n", e.what());
        return 0;
    }
}

// Update data in the database
int Database::update(string table_name, string id, vector<string> data)
{
    try
    {
        // Check if the table exists
        if (!this->table_exists(table_name))
        {
            printf("Table does not exist\n");
            return 0;
        }

        // Open the file in read mode
        string filepath = this->get_filepath(table_name);
        ifstream file(filepath);
        string line;

        // New data to be written
        string new_data = "";

        while (getline(file, line))
        {
            // If the line starts with the id, update it
            if (line.rfind(id, 0) == 0)
            {
                // Replace the line with the new data
                new_data += this->parse_data(table_name, id, data);
                continue;
            }

            // Otherwise, add it to the new data
            new_data += line + "\n";
        }

        file.close();

        // Write new content to file
        ofstream new_file(filepath);
        new_file << new_data;
        new_file.close();

        return 1;
    }
    catch (exception e)
    {
        printf("Error: %s\n", e.what());
        return 0;
    }
}

// Clear database
int Database::clear_table(string table)
{
    try
    {
        // Open the file in write mode
        string filepath = this->get_filepath(table);
        ofstream file(filepath);

        // Clear the file
        file << "";

        file.close();

        return 1;
    }
    catch (exception e)
    {
        printf("Error: %s\n", e.what());
        return 0;
    }
}

// Create table in the database
int Database::create_table(string table, vector<string> columns)
{
    try
    {
        // Open the file in write mode
        string filepath = this->get_filepath(table);
        ofstream file(filepath);

        // Add columns information (first line of the metadata)
        string columns_metadata = "id|";

        for (int i = 0; i < columns.size(); i++)
        {
            // Add the column name and a separator
            columns_metadata += columns[i] + "|";
        }

        // Remove the last separator
        columns_metadata = columns_metadata.substr(0, columns_metadata.size() - 1);

        // Write the columns to the file as the header
        file << columns_metadata + "\n";

        file.close();

        return 1;
    }
    catch (exception e)
    {
        printf("Error: %s\n", e.what());
        return 0;
    }
}

// Drop table from the database
int Database::drop_table(string table)
{
    try
    {
        // Find the file and remove it
        string filepath = this->get_filepath(table);
        std::remove(filepath.c_str());

        return 1;
    }
    catch (exception e)
    {
        printf("Error: %s\n", e.what());
        return 0;
    }
}