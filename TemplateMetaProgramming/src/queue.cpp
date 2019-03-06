/*
 * queue.cpp
 *
 *  Created on: 13.11.2018
 *      Author: ctoma
 */
#include <iostream>
#include <list>
#include <iterator>
#include "queue.hpp"
using namespace std;

struct node

{

    int data;

    node *next;

}*front = NULL,*rear = NULL,*p = NULL,*np = NULL;

   bool Queue::isEmpty() {
	   return front == NULL;
   }

   int Queue::dequeue() {
	   int x;
	   if (front == NULL) {

	   } else {
		   p = front;
		   x = front->data;
		   front = front->next;
		   delete p;
		   return x;
	   }
   }

   void Queue::enqueue(int e) {
	   np = new node;
	   np->data = e;
	   np->next = NULL;
	   if (front == NULL) {
		   front = rear = np;
		   rear->next = NULL;
	   } else {
		   rear->next = np;
		   rear = np;
		   rear->next = NULL;
	   }
   }

   void Queue::clear() {
	   front = NULL;
	   rear = NULL;
	   p = NULL;
	   np = NULL;
   }

