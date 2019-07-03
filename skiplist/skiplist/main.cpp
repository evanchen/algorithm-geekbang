#include<iostream>
#include "skiplist.h"
#include<cassert>


int main() {
	SkipList sl;
	assert(sl.Init(5,2));
	sl.Print();

	for (int i = 0; i < 13; i++) {
		sl.Insert(i+1);
		sl.Print();
	}
	return 0;
}
