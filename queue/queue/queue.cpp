#include<iostream>


//循环队列

//无论是 tail 还是 head, 队列里当前位置往前一位 都是 (tail or head + 1) % size
//队列空时,head == tail
//队列满时,(tail + 1) % size = head
//为了简便,循环队列里, tail 位置的值始终当作是空置的
class RoundQueue {
public:
	RoundQueue(int size):m_size(size),m_head(0),m_tail(0),m_arr(nullptr){}
	~RoundQueue() {}
public:
	bool Init() {
		m_arr = (int*)malloc(sizeof(int) * m_size);
		if (!m_arr) return false;
		return true;
	}

	void Reset() {
		m_head = 0;
		m_tail = 0;
	}

	bool Enqueue(int val) {
		int pos = (m_tail + 1) % m_size;
		if (pos == m_head) return false; //满了
		m_arr[m_tail] = val;
		m_tail = pos;
		return true;
	}

	bool Dequeue() {
		if (m_tail == m_head) return false; //空队列
		int pos = (m_head + 1) % m_size;
		m_arr[m_head] = 0;
		m_head = pos;
		return true;
	}

	void PrintAll() {
		std::cout << "Before PrintAll:\n";
		int i = m_head;
		while (i != m_tail) {
			std::cout << m_arr[i] << " ";
			i = (i + 1) % m_size;
		}
		std::cout << "\nAfter PrintAll:\n";
	}
public:
	int m_size;
	int m_head;
	int m_tail;
	int* m_arr;
};

int main() {
	int size = 11;
	RoundQueue que(size);
	que.Init();

	for (int i = 1; i < size; i++) {
		que.Enqueue(i);
	}
	que.PrintAll();

	que.Dequeue();
	que.Dequeue();
	que.PrintAll();

	for (int i = 1; i < size; i++) {
		que.Dequeue();
	}
	que.PrintAll();
	printf("empty queue: head=%d, tail=%d\n",que.m_head,que.m_tail);

	std::cout << "next:\n";
	size = 8;
	for (int i = 1; i < size; i++) {
		que.Enqueue(i);
	}
	que.PrintAll();
	printf("queue: head=%d, tail=%d\n", que.m_head, que.m_tail);
	return 0;
}