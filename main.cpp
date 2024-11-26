#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <Windows.h>

using namespace std;

struct TreeNode {
    string word;
    string translation;
    int counter;
    TreeNode* left;
    TreeNode* right;

    TreeNode(const string& w, const string& t, int c = 0)
        : word(w), translation(t), counter(c), left(nullptr), right(nullptr) {}
};

class Dictionary {
private:
    TreeNode* root;

    TreeNode* addNode(TreeNode* node, const string& word, const string& translation, int counter = 0) {
        if (!node) {
            return new TreeNode(word, translation, counter);
        }
        if (word < node->word) {
            node->left = addNode(node->left, word, translation, counter);
        }
        else if (word > node->word) {
            node->right = addNode(node->right, word, translation, counter);
        }
        else {
            node->translation = translation;
            node->counter = counter;
        }
        return node;
    }

    void inOrderTraversal(TreeNode* node, vector<TreeNode*>& nodes) {
        if (!node) return;
        inOrderTraversal(node->left, nodes);
        nodes.push_back(node);
        inOrderTraversal(node->right, nodes);
    }

    void deleteTree(TreeNode* node) {
        if (!node) return;
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }

public:
    Dictionary() : root(nullptr) {}

    ~Dictionary() {
        deleteTree(root);
    }

    void addWord(const string& word, const string& translation, int counter = 0) {
        root = addNode(root, word, translation, counter);
    }

    void saveToFile(const string& filename) {
        ofstream outFile(filename);
        if (!outFile) {
            cerr << "������� ��� ������� ����� ��� ������!\n";
            return;
        }

        vector<TreeNode*> nodes;
        inOrderTraversal(root, nodes);

        for (auto node : nodes) {
            outFile << node->word << "," << node->translation << "," << node->counter << "\n";
        }

        outFile.close();
        cout << "��� ��������� � ���� " << filename << endl;
    }

    void loadFromFile(const string& filename) {
        ifstream inFile(filename);
        if (!inFile) {
            cerr << "������� ��� ������� ����� ��� �������!\n";
            return;
        }

        string line;
        while (getline(inFile, line)) {
            stringstream ss(line);
            string word, translation, counterStr;

            getline(ss, word, ',');
            getline(ss, translation, ',');
            getline(ss, counterStr);

            int counter = stoi(counterStr);
            addWord(word, translation, counter);
        }

        inFile.close();
        cout << "��� ����������� � ����� " << filename << endl;
    }
};

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    Dictionary dict;

    dict.addWord("hello", "�����");
    dict.addWord("world", "���");
    dict.addWord("programming", "�������������");

    cout << "���������� ����� � ����...\n";
    dict.saveToFile("dictionary.csv");

    Dictionary newDict;
    cout << "\n������������ ����� �� �����...\n";
    newDict.loadFromFile("dictionary.csv");

    cout << "\n�������� ������������ �����:\n";
    newDict.saveToFile("output.csv"); 
    return 0;
}
