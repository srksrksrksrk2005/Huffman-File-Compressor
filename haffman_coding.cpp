#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <fstream>
#include <string>

using namespace std;

// Node class for Huffman Tree
class Node {
public:
    int freq;
    char data;
    Node* left;
    Node* right;

    Node(int freq, char data) {
        this->freq = freq;
        this->data = data;
        this->left = NULL;
        this->right = NULL;
    }
};

// Comparator for priority_queue min-heap based on node frequency
struct NodeCompare {
    bool operator()(const Node* a, const Node* b) const {
        return a->freq > b->freq;
    }
};

// Free allocated memory for the tree
void freeTree(Node* root) {
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    delete root;
}

void serialize_dfs(Node* root, string& s) {
    if (root == NULL) {
        s += ",#";
        return;
    }
    s += ("," + to_string(static_cast<int>(static_cast<unsigned char>(root->data))));
    if (root->left == NULL && root->right != NULL) {
        s += ",#";
        serialize_dfs(root->right, s);
    } else if (root->left != NULL && root->right == NULL) {
        serialize_dfs(root->left, s);
        s += ",#";
    } else {
        serialize_dfs(root->left, s);
        serialize_dfs(root->right, s);
    }
}

string serialize(Node* root) {
    string ans = "";
    serialize_dfs(root, ans);
    return ans;
}

void DFS(Node* node, string str, unordered_map<char, string>& compressions) {
    if (node == NULL) return;
    if (node->left == NULL && node->right == NULL) {
        compressions[node->data] = str;
        return;
    }
    DFS(node->left, str + "0", compressions);
    DFS(node->right, str + "1", compressions);
}

string encoding(string s) {
    if (s.empty()) return "";

    unordered_map<char, int> freq;
    unordered_map<char, string> compressions;
    for (auto i : s) {
        freq[i]++;
    }

    // Min-heap priority queue using NodeCompare
    priority_queue<Node*, vector<Node*>, NodeCompare> pq;
    for (auto i : freq) {
        pq.push(new Node(i.second, i.first));
    }

    // Handle single unique character edge case
    if (pq.size() == 1) {
        Node* child = pq.top();
        pq.pop();
        Node* parent = new Node(child->freq, '#');
        parent->left = child;
        pq.push(parent);
    }

    while (pq.size() > 1) {
        Node* left = pq.top();
        pq.pop();
        Node* right = pq.top();
        pq.pop();
        Node* parent = new Node(left->freq + right->freq, '#');
        parent->left = left;
        parent->right = right;
        pq.push(parent);
    }

    Node* root = pq.top();
    DFS(root, "", compressions);

    string compressed_string = "";
    for (auto i : s) {
        compressed_string += compressions[i];
    }

    // Padding length to multiple of 8 bits
    int padding = (8 - compressed_string.length() % 8) % 8;
    for (int i = 0; i < padding; i++) {
        compressed_string += "1";
    }

    string compressed_string_final = "";
    // Converting each 8 bits to character
    for (size_t i = 0; i < compressed_string.length(); i += 8) {
        int num = 0;
        for (int j = 0; j < 8; j++) {
            num = num * 2 + (compressed_string[i + j] - '0');
        }
        compressed_string_final += static_cast<char>(num);
    }

    cout << "Encoded payload byte size: " << compressed_string_final.length() << " bytes" << endl;

    string result = serialize(root) + "\n" + to_string(padding) + "\n" + to_string(compressed_string_final.length()) + "\n" + compressed_string_final;
    freeTree(root);
    return result;
}

void dfs_decode(Node* node, string str, unordered_map<string, char>& decompose) {
    if (node == NULL) return;
    if (node->left == NULL && node->right == NULL) {
        decompose[str] = node->data;
        return;
    }
    dfs_decode(node->left, str + "0", decompose);
    dfs_decode(node->right, str + "1", decompose);
}

Node* deserialize_dfs(const string& s, int& i) {
    if (i < (int)s.length() && s[i] == ',' && s[i + 1] == '#') {
        i += 2;
        return NULL;
    }
    i += 1;
    string num = "";
    while (i < (int)s.length() && s[i] != ',') {
        num += s[i];
        i++;
    }
    if (num.empty()) return NULL;
    Node* root = new Node(0, static_cast<char>(stoi(num)));
    root->left = deserialize_dfs(s, i);
    root->right = deserialize_dfs(s, i);
    return root;
}

Node* deserialize(const string& data) {
    int i = 0;
    return deserialize_dfs(data, i);
}

string decoding(string compressed_data) {
    if (compressed_data.empty()) return "";

    size_t p1 = compressed_data.find("\n");
    if (p1 == string::npos) return "";
    string tree_serialized = compressed_data.substr(0, p1);

    size_t p2 = compressed_data.find("\n", p1 + 1);
    if (p2 == string::npos) return "";
    string padding_str = compressed_data.substr(p1 + 1, p2 - (p1 + 1));

    size_t p3 = compressed_data.find("\n", p2 + 1);
    if (p3 == string::npos) return "";
    string payload_len_str = compressed_data.substr(p2 + 1, p3 - (p2 + 1));

    int padding = stoi(padding_str);
    int payload_len = stoi(payload_len_str);

    string payload = compressed_data.substr(p3 + 1, payload_len);

    unordered_map<string, char> decompose;
    Node* root = deserialize(tree_serialized);
    dfs_decode(root, "", decompose);
    freeTree(root);

    string compressed_string_final = "";
    for (unsigned char c : payload) {
        int num = static_cast<int>(c);
        string binary = "";
        for (int j = 0; j < 8; j++) {
            binary = to_string(num % 2) + binary;
            num /= 2;
        }
        compressed_string_final += binary;
    }

    // Remove padding
    if ((int)compressed_string_final.length() >= padding) {
        compressed_string_final = compressed_string_final.substr(0, compressed_string_final.length() - padding);
    }

    size_t i = 0;
    string decoded_string = "";
    while (i < compressed_string_final.length()) {
        string temp = "";
        while (i < compressed_string_final.length() && decompose.find(temp) == decompose.end()) {
            temp += compressed_string_final[i];
            i++;
        }
        if (decompose.find(temp) != decompose.end()) {
            decoded_string += decompose[temp];
        } else {
            break;
        }
    }
    return decoded_string;
}

int main() {
    string s;
    cout << "Enter the string you want to encode and compress: ";
    getline(cin, s);

    cout << "\nInput string: " << s << "\n" << endl;
    string Encoded_string = encoding(s);
    string decoded_string = decoding(Encoded_string);
    cout << "Decoded string: " << decoded_string << "\n" << endl;

    if (s == decoded_string) {
        cout << "[SUCCESS] Encoding and Decoding matched perfectly!" << endl;
    } else {
        cout << "[ERROR] Mismatch between original and decoded string!" << endl;
    }

    return 0;
}
