/*
 * queue.hpp
 *
 *  Created on: 13.11.2018
 *      Author: ctoma
 */

#ifndef QUEUE_HPP_
#define QUEUE_HPP_

class Queue {
   public:
   bool isEmpty();

   int dequeue();

   void enqueue(int e);

   void clear();
};
#endif /* QUEUE_HPP_ */
