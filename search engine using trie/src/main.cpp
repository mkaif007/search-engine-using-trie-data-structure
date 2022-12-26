#include <bits/stdc++.h>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>

using namespace std;


// class for each node in trie
class TrieNode
{
public:
    unordered_map<char, TrieNode *> children;
    bool isEndOfWord;
    //denotes if the letter is the last letter in the word
    TrieNode()
    {
        isEndOfWord = false;
    }
};

class Trie
{
public:
    int total_nodes;
    int total_words;
    TrieNode *root;

    Trie()
    {
        root = new TrieNode();
    }

    void insert(string str);
    TrieNode *search_exact(string str);
    vector<string> search_pre(string str);
};

void Trie::insert(string str)
{
    TrieNode *current = root;

    for (int i = 0; i < str.size(); ++i)
    {
        char ch = str[i];

        //if character exists in the current node
        if (current->children.find(ch) != current->children.end())
        {
            current = current->children[ch];
        }

        //if character does not exist in the current node
        else
        {
            current->children.insert(make_pair(ch, new TrieNode()));
            current = current->children[ch];
        }
    }
    
    current->isEndOfWord = true;
}

TrieNode *Trie::search_exact(string str)
{
    TrieNode *current = root;

    auto start = chrono::high_resolution_clock::now();

    for (int i = 0; i < str.size(); ++i)
    {
        char ch = str[i];
        if (current->children.find(ch) != current->children.end())
        {
            current = current->children[ch];
        }
        //returns null even if single comparison fails
        else
        {
            return NULL;
        }
    }

    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::nanoseconds>(stop - start);
    cout << "\033[32m\n"
         << current->isEndOfWord << " results in " << double(duration.count() / double(1000000)) << " ms.\033[0m\n\n";

    //isEndOfWord : true when the word being searched is exactly found
    return current->isEndOfWord ? current : NULL;
}

void get_words_dfs(TrieNode *current, string pre, vector<string> &results)
{
    if (current == NULL)
    {
        return;
    }
    if (current->isEndOfWord)//Example : (held,help)
    {
        results.push_back(pre);
    }

    for (auto i : current->children)
    {
        get_words_dfs(i.second, (pre + i.first), results);
    }
}

vector<string> Trie::search_pre(string str)
{
    auto start = chrono::high_resolution_clock::now();

    TrieNode *current = root;
    vector<string> results;

    for (int i = 0; i < str.size(); ++i)
    {
        char ch = str[i];

        //traverses till the corresponding characters pattern and text match
        if (current->children.find(ch) != current->children.end())
        {
            current = current->children[ch];
        }
        else
        {
            return results;
        }
    }

    get_words_dfs(current, str, results);

    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::nanoseconds>(stop - start);

    cout << "\033[32m\n"
         << results.size() << " results in " << double(duration.count() / double(1000000)) << " ms.\033[0m\n\n";

    return results;
}


//accessing data from file
vector<string> get_array_from_file(string src)
{
    fstream file(src);

    string word;
    vector<string> words;

    if (!file)
    {
        cerr << "\nCan't Open File " << src << " Some Error Occurred.!\n";
        return words;
    }

    while (!file.eof())
    {
        getline(file, word);
        if (word.size())
            words.push_back(word);
        word = "\0";
    }

    return words;
}

void computeLPSArray(string pat, int M, int *lps)
{
    // length of the previous longest prefix suffix

    //Example : "AABCA" : [0,1,0,0,1]
    int j = 0;

    lps[0] = 0; // lps[0] is always 0

    int i = 1;
    while (i < M)
    {
        if (pat[i] == pat[j])
        {
            j++;
            lps[i] = j;
            i++;
        }
        else 
        {
            if (j != 0)
            {
                j = lps[j - 1];

            }
            else // if (j == 0)
            {
                lps[i] = 0;
                i++;
            }
        }
    }
}

int KMPSearch(string pat, string txt)
{
    int M = pat.size();
    int N = txt.size();

    int lps[M];

    // Preprocess the pattern (calculate lps[] array)
    computeLPSArray(pat, M, lps);

    int i = 0; // index for txt[]
    int j = 0; // index for pat[]
    while (i < N)
    {
        if (pat[j] == txt[i])
        {
            //if values at index i and j match,increment i and j by 1
            j++;
            i++;
        }

        if (j == M)
        {
            return i - j;
            
        }

        // mismatch after j matches
        else if (i < N && pat[j] != txt[i])
        {
            if (j != 0)
                j = lps[j - 1];
            else
                i = i + 1;
        }
    }
    
    return -1;
}

// Universal Search

vector<string> search(vector<string> v, string pat)
{

    vector<string> results;

    auto start = chrono::high_resolution_clock::now();

    for (auto str : v)
    {
        if (KMPSearch(pat, str) != -1)
        {
            results.push_back(str);
        }
    }

    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::nanoseconds>(stop - start);

    cout << "\033[32m\n"
         << results.size() << " results in " << double(duration.count() / double(1000000)) << " ms.\033[0m\n\n";

    return results;
}


int main()
{

    string filename;
    Trie T;
    vector<string> data;

    
    cout << "Enter File name to load data from. ( music | movies | words ) :\n";
    cin >> filename;  //accessing backend
    
    data = get_array_from_file(filename + ".txt");

    for (string word : data)
    {
        T.insert(word);
    }

    int choice;

    do
    {

        cout << "------------------------\n";
        cout << "MENU\n";
        cout << "1. Exact Search.\n";
        cout << "2. Prefix Search.\n";
        cout << "3. Universal Search.\n";
        cout << "0. Exit.\n";

        cin >> choice;
        getchar();
        string pattern; //word to be searched for

        if (choice)
        {
            cout << "Enter Search Text (leave empty to list all) : ";
            getline(cin, pattern);
        }

        switch (choice)
        {
        case 1:
            //exact search
            if (T.search_exact(pattern))
            {
                cout << "String Exists.\n";
            }
            else
            {
                cout << "NOT Found.\n";
            }
            break;
        case 2:
            //prefix search
            for (string w : T.search_pre(pattern))
            {
                cout << w << "\n";
            }
            break;

        case 3:
            //universal search
            for (string w : search(data, pattern))
            {
                cout << w << "\n";
            }
            break;

        default:
            break;
        }

        cout << "\n\npress any key to continue.";
        getchar();
        system("clear");

    } while (choice);

    return 0;
}