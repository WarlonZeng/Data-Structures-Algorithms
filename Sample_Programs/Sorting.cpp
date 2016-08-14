#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <new>
#include <vector>

// Warlon Zeng

using namespace std;

void sorting()
{
	ListNode * temphead = head;
	ListNode * tempnode = NULL;
	int counter = 0;
	while (temphead)
	{
		temphead = temphead->next;
		counter++;
	}
	for (int i = 0; i < counter; i++)
	{
		for (int j = 0; j<counter - i; j++)
		{
			if (temphead->roll > temphead->next->roll)
			{
				tempnode = temphead;
				temphead = temphead->next;
				temphead->next = tempnode;
			}
		}
	}
}