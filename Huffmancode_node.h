#ifndef HUFFMANCODE_NODE_H
#define HUFFMANCODE_NODE_H
#include<string>
using namespace std;
class Huffmancode_node   //存储字符的哈夫曼编码
{
public:
	unsigned char name;  //字符的名称
	vector<int> code;    //用vector容器存储哈夫曼编码
	Huffmancode_node()
	{
		name = '\0';
	}
};
#endif

