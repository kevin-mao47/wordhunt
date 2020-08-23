#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

string board[4];
vector<string> solution;
vector<string> dictWords;
const int ALPHABET_SIZE = 26; 

struct compare {
    inline bool operator()(const string& first, const string& second) const {
        return first.size() < second.size();
    }
};

struct TrieNode{
    struct TrieNode *children[ALPHABET_SIZE];
    bool wordEnd;
};

struct TrieNode *getNode(void) 
{ 
    struct TrieNode *pNode =  new TrieNode; 
  
    pNode->wordEnd = false; 
  
    for (int i = 0; i < ALPHABET_SIZE; i++) 
        pNode->children[i] = NULL; 
  
    return pNode; 
} ;

void insert(struct TrieNode *root, string key) 
{ 
    struct TrieNode *pCrawl = root; 
  
    for (int i = 0; i < key.length(); i++) 
    { 
        int index = key[i] - 'a'; 
        if (!pCrawl->children[index]) 
            pCrawl->children[index] = getNode(); 
  
        pCrawl = pCrawl->children[index]; 
    } 
    pCrawl->wordEnd = true; 
} 

TrieNode* dictTrie() {
    ifstream dictionary;
    dictionary.open("words.txt");
    string word;
    if (dictionary.is_open()) {
        while (!dictionary.eof()) {
            dictionary >> word;
            dictWords.push_back(word);
        }
    }
    struct TrieNode *root = getNode();
    for (int i = 0; i < dictWords.size(); i++) {
        insert(root, dictWords[i]);
    }
    return root;
}

string addDirections(string directions, int i, int j) {
    if (i == -1) {
        directions = directions + "U";
    }
    else if (i == 0) {
        directions = directions;
    }
    else {
        directions = directions + "D";
    }
    if (j == -1) {
        directions = directions + "L, ";
    }
    else if (j == 0) {
        directions = directions + ", ";
    }
    else {
        directions = directions + "R, ";
    }
    return directions;
}

bool substringSearch(vector<string> solution, string target) {
    for (int i = 0; i < solution.size(); i++) {
        string firstWord = solution[i].substr(0, solution[i].find(' '));
        if (firstWord == target) {
            return true;
        }
    }
    return false;
}

void findWord(int row, int col, string word, bool checked[4][4], struct TrieNode *node, string directions) {
    // out of bounds
    if (row < 0 || row > 3 || col < 0 || col > 3) {
        return;
    }
    // current square has already been checked for current word being built
    if (checked[row][col]) {
        return;
    }
    // get letter of current square
    string letter = string(1,board[row][col]);
    
    int index = board[row][col] - 'a';
    // check to see if current word is an existing prefix, if not, terminate
    if (!node->children[index]) {
        return;
    }
    word += letter;
    checked[row][col] = true;

    // see if current word forms a full word
    if (word.length() >= 3 && node->children[index]->wordEnd) {
        if (!substringSearch(solution, word)) {
            solution.push_back(word + directions.substr(0, directions.size()-2));
        }
    }

    for (int i = -1; i < 2; i++) {
        if (i == -1 && row == 0) {
            continue;
        }
        if (i == 1 && row == 3) {
            continue;
        }
        for (int j = -1; j < 2; j++) {
            if (j == -1 && col == 0) {
                continue;
            }
            if (j == 1 && col == 3) {
                continue;
            }
            string newDirections = addDirections(directions, i, j);
            findWord(row + i, col + j, word, checked, node->children[index], newDirections);
        }
    }
    checked[row][col] = false;
}

int main() {
    cout << "Enter the words of the board, left to right, top to bottom: ";
    string input;
    cin >> input;
    transform(input.begin(), input.end(), input.begin(), ::tolower);
  
    board[0] = input.substr(0, 4);
    board[1] = input.substr(4, 8);
    board[2] = input.substr(8, 12);
    board[3] = input.substr(12, 16);

    bool checked[4][4];
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            checked[i][j] = false;
        }
    }

    struct TrieNode *root = dictTrie(); 

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            findWord(i, j, "", checked, root, " (" + to_string(i + 1) + "," + to_string(j+ 1) + ") ");
        }
    }

    // sort(solution.begin(), solution.end());
    // solution.erase(unique(solution.begin(), solution.end()), solution.end());

    compare c;
    sort(solution.begin(), solution.end(), c);

    for(int i = 0; i < solution.size(); i++) {
        cout << solution[i] << '\n';
    }
}