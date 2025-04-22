#include <vector>
#include <fstream>
#include <unordered_set>
using namespace std;



vector<unsigned char> generateTable(const string& key) {
    unordered_set<unsigned char> used;
    vector<unsigned char> table;
    //doda vse nepovjajoce crke kljuca 
    for (char c : key) {
        if (used.find((unsigned char)c) == used.end()) { //je true ce se crka ni bila dodana v used 
            table.push_back((unsigned char)c);
            used.insert((unsigned char)c);
        }
    }
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

