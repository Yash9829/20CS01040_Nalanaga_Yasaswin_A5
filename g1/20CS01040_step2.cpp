#include<bits/stdc++.h>
#include<sstream>
using namespace std;

string removeSpaces(string str) {

    string temp = "";
    for(int i = 0; i < str.size(); i++) {

        if(str[i] != ' '){

            temp += str[i];

        }
    }
    return temp;
}

string findSamePrefix(vector<string> temp) {

    // find the prefix that occurs atleast twice in the given rule
    int max_length = 0;
    int first = -1;
    for(int i = 1; i < temp.size(); i++) {

        for(int k = i + 1; k < temp.size(); k++) {

            int count = 0;
            for(int l = 0; l < min(temp[k].size(), temp[i].size()); l++) {

                if(temp[i][l] != temp[k][l]) {

                    break;

                }

                count++;

            }

            max_length = max(max_length, count);

            if(max_length != 0){

                first = i;
                break;
            }
        }

        if(max_length != 0){

            break;
        }

        
    }

    string prefix = "";
    if (first == -1) {

        // cout << "Hi I am prefix returned at findPrefix: " << prefix << endl;
        return prefix;

    } else {

        for(int i = 0; i < max_length; i++) {

            prefix += temp[first][i];

        }

        // cout << "Hi I am prefix returned at findPrefix: " << prefix << endl;
        return prefix;

    }

}

vector<string> eliminateSamePrefix(string prefix,vector<string> &temp, unordered_map<char, int> &rem_chars) {

    int flag = 0, first = 0;
    //new productions after eliminating the common prefix in the productions
    vector<string> new_productions, temp2;
    temp2.push_back(temp[0]);
    int next_char;
    for(int i = 0; i < 26; i++){

        char c = char('A' + i);
        if(rem_chars[c] == 0){

            next_char = i;
            break ;

        }

    }

    char c = (char)('A' + next_char);
    string C = "";
    C += c;

    for(int i = 1; i < temp.size(); i++){

        flag = 0;
        string temp_string = "";
        if(prefix.size() > temp[i].size()){

            temp2.push_back(temp[i]);
            continue ;

        }

        for(int j = 0; j < prefix.size(); j++) {

            if(temp[i][j] != prefix[j]) //common prefix mismatch
            {
                flag = 1;
                break;
            }

        }

        if(flag == 1){

            temp2.push_back(temp[i]);
            continue ;

        }

        //take the remaining string and form a new production rules
        int n = temp[i].size();

        if(first == 0)
        new_productions.push_back(C);


        if(first == 0){

            temp_string = prefix;
            temp_string += c;
            temp2.push_back(temp_string);
            first++;
            rem_chars[char('A' + next_char)] = 1;

        }

        string t = "";
        for(int k = prefix.size(); k < temp[i].size(); k++) {

            t += temp[i][k];

        }

        if(prefix.size() < n){

            new_productions.push_back(t);

        } else {

            if(prefix.size() == n){

                new_productions.push_back("ϵ");

            }
        }

    }

    temp = {};
    for(int i = 0; i < temp2.size(); i++){

        temp.push_back(temp2[i]);

    }

    return new_productions;
}

// abcD|abc
void doLeftFactor(vector<string> curr_rule, vector<vector<string>> &new_grammar, unordered_map<char, int> &rem_chars) {

    // take the curr rule and left factor it, find the common prefix
    // trie is a good idea to implement this but, I will implement without it, deadline is mon and also I got other things to look at
    new_grammar.push_back(curr_rule);
    bool is_same_prefix = false;
    for(int i = 0; i < new_grammar.size(); i++) {

        string prefix = findSamePrefix(new_grammar[i]);
        if(prefix.size() == 0){

            is_same_prefix = false;

        } else {

            is_same_prefix = true;

        }

        while(is_same_prefix) {

            new_grammar.push_back(eliminateSamePrefix(prefix, new_grammar[i], rem_chars));
            prefix = findSamePrefix(new_grammar[i]);
            if(prefix.size() == 0){

                is_same_prefix = false;
                break;

            } else {

                is_same_prefix = true;

            }

        }

    }
    return ;
}

void processInput(string str, vector<string> &temp, unordered_map<char, int> &rem_chars) {

    string k = removeSpaces(str);
    string token;
    istringstream iss(k);
    int count = 0;
    while(getline(iss, token, '|')) {

        if(count == 0) {

            string t = "";
            t = t + token[0];
            
            if(token[0] >= 'A' && token[0] <= 'Z')
            rem_chars[token[0]] = 1;

            temp.push_back(t);

            t = "";
            for(int i = 3; i < token.size(); i++) {

                t += token[i];

                if(token[i] >= 'A' && token[i] <= 'Z')
                rem_chars[token[i]] = 1;

            }

            temp.push_back(t);

        } else {

            for(int i = 0; i < token.size(); i++){

                if(token[i] >= 'A' && token[i] <= 'Z')
                rem_chars[token[i]] = 1;

            } 

            temp.push_back(token);

        }

        count++;

    }
}

int main() {

    cout << "NOTE 1: General instructions, non terminals are capital letters with single character\n";
    cout << "NOTE 2: General instructions, terminals are small letters with single character\n";
    cout << "\nEnter the no.of lines in the grammar\n";

    int n;
    cin >> n;
    int count = 0;

    unordered_map <char, int> rem_chars;
    vector<vector<string>> grammar;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Enter grammar productions\n";
    // fflush(stdin);
    for(int i = 0; i < n; i++) {
        
        string str;
        // fflush(stdin);
        getline(cin, str);

        vector<string> temp;
        processInput(str, temp, rem_chars);

        grammar.push_back(temp);

    }

    vector<vector<string>> new_grammar;
    // int back = 0;
    for(int i = 0; i < grammar.size(); i++) {

        //find longest prefix among all the productions of a non terminal
        doLeftFactor(grammar[i], new_grammar, rem_chars);

    }

    cout << "**************The output*****************\n";
    cout << "\nThe required left factored grammar is : \n";
    for(int i = 0; i < new_grammar.size(); i++) {

        cout << new_grammar[i][0] <<" -> ";
        for(int j = 1; j < new_grammar[i].size(); j++){

            if(j != new_grammar[i].size() - 1)
            cout << new_grammar[i][j] << " | ";
            else
            cout << new_grammar[i][j];

        }
        cout << endl;
    }


}

// Test cases(Gate questions)
// A -> aAB | aBc | aAc

// S -> aSSbS | aSaSb | abb | b

// S->iEtS|iEtSeS|a 
// E->b