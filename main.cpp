#include <iostream>
#include <fstream>
#include "database.h"


int main()
{
    string table_name = "users";

    // Initialise the database
    Database db;
    db.init("data");

    // Clear the database
    db.clear(table_name);

    // Write to the database
    ifstream file("data/test-users.txt");

    string line;

    while (getline(file, line))
    {
        db.write(table_name, line);
    }

    file.close();

    // Print length
    vector<string> data = db.read(table_name);

    cout << "Number of rows: " << data.size() << endl;

    // Print data
    db.print(data);

    return 0;
}