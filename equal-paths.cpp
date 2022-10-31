#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
int helper(Node * root){
    if(root == NULL){
        return 0;
    }

    Node * l = root -> left;
    Node * r = root -> right;
    int lLength = helper(l);
    int rLength = helper(r);

    if(lLength > rLength){
        return lLength + 1;
    }
    else{
        return rLength + 1;
    }
}


bool equalPaths(Node * root)
{
    if(root == NULL){
        return true;
    }
    else if (helper(root) <= 2){
        return true;
    }

    // Add your code below
    Node* l = root -> left;
    Node* r = root -> right;
    
    if(helper(l) == helper(r)){
        return equalPaths(l) && equalPaths(r);
    }
    else{
        return false;
    }
}
