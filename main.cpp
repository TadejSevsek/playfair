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

    //naredi isto se za ostale ascii znake
    for (int i = 0; i < 256; i++) {
        if (used.find((unsigned char)i) == used.end()) {
            table.push_back((unsigned char)i);
        }
    }
    return table;
}


void findPos(const vector<unsigned char>& table, unsigned char c, int& row, int& col) {
    for (int i = 0; i < table.size(); i++) {
        if (table[i] == c) {
            row = i / 16;
            col = i % 16;
            break;
        }
    }
}

pair<unsigned char, unsigned char> encryptPair(const vector<unsigned char>& table, unsigned char a, unsigned char b) {
    int row1, col1, row2, col2;
    findPos(table, a, row1, col1);
    findPos(table, b, row2, col2);

    if (row1 == row2)
        return { table[row1 * 16 + (col1 + 1) % 16], table[row2 * 16 + (col2 + 1) % 16] };
    else if (col1 == col2)
        return { table[((row1 + 1) % 16) * 16 + col1], table[((row2 + 1) % 16) * 16 + col2] };
    else
        return { table[row1 * 16 + col2], table[row2 * 16 + col1] };
}

vector<unsigned char> readFile(const string& filename) {
    ifstream file(filename, ios::binary);
    return vector<unsigned char>((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
}

void writeFile(const string& filename, const vector<unsigned char>& data) {
    ofstream file(filename, ios::binary);
    file.write((const char*)data.data(), data.size());
}

