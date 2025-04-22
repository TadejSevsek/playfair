#include <vector>
#include <fstream>
#include <unordered_set>
using namespace std;



vector<unsigned char> generateTable(const string& key) {
    unordered_set<unsigned char> used;
    vector<unsigned char> table;
    return table;
}
vector<unsigned char> readFile(const string& filename) {
    ifstream file(filename, ios::binary);
    return vector<unsigned char>((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
}

void writeFile(const string& filename, const vector<unsigned char>& data) {
    ofstream file(filename, ios::binary);
    file.write((const char*)data.data(), data.size());
}

