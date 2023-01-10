#define FILE_EXTENSION ".txt"
#include<fstream>
#include<string>
#include<cstring>
#include<vector>
#include<iostream>
#include<set>
#include<bits/stdc++.h>

#define ALPHABET 26

using namespace std;

// Utility Func

typedef struct Trie
{
	struct Trie *child[ALPHABET];
	char ch;
	set<int> isEndOfWord;
	set<int> essay;
}TrieNode;

set<int> exact;
vector<string> all_title;
void insert_key(TrieNode* root, string& key, int essay);
set<int> search_key(TrieNode* root, string& key);//exact search

void insert_key(TrieNode *root, string& key, int essay)
{
 
    TrieNode *current = root;
	
	for(auto c : key)
	{
		int x = c - 'a';
		if(current -> child[x] == NULL)
		{
			TrieNode* newnode = new TrieNode();
			newnode -> ch = c;
			current -> child[x] = newnode;
			current -> child[x] -> essay.insert(essay);
		}
		current = current -> child[x];
	}
	current -> isEndOfWord.insert(essay);
}

set<int> search_key(TrieNode* root, string& key)
{
	TrieNode* current = root;
	exact.clear();
	for(auto c : key)
	{
		int x = c - 'a';
		if(current -> child[x] == NULL)
		{
			return exact;
		}
		current = current -> child[x];
	}
	for(auto it : current -> isEndOfWord)
	{
		exact.insert(it);
	}
	return exact;
}
// string parser : output vector of strings (words) after parsing
vector<string> word_parse(vector<string> tmp_string){
	vector<string> parse_string;
	for(auto& word : tmp_string){
		string new_str;
    	for(auto &ch : word){
			if(isalpha(ch))
				new_str.push_back(ch);
		}
		parse_string.emplace_back(new_str);
	}
	return parse_string;
}

vector<string> split(const string& str, const string& delim) {
	vector<string> res;
	if("" == str) return res;
	//先將要切割的字串從string型別轉換為char*型別
	char * strs = new char[str.length() + 1] ; //不要忘了
	strcpy(strs, str.c_str());

	char * d = new char[delim.length() + 1];
	strcpy(d, delim.c_str());

	char *p = strtok(strs, d);
	while(p) {
		string s = p; //分割得到的字串轉換為string型別
		res.push_back(s); //存入結果陣列
		p = strtok(NULL, d);
	}

	return res;
}

vector<string> word_parse_smallL(vector<string> tmp_string){
	vector<string> parse_string;
	for(auto& word : tmp_string){
		string new_str;
    	for(auto &ch : word){
			if(isalpha(ch))
				if(ch < 'a')
					new_str.push_back(ch+32);
				else
					new_str.push_back(ch);
		}
		parse_string.emplace_back(new_str);
	}
	return parse_string;
}

int main(int argc, char *argv[])
{

    // INPUT :
	// 1. data directory in data folder
	// 2. number of txt files
	// 3. output route

    string data_dir = argv[1] + string("/");
	string query = string(argv[2]);
	string output = string(argv[3]);
	TrieNode* root = new TrieNode();
	// Read File & Parser Example


	string file, title_name, tmp;
	fstream fi;
	vector<string> tmp_string;
	char save[512];//for dir name


	for(int i = 0; i < 100; i++)
	{
		sprintf(save, "data/%d.txt", i);
		fi.open(save, ios::in);

		// GET TITLENAME
		getline(fi, title_name);
		//cout << title_name << endl;
		all_title.emplace_back(title_name);

		// GET TITLENAME WORD ARRAY
		tmp_string = split(title_name, " ");

		vector<string> title = word_parse_smallL(tmp_string);

		for(auto it : title){
			//cout << it << " ";
			insert_key(root, it, i);
		}

		// GET CONTENT LINE BY LINE
		while(getline(fi, tmp))
		{

			// GET CONTENT WORD VECTOR
			tmp_string = split(tmp, " ");

			// PARSE CONTENT
			vector<string> content = word_parse_smallL(tmp_string);

			for(auto it : content)
			{
				//cout << it << endl;
				insert_key(root, it, i);
			}
			//......
		}

		// CLOSE FILE
		fi.close();

	}
	// from data_dir get file ....
	// eg : use 0.txt in data directory
	fi.open(argv[2], ios::in);

	fstream fo;
	fo.open(argv[3], ios::out);

	while(getline(fi, tmp))
	{
		//cout << tmp << endl;
		tmp_string = split(tmp, " ");

		vector<string> content = word_parse_smallL(tmp_string);
		for(auto it : content)
		{
			//cout << it << endl;
			search_key(root, it);
		}
		for(auto it : exact)
		{
			fo << all_title[it] << endl;
		}
	}
	
	fi.close();
	fo.close();
	return 0;
}


// 1. UPPERCASE CHARACTER & LOWERCASE CHARACTER ARE SEEN AS SAME.
// 2. FOR SPECIAL CHARACTER OR DIGITS IN CONTENT OR TITLE -> PLEASE JUST IGNORE, YOU WONT NEED TO CONSIDER IT.
//    EG : "AB?AB" WILL BE SEEN AS "ABAB", "I AM SO SURPRISE!" WILL BE SEEN AS WORD ARRAY AS ["I", "AM", "SO", "SURPRISE"].
// 3. THE OPERATOR IN "QUERY.TXT" IS LEFT ASSOCIATIVE
//    EG : A + B / C == (A + B) / C

//

//////////////////////////////////////////////////////////
