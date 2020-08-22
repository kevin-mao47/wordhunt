#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <utility>
#include <algorithm>
#include <map>
#include <time.h> 
#include <chrono>
using namespace std;

string board[4];

// map<string,int> subword;
vector<string> solution;

struct compare {
    inline bool operator()(const string& first, const string& second) const {
        return first.size() < second.size();
    }
};


struct TrieNode {
  map<string, TrieNode*> child;
  bool complete = false;
};

struct TrieNode *root = new TrieNode;

void dictTrie()
{   
  ifstream dictionary_file;
  dictionary_file.open("words.txt");
  string word;
  if (dictionary_file.is_open()) {
      while (!dictionary_file.eof()) {
          struct TrieNode *curr = root;
          dictionary_file >> word;
          for(int i = 0; i < word.length(); i++)
          {
              string letter = string(1, word[i]);
              if((curr->child).find(letter) == (curr->child).end())
                  (curr->child)[letter] = new TrieNode;
              curr = (curr->child)[letter];
              if(i == word.length() - 1)
                  curr -> complete = true;
          }
      }
  }
}

void findWord(int row, int col, string word, bool checked[4][4], struct TrieNode *node) {
  cout << word << row << col << "\n";
  if (row < 0 || row > 3 || col < 0 || col > 3) {
    cout << "option 1 \n";
    return;
  }
  if (checked[row][col]) {
    cout << "option 2 \n";
    return;
  }
  
  string letter = string(1, board[row][col]);

  if ((node->child).find(letter) == (node->child).end()) {
    cout << "option 3 \n";
    return;
  }
  else {
    word += letter;
    checked[row][col] = true;
  }

  
  if (word.length() > 3 && (node -> child)[letter]-> complete) {
    solution.push_back(word);
  }
  cout << "option 4 \n";
  
  checked[row][col] = false;
}

int main() {
  cout << "Enter the words of the board, left to right, top to bottom: ";
  string input;
  cin >> input;

  dictTrie();
  
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

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      findWord(i, j, "", checked, root);
    }
  }

  compare c;
  sort(solution.begin(), solution.end(), c);

  cout << "printing \n";

  for(int i = 0; i < solution.size(); i++) {
    cout << solution[i] << '\n';
  }

}