#include<iostream>
#include "skiplist.h"

SkipList::~SkipList() {

}

bool SkipList::Init(int level, int span) {
	if (level <= 0 || level >= MAX_LEVEL)
		level = MAX_LEVEL;
	if (span <= 0)
		span = 2;

	m_level = level;
	m_span = span;

	m_index = new List[ sizeof(List*) * m_level ];
	Node* header = new Node(0);
	LevelNode* up = nullptr;
	for (level = 0; level < m_level; level++) {
		m_index[level] = new LevelNode(header);

		if (level > 0) {
			m_index[level]->m_down = m_index[level - 1];
			m_index[level - 1]->m_up = m_index[level];
		}
	}
	return true;
}

bool SkipList::Insert(int val) {
	int level = m_level - 1;
	//找到起始跨度节点
	LevelNode* spanNode = Find(val, level);
	if (!spanNode) return false;
	LevelNode* preNode = FindInsert(val, spanNode);
	Node* newReal = new Node(val);
	if (!newReal) return false;
	LevelNode* newNode = new LevelNode(newReal);
	if (!newNode) return false;
	
	if (preNode->m_next) {
		newNode->m_next = preNode->m_next;
		preNode->m_next->m_prev = newNode;
	}
	newNode->m_prev = preNode;
	preNode->m_next = newNode;

	//从下往上更新层级索引
	while (spanNode) {
		RebuildIndex(spanNode);
		spanNode = spanNode->m_up;
	}
	return true;
}

//计算交叉节点后面到下一个交叉节点(包含)的节点个数
//当原始链的一个跨度之间(两交叉节点之间)的节点个数 tailNum >= 2*span, 则找到他们中位数节点并将它设置为新的交叉节点
void SkipList::RebuildIndex(LevelNode* spanNode) {
	int tailNum = 0;
	LevelNode* slow = spanNode;
	LevelNode* fast = spanNode;
	do {
		fast = fast->m_next;
		tailNum++;
		if (tailNum > m_span) {
			slow = slow->m_next;
		}
	} while (fast && !fast->m_up && fast->m_next);
	//已经过半跨度节点了
	if (slow != spanNode && tailNum >= 2 * m_span) {
		UpdateIndex(spanNode, slow);
	}
}

//从下到上更新索引
void SkipList::UpdateIndex(LevelNode* spanNode, LevelNode* newNode) {
	LevelNode* up = spanNode->m_up;
	LevelNode* newUp = new LevelNode(newNode->m_real);
	//前后节点链接
	if (up->m_next) {
		up->m_next->m_prev = newUp;
		newUp->m_next = up->m_next;
	}
	up->m_next = newUp;
	newUp->m_prev = up;

	//上下节点链接
	newUp->m_down = newNode;
	newNode->m_up = newUp;
}

bool SkipList::Delete(int val) {
	int level = m_level - 1;
	LevelNode* node = FindNode(val, level);
	if (!node) return false;

	//删除0级原始链表的 m_real 节点
	delete node->m_real;
	//如果是交叉节点(它的down或者up指针不为空),则需要把它从相关的层级链表里移除
	//因为是0级链表节点,所以只有up指针不为空即可
	LevelNode* up = node;
	while (up) {
		up->m_real = nullptr;
		if (up->m_next) {
			up->m_next->m_prev = up->m_prev;
			up->m_prev->m_next = up->m_next;
		}
		else {
			up->m_prev->m_next = nullptr;
		}
		node = up;
		up = node->m_up;
		delete node;
	}

	return true;
}

LevelNode* SkipList::Find(int val, int& level) {
	if (level < 0 || level >= m_level)
		return nullptr;
	
	LevelNode* lNode = nullptr;
	LevelNode* spanNode = nullptr;
	while (level >= 0) {
		int sLevel = level;
		lNode = (LevelNode*)m_index[level];
		if (lNode->m_down || lNode->m_up) { //记录跨度开始节点
			spanNode = lNode;
		}

		while (lNode && lNode->m_next) {
			//当前层级的各个节点比较,直到 val 找到当前层级的插入位置
			if (val >= lNode->m_next->m_real->m_val) {
				lNode = lNode->m_next;
				if (lNode->m_down || lNode->m_up) { //节点更换, 如果是跨度开始节点, 记录下来
					spanNode = lNode;
				}
			} 
			else {
				//下沉到下个层级索引
				if (spanNode->m_down) {
					level--;
					spanNode = spanNode->m_down;
				}
				else {
					break;
				}
			}
		}
		if (level != sLevel) { //在某个 level 层有找到符合的跨度
			break;
		}
		else {
			level--;
		}
	}

	//下沉到0级,即原始链表
	while (spanNode && spanNode->m_down) {
		spanNode = spanNode->m_down;
	}
	return spanNode;
}

LevelNode* SkipList::FindNode(int val, int& level) {
	LevelNode* res = nullptr;
	LevelNode* node = Find(val, level);
	while (node && node->m_next) {
		if (val == node->m_real->m_val) {
			res = node;
			break;
		}
		else {
			node = node->m_next;
		}
	}
	return res;
}

LevelNode* SkipList::FindInsert(int val, LevelNode* spanNode) {
	while (spanNode->m_next) {
		if (val >= spanNode->m_next->m_real->m_val) {
			spanNode = spanNode->m_next;
		}
		else {
			break;
		}
	}
	return spanNode;
}

void SkipList::Print() {
	std::cout << "===========\n";
	for (int level = m_level-1; level >= 0; level--) {
		std::cout << "level:" << level << "--";
		LevelNode* node = m_index[level];
		while (node) {
			std::cout << node->m_real->m_val << " ";
			node = node->m_next;
		}
		std::cout << std::endl;
	}
}