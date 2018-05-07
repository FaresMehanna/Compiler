#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include "Regulars.h"

using namespace std;

bool vis[2000][2000];

class Node{
private:
    bool acceptance;
    int label;
    string name;
    int priority;

public:
    Node(int index) {
        label = index;
    }
    vector<pair<RegularTransition , Node*> > transition;
    void setState(bool acceptanceFlag)  {
        acceptance = acceptanceFlag;
    }
    bool getState() {
        return acceptance;
    }

    string getLabelStr() {

        int temp = label;
        string toReturn = "";

        while(temp){
            toReturn += (temp%10 + '0');
            temp = temp/10;
        }

        if(toReturn == "")
            toReturn += '0';

        reverse(toReturn.begin(),toReturn.end());
        return toReturn;
    }

    int getLabel() {
        return label;
    }

    string getName(){
        return name;
    }
    int getPriority(){
        return priority;
    }
    void setName(string expressionName){
        name = expressionName;
    }
    void setPriority(int pr){
        priority = pr;
    }

};


class debug{
public:
    void DFS(Node* Start ,bool vis[2000][2000]){
        for(int i=0;i<Start->transition.size();i++){
            if(!vis[Start->getLabel()][Start->transition[i].second->getLabel()]){
                vis[Start->getLabel()][Start->transition[i].second->getLabel()] = true;
                if(Start->transition[i].second->getState()){
                    cout<<Start->getLabel() << " -> " << Start->transition[i].second->getLabel() << "[label=\""<<Start->transition[i].first.getTransitionName() << " - " << Start->transition[i].second->getPriority() << " - " << Start->transition[i].second->getName() <<"\"]"  << endl;
                }else{
                    cout<<Start->getLabel() << " -> " << Start->transition[i].second->getLabel() << "[label=\""<<Start->transition[i].first.getTransitionName()<<"\"]"  << endl;
                }
                DFS(Start->transition[i].second,vis);
            }
        }
    }
};

class NFA{

private:
    vector<RegularExpressionTokens*> expression;
    Node *startingPoint;
    Node* endingPoint;
    RegularTransition epsilon;
    stack< pair <RegularTransition,pair <Node*,Node*> > >s;
    int index =0;
    RegularTransition trivial;
    int priority;
    string expressionName;

public:
    void setExpressions(vector<RegularExpressionTokens*> rE) {
        expression = rE;
    }
    void setAttributes(string name , int pr){
        expressionName = name;
        priority = pr;
    }
    Node getStartingNode() {
        return *startingPoint;
    }
    Node* convertToNFA(){

        for(int i=0;i<2000;i++){
            for(int j=0;j<2000;j++){
                vis[i][j] = false;
            }
        }
        startingPoint = NULL;
        endingPoint = NULL;


        epsilon.allowEpsilon();
        epsilon.setTransitionName("Epsilon");
        for(int i=0;i<expression.size();i++){
            if((*expression[i]).getType()=="Def"){

                RegularTransition* temp= (RegularTransition*) expression[i];
                Node *first;
                first = new Node(index++);
                Node *second;
                second = new Node(index++);
                if(startingPoint == NULL) {
                    startingPoint = first;
                    endingPoint = second;
                }
                first->transition.push_back(make_pair( *temp,second));
                s.push(make_pair(*temp ,make_pair(first,second)));
            }
            else {
                RegularSymbols* temp= (RegularSymbols*) &(*expression[i]);
                if(temp->getSymbol()=='|'){
                    pair <RegularTransition,pair<Node*,Node*> > two=s.top();
                    s.pop();
                    pair <RegularTransition,pair<Node*,Node*> > one = s.top();
                    s.pop();

                    oring(one.second.first , one.second.second , two.second.first ,two.second.second);
                }
                else if(temp->getSymbol()=='*'){
                    pair <RegularTransition,pair<Node*,Node*> > one=s.top();
                    s.pop();

                    kleen_clouser(one.second.first,one.second.second);
                }
                else if(temp->getSymbol()=='+'){
                    pair <RegularTransition,pair<Node*,Node*>  > one=s.top();
                    s.pop();

                    positive_clouser(one.second.first,one.second.second);
                }
                else if(temp->getSymbol()=='!'){
                    pair <RegularTransition,pair<Node*,Node*> > two=s.top();
                    s.pop();
                    pair <RegularTransition,pair<Node*,Node*> > one = s.top();
                    s.pop();

                    concatenate(one.second.first , one.second.second , two.second.first ,two.second.second);
                }
            }
        }
        while(!s.empty()){
            pair <RegularTransition,pair<Node*,Node*> > top = s.top();
            s.pop();
        }
        endingPoint->setState(true);
        endingPoint->setPriority(priority);
        endingPoint->setName(expressionName);
        return startingPoint;
    }

    void oring (Node* startA , Node* endA , Node* startB , Node* endB){
        Node* temp1 = new Node(index++);
        Node* temp2 =  new Node(index++);

        endA->transition.push_back(make_pair(epsilon , temp2));
        endB->transition.push_back(make_pair(epsilon , temp2));

        temp1->transition.push_back(make_pair(epsilon , startA));
        temp1->transition.push_back(make_pair(epsilon , startB));


        startingPoint = temp1;
        endingPoint = temp2;

        s.push(make_pair(trivial,make_pair(startingPoint,endingPoint)));
    }


    void kleen_clouser(Node* startA , Node* endA  ){
        Node* temp1 = new Node(index++);
        Node* temp2 =  new Node(index++);
        endA->transition.push_back(make_pair(epsilon,startA));

        endA->transition.push_back(make_pair(epsilon,temp2));
        temp1->transition.push_back(make_pair(epsilon,temp2));
        temp1->transition.push_back(make_pair(epsilon,startA));

        startingPoint=temp1;
        endingPoint=temp2;
        s.push(make_pair(trivial,make_pair(startingPoint,endingPoint)));
    }

    void positive_clouser(Node* startA , Node* endA  ){
        Node* temp1 =  new Node(index++);
        Node* temp2 =  new Node(index++);
        endA->transition.push_back(make_pair(epsilon,startA));
        temp1->transition.push_back(make_pair(epsilon,startA));
        endA->transition.push_back(make_pair(epsilon,temp2));
        startingPoint=temp1;
        endingPoint=temp2;
        s.push(make_pair(trivial,make_pair(startingPoint,endingPoint)));

    }

    void concatenate(Node* startA ,Node* endA ,Node* startB,Node* endB){
        endA->transition.push_back(make_pair(epsilon,startB));
        startingPoint =startA;
        endingPoint=endB;
        s.push(make_pair(trivial,make_pair(startingPoint,endingPoint)));
    }

    int getSize(){
        return index+1;
    }

};


class combinedNfa{
public:
    NFA graph = NFA();
    Node* graphStart ;
    vector<RegularExpression> expressions;
    RegularTransition epsilon;

    combinedNfa(vector<RegularExpression> rE){
        expressions = rE;
    }
    void generateNFA(){
        epsilon.allowEpsilon();
        epsilon.setTransitionName("Epsilon");
        graphStart = new Node(155);
        for(int i=0;i<expressions.size();i++){
            graph.setExpressions(expressions[i].getDefinition());
            graph.setAttributes(expressions[i].getName() , expressions[i].getPriority());
            graphStart->transition.push_back(make_pair(epsilon , graph.convertToNFA()));
        }
    }

    Node getStartingNode(){
        return *graphStart;
    }

    int getSize(){
        return graph.getSize();
    }


};




























































using namespace std;


class dfaNode {
private:
    int nodeName;
    string fakeName = "";
    bool acceptNode = false;

public:
    void getClosure(Node parent, bool visited[], string &name) { // wait for epsilon
        int index = atoi(parent.getLabelStr().c_str());    //call initialize func
        if (visited[index]) {
            return;
        }
        name += parent.getLabelStr();
        name += " ";
        vector<pair<RegularTransition, Node *>> listOfChildren = parent.transition;// get vector from nfa
        visited[index] = true;
        for (int i = 0; i < listOfChildren.size(); i++) { // get node from the list
            if (listOfChildren[i].first.isEpsilonAvailable()) {   // epsilon
                string label = listOfChildren[i].second->getLabelStr();   // convert from int to string
                //if (!nodeIsExist(label, name)) {
                   // name += listOfChildren[i].second->getLabelStr() + " ";
                    Node child = *(listOfChildren[i].second);
                    getClosure(child, visited, name);
                //}
            }
        }
    }

    void initializeVisitedArray(bool visited[], int nfaSize) {
        for (int i = 0; i < nfaSize; i++) {
            visited[i] = false;
        }
    }
//    bool visitedNode(string index , bool visited []){
//        int ind = atoi (index.c_str());
//        return visited [ind];
//
//    }

    // get a specific node from starting node
    Node getNfaNode(string nodeName, Node node, bool visited[]) { // must call initializeVisitedArray function
       
        int index = atoi(node.getLabelStr().c_str());

        if(visited[index]){
            Node temp(10000);
            return temp;
        }


        if (node.getLabelStr() == nodeName) {
            visited[index] = true;
            return node;
        }

        for (int i = 0; i < node.transition.size(); i++) {
            visited[index] = true;
            Node child = *(node.transition[i].second);
            Node y = getNfaNode(nodeName, child, visited);
            if(y.getLabelStr() == nodeName){
                return y;
            }
        }

        Node temp(10000);
        return temp;

    }

//    bool isEpsilonAvailable();
//    std::vector<char> getAvailableTransitions();
//    std::string getType();

    vector<string> divideFakeName(string token) {
        vector<string> subString;
        string x = "";
        for (int i = 0; i < token.size(); i++) {
            switch(token[i]){
                case ' ':
                    if(x != ""){
                        subString.push_back(x);
                        x = "";
                    }
                break;
                default:
                    x += token[i];
                break;
            }
        }
        if(x != ""){
            subString.push_back(x);
        }
        return subString;
    }

    void getByInput(Node node, vector<vector<string>>& dfaNodesName, int row, bool visited[], int nfaSize) {
        
        vector<pair<RegularTransition, Node *>> childrenList = node.transition;

        for (int i = 3; i < 131; i++) {
            for (int j = 0; j < childrenList.size(); j++) {
                //   vector<> inputs = childrenList[j].first();

                pair<RegularTransition, Node *> pairOfChild = childrenList[j];
                vector<char> inputs = pairOfChild.first.getAvailableTransitions();
                Node temp = *(pairOfChild.second);
                string data = temp.getLabelStr();

                for (int k = 0; k < inputs.size(); k++) {

                    if (!pairOfChild.first.isEpsilonAvailable()) {
                        if (i == inputs[k] + 3) {
                            dfaNodesName[row][i] += data;
                            dfaNodesName[row][i] += " ";
                            initializeVisitedArray(visited, nfaSize);
                            getClosure(temp, visited, dfaNodesName[row][i]);

                        }
                    }
                }
            }
        }
    }

    bool nodeIsExist(string node, string name) {   // done
        bool found = false;
        if (name.find(node) != std::string::npos) {
            found = true;
        }
        return found;
    }

    int getNodeByName(string s, vector<string> dfaNodesName) { // done
        bool stringExist = false;
        for (int k = 0; k < dfaNodesName.size(); k++) {
            stringExist = checkName(s, dfaNodesName[k]);
            if (stringExist) {
                return k;
            }
        }
        return -1;
    }

    bool checkName(string s1, string s2) {  // check for two names duplicated // will call inside loop(done)

        vector<string> x1 = divideFakeName(s1);
        vector<string> x2 = divideFakeName(s2);
        

        bool charExist = false;
        for (int i = 0; i < x1.size(); i++) {
            charExist = false;
            for (int j = 0; j < x2.size(); j++) {
                if (x1[i] == x2[j]) {
                    charExist = true;
                    break;
                }
            }
            if (!charExist) {
                return false;
            }
        }

        for (int i = 0; i < x2.size(); i++) {
            charExist = false;
            for (int j = 0; j < x1.size(); j++) {
                if (x2[i] == x1[j]) {
                    charExist = true;
                    break;
                }
            }
            if (!charExist) {
                return false;
            }
        }

        return true;
    }


    string convertIntToStr(int index) {

        int temp = index;
        string toReturn = "";

        while (temp) {
            toReturn += (temp % 10 + '0');
            temp = temp / 10;
        }

        if (toReturn == "")
            toReturn += '0';

        reverse(toReturn.begin(), toReturn.end());
        return toReturn;
    }
};

class DFA {
public:
    int nfaSize;
    Node& startingNode;
    vector<vector<string>> dfaNodesName;
    vector<string> uniqeDfaName;
    vector<int> priorities;
    int dfaNodes = 0; // counter for dfa Nodes
    DFA(Node& startNode) : startingNode(startNode){

    }
    void setNFA(int nfasize) {
        nfaSize = 900;
    }

    void convertToDFA() {
        bool visited[nfaSize];
        string startDfa = "";
        vector<string> subString;
        dfaNode *dfaObj = new dfaNode();
        dfaObj->initializeVisitedArray(visited, nfaSize);
        dfaObj->getClosure(startingNode, visited, startDfa);
        dfaNodesName.push_back({});
        dfaNodesName[0].push_back("0");
        uniqeDfaName.push_back(startDfa);
        int m  = 1;
        for (int k = 0; k < uniqeDfaName.size(); k++) {


            for(int i=1;i<131;i++){
                dfaNodesName[k].push_back("");
            }

            dfaObj->initializeVisitedArray(visited, nfaSize);
            subString = dfaObj->divideFakeName(uniqeDfaName[k]);

            for (int i = 0; i < subString.size(); i++) {

                dfaObj->initializeVisitedArray(visited, nfaSize);
                Node node = dfaObj->getNfaNode(subString[i], startingNode, visited);
                if(node.getLabelStr() != subString[i]){
                    cout << subString[i] << endl;
                    cout << "ERROR" << endl;
                }
                dfaObj->getByInput(node, dfaNodesName, k, visited, nfaSize);

            }

            for (int i = 3; i < 131; i++) {
                int found = dfaObj->getNodeByName(dfaNodesName[k][i], uniqeDfaName);
                string ind = dfaObj->convertIntToStr(found);

                if (found != -1) { //found duplication
                    //   dfaNodesName.push_back({});
                    dfaNodesName[k][i]=ind;
                } else {
                    
                    if(dfaNodesName[k][i] == ""){
                        dfaNodesName[k][i] = "-1";
                        continue;
                    }
                    uniqeDfaName.push_back(dfaNodesName[k][i]);

                    //   dfaNodesName.push_back({});
                    string indFromSize = dfaObj->convertIntToStr(uniqeDfaName.size()-1);
                    dfaNodesName[k][i]=indFromSize;
                }
            }
            for(;m<uniqeDfaName.size(); m++){
                string ind =dfaObj->convertIntToStr(m);
                dfaNodesName.push_back({});
                dfaNodesName[m].push_back(ind);
            }

        }

        for(int i=0;i<uniqeDfaName.size();i++){
            
            vector<string> nodes = dfaObj->divideFakeName(uniqeDfaName[i]);
            int p = INT_MIN;
            string expr;
            
            for(int j=0;j<nodes.size();j++){
                
                dfaObj->initializeVisitedArray(visited, nfaSize);
                Node node = dfaObj->getNfaNode(nodes[j], startingNode, visited);
                
                if(node.getState()){
                    if(node.getPriority() > p){
                        expr = node.getName();
                        p = node.getPriority();
                    }
                }
            }

            dfaNodesName[i][1] = p==INT_MIN?"0":"1";
            dfaNodesName[i][2] = expr;
        }
    }

    vector<vector<string>> getDFATable() {
        return dfaNodesName;
    }
};

