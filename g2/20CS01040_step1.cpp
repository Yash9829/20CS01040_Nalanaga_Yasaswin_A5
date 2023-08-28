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

            // token[3] to token.size()

            t = "";
            for(int i = 3; i < token.size(); i++) {

                t += token[i];
                if(token[0] >= 'A' && token[0] <= 'Z')
                rem_chars[token[i]] = 1;

            }

            temp.push_back(t);

        } else {

            for(int i = 0; i < token.size(); i++) {

                if(token[0] >= 'A' && token[0] <= 'Z')
                rem_chars[token[i]] = 1;

            }

            temp.push_back(token);

        }

        count++;

    }
}

void eliminateImmediateLeftRecursion(vector<vector<string>> &grammar, unordered_map<char, int> &rem_chars, int ind){

    char non_terminal = grammar[ind][0][0];
    // char next_non_terminal = '?';
    vector<string> temp1, temp2;
    temp1.push_back(grammar[ind][0]);

    int is_left_recur = 0;

    string new_non_terminal = grammar[ind][0];
    new_non_terminal += '\'';
    temp2.push_back(new_non_terminal);

    for(int i = 1; i < grammar[ind].size(); i++){

        if(grammar[ind][i][0] == non_terminal){

            //we got an immediate left recursion here
            is_left_recur = 1;

            // append remaining part with new non terminal at last
            string t = "";
            for(int j = 1; j < grammar[ind][i].size(); j++){

                t += grammar[ind][i][j];

            }

            t += new_non_terminal;

            temp2.push_back(t);

        } else {

            string t = grammar[ind][i];
            t += new_non_terminal;
            temp1.push_back(t);

        }
    }

    temp2.push_back("ϵ");

    if(is_left_recur == 0){

        return ;

    } else {

        grammar[ind] = {};
        // grammar[ind].push_back(temp1);
        for(int i = 0; i < temp1.size(); i++){

            grammar[ind].push_back(temp1[i]);

        }

        grammar.push_back(temp2);

    }
}
void eliminateLeftRecursion(vector<vector<string>> &grammar){

    unordered_map<char, int> visited;
    for(int i = 0; i < grammar.size(); i++){

        vector<string> temp;
        temp.push_back(grammar[i][0]);

        for(int k = 1; k < grammar[i].size(); k++){
            
            if(grammar[i][k][0] >= 'A' && grammar[i][k][0] <= 'Z' && visited[grammar[i][k][0]] != 0){

                //we got a non terminal at left most side and we have visited it
                //we have to substitute this non terminal with the productions of this non terminal itself and the remianing part of the rule

                string rem_part = "";
                // copy the remaining part
                for(int l = 1; l < grammar[i][k].size(); l++){

                    rem_part += grammar[i][k][l];

                }
                
                //for every production in non terminal grammar[i][k][0] append rem_part and push it into temp
                //the productions of gramar[i][k][0] are present in visited[grammar[i][k][0]] th line of grammar

                int non_term_pos = visited[grammar[i][k][0]] - 1;
                for(int l = 1; l < grammar[non_term_pos].size(); l++){
                    
                    string new_production = grammar[non_term_pos][l]; //take a rule
                    new_production += rem_part;//concantenate the remaining part
                    temp.push_back(new_production);//push it into temp

                }

                //we have successfully pushed the new production rules eliminating the non terminal

            } else {

                temp.push_back(grammar[i][k]);

            }
        }

        grammar[i] = {};// make this line empty
        // grammar[i].push_back(temp);

        //now push the above calculated line
        for(int j = 0; j < temp.size(); j++){

            grammar[i].push_back(temp[j]);

        }

        //now we have to eliminate immediate left recursion of the pushed line
        eliminateImmediateLeftRecursion(grammar, rem_chars, i);

        visited[grammar[i][0][0]] = i + 1;

    }
}
int main() {

    // cout << "NOTE 1: The implementation is for non cyclic and no epsilon production grammar\n";
    // cout << "NOTE 2: In the input each non terminal should occur only once in the LHS, write pipe separated productions in such cases\n";
    // cout << "NOTE 3: Non terminals are from A to Z only, remaining all considered as terminals\n";
    // cout << "NOTE 4: The terminals of the grammar should not start with capital letters\n";
    // cout << "Enter the no.of lines in the grammar\n";
    
    int n;
    cin >> n;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    int count = 0;

    unordered_map<char, int> rem_chars;

    vector<vector<string>> grammar;
    cout << "enter a non cyclic, no epsilon production grammar\n";
    for(int i = 0; i < n; i++) {
        
        string str;
        getline(cin, str);
        vector<string> temp;
        processInput(str, temp, rem_chars);
        grammar.push_back(temp);

    }

    cout << "The processed grammar \n";
    for(int i = 0; i < n; i++){

        for(int j = 0; j < grammar[i].size(); j++){

            cout << grammar[i][j] << " ";

        }
        cout << endl;
    }

    //the algo start from a non terminal and look for the non terminals below it
    eliminateLeftRecursion(grammar);

    cout << "*************The Output************************\n\n";
    cout << "The grammar after removing left recursion is as shown below : \n";
    for(int i = 0; i < grammar.size(); i++){

        cout << grammar[i][0] << " -> ";
        for(int j = 1; j < grammar[i].size(); j++){

            if(j != grammar[i].size() - 1)
            cout << grammar[i][j] << " | ";
            else
            cout << grammar[i][j];

        }

        cout << "\n";

    }


}
