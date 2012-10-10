/*
 * =====================================================================================
 *
 *       Filename:  linked_list.h
 *
 *    Description:  create a linked_list structure 
 *
 *        Version:  1.0
 *        Created:  10/08/2012 23:09:57
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jianfeng Jia (Javier), jianfeng.jia@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef _UTIL_LINKED_LIST_H_
#define _UTIL_LINKED_LIST_H_

#include <cstdlib>

namespace util{

template <class T>
class LinkedList{

protected:
    struct Node{
        T       data;
        Node*   next;

        Node(const T &d, Node* n=0)
            :data(d), next(n)
        {}
    };

public:
    LinkedList();                               /* constructor */
    ~LinkedList();                              /* deconstructor */

    bool IsEmpty()const;                        /* test if the list is empty */
    const T* Contains (const T &data)const;     /* test if data exist in list, return pointer if exsit */
    int  DeleteNode(const T &data);             /* delete a specific data, only delete the first matched node */
    int  AppendNode(const T &data);             /* append a specific data at the end */
protected:
    void Delete();
    Node* head;
    Node* tail;
};


template <class T>
LinkedList<T>::LinkedList(){
    T data;
    head = new Node(data);
    tail = head;
}

template <class T>
LinkedList<T>::~LinkedList(){
    Delete();
}

template <class T>
void LinkedList<T>::Delete(){
    while (head){
        Node* node = head;
        head = head->next;
        delete node;
    }
}
template <class T>
bool LinkedList<T>::IsEmpty() const{
    return head == tail;
}

template <class T>
const T* LinkedList<T>::Contains (const T &data)const{
    const Node* pre = head;
    const Node* cur = pre->next;
    while (cur){
        if ( cur->data == data){
            return &(cur->data);
        }
        pre = cur;
        cur = pre->next;
    }
    return 0;
}

template <class T>
int LinkedList<T>::DeleteNode(const T &data){
    Node* pre = head;
    Node* cur = pre->next;
    while (cur){
        if ( cur->data == data){
            pre->next = cur->next;
            if (cur == tail){
                tail = pre;
            }
            delete cur;
            return 0;
        }
        pre = cur;
        cur = pre->next;
    }
    return -1;
}

template <class T>
int LinkedList<T>::AppendNode(const T &data){
    Node* node = new Node(data);
    if (!node){
        return -1;
    }
    tail->next = node;
    tail = tail->next;
    return 0;
}

};

#endif

