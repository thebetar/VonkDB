#include <iostream>
#include "commands.h"
#include "database/database.h"

void command_get(Database &db)
{
    cout << "Enter table name: ";
    string table;
    cin >> table;
    db.print(db.get(table));
}

void command_get_by_id(Database &db)
{
    cout << "Enter table name: ";
    string table;
    cin >> table;
    cout << "Enter id: ";
    string id;
    cin >> id;
    cout << db.get_by_id(table, id) << endl;
}

void command_get_joined(Database &db)
{
    cout << "Enter table 1 name: ";
    string table1;
    cin >> table1;
    cout << "Enter table 2 name: ";
    string table2;
    cin >> table2;
    cout << "Enter table 1 column: ";
    string table1_col;
    cin >> table1_col;
    cout << "Enter table 2 column: ";
    string table2_col;
    cin >> table2_col;

    db.print(db.get_joined(table1, table2, table1_col, table2_col));
}

void command_insert(Database &db)
{
    cout << "Enter table name: ";
    string table;
    cin >> table;

    vector<string> columns = db.get_table_column_names(table);
    vector<string> data;
    cout << "Enter data: \n";

    for (int i = 0; i < columns.size(); i++)
    {
        cout << columns[i] << ": ";

        string value;
        cin >> value;
        data.push_back(value);
    }

    cout << "Created with ID: " << db.insert(table, data) << endl;
}

void command_update(Database &db)
{
    cout << "Enter table name: ";
    string table;
    cin >> table;
    cout << "Enter id: ";
    string id;
    cin >> id;

    vector<string> columns = db.get_table_column_names(table);
    vector<string> data;
    cout << "Enter data: \n";

    for (int i = 0; i < columns.size(); i++)
    {
        cout << columns[i] << ": ";

        string value;
        cin >> value;
        data.push_back(value);
    }

    cout << "Updated: " << db.update(table, id, data) << endl;
}

void command_remove(Database &db)
{
    cout << "Enter table name: ";
    string table;
    cin >> table;
    cout << "Enter id: ";
    string id;
    cin >> id;
    cout << "Removed: " << db.remove(table, id) << endl;
}

void command_create_table(Database &db)
{
    cout << "Enter table name: ";
    string table;
    cin >> table;

    vector<string> columns;
    cout << "Enter columns (type 'done' to finish): \n";

    while (true)
    {
        cout << "Column: ";
        string column;
        cin >> column;

        if (column == "done")
        {
            break;
        }

        columns.push_back(column);
    }

    int status = db.create_table(table, columns);

    if (status)
    {
        cout << "Table created successfully with columns: ";
        db.print(columns);
    }
    else
    {
        cout << "Error creating table\n";
    }
}

void command_drop_table(Database &db)
{
    cout << "Enter table name: ";
    string table;
    cin >> table;

    int status = db.drop_table(table);

    if (status)
    {
        cout << "Table dropped successfully\n";
    }
    else
    {
        cout << "Error dropping table\n";
    }
}

void command_get_table_names(Database &db)
{
    db.print(db.get_table_names());
}

void command_get_table_column_names(Database &db)
{
    cout << "Enter table name: ";
    string table;
    cin >> table;
    db.print(db.get_table_column_names(table));
}

void command_clear_table(Database &db)
{
    cout << "Enter table name: ";
    string table;
    cin >> table;

    int status = db.clear_table(table);

    if (status)
    {
        cout << "Table cleared successfully\n";
    }
    else
    {
        cout << "Error clearing table\n";
    }
}

void command_help()
{
    cout << "Commands:\n\n";
    cout << "  Database:\n";
    cout << "    - create_table: Create a new table\n";
    cout << "    - drop_table: Drop a table\n";
    cout << "    - get_table_names: Get all table names\n";
    cout << "    - get_table_column_names: Get columns of a table\n";
    cout << "    - clear: Clear a table\n";
    cout << "  Table:\n";
    cout << "    - get: Get all data from a table\n";
    cout << "    - get_by_id: Get data by id\n";
    cout << "    - get_joined: Get joined data from two tables\n";
    cout << "    - insert: Insert data into a table\n";
    cout << "    - update: Update data in a table\n";
    cout << "    - remove: Remove data from a table\n";
    cout << "    - help: Show this help message\n\n";
}