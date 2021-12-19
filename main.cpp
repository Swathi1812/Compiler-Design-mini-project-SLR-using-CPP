/******************************************************************************

                              Online C++ Compiler.
               Code, Compile, Run and Debug C++ program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include<bits/stdc++.h>
#define error(x) cerr<<#x<<" = "<<x<<'\n'
using namespace std;

set<char> ss;
map<char,vector<vector<char>>> mp;
bool dfs(char i, char org, char last, map<char,vector<vector<char>>> &mp){
    bool rtake = false;
    for(auto r : mp[i]){
        bool take = true;
        for(auto s : r){
            if(s == i) break;
            if(!take) break;
            if(!(s>='A'&&s<='Z')&&s!='e'){
                ss.insert(s);
                break;
            }
            else if(s == 'e'){
                if(org == i||i == last)
                ss.insert(s);
                rtake = true;
                break;
            }
            else{
                take = dfs(s,org,r[r.size()-1],mp);
                rtake |= take;
            }
        }
    }
    return rtake;
}

map<int,map<char,set<pair<deque<char>,deque<char>>>>> f;
map<int,vector<pair<int,char>>> g;

int num = -1;
void dfs2(char c, char way, int last, pair<deque<char>,deque<char>> curr){
    map<char,set<pair<deque<char>,deque<char>>>> mp2;
    int rep = -2;
    if(last != -1){
        for(auto q : g[last]){
            if(q.second == way){
                rep = q.first;
                mp2 = f[q.first];
            }
        }
    }
    mp2[c].insert(curr);
    int count = 10;
    while(count--){
        for(auto q : mp2){
            for(auto r : q.second){
                if(!r.second.empty()){
                    if(r.second.front()>='A'&&r.second.front()<='Z'){
                        for(auto s : mp[r.second.front()]){
                            deque<char> st,emp;
                            for(auto t : s) st.push_back(t);
                            mp2[r.second.front()].insert({emp,st});
                        }
                    }
                }
            }
        }
    }
    for(auto q : f){
        if(q.second == mp2){
            g[last].push_back({q.first,way});
            return;
        }
    }
    if(rep == -2){
        f[++num] = mp2;
        if(last != -1)
        g[last].push_back({num,way});
    }
    else{
        f[rep] = mp2;
    }
    int cc = num;
    for(auto q : mp2){
        for(auto r : q.second){
            if(!r.second.empty()){
                r.first.push_back(r.second.front());
                r.second.pop_front();
                dfs2(q.first,r.first.back(),cc,r);
            }
        }
    }
}


int main(){
    int i,j;
    ifstream fin("inputslr.txt");
    string num;
    vector<int> fs;
    vector<vector<int>> a;
    char start;
    bool flag = 0;
    cout<<"Grammar: "<<'\n';
    while(getline(fin,num)){
        if(flag == 0) start = num[0],flag = 1;
        cout<<num<<'\n';
        vector<char> temp;
        char s = num[0];
        for(i=3;i<num.size();i++){
            if(num[i] == '|'){
                mp[s].push_back(temp);
                temp.clear();
            }
            else temp.push_back(num[i]);
        }
        mp[s].push_back(temp);
    }
    map<char,set<char>> fmp;
    for(auto q : mp){
        ss.clear();
        dfs(q.first,q.first,q.first,mp);
        for(auto g : ss) fmp[q.first].insert(g);
    }

    cout<<'\n';
    cout<<"FIRST: "<<'\n';
    for(auto q : fmp){
        string ans = "";
        ans += q.first;
        ans += " = {";
        for(char r : q.second){
            ans += r;
            ans += ',';
        }
        ans.pop_back();
        ans+="}";
        cout<<ans<<'\n';
    }

    map<char,set<char>> gmp;
    gmp[start].insert('$');
    int count = 10;
    while(count--){
        for(auto q : mp){
            for(auto r : q.second){
                for(i=0;i<r.size()-1;i++){
                    if(r[i]>='A'&&r[i]<='Z'){
                        if(!(r[i+1]>='A'&&r[i+1]<='Z')) gmp[r[i]].insert(r[i+1]);
                        else {
                            char temp = r[i+1];
                            int j = i+1;
                            while(temp>='A'&&temp<='Z'){
                                if(*fmp[temp].begin()=='e'){
                                    for(auto g : fmp[temp]){
                                        if(g=='e') continue;
                                        gmp[r[i]].insert(g);
                                    }
                                    j++;
                                    if(j<r.size()){
                                        temp = r[j];
                                        if(!(temp>='A'&&temp<='Z')){
                                            gmp[r[i]].insert(temp);
                                            break;
                                        }
                                    }
                                    else{
                                        for(auto g : gmp[q.first]) gmp[r[i]].insert(g);
                                        break;
                                    }
                                }
                                else{
                                    for(auto g : fmp[temp]){
                                        gmp[r[i]].insert(g);
                                    }
                                    break;
                                }
                            }
                        }
                    }
                }
                if(r[r.size()-1]>='A'&&r[r.size()-1]<='Z'){
                    for(auto g : gmp[q.first]) gmp[r[i]].insert(g);
                }
            }
        }
    }

    cout<<'\n';
    cout<<"FOLLOW: "<<'\n';
    for(auto q : gmp){
        string ans = "";
        ans += q.first;
        ans += " = {";
        for(char r : q.second){
            ans += r;
            ans += ',';
        }
        ans.pop_back();
        ans+="}";
        cout<<ans<<'\n';
    }
    string temp = "";
    temp+='.';
    temp+=start;

    deque<char> emp;
    deque<char> st;
    st.push_back(start);
    dfs2('!','k',-1,{emp,st});

    cout<<"\nProductions: "<<'\n';
    int cc = 1;
    set<char> action,go;
    map<pair<char,deque<char>>,int> pos;
    for(auto q : mp){
        go.insert(q.first);
        for(auto r : q.second){
            cout<<"r"<<cc<<": ";
            string ans = "";
            ans += q.first;
            ans+="->";
            deque<char> temp;
            for(auto s : r) ans += s,temp.push_back(s);
            pos[{q.first,temp}] = cc;
            for(auto s : r){
                if(s>='A'&&s<='Z') go.insert(s);
                else action.insert(s);
            }
            cout<<ans<<'\n';
            cc++;
        }
    }

    // for(auto x : action){
    //     cout<<x<<" "<<endl;
    // }
    // cout<<endl<<endl;
    // for(auto x : go){
    //     cout<<x<<" "<<endl;
    // }
//     map<int,map<char,set<pair<deque<char>,deque<char>>>>> f;
// map<int,vector<pair<int,char>>> g;
    cout<<"\nGraph: "<<'\n';
    for(auto mp2 : f){
        cout<<'\n';
        cout<<"I";
        cout<<mp2.first<<": \n";
        for(auto q : mp2.second){
            string ans = "";
            ans += q.first;
            ans += "->";
            for(auto r : q.second){
                for(auto t : r.first) ans+=t;
                ans+='.';
                for(auto t : r.second) ans+=t;
                ans+='|';
            }
            ans.pop_back();  
            for(auto tt : ans){
                if(tt == '!') cout<<start<<'\'';
                else cout<<tt;
            }
            cout<<'\n';
        }
    }
    cout<<'\n';
    cout<<"Edges: "<<'\n';
    for(auto q : g){
        for(auto r : q.second){
            cout<<"I"<<q.first<<" -> "<<r.second<<" -> "<<"I"<<r.first<<"\n";
        }
    }
    action.insert('$');
    cout<<"\nParsing Table:"<<'\n';
    cout<<"St.\t\tAction & Goto"<<'\n';
    int tot = f.size();
    cout<<"  \t";
    map<int , map<char,string>> parseTable;
    for(auto q : action) cout<<q<<'\t';
    for(auto q : go) cout<<q<<'\t';
    cout<<'\n';
    for(i=0;i<tot;i++){
        cout<<"I"<<i<<'\t';
        for(auto q : action){
            if(g.count(i)){
                int flag = 0;
                for(auto r : g[i]){
                    if(r.second == q){
                        flag = 1;
                        cout<<"S"<<r.first<<"\t";
                        map<char,string> temp = parseTable[i];
                        string s = "S" + to_string(r.first);
                        temp[q] = s;
                        parseTable[i] = temp;
                       
                        break;
                    }
                }
                if(!flag) cout<<"-"<<'\t';
            }
            else{
                int flag = 0;
                for(auto r : f[i]){
                    if(r.first == '!'){
                        if(q == '$'){
                        cout<<"AC\t";
                        map<char,string> temp = parseTable[i];
                        string s = "AC";
                        temp[q] = s;
                        parseTable[i] = temp;
                        flag = 1;
                        }
                        else cout<<"-\t";
                    }  

                }
                if(!flag){
                    for(auto r : f[i]){
                        char ccc = r.first;
                        deque<char> chk = (*r.second.begin()).first;
                        int cou = 1;
                        for(auto r : gmp[ccc]){
                            if(q == r){
                                cout<<"r"<<pos[{ccc,chk}]<<"\t";
                                map<char,string> temp = parseTable[i];
                                string s = "r" + to_string(pos[{ccc,chk}]);
                                temp[q] = s;
                                parseTable[i] = temp;
                            }
                            cou++;
                        }
                    }
                }
            }
        }
        for(auto q : go){
            if(g.count(i)){
                int flag = 0;
                for(auto r : g[i]){
                    if(r.second == q){
                        flag = 1;
                        cout<<r.first<<"\t";
                        map<char,string> temp = parseTable[i];
                        string s = to_string(r.first);
                        temp[q] = s;
                        parseTable[i] = temp;
                        break;
                    }
                }
                if(!flag) cout<<"-"<<'\t';
            }
            else{
                cout<<"-"<<'\t';
            }
        }
        cout<<"\n'\n'";

      


    }
    //   for(auto x : parseTable){
    //         cout<<"I"<<x.first<<" "<<endl;
    //         for(auto y : x.second){
    //             cout<<y.first<<"="<<y.second<<endl;
    //         }

    //     }

    string inputString;
    cout<<"enter the input string"<<endl;
    cin>>inputString;
    inputString.push_back('$');
    stack<string> stk;
    stk.push("0");
    int starrt = 0;
    int length = inputString.length();
    
    cout<<"stack\t\t\tinput\t\t\taction"<<endl; 

    while(starrt < length){
        //find which operation
        char inputChar = inputString[starrt];
        int state = stoi(stk.top());

        stack<string> ss = stk;
        while(!ss.empty()){
            cout<<ss.top()<<" ";
            ss.pop();
        }
        cout<<"\t\t"<<inputString.substr(starrt,inputString.length()-starrt);
       
        string operation;
        map<char,string> listOfActionAndGoto = parseTable[state];
        if(listOfActionAndGoto.find(inputChar) != listOfActionAndGoto.end()){
            operation = listOfActionAndGoto[inputChar];
            if(operation.compare("AC")==0){
                cout<<endl<<"---------------------accepted !!----------------------"<<endl;
                break;
           
            }
           
        }

        if(operation[0] == 'S'){
            stk.push(to_string(inputChar));
            stk.push(operation.substr(1,operation.length()-1));
            cout<<"\t\t\t"<<"shift"<<operation.substr(1,operation.length()-1)<<endl;
            starrt++;

        }
        else if(operation[0] == 'r'){
            // map<char,vector<vector<char>>> mp;
            //get which production
            char nonTerminal;
            int productionSize;
            int count = stoi(operation.substr(1,operation.length()-1));
            
            for(auto productionSet : mp){
            
                for(int i = 0 ; i < productionSet.second.size() ; i++){
                    
                   if(--count == 0){
                      cout<<"\t\t\treduce "<<productionSet.first<<"->";
                       productionSize = productionSet.second[i].size();
                       nonTerminal = productionSet.first;
                       for(int k = 0 ; k < productionSet.second[i].size() ; k++){
                           cout<<productionSet.second[i][k];
                       }
                       cout<<endl;
                       break;
                   }
                }
            }

            // pop 2*productionSize
            int sc = 0;
           
            while(2*productionSize != sc){
                stk.pop();
                sc++;
            }
            
            
            int state = stoi(stk.top());
            stk.push(to_string(nonTerminal));
            
            map<char,string> listOfActionAndGoto = parseTable[state];
            for(auto p : listOfActionAndGoto){
                
                if(p.first == nonTerminal){
                    stk.push(p.second);
                    break;
                }

            }
        }
     

    }

    return 0;
}
