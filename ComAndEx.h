#ifndef COMANDEX_H
#define COMANDEX_H
#include<vector>
#include"Hnode.h"
#include"Huffmancode_node.h"
#include<iostream>
#include<fstream>
using namespace std;
class ComAndEx   //压缩解压类的头文件
{
public:
	void Compress(); //压缩文件的函数
	void Extract();  //解压文件的函数
	string ScanCharacter();//扫描源文件种字符的种类及个数
	void CreateHuffmanTree();//建立哈夫曼树
	void CreateHuffmanCode();//生成哈夫曼编码
protected:
	vector<Hnode> HuffmanTree;  //存储哈夫曼树的数组
	vector<Huffmancode_node>Huffmancode;  //存储哈夫曼编码的数组
	int H_number; //字符的种类数
};

#endif
