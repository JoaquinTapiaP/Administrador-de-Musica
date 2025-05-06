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

    //new->lower_than = lower_than;
    
    TreeMap *new = (TreeMap *)malloc(sizeof(TreeMap));

    new->current = NULL;
    new->root = NULL;
    new->lower_than = lower_than;

    return new;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
    if (searchTreeMap(tree, key) != NULL){
        return;
    }
    int valor = 1;
    //crear el nodo, poner los valores en el lugar correcto. usar createTreeNode

    TreeNode *new = createTreeNode(key, value);

    while(valor == 1){
        if (tree->lower_than(key, tree->current->pair->key)){ //si el dato es menor al del arbol
            if (tree->current->left == NULL){
                //tree->current = tree->current->left;
                //tree->current->left->pair->key = key;
                //tree->current->left->pair->value = value;
                tree->current->left = new;
                valor = 0;
                new->parent = tree->current;
                tree->current = new;
            }else{
                tree->current = tree->current->left;
            }


        }else{ //si el dato es mayor al del arbol
            if (tree->current->right == NULL){
                //tree->current = tree->current->right;
                //tree->current->right->pair->key = key;
                //tree->current->right->pair->value = value;
                tree->current->right = new;
                valor = 0;
                new->parent = tree->current;
                tree->current = new;           
            }else{
                tree->current = tree->current->right;
            }
        }    
    }
}

TreeNode * minimum(TreeNode * x){
    while (x->left != NULL){
        x = x->left;
    }
    return x;
}


void removeNode(TreeMap * tree, TreeNode* node) {
    if (node->left == NULL && node->right == NULL){ //sin hijos
        if (node->parent->left == node){
            node->parent->left = NULL;
        }else{
            node->parent->right = NULL;
        }
        free(node);
    }

    if ((node->left == NULL && node->right != NULL) || (node->left != NULL && node->right == NULL)){ // 1 hijo
        if (node->left != NULL) {
            if (node->parent->left == node) {
                node->parent->left = node->left;
            } else {
                node->parent->right = node->left;
            }
    
            node->left->parent = node->parent;
        } else {
            if (node->parent->left == node) {
                node->parent->left = node->right;
            } else {
                node->parent->right = node->right;
            }
    
            node->right->parent = node->parent;
        }
    
        free(node);
    }

    if (node->left != NULL && node->right != NULL){ //2 hijos
        TreeNode* aux = minimum(node->right);
        node->pair = aux->pair;
        TreeNode* padre = aux->parent;
        TreeNode* hijo;

        if (aux->left != NULL){
            hijo = aux->left;
        }else{
            hijo = aux->right;
        }

        if (padre->left == aux){
            padre->left = hijo;
        }else{
            padre->right = hijo;
        }

        if (hijo != NULL){
            hijo->parent = padre;
        }

        free (aux);
    }
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {
    tree->current = tree->root;
    int valor = 1;

    while (valor == 1){
        if(is_equal (tree, tree->current->pair->key, key) == 1){
            return tree->current->pair;
        }

        if (tree->lower_than(key, tree->current->pair->key)){ //si el dato es menor al del arbol
            if (tree->current->left != NULL){
                tree->current = tree->current->left;
            }else{
                valor = 0;
            }

        }else{ //si el dato es mayor al del arbol
            if (tree->current->right!= NULL){
            tree->current = tree->current->right;                
            }else{
                valor = 0;
            }
        }

    }
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    TreeNode* current = tree->root;
    TreeNode* ub_node = NULL;

    while (current != NULL) {
        if (!tree->lower_than(key, current->pair->key) && !tree->lower_than(current->pair->key, key)) { //clave igual
            tree->current = current;
            return current->pair;
        }

        if (tree->lower_than(key, current->pair->key)) { //clave menor
            ub_node = current;
            current = current->left;
        } else { //clave mayor
            current = current->right;
        }
    }

    if (ub_node != NULL) { //no se encuentra / existe
        tree->current = ub_node;
        return ub_node->pair;
    }

    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    TreeNode *first = minimum(tree->root);
    tree->current = first;
    return first->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
    if (tree->current->right != NULL){
        TreeNode* aux = tree->current->right;
        while (aux->left != NULL){
            aux = aux->left;        
        }
        tree->current = aux;
        return aux->pair;
    }

    TreeNode* padre = tree->current->parent;

    while (padre != NULL && tree->current == padre->right){
        tree->current = padre;
        padre = padre->parent;
    }

    if (padre == NULL) {
        tree->current = NULL;
        return NULL;
    }

    tree->current = padre;
    return padre->pair;
}
