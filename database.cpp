#include "database.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>

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
    return this->folder + "/" + table + ".txt";
}


// Initialise the database
void Database::init(string folder)
{
    this->folder = folder;
}

// Read table from the database
vector<string> Database::read(string table)
{
    // Vector to store results
    vector<string> data;

    string filepath = this->get_filepath(table);
    ifstream file(filepath);
    string line;

    while (getline(file, line))
    {
        data.push_back(line);
    }

    file.close();

    return data;
}

// Write to table in the database
string Database::write(string table, string data)
{
    try {
        string filepath = this->get_filepath(table);
        ofstream file(filepath, ios::app);

        string row_id = generate_id();

        file << row_id << ":" << data << "\n";
        file.close();

        return row_id;
    } catch (exception e) {
        printf("Error: %s\n", e.what());
        return "";
    }
}

// Remove data from the database
int Database::remove(string table, string id)
{
    try {
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
            new_data += line + "\n";
        }

        file.close();

        ofstream new_file(filepath);
        new_file << new_data;
        new_file.close();

        return 1;
    } catch (exception e) {
        printf("Error: %s\n", e.what());
        return 0;
    }
}

// Update data in the database
int Database::update(string table, string id, string data)
{
    try {
        string filepath = this->get_filepath(table);
        ifstream file(this->folder);
        string line;

        // New data to be written
        string new_data = "";

        while (getline(file, line))
        {
            // If the line starts with the id, update it
            if (line.rfind(id, 0) == 0)
            {
                new_data += id + ":" + data + "\n";
                continue;
            }

            new_data += line + "\n";
        }

        file.close();

        ofstream new_file(filepath);
        new_file << new_data;
        new_file.close();

        return 1;
    } catch (exception e) {
        printf("Error: %s\n", e.what());
        return 0;
    }
}

int Database::clear(string table)
{
    try {
        string filepath = this->get_filepath(table);
        ofstream file(filepath);

        file << "";

        file.close();

        return 1;
    } catch (exception e) {
        printf("Error: %s\n", e.what());
        return 0;
    }
}

void Database::print(vector<string> data)
{
    for (int i = 0; i < data.size(); i++)
    {
        cout << data[i] << endl;
    }
}