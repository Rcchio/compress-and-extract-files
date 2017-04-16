#ifndef HNODE_H
#define HNODE_H
class Hnode  //哈夫曼树的存储存储结构
{
public:
	unsigned char name;    //8位的存储单元来存储字符（256种）
	unsigned  weight;  //存储字符的频度
	int p; //双亲节点
	int lchild; //左孩子
	int rchild; //右孩子
	int Hnodeindex;  //节点索引
	Hnode() //初始化数据成员
	{
		name = '\0';
		weight = 0;
		p = 0;
		Hnodeindex = 0;
		lchild = rchild = 0;
	}
};
#endif


