#include<iostream>
#include "skiplist.h"
#include<cassert>


int main() {
	SkipList sl;
	assert(sl.Init(5,2));

	for (int i = 0; i < 50; i++) {
		sl.Insert(i+1);
	}
	sl.Print();

	int level = sl.GetMaxLevel() - 1;
	LevelNode* node = sl.FindNode(2, level);
	if (node) {
		std::cout << "found:" << node->m_real->m_val << ", level: " << level << std::endl;
	}
	return 0;
}
