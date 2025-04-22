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

vector<unsigned char> preprocess(const vector<unsigned char>& input) {
    vector<unsigned char> result;
    for (size_t i = 0; i < input.size(); i++) {
        unsigned char a = input[i];
        unsigned char b;
        if (i + 1 < input.size()) {
            b = input[i + 1];
            if (a == b) {
                result.push_back(a);
                result.push_back(0x00);
                i++;  
            }
            else {
                result.push_back(a);
                result.push_back(b);
                i++;
            }
        }
        else {
            result.push_back(a);
            result.push_back(0x03);
        }
    }

  

    return result;
}


vector<unsigned char> fairplayEncrypt(const vector<unsigned char>& input, const vector<unsigned char>& table) {
    vector<unsigned char> processed = preprocess(input);
    vector<unsigned char> encrypted;
    for (size_t i = 0; i < processed.size(); i += 2) {
        std::pair<unsigned char, unsigned char> enc = encryptPair(table, processed[i], processed[i + 1]);
        encrypted.push_back(enc.first);
        encrypted.push_back(enc.second);
    }
    return encrypted;
}

pair<unsigned char, unsigned char> decryptPair(const vector<unsigned char>& table, unsigned char a, unsigned char b) {
    int row1, col1, row2, col2;
    findPos(table, a, row1, col1);
    findPos(table, b, row2, col2);

    if (row1 == row2)
        return { table[row1 * 16 + (col1 - 1 + 16) % 16], table[row2 * 16 + (col2 - 1 + 16) % 16] };
    else if (col1 == col2)
        return { table[((row1 - 1 + 16) % 16) * 16 + col1], table[((row2 - 1 + 16) % 16) * 16 + col2] };
    else
        return { table[row1 * 16 + col2], table[row2 * 16 + col1] };
}

vector<unsigned char> fairplayDecrypt(const vector<unsigned char>& input, const vector<unsigned char>& table) {
    vector<unsigned char> decrypted;

    for (size_t i = 0; i < input.size(); i += 2) {
        std::pair<unsigned char, unsigned char> dec = decryptPair(table, input[i], input[i + 1]);

        // ce je drug duppe pol na output dej prvega dvakrat
        if (dec.second == 0x00) {
            decrypted.push_back(dec.first);
            decrypted.push_back(dec.first); 
        }
        else {
            decrypted.push_back(dec.first);
            decrypted.push_back(dec.second);
        }
    }

    // ce je EOF zadn znak ga damo stran
    if (!decrypted.empty() && decrypted.back() == 0x03)
        decrypted.pop_back();

    return decrypted;
}

vector<unsigned char> readFile(const string& filename) {
    ifstream file(filename, ios::binary);
    return vector<unsigned char>((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
}

void writeFile(const string& filename, const vector<unsigned char>& data) {
    ofstream file(filename, ios::binary);
    file.write((const char*)data.data(), data.size());
}

int main(int argc, const char* const argv[]) {
    if (argc != 4) {
        return -1;
    }
    string key = argv[2];
    vector<unsigned char> input = readFile(argv[3]);
    vector<unsigned char> table = generateTable(key);
    return 0;
}