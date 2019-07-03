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
	//�ҵ���ʼ��Ƚڵ�
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

	//�������ϸ��²㼶����
	while (spanNode) {
		RebuildIndex(spanNode);
		spanNode = spanNode->m_up;
	}
	return true;
}

//���㽻��ڵ���浽��һ������ڵ�(����)�Ľڵ����
//��ԭʼ����һ�����֮��(������ڵ�֮��)�Ľڵ���� tailNum >= 2*span, ���ҵ�������λ���ڵ㲢��������Ϊ�µĽ���ڵ�
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
	//�Ѿ������Ƚڵ���
	if (slow != spanNode && tailNum >= 2 * m_span) {
		UpdateIndex(spanNode, slow);
	}
}

//���µ��ϸ�������
void SkipList::UpdateIndex(LevelNode* spanNode, LevelNode* newNode) {
	LevelNode* up = spanNode->m_up;
	LevelNode* newUp = new LevelNode(newNode->m_real);
	//ǰ��ڵ�����
	if (up->m_next) {
		up->m_next->m_prev = newUp;
		newUp->m_next = up->m_next;
	}
	up->m_next = newUp;
	newUp->m_prev = up;

	//���½ڵ�����
	newUp->m_down = newNode;
	newNode->m_up = newUp;
}

bool SkipList::Delete(int val) {
	int level = m_level - 1;
	LevelNode* node = FindNode(val, level);
	if (!node) return false;

	//ɾ��0��ԭʼ����� m_real �ڵ�
	delete node->m_real;
	//����ǽ���ڵ�(����down����upָ�벻Ϊ��),����Ҫ��������صĲ㼶�������Ƴ�
	//��Ϊ��0������ڵ�,����ֻ��upָ�벻Ϊ�ռ���
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
		if (lNode->m_down || lNode->m_up) { //��¼��ȿ�ʼ�ڵ�
			spanNode = lNode;
		}

		while (lNode && lNode->m_next) {
			//��ǰ�㼶�ĸ����ڵ�Ƚ�,ֱ�� val �ҵ���ǰ�㼶�Ĳ���λ��
			if (val >= lNode->m_next->m_real->m_val) {
				lNode = lNode->m_next;
				if (lNode->m_down || lNode->m_up) { //�ڵ����, ����ǿ�ȿ�ʼ�ڵ�, ��¼����
					spanNode = lNode;
				}
			} 
			else {
				//�³����¸��㼶����
				if (spanNode->m_down) {
					level--;
					spanNode = spanNode->m_down;
				}
				else {
					break;
				}
			}
		}
		if (level != sLevel) { //��ĳ�� level �����ҵ����ϵĿ��
			break;
		}
		else {
			level--;
		}
	}

	//�³���0��,��ԭʼ����
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