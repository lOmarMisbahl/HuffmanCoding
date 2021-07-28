#include <bits/stdc++.h>
#include<iostream>
#include<string>
#include <streambuf>
using namespace std;
#define EMPTY_STRING ""


struct Node
{
    char ch;
    int freq;
    Node *left, *right;
};

Node* CreateNode(char ch, int freq, Node* left, Node* right)
{
    Node* node = new Node();

    node->ch = ch;
    node->freq = freq;
    node->left = left;
    node->right = right;

    return node;
}

struct comp
{
    bool operator()(Node* l, Node* r)
    {
        return l->freq > r->freq;
    }
};

bool isLeaf(Node* root) {
    return root->left == nullptr && root->right == nullptr;
}

void Encode(Node* root, string str, unordered_map<char, string> &huffmanCode)
{
    if (root == nullptr) {
        return;
    }

    if (isLeaf(root)) {
        huffmanCode[root->ch] = (str != EMPTY_STRING) ? str : "1";
    }

    Encode(root->left, str + "0", huffmanCode);
    Encode(root->right, str + "1", huffmanCode);
}

string decode(Node* root, int &index, string str, string &output)
{
    if (root == nullptr) {
        return EMPTY_STRING;
    }

    if (isLeaf(root)) {
        output.push_back(root->ch);
        return EMPTY_STRING;
    }

    index++;

    if (str[index] == '0') {
        decode(root->left, index, str, output);
    } else {
        decode(root->right, index, str, output);
    }

    return output;

}

string EncodeCall(Node* root, string str, unordered_map<char, string> &huffmanCode, string text){
    Encode(root, EMPTY_STRING, huffmanCode);


    string string;
    for (char ch: text) {
        str += huffmanCode[ch];
    }

    return str;

}

string DecodeCall(Node* root, string str, string output){
//    cout << "\nDecoded string is:\n";

    if (isLeaf(root))
    {
        while (root->freq--) {
            output.push_back(root->ch);
        }
    }
    else
    {
        int index = -1;
        while (index < (int)str.size() - 1) {
            decode(root, index, str,output);
        }
    }

    return output;

}

Node* buildMap(string text){
    if (text == EMPTY_STRING) {
        return NULL;
    }

    unordered_map<char, int> freq;
    for (char ch: text) {
        freq[ch]++;
    }

    priority_queue<Node*, vector<Node*>, comp> pq;

    for (auto pair: freq) {
        pq.push(CreateNode(pair.first, pair.second, nullptr, nullptr));
    }

    while (pq.size() != 1)
    {

        Node *left = pq.top();
        pq.pop();
        Node *right = pq.top();
        pq.pop();

        int sum = left->freq + right->freq;
        pq.push(CreateNode(NULL, sum, left, right));
    }

    Node* root = pq.top();

    unordered_map<char, string> huffmanCode;

    return root;

}

string buildHuffmanTree(string text)
{
    if (text == EMPTY_STRING) {
        return NULL;
    }

    unordered_map<char, int> freq;
    for (char ch: text) {
        freq[ch]++;
    }

    priority_queue<Node*, vector<Node*>, comp> pq;

    for (auto pair: freq) {
        pq.push(CreateNode(pair.first, pair.second, nullptr, nullptr));
    }

    while (pq.size() != 1)
    {

        Node *left = pq.top();
        pq.pop();
        Node *right = pq.top();
        pq.pop();

        int sum = left->freq + right->freq;
        pq.push(CreateNode(NULL, sum, left, right));
    }

    Node* root = pq.top();

    unordered_map<char, string> huffmanCode;
    string str = EncodeCall(root, EMPTY_STRING, huffmanCode,text);
    string output = DecodeCall(root, str, EMPTY_STRING);
    return str;
}

int main()
{
    std::ifstream t("Input.txt");
    std::stringstream buffer;
    buffer << t.rdbuf();
    t.close();


    string Encoded = buildHuffmanTree(buffer.str());
    int i;
    float l = Encoded.length();
    int NeededBits = ceil(l/8) * 8;

    for(i=0;i<NeededBits-l;i++)
    {
        Encoded = "0" + Encoded;
    }




    std::stringstream Stream(Encoded);
    std::string output;
    while(Stream.good())
    {
        std::bitset<8> bits;
        Stream >> bits;
        char c = char(bits.to_ulong());
        output += c;
    }


    std::ofstream file("Compressed.txt");
    file << output;
    file.close();

    char character;
    string CodeString;
    ifstream inputStream;
    inputStream.open("Compressed.txt", ios::in);
    while (inputStream.get(character)) {
        bitset<8> bits(character);
        CodeString += bits.to_string();
    }
    inputStream.close();

    string Decoded = DecodeCall(buildMap(buffer.str()),CodeString,EMPTY_STRING);

    std::ofstream out("Decompressed.txt");
    out << Decoded;
    out.close();
    return 0;
}