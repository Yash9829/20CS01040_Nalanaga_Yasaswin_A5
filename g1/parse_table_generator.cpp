#include <bits/stdc++.h>
#include <regex>
using namespace std;

string removeSpaces(string str)
{

    string temp = "";
    for (int i = 0; i < str.size(); i++)
    {

        if (str[i] != ' ')
        {

            temp += str[i];
        }
    }
    return temp;
}

void processInput(string str, vector<string> &temp)
{

    int n = str.size();
    // cout << "******************\n";
    // cout << str << "\n";
    string t = "";
    // cout << str[0] << "&&&&&&&&&&&&\n";
    for (int i = 0; i < n;)
    {

        if (i + 1 < n && str[i] == '-' && str[i + 1] == '>')
        {

            i = i + 2; // skip '>' also
            // cout << t << " ";
            temp.push_back(t);
            t = "";

            // cout << temp[0] << "hiii\n";
            continue;
        }

        if (str[i] == '|')
        {

            // cout << t << " ";
            temp.push_back(t);
            t = "";
            i++;
            continue;
        }

        t = t + str[i];
        // cout << t << " " <<i << endl;
        // cout << str[i] << " " << i << " " << t <<endl;
        i++;
    }
    // cout << t << " ";
    temp.push_back(t);
    // cout << endl;
}

void removeTrailingSpaces(vector<vector<string>> &grammar)
{

    for (int i = 0; i < grammar.size(); i++)
    {

        for (int j = 0; j < grammar[i].size(); j++)
        {

            // grammar[i][j] = grammar[i][j].find_first_not_of(" ");
            // grammar[i][j] = grammar[i][j].find_last_not_of(" ");
            regex pattern("^\\s+|\\s+$"); // Matches leading and trailing spaces
            grammar[i][j] = regex_replace(grammar[i][j], pattern, "");
            cout << grammar[i][j] << " ";
        }

        cout << endl;
    }
}

set<string> findFirstOf(string curr, map<string, set<string>> &first, map <string, int> &visited,  map<string, int> is_non_term, vector<vector<string>> grammar)
{

    visited[curr] = 1;
    for (int i = 0; i < grammar[is_non_term[curr] - 1].size(); i++)
    {

        // i picked a production
        string t = "";
        for (int j = 0; j < grammar[is_non_term[curr] - 1][i].size(); j++)
        {
            if (grammar[is_non_term[curr] - 1][i][j] == ' ')
            {
                if (visited[t] == 0)
                {
                    visited[t] = 1;
                    if (is_non_term[t] == 0)
                    {
                        if (first[t].find("epsilon") == first[t].end())
                        {
                            for(auto x : first[t])
                            first[curr].insert(x);

                            break;
                        }
                        else
                        {
                            for(auto x : first[t])
                            first[curr].insert(x);

                            first[curr].erase("epsilon");
                        }
                    }
                    else
                    {
                        // non terminal, unvisited = headache
                        set<string> temp = findFirstOf(t, first, visited, is_non_term, grammar);
                        if (temp.find("epsilon") == temp.end())
                        {
                            for(auto x : temp)
                            first[curr].insert(x);

                            break;
                        }
                        else
                        {
                            for(auto x : temp)
                            first[curr].insert(x);

                            first[curr].erase("epsilon");
                            // first[curr].insert(temp);
                        }
                        // first[curr].insert(findFirstOf(t, first_set, visited, is_non_term, grammar));
                    }
                }
                else
                {
                    set<string> temp = first[t];
                    if (first[t].find("epsilon") == first[t].end())
                    {
                        for(auto x : first[t])
                        first[curr].insert(x);

                        break;
                    }
                    else
                    {
                        for(auto x : first[t])
                        first[curr].insert(x);

                        first[curr].erase("epsilon");
                    }
                }

                t = "";
            }
            t = t + grammar[is_non_term[curr] - 1][i][j];
        }

        if (visited[t] == 0)
        {
            visited[t] = 1;
            if (is_non_term[t] == 0)
            {
                for(auto x : first[t])
                first[curr].insert(x);
            }
            else
            {
                // non terminal, unvisited = headache
                set<string> temp = findFirstOf(t, first, visited, is_non_term, grammar);
                for(auto x : temp)
                first[curr].insert(x);
                // first[curr].insert(findFirstOf(t, first_set, visited, is_non_term, grammar));
            }
        }
        else
        {
            for(auto x : first[t])
            first[curr].insert(x);
        }
    }

    return first[curr];
}

void calculateFirst(vector<vector<string>> grammar, map<string, set<string>> &first_set, map<string, int> is_non_term)
{

    // algorithm to calculate first set
    map<string, int> visited;
    cout << "The first sets are as shown below\n";
    for (int i = 0; i < grammar.size(); i++)
    {

        if (visited[grammar[i][0]] == 0)
            findFirstOf(grammar[i][0], first_set, visited, is_non_term, grammar);

        cout << grammar[i][0] << " : ";
        for(auto x : first_set[grammar[i][0]]){

            cout << x << " ";

        }
        cout << endl;
    }
}

int main()
{

    ifstream cin("GrammarLL.txt");
    // ofstream cout("output.txt");
    int n;
    cin >> n;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    vector<vector<string>> grammar(n, vector<string>());
    map<string, int> is_non_term;
    for (int i = 0; i < n; i++)
    {
        string temp;
        getline(cin, temp);
        // take a production and store it in a cell
        cout << temp << "\n";
        processInput(temp, grammar[i]);
    }

    removeTrailingSpaces(grammar);
    cout << "\nThe grammar is : \n";
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < grammar[i].size(); j++)
        {   
            if(j == 0){

                is_non_term[grammar[i][j]] = i + 1;

            }
            cout << grammar[i][j] << " ";
        }
        cout << endl;
    }

    // now calculate first and follow of all non terminals and store them in a data structure
    int no_of_terms;
    cin >> no_of_terms;
    map<string, int> is_terminal;
    map<int, string> terminal_id;
    // int count = 0;
    vector<vector<string>> parse_table(n + 1, vector<string>(no_of_terms + 2, ""));
    for (int i = 0; i < no_of_terms; i++)
    {
        string temp;
        cin >> temp;
        is_terminal[temp] = i + 1;
        terminal_id[i + 1] = temp;
        parse_table[0][i + 1] = temp;
    }

    is_terminal["$"] = no_of_terms + 1;
    terminal_id[no_of_terms + 1] = "$";
    parse_table[0][no_of_terms + 1] = "$";

    for (int i = 0; i < n; i++)
    {
        parse_table[i + 1][0] = grammar[i][0];
    }

    map<pair<string, string>, set<string>> parseTable;
    map<string, set<string>> first_set;
    calculateFirst(grammar, first_set, is_non_term);
    parse_table[0][0] = "%";

    // the parse table
    // implement parse table as a 2d map, parse_table[non terminal][terminal] = a production string
    cout << "The parse table is below : \n";
    for (int i = 0; i < n + 1; i++)
    {
        for (int j = 0; j < no_of_terms + 2; j++)
        {
            cout << parse_table[i][j] << "\t";
        }
        cout << "\n";
    }

    // unordered_set<string> first()
    // computeFirst()
}