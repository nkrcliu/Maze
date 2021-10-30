#ifndef  _MYQUEUE_H_ 
#define MYQUEUE_H
#include<iostream>
#include <assert.h>
using namespace std;

template <class dataType>
class myqueue
{     
public:
    myqueue()//Initialize the pointer field to nulle and length to 0
    {
        pFront = NULL;
        pBack = NULL;
        length = 0;
    }

    ~myqueue()//destructor
    {
        Node* tempNode;
        while (pFront != NULL)
        {
            tempNode = pFront->next;
            delete pFront;
            pFront = tempNode;
        }
    }

    int push(dataType data)
    {
        Node* newNode = new Node;
        newNode->val = data;
        if (empty())
        {
            newNode->next = NULL;
            newNode->prev = NULL;
            pBack = pFront = newNode;
        }
        else
        {
            newNode->prev = pBack;
            newNode->next = pBack->next;
            pBack->next = newNode;
            pBack = newNode;
        }
        length++;

        return 0;
    }

    int pop()
    {
        assert(!empty());
        Node* tempNode = pFront;

        if (pFront->next != NULL)
        {
            pFront->next->prev = pFront->prev;
        }
        pFront = pFront->next;
        delete tempNode;
        length--;

        return 0;
    }

    dataType front()
    {
        assert(!empty());
        return pFront->val;
    }

    int size()
    {
        return length;
    }

    bool empty()
    {
        return length == 0;
    }
private:
    struct Node//define a bidirectional linked list node 
    {
        dataType val;
        Node* next;
        Node* prev;
    };
    Node* pFront;
    Node* pBack;
    int length;
};

#endif