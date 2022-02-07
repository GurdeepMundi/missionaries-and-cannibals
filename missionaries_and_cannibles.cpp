/**
 * @file missionaries_and_cannibles.cpp
 * @author Gurdeep Singh Mundi (g.null@uleth.ca)
 * @copyright 2022 all rights reserved
 * @date Febuary 04, 2022
 */
#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <algorithm>
using namespace std;
/**This class will be represnting each state of the state space.
 * */
class State {
    public:
    ///will store values in form of [M, C, B] whicih stands for missionaries, cannibals, and boats on the left side
    int value[3];
    ///will be used to store child states
    vector <State*> childStates;
    
    /** This constructor can be used to intialize the state uisng an
     * integer array
     * @arg v arress of an integer array containing three elemnts.
     */
    State(int* v) {
        //ctr
        for(int i = 0; i < 3; i++) {
            this->value[i] = v[i];
        }
    }

    /** This constructor can be used to intialize the state uisng an
     * integer array
     * @arg m number of misionaries
     * @arg c number of cannibals
     * @arg b where is the boat? 1: left bank, 0: right bank
     */
    State(int m, int c, int b) {
        //ctr
        this->value[0] = m;
        this->value[1] = c;
        this->value[2] = b;
    }

    /** @brief add a child state to the tree
     *  @arg c state
     * */
    void addChild(State* c) {
        this->childStates.push_back(c);
    }

    /**can be used to print the current state.
     * */
    void printState() {
        cout << "(" << value[0] << "," << value[1] << "," << value[2] << ")";
    }

    /***remember you need to pass two objects not pointers
     * */
    bool operator==(State s) {
        if(this->value[0] == s.value[0] && this->value[1] == s.value[1] && this->value[2] == s.value[2]) {
            return true;
        } else {
            return false;
        }
    }

    /***remember you need to pass two objects not pointers
     * */
    bool operator!=(State s) {
        if(this->value[0] == s.value[0] && this->value[1] == s.value[1] && this->value[2] == s.value[2]) {
            return false;
        } else {
            return true;
        }
    }
    
    /*** remember you nedd to use on object, not pointer
     * @arg a an integer array
     * */
    bool operator-=(int* a) {
        for(int i = 0; i < 3; i++) {
            value[i] -= a[i];
        }
    }

    /***remember you nedd to use on object, not pointer
     * */
    bool operator+=(int* a) {
        for(int i = 0; i < 3; i++) {
            value[i] += a[i];
        }
    }

};

/** checks if the given state will pose a cannible situation on either side of the river.
 *  @return true if canible situation
 * */
bool isCanibal(State* checkState) {
    if((checkState->value[0] < checkState->value[1] && checkState->value[0] !=0) || 
    checkState->value[0] == 2 && checkState->value[1] == 1 ||
    checkState->value[0] == 2 && checkState->value[1] == 0)
        return true;
    return false;
}

/** This function will generate all the possible states from the current state
 *  @return a vector of new possible states from the current state
 * */
vector<State*> stateGenerator(State* current) {
    vector<State*> childStates;
    int ltr[5][3] = {{0,1,1},
                     {1,0,1},
                     {0,2,1},
                     {2,0,1},
                     {1,1,1}}; //operations
    State* tempState;
    // check if already at the goalstate
    State goalState(0,0,0);
    if(goalState == *current)
    return childStates;

    //if boat is on left bank
    if(current->value[2] == 1) {
        for(int i = 0; i < 5; i++) {
            tempState = new State(current->value[0] - ltr[i][0],
            current->value[1] - ltr[i][1],
            current->value[2] - ltr[i][2]);
            if(tempState->value[0] < 0 || tempState->value[1] < 0 || tempState->value[2] < 0) 
            continue;
            if(tempState->value[0] > 3 || tempState->value[1] > 3 || tempState->value[2] > 1)
            continue;
            if(isCanibal(tempState))
            continue;
            childStates.push_back(tempState);
        }
    }else {
        //if boat is on right bank
        for(int i = 0; i < 5; i++) {
            tempState = new State(current->value[0] + ltr[i][0],
            current->value[1] + ltr[i][1],
            current->value[2] + ltr[i][2]);
            if(tempState->value[0] < 0 || tempState->value[1] < 0 || tempState->value[2] < 0)
            continue;
            if(tempState->value[0] > 3 || tempState->value[1] > 3 || tempState->value[2] > 1)
            continue;
            if(isCanibal(tempState))
            continue;
            childStates.push_back(tempState);
        }
    }
    return childStates;
}

vector<State*> alreadyDiscovered;// used to store already discovered nodes
/** This function will be used to generate whole tree or we can say state space in this case.
 * */
void generateTree(State* currentState) {
    vector<State*> derivedStates = stateGenerator(currentState);
    //no new states from here
    if(derivedStates.size() == 0)
    return;
    //remove already discovered states
    for(int i = 0; i < alreadyDiscovered.size(); i++) {
        for(int j = derivedStates.size()-1; j >= 0; j--) {
            if(*alreadyDiscovered.at(i) == *derivedStates.at(j)) {
                delete derivedStates.at(j);
                derivedStates.erase(derivedStates.begin()+j);
            }
        }
    }
    //add new states to the parent nodes.
    for(auto& a: derivedStates) {
        currentState->addChild(a);
        alreadyDiscovered.push_back(a);
    }

    for (auto& a: currentState->childStates) {
        generateTree(a);
        //if the goal state is found, no need to search further
            if(a->value[0] == 0 && a->value[0] == 0 && a->value[0] == 0)
            break;
    }
}

/** this function can be used to print a tree or subtree
 * @param node the initial or substate from where you want to print
 * */
void printTree(State* node) {
    if(node->childStates.size() == 0) {
        return;
    }
    cout << "parant: "; node->printState(); cout << endl;
    for(auto a: node->childStates) {
        a->printState();
    }
    cout << endl << endl;
    for(auto a: node->childStates) {
        printTree(a);
    }
}

/*** This algortithm searches for the optimal path in this tree structure.
 * It uses the depth first search to get to the goal state
 *  @param currentState current state in [MCB] form.
 *  @param goalState goal state in [MCB] form.
 *  @param path reference to the stack which you want to store the resulting path in.
 **/ 
void dfs(State* currentState, State* goalState, stack<State*, vector<State*>> * path) {
    path->push(currentState);
    if(*currentState != *goalState && !currentState->childStates.empty()) {
        for (auto c: path->top()->childStates) {
            dfs(c, goalState, path);
        }
    } else{
        if(*currentState != *goalState){
            path->pop();
        } 
    }
}

int main() {
    int initial[3] = {3,3,1}; /// start state values
    State* initialState = new State(initial); ///Start State
    State* goalState = new State(0,0,0);
    stack<State*, vector<State*>> dfsStack; /// this stack will be used by DFS search
    alreadyDiscovered.push_back(initialState);  
    generateTree(initialState); //generate the state space
    dfs(initialState, goalState, &dfsStack); // perform dfs to find the optimal path
    stack<State*, vector<State*>> printStack; //temporary stack to format output
    //print reverseing the stack for printing
    while(!dfsStack.empty()){
        printStack.push(dfsStack.top());
        dfsStack.pop();
    }
    //output of the program
    cout << "You need minimum " << printStack.size() << " steps to transfer missionaries and cannibles" << endl;
    cout << "Here are the transition states that you need to go through: " << endl;
    while(!printStack.empty()){
        printStack.top()->printState(); cout << endl;
        printStack.pop();
    }
    return 0;
}