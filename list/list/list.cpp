#include<iostream>
#include<cassert>
#include <time.h> 


struct node {
public:
	node(int val) : m_a(val), pNext(nullptr) {}
	~node() {}

public:
	int m_a;
	node * pNext;
};

struct mList {
public:
	mList() :m_pHead(nullptr) {}
	~mList() {}
public:
	bool Init() {
		assert(m_pHead == nullptr);
		m_pHead = new node(0);
		if (!m_pHead) {
			return false;
		}
		return true;
	}
	node* InsertNode(node* pCur,node* pNew) {
		if (pCur->pNext) {
			pNew->pNext = pCur->pNext;
			pCur->pNext = pNew;
		}
		else {
			pCur->pNext = pNew;
		}
		return pNew;
	}
	node* GetHead() { return m_pHead; }

	void PrintAll() {
		std::cout << "print list begin:" << std::endl;
		node* pCur = m_pHead->pNext;
		while (pCur) {
			std::cout << pCur->m_a << " ";
			pCur = pCur->pNext;
		}
		std::cout << "\nprint list end:" << std::endl;
	}

	void Reverse() {
		node* pCur = m_pHead->pNext; //˳��
		m_pHead->pNext = nullptr; //���°�˳��ڵ㷴�����head�ڵ����
		while (pCur)
		{
			node* singleNode = pCur;
			pCur = pCur->pNext;
			singleNode->pNext = nullptr;
			InsertNode(m_pHead, singleNode);
		}
	}

	void Merge(mList& srcList) {
		node* p1 = m_pHead->pNext;
		node* p2 = srcList.GetHead()->pNext;
		if (!p1) {
			InsertNode(m_pHead, p2);
			return;
		}
		if (!p2) {
			return;
		}
		node *pNew = nullptr;
		if (p1->m_a <= p2->m_a) {
			pNew = p1;
			p1 = p1->pNext;
		}
		else {
			pNew = p2;
			p2 = p2->pNext;
		}
		m_pHead->pNext = pNew;

		while (p1 && p2) {
			if (p1->m_a <= p2->m_a) {
				pNew->pNext = p1;
				p1 = p1->pNext;
			}
			else {
				pNew->pNext = p2;
				p2 = p2->pNext;
			}
			pNew = pNew->pNext;
		}

		if (!p1) {
			pNew->pNext = p2;
		}
		if (!p2) {
			pNew->pNext = p1;
		}
	}

	node* HasLoop() {
		node* slow = m_pHead->pNext, *fast = m_pHead->pNext;
		if (!fast || !fast->pNext) { //���������������ڵ�
			return nullptr;
		}
		while (fast && fast->pNext) {
			slow = slow->pNext;
			fast = fast->pNext->pNext;
			if (slow == fast) {
				return slow;
			}
		}
		return nullptr;
	}

	node* FindLoopNode(int& ncount) {
		node* slow = HasLoop();
		if (!slow) return nullptr;
		node* meet = slow;
		slow = m_pHead->pNext;
		ncount = 0; //���ش� m_pHead->pNext �ڵ㵽����ڽڵ�ľ���
		while (slow != meet) {
			slow = slow->pNext;
			meet = meet->pNext->pNext;
			ncount++;
		}
		return meet;
	}
	//�Ƴ�����ĵ����� rcount ���ڵ�
	void DelNode(int rcount) {
		if (rcount <= 0) return;

		int ncount = 0; 
		node* meet = FindLoopNode(ncount);
		int size = 0; //������ܳ���
		if (meet) {
			int lcount = 1; //���ĳ���,��ʼ��Ϊ 1 ����Ϊ����ڵ���¸��ڵ㿪ʼ��
			node* slow = meet->pNext;
			while (meet != slow) {
				slow = slow->pNext;
				lcount++;
			}
			size = ncount + lcount;
		}
		else {
			node* slow = m_pHead->pNext;
			while (slow) {
				slow = slow->pNext;
				size++;
			}
		}
		if (size == rcount) {
			node* del = m_pHead->pNext;
			if (del) {
				m_pHead->pNext = del->pNext;
				delete del;
			}
			return;
		}
		if (rcount < size) {
			size = size - rcount - 1; //��˳���ҵ�Ҫɾ���Ľڵ��ǰһ���ڵ�λ��
			node* slow = m_pHead->pNext;
			for (int i = 0; i < size; i++) {
				slow = slow->pNext;
			}
			node* del = slow->pNext;
			if (del) {
				slow->pNext = del->pNext;
				delete del;
			}
			return;
		}
	}

	//����ָ���˼·,��� rcount ���ڵ�
	void DelNode2(int rcount) {
		if (rcount <= 0) return;

		node* preDel = nullptr;
		node* actNode = m_pHead->pNext;
		preDel = actNode;
		int c = 0;
		while (actNode && actNode->pNext) {
			c++;
			if (c > rcount) {
				preDel = preDel->pNext;
			}
			actNode = actNode->pNext;
		}
		if (preDel == m_pHead->pNext) { //rcount С�ڻ���� ���ĳ���(preDelû��Ų����)
			m_pHead->pNext = nullptr;
			delete preDel;
			return;
		}
		if (preDel->pNext) {//�������
			actNode = preDel->pNext;
			preDel->pNext = actNode->pNext;
			delete actNode;
		}
	}

	//����ָ���˼·,��� rcount ���ڵ�
	void DelNode3(int rcount) {
		node* actNode = m_pHead;
		node* preDel = actNode;
		for (int i = 0; i <= rcount; i++) {
			if (actNode)
				actNode = actNode->pNext;
			else
				break;
		}
		while (actNode) {
			preDel = preDel->pNext;
			actNode = actNode->pNext;
		}
		if (preDel->pNext) {
			actNode = preDel->pNext;
			preDel->pNext = actNode->pNext;
			delete actNode;
		}
	}

public:
	node* m_pHead;
};

/* 
struct ListNode { int val; struct ListNode *next; ListNode(int x) : val(x), next(NULL) { } };
class Solution {
public: 
	ListNode* ReverseList(ListNode* pHead) {
		ListNode* newh = NULL; 
		for (ListNode* p = pHead; p; )//pΪ����ָ�� 
		{ 
			ListNode* tmp = p -> next;//temp������һ����� 
			p -> next = newh; 
			newh = p; 
			p = tmp; 
		} 
		return newh; 
	}
};
*/


int main() {
	srand(unsigned int(::time(NULL)));

	mList list;
	list.Init();
	node* pCur = list.GetHead();
	for (int i = 0; i < 10; i++) {
		node* pNew = new node(rand() % 100);
		pCur = list.InsertNode(pCur,pNew);
	}
	list.PrintAll();
	list.Reverse();
	list.PrintAll();


	mList lista;
	lista.Init();
	node* apCur = lista.GetHead();

	mList listb;
	listb.Init();
	node* bpCur = listb.GetHead();
	for (int i = 0; i < 5; i++) {
		node* pNew1 = new node(2*i+1);
		apCur = lista.InsertNode(apCur, pNew1);

		node* pNew2 = new node(2*(i+1));
		bpCur = listb.InsertNode(bpCur, pNew2);
	}
	std::cout << "before merge:\n";
	lista.PrintAll();
	listb.PrintAll();
	std::cout << "after merge:\n";
	lista.Merge(listb);
	lista.PrintAll();

	std::cout << "before delete:\n";
	lista.PrintAll();
	lista.DelNode(2);
	std::cout << "after delete:\n";
	lista.PrintAll();

	lista.DelNode2(2);
	std::cout << "after DelNode2:\n";
	lista.PrintAll();

	lista.DelNode3(2);
	std::cout << "after DelNode3:\n";
	lista.PrintAll();
	return 0;
}

