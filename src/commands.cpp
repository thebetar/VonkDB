#include <iostream>
#include <ncurses.h>
#include <sstream>
#include <cstring>
#include "commands.h"
#include "database/database.h"

// Cell width is equal to ID length + 1
#define CELL_WIDTH 33

using namespace std;

void print_table_columns(vector<string> column_names)
{
    for (int i = 0; i < column_names.size(); i++)
    {
        char column_name[CELL_WIDTH + 1];

        for (int j = 0; j < CELL_WIDTH; j++)
        {
            if (j < column_names[i].size())
            {
                column_name[j] = column_names[i][j];
            }
            else
            {
                column_name[j] = ' ';
            }
        }

        printw(column_name);

        if (i != column_names.size() - 1)
        {
            printw("|");
        }
    }

    printw("\n");
}

void print_table_row(string row)
{
    string cell_value = "";
    stringstream row_stream(row);
    vector<string> row_values = {};

    while (getline(row_stream, cell_value, '|'))
    {
        row_values.push_back(cell_value);
    }

    for (int i = 0; i < row_values.size(); i++)
    {
        char cell_value[CELL_WIDTH + 1];

        // If value exceeds the cell width, truncate it
        if (row_values[i].size() > CELL_WIDTH)
        {
            row_values[i] = row_values[i].substr(0, CELL_WIDTH - 2) + "..";
        }

        // Add characters to cell value
        for (int j = 0; j < CELL_WIDTH; j++)
        {
            if (j < row_values[i].size())
            {
                cell_value[j] = row_values[i][j];
            }
            else
            {
                cell_value[j] = ' ';
            }
        }

        printw(cell_value);

        if (i != row_values.size() - 1)
        {
            printw("|");
        }
        else
        {
            printw("\n");
        }
    }
}

void command_get(Database &db)
{
    printw("Enter table name: ");

    char table_name[256];

    echo();
    getstr(table_name);
    noecho();

    move(0, 0);
    clrtoeol();

    printw("Table: %s\n", table_name);

    vector<string> table_column_names = db.get_table_column_names(table_name);

    if (table_column_names.size() == 0)
    {
        printw("Table does not exist\n");
        return;
    }

    print_table_columns(table_column_names);

    vector<string> table_rows = db.get(table_name);

    for (int i = 0; i < table_rows.size(); i++)
    {
        print_table_row(table_rows[i]);
    }

    refresh();
}

void command_get_by_id(Database &db)
{
    printw("Enter table name: ");

    char table_name[256];

    echo();
    getstr(table_name);
    noecho();

    // Get table to check if exists
    vector<string> table_column_names = db.get_table_column_names(table_name);

    if (table_column_names.size() == 0)
    {
        printw("Table does not exist\n");
        return;
    }

    printw("Enter id: ");

    char id[256];

    echo();
    getstr(id);
    noecho();

    string table_row = db.get_by_id(table_name, id);

    printw("\nResult:\n");

    if (table_row.size() == 0)
    {
        printw("No row find with that id");
        return;
    }

    print_table_columns(table_column_names);

    print_table_row(table_row);
}

void command_get_joined(Database &db)
{
    printw("Enter table 1 name: ");

    char table1_name[256];

    echo();
    getstr(table1_name);
    noecho();

    printw("Enter table 2 name: ");

    char table2_name[256];

    echo();
    getstr(table2_name);
    noecho();

    printw("Enter table 1 column: ");

    char table1_column_name[256];

    echo();
    getstr(table1_column_name);
    noecho();

    printw("Enter table 2 column: ");

    char table2_column_name[256];

    echo();
    getstr(table2_column_name);
    noecho();

    vector<string> table1_column_names = db.get_table_column_names(table1_name);
    vector<string> table2_column_names = db.get_table_column_names(table2_name);

    if (table1_column_names.size() == 0 || table2_column_names.size() == 0)
    {
        printw("Table does not exist\n");
        return;
    }

    vector<string> joined_columns = table1_column_names;

    for (int i = 0; i < table2_column_names.size(); i++)
    {
        string joined_column = table2_name;
        joined_column += ".";
        joined_column += table2_column_names[i];

        joined_columns.push_back(joined_column);
    }

    print_table_columns(joined_columns);

    vector<string> joined_data = db.get_joined(table1_name, table2_name, table1_column_name, table2_column_name);

    for (int i = 0; i < joined_data.size(); i++)
    {
        print_table_row(joined_data[i]);
    }
}

void command_insert(Database &db)
{
    printw("Enter table name: ");

    char table_name[256];

    echo();
    getstr(table_name);
    noecho();

    vector<string> columns = db.get_table_column_names(table_name);

    if (columns.size() == 0)
    {
        printw("Table does not exist\n");
        return;
    }

    vector<string> data;
    printw("\nEnter data for new row: \n");

    // Iterate over columns
    for (int i = 1; i < columns.size(); i++)
    {
        printw("  %s: ", columns[i].c_str());

        char value[256];

        echo();
        getstr(value);
        noecho();

        data.push_back(value);
    }

    string new_row_id = db.insert(table_name, data);
    printw("Created with ID: %s\n", new_row_id.c_str());
}

void command_update(Database &db)
{
    printw("Enter table name: ");

    char table_name[256];

    echo();
    getstr(table_name);
    noecho();

    vector<string> table_columns = db.get_table_column_names(table_name);

    if (table_columns.size() == 0)
    {
        printw("Table does not exist\n");
        return;
    }

    printw("Enter id: ");

    char id[256];

    echo();
    getstr(id);
    noecho();

    string table_row = db.get_by_id(table_name, id);

    if (table_row.size() == 0)
    {
        printw("No row found with that id\n");
        return;
    }

    vector<string> data;
    printw("\nEnter new data for row: \n");

    for (int i = 1; i < table_columns.size(); i++)
    {
        printw("  %s: ", table_columns[i].c_str());

        char value[256];

        echo();
        getstr(value);
        noecho();

        data.push_back(value);
    }

    int status = db.update(table_name, id, data);

    if (status)
    {
        printw("Row with id \"%s\" updated successfully\n", id);
    }
    else
    {
        printw("Error updating row with id \"%s\"\n", id);
    }
}

void command_delete(Database &db)
{
    printw("Enter table name: ");

    char table_name[256];

    echo();
    getstr(table_name);
    noecho();

    int table_exists = db.table_exists(table_name);

    if (!table_exists)
    {
        printw("Table does not exist\n");
        return;
    }

    printw("Enter id: ");

    char id[256];

    echo();
    getstr(id);
    noecho();

    int removed = db.remove(table_name, id);

    if (removed)
    {
        printw("Row with id \"%s\" removed successfully\n", id);
    }
    else
    {
        printw("Error removing row with id \"%s\"\n", id);
    }
}

void command_create_table(Database &db)
{
    printw("Enter table name: ");

    char table_name[256];

    echo();
    getstr(table_name);
    noecho();

    int table_exists = db.table_exists(table_name);

    if (table_exists)
    {
        printw("Table already exists\n");
        return;
    }

    vector<string> columns;

    printw("Enter columns (type 'done' to finish): \n");

    while (true)
    {
        printw("  Column: ");

        char column[256];

        echo();
        getstr(column);
        noecho();

        if (strcmp(column, "done") == 0)
        {
            break;
        }

        columns.push_back(column);
    }

    int status = db.create_table(table_name, columns);

    if (status)
    {
        printw("Table created successfully with columns: ");

        for (int i = 0; i < columns.size(); i++)
        {
            printw("%s", columns[i].c_str());

            if (i != columns.size() - 1)
            {
                printw(", ");
            }
        }
    }
    else
    {
        printw("Error creating table");
    }
}

void command_drop_table(Database &db)
{
    printw("Enter table name: ");

    char table_name[256];

    echo();
    getstr(table_name);
    noecho();

    int status = db.drop_table(table_name);

    if (status)
    {
        printw("Table dropped successfully");
    }
    else
    {
        printw("Error dropping table");
    }
}

void command_get_table_names(Database &db)
{
    vector<string> table_names = db.get_table_names();

    printw("Tables:\n");

    for (int i = 0; i < table_names.size(); i++)
    {
        vector<string> table_column_names = db.get_table_column_names(table_names[i]);

        printw("  %s (", table_names[i].c_str());

        for (int j = 0; j < table_column_names.size(); j++)
        {
            printw("%s", table_column_names[j].c_str());

            if (j != table_column_names.size() - 1)
            {
                printw(", ");
            }
        }

        printw(")\n");
    }
}

void command_get_table_column_names(Database &db)
{
    printw("Enter table name: ");

    char table_name[256];

    echo();
    getstr(table_name);
    noecho();

    vector<string> table_column_names = db.get_table_column_names(table_name);

    printw("Columns:\n");

    for (int i = 0; i < table_column_names.size(); i++)
    {
        printw("  %s\n", table_column_names[i].c_str());
    }
}

void command_clear_table(Database &db)
{
    printw("Enter table name: ");

    char table_name[256];

    echo();
    getstr(table_name);
    noecho();

    int status = db.clear_table(table_name);

    if (status)
    {
        printw("Table cleared successfully");
    }
    else
    {
        printw("Error clearing table");
    }
}

void command_help()
{
    printw("Commands:\n\n");
    printw("  Database:\n");
    printw("    - create_table: Create a new table\n");
    printw("    - drop_table: Drop a table\n");
    printw("    - get_table_names: Get all table names\n");
    printw("    - get_table_column_names: Get columns of a table\n");
    printw("    - clear: Clear a table\n");
    printw("  Table:\n");
    printw("    - get: Get all data from a table\n");
    printw("    - get_by_id: Get data by id\n");
    printw("    - get_joined: Get joined data from two tables\n");
    printw("    - insert: Insert data into a table\n");
    printw("    - update: Update data in a table\n");
    printw("    - delete: Remove data from a table\n");
    printw("    - help: Show this help message\n\n");
}