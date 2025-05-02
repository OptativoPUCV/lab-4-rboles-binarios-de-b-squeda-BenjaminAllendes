#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap *arbol = (TreeMap *) malloc(sizeof(TreeMap)) ; 
    arbol->root = NULL ;
    arbol->lower_than = lower_than;
    return arbol;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
    
    TreeNode *nodo = createTreeNode(key, value) ;
    if (tree->root == NULL) {
        tree->root = nodo ;
        return ;
    }
    tree->current = tree->root ;
    TreeNode *nodoAux ;
    while (tree->current != NULL){
        if (is_equal(tree, tree->current->pair->key, key)) return ;
        int resultado = tree->lower_than(tree->current->pair->key, key) ;
        nodoAux = tree->current ;
        if (resultado == 1) {
            tree->current = tree->current->right ;
        }
        else {
            tree->current  = tree->current->left ;
        }
        
        if (tree->current == NULL){
            if (resultado == 1) {
                nodoAux->right = nodo ;
                nodo->parent = nodoAux ; 
            }
            else {
                nodoAux->left = nodo ;
                nodo->parent = nodoAux ; 
            }
            tree->current = nodo ;
        }
    }
}

TreeNode * minimum(TreeNode * x){
    TreeNode *nodo ;
    nodo = x ;
    while (nodo->left != NULL){
        nodo = nodo->left ;
    }

    return nodo;
}


void removeNode(TreeMap * tree, TreeNode* node) {
    if (node->left == NULL && node->right == NULL) {
        if (tree->lower_than(node->parent->pair->key, node->pair->key)){
            node->parent->right = NULL ;
        }
        else {
            node->parent->left = NULL ;
        }
        free(node) ;
        return ;
    }

    if (node->right != NULL && node->left != NULL){
        TreeNode *nodoDere = node->right ;
        TreeNode *dereMin = minimum(nodoDere) ;
        node->pair->key = dereMin->pair->key ;
        node->pair->value = dereMin->pair->value ;
        removeNode(tree, dereMin) ;

        return ;
    }
    else {
        if (tree->lower_than(node->parent->pair->key, node->pair->key)){
            if (node->left != NULL) {
                node->parent->right = node->left ;
                node->left->parent = node->parent ;
            }
            else {
                node->parent->right = node->right ;
                node->right->parent = node->parent ;
            }
            
        }
        else {
            if (node->left != NULL) node->parent->left = node->left ;
            else node->parent->left = node->right ;
            
        }
        free(node) ;
    }

    
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {
    tree->current = tree->root ;
    while (tree->current != NULL)
    {
        if (is_equal(tree, tree->current->pair->key, key)) return tree->current->pair ;
        int resultado = tree->lower_than(tree->current->pair->key, key) ;
        if (resultado == 1) {
            tree->current = tree->current->right ;
        }
        else {
            tree->current  = tree->current->left ;
        }
    }
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    TreeNode *minimo = minimum(tree->root) ;
    tree->current = minimo ;
    return minimo->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
    TreeNode *siguiente ;
    if(tree->current == tree->root && tree->root->right != NULL){
        tree->current = minimum(tree->current->right) ;
        return tree->current->pair ;
    }
    printf("Primer debug") ;
    if (tree->lower_than(tree->current->pair->key, tree->root)){

        if (tree->lower_than(tree->current->pair->key, tree->current->parent->pair->key) && tree->current->right == NULL){
            printf("DEBUGGIN") ;
            siguiente = tree->current->parent ;
            tree->current = siguiente ;
            return tree->current->pair ;
        }
        else if (tree->lower_than(tree->current->pair->key, tree->current->pair->key) && tree->current->right == NULL){
            TreeNode *aux = tree->current ;
            while (tree->lower_than(tree->current->parent->pair->key, aux->pair->key)){
                tree->current = tree->current->parent ;
            }
            printf("mas debug") ;
            return tree->current->pair ;
        }
        else if (tree->current->right != NULL ){
            siguiente = tree->current->right ;
            tree->current = siguiente ;
            return siguiente->pair ;
        }
    }
    else {
        printf("Debuging") ;
        if (tree->current->left != NULL && tree->lower_than(tree->current, minimum(tree->current))){
            if (tree->lower_than(tree->current->pair->key, tree->current->parent->pair->key) && tree->current->right == NULL){
                siguiente = tree->current->parent ;
                tree->current = siguiente ;
                return siguiente->pair ;
            }
            else if (tree->lower_than(tree->current->parent->pair->key, tree->current->pair->key) && tree->current->right == NULL){
                siguiente = tree->current->parent->parent ;
                tree->current = siguiente ;
                return siguiente->pair ;
            }
            else if (tree->current->right != NULL ){
                siguiente = tree->current->parent->right ;
                tree->current = siguiente ;
                return siguiente->pair ;
            }
        }
        else {
            printf("Debugewdwing") ;
            siguiente = tree->current->right ;
            tree->current = siguiente ;
            return siguiente->pair ;
        }

    }
    printf("Wut") ;
    return NULL ;
}
