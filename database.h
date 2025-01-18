#include <string>
#include <vector>

using namespace std;

class Database
{
public:
    string folder;

    void init(string folder);
    vector<string> read(string table);
    string write(string table, string data);
    int remove(string table, string id);
    int update(string table, string id, string data);
    int clear(string table);
    void print(vector<string> data);

private:
    string get_filepath(string table);
};
