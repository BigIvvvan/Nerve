#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <deque> 
#include <list>
#include <math.h>
using namespace std;

struct node {
	int data;
	node* next;
	node* previous;
};

struct nerve_location {//用来存储神经元的位置信息，支持访问
	//nerve* loc;
	long x;
	long y;
	long z;
};




//class conductor {
//public:
//	int signal = 0;
//	int step = 0;
//	nerve* output_n = new nerve;
//	nerve* input_n = new nerve;
//	list<int> stack;
//	void init(nerve* input_nerve,nerve* output_nerve) {
//		input_n = input_nerve;
//		output_n = output_nerve;
//		int tx = input_n->this_location.x;
//		int ty = input_n->this_location.y;
//		int myx = output_n->this_location.x;
//		int myy = output_n->this_location.y;
//		step = int(((tx - myx) ^ 2 + (ty - myy) ^ 2) ^ (1 / 2));
//	}
//	void receive_message(int message) {
//		signal = message;
//		if (step == 0) {
//			process();
//		}
//	}
//	void process() {
//		if (step == 0) {
//			output_n->receive_signal(signal);
//		}
//		else {
//			step -= 1;
//		}
//	}
//};


class nerve {//神经元的基类
public:
	int nerve_type;
	bool is_activate;
	nerve_location this_location;//申明此神经元的位置

	struct nerve_chain {//将所有连接的神经元用链表存储，支持查找读取
		nerve_chain* next;
		nerve* this_nerve;
		list<int> stack = {};

	};

	nerve *get_nerve(nerve_chain* the_nerve_chain,long pos) {//获取相应神经元的指针
		nerve_chain* p = new nerve_chain;
		p = the_nerve_chain;
		for (int i = 0; i < pos; i++) {
			p = p->next;
		}
		return p->this_nerve;
	}
	nerve_chain* get_nerve_chain(nerve_chain* the_nerve_chain, long pos) {//获取相应神经元的指针
		nerve_chain* p = new nerve_chain;
		p = the_nerve_chain;
		for (int i = 0; i < pos; i++) {
			p = p->next;
		}
		return p;
	}

	nerve_chain* input_to_this_nerve = new nerve_chain;
	long input_amount = 1;//输入神经元的数量，最小为1，包含了自己
	nerve_chain* output_from_this_nerve = new nerve_chain;
	long output_amount = 1; //输出神经元的数量

	bool nerve_init(nerve_location this_nerve_location) {//初始化函数
		this_location = this_nerve_location;
	
		input_to_this_nerve->this_nerve = this;
		input_to_this_nerve->next = NULL;
		output_from_this_nerve->this_nerve = this;
		output_from_this_nerve->next = NULL;

		cout << "nerve initial success\n";
		return true;
	}

	bool activate() {
		is_activate = true;
		return true;
	}
	bool deactivate() {
		is_activate = false;
		return true;
	}

	//bool input_connect_to(nerve* link_nerve,bool isFirstConnection) {//连接神经元，并且调用被连接的神经元，双向连接（暂定）
	//	nerve_chain* i = new nerve_chain;
	//	i->this_nerve = link_nerve;
	//	nerve_chain* pre = input_to_this_nerve;
	//	while (pre->next != NULL) {
	//		pre = pre->next;
	//	};
	//	i->next = pre->next;
	//	pre->next = i;
	//	if (isFirstConnection) {
	//		link_nerve->output_connect_to(this,false);
	//	}
	//	input_amount += 1;
	//	return true;
	//}
	//发现只要一个输出就够了，节省内存

	bool output_connect_to(nerve* link_nerve, bool isFirstConnection) {
		nerve_chain* i = new nerve_chain;
		i->this_nerve = link_nerve;
		nerve_chain* pre = output_from_this_nerve;
		while (pre->next != NULL) {
			pre = pre->next;
		};
		i->next = pre->next;
		pre->next = i;
		//if (isFirstConnection) {
		//	link_nerve->input_connect_to(this, false);
		//}
		output_amount += 1;
		return true;
	}

	void receive_signal(int message) {//接收函数，把收到的信号进行存储。
		cout << message;
		cout << " ";
	}

	void send_signal(int message) {//发送函数，计算神经元之间的距离，并且发送信号
		nerve_chain* p = output_from_this_nerve;
		for (int i = 1; i < output_amount; i++) {
			nerve* t = get_nerve(p, i);
			float tx = (float)t->this_location.x;
			float ty = (float)t->this_location.y;
			float myx = (float)this_location.x;
			float myy = (float)this_location.y;
			float distance = sqrt(pow(abs(tx-myx),2)+ pow(abs(ty - myy), 2));//求相对距离

			nerve_chain* c = get_nerve_chain(p,i);
			if (sizeof(c->stack) < (int)distance) {
				c->stack.push_front(message);//队列未满，压栈
			}
			else {
				c->stack.push_front(message);//发出信息，出栈
				t->receive_signal(c->stack.back());
				c->stack.pop_back();
				
			}
			//t->recive_signal(message);
		}
	}
	
};

int main() {

	nerve test[50][50];
	for (int i = 0; i < 50; i++) {
		for (int j = 0; j < 50; j++) {
			nerve_location temp;
			temp.x = i + 1;
			temp.y = j + 1;
			temp.z = 1;
			test[i][j].nerve_init(temp);
		}
	}
	for (int i = 0; i < 50; i++) {
		for (int j = 0; j < 50; j++) {
			/*test[1][1].input_connect_to(&test[i][j],true);*/
			test[1][1].output_connect_to(&test[i][j], true);
		}
	}

	nerve p = test[1][1];
	p.send_signal(3);

	return 0;
}

