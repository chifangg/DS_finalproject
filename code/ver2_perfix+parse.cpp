#define FILE_EXTENSION ".txt"
#include<fstream>
#include<string>
#include<cstring>
#include<vector>
#include<iostream>
#include<set>
#include<bits/stdc++.h>
#include<algorithm>

#define ALPHABET 26

using namespace std;

// Utility Func

typedef struct Trie
{
	struct Trie *child[ALPHABET];
	char ch;
	set<int> isEndOfWord;
	set<int> essay;//prefix
}TrieNode;

set<int> exact;
set<int> prefix;
set<int> suffix;
vector<int> command;

vector<string> all_title;
void insert_key(TrieNode* root, string& key, int essay);
set<int> search_key(TrieNode* root, string& key);//exact search
set<int> search_key_prefix(TrieNode* root, string& key);//prefix search
vector<string> query_to_command(vector<string> tmp_string);
void DFS(TrieNode* root);

void DFS(TrieNode* root)
{
	for(auto it : root -> essay)
		prefix.insert(it);
	for(int i = 0; i < ALPHABET; i++)
	{
		if(root -> child[i] != NULL)
			DFS(root -> child[i]);
	}
}

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

set<int> search_key_prefix(TrieNode* root, string& key)
{
	TrieNode* current = root;
	prefix.clear();
	for(auto c : key)
	{
		int x = c-'a';
		if(current -> child[x] == NULL)
		{
			return prefix;
		}
		current = current -> child[x];
	}
	DFS(current);
	return prefix;
}


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


vector<string> query_to_command(vector<string> tmp_string)
{
	//command 0->exact 1->prefix 2->suffix 3->or 4->and
	vector<string> parse;
	command.clear();
	for(auto& word : tmp_string)
	{
		string new_s;
		int isleft = 0;
		for(auto &c : word)
		{
			if(c == '"' && isleft == 0)
			{
				command.push_back(0);
				isleft = 1;
			}
			else if(c == '*' && isleft == 0)
			{
				command.push_back(2);
				isleft = 1;
			}
			else if(isalpha(c))
			{
				if(c < 'a')
					new_s.push_back(c+32);
				else
					new_s.push_back(c);
			}
			else if(c == '/' && isleft == 0)
			{
				command.push_back(3);
				isleft = 1;
			}
			else if(c == '+' && isleft == 0)
			{
				command.push_back(4);
				isleft = 1;
			}
		}
		if(isleft == 0)
			command.push_back(1);
		if(new_s.size() != 0)
			parse.emplace_back(new_s);
	}
	return parse;
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

	set<int> filter;
	int i = 0;
	while(1)
	{
		sprintf(save, "%s%d.txt", data_dir.c_str(), i);
		fi.open(save, ios::in);
		cout << i << " ";
		//i++;
		if(!fi) break;

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
		i++;

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

		vector<string> content = query_to_command(tmp_string);
		// for(auto it : content)
		// {
		// 	//cout << it << endl;
		// 	search_key(root, it);
		// }
		// for(auto it : exact)
		// {
		// 	fo << all_title[it] << endl;
		// }

		int cnt = 0;
		int start = 1;
		set<int> usetmp;
		set<int> usetmp2;

		for(auto it : content)
		{
			if(command[cnt] == 0)//exact
			{
				if(start)
				{
					filter = search_key(root, it);
					cnt++;
					start = 0;
				}
			}
			else if(command[cnt] == 1)//prefix
			{
				if(start)
				{
					filter = search_key_prefix(root, it);
					cnt++;
					start = 0;
				}
			}
			else if(command[cnt] == 2){}
			else if(command[cnt] == 3)
			{
				cnt++;
				if(command[cnt] == 0)//ex
				{
					usetmp = search_key(root, it);
					cnt++;
					start = 0;
				}
				else if(command[cnt] == 1)//pr
				{
					usetmp = search_key_prefix(root, it);
					cnt++;
					start = 0;
				}
				usetmp2 = filter;
				filter.clear();
				set_union(usetmp.begin(), usetmp.end(), usetmp2.begin(), usetmp2.end(), inserter(filter, filter.begin()));
			
			}
			else if(command[cnt] == 4)//and
			{
				cnt++;
				if(command[cnt] == 0)//ex
				{
					usetmp = search_key(root, it);
					cnt++;
					start = 0;

				}
				else if(command[cnt] == 1)//pr
				{
					usetmp = search_key_prefix(root, it);
					cnt++;
					start = 0;
				}
				usetmp2=filter;
    			filter.clear();
    			set_intersection(usetmp.begin(), usetmp.end(), usetmp2.begin(), usetmp2.end(), inserter(filter, filter.begin()));
			}
		}
		for(auto it : filter)
		{
			fo << all_title[it] << endl;
		}
		if(filter.empty())
		{
			fo << "Not Found!\n";
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
