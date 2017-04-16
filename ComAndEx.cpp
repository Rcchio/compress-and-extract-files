#include"ComAndEx.h"
void ComAndEx::Compress()
{
	string outfilename=ScanCharacter(); //扫描源文件种字符的种类及个数，返回源文件的文件名并用string类型变量保存
	if (outfilename == "NULL") return;
	CreateHuffmanTree(); //构造哈夫曼树
	CreateHuffmanCode(); //生成哈夫曼编码
	cout << "请输入被压缩后的文件的名字:";
	string filename;
	cin >> filename;
	ofstream out(filename, ios::binary);
	if (!out)
	{
		cout << "文件打开失败.." << endl;
		return;
	}
	out.write((char*)&H_number, sizeof(int));
	for (int i = 1; i <= H_number; ++i)//依次输入字符及字符的频度
	{
		out.write((char*)&(HuffmanTree[i].name), sizeof(char));
		out.write((char*)&(HuffmanTree[i].weight), sizeof(int));
	}
	ifstream in(outfilename, ios::binary);//第二遍扫描源文件
	if (!in)
	{
		cout << "打开源文件失败" << endl<<endl;
		return;
	}
	unsigned char char_temp = '\0';//用8位的无符号变量作为读写字符的单元
	string unit;//存储哈夫曼编码，编码的个数大于等于八位就和char_temp进行|操作
	while (true)
	{
		char_temp=in.get();//在原文件中读取一个字符
		if (in.eof()) break;//判断是否到达文件的末尾
		for (int i = 1; i <= H_number; ++i)//在存取哈夫曼编码的数组中找到该字符对应的编码并将编码存储到unit中
		{
			if (Huffmancode[i].name == char_temp)
			{
				for (auto elem : Huffmancode[i].code)//向unit中输入编码
				{
					if (elem == 1)
						unit.push_back('1');
					else
						unit.push_back('0');
				}
				break;
			}
		}
		while (unit.length() >= 8)//如果unit中存储的哈夫曼编码的位数大于等于8，就将前八位通过位操作写入char_temp中
		{
			char_temp = '\0';
			for (int i = 0; i < 8; ++i)
			{
				char_temp =char_temp<< 1;
				if (unit[i] == '1')
					char_temp = char_temp | 1;
			}
			auto p = unit.begin();
			unit.erase(p, p + 8);//清空unit中的前八位编码
			out.write((char*)&char_temp, sizeof(unsigned char));//将载有哈夫曼编码信息的字符写入到压缩文件中去
		}
	}
	if (unit.length() > 0)//处理最后不足八位的哈夫曼编码
	{
		char_temp = '\0';
		for (auto i = 0; i < unit.length(); ++i)
		{
			char_temp = char_temp << 1;
			if (unit[i] == '1')
				char_temp = char_temp | 1;
		}
		for (auto i = 0; i < (8 - unit.length()); ++i)//将编码移到到高位
			char_temp = char_temp << 1;
		out.write((char*)&char_temp, sizeof(unsigned char));
	}
	in.close();
	out.close();
	//压缩文件完成，将哈夫曼树的数组和存储哈夫曼编码的数组的内存释放掉，并将字符的种类数置为0
	HuffmanTree.resize(0);
	Huffmancode.resize(0);
	H_number = 0;
}

//生成哈夫曼编码的函数
void ComAndEx::CreateHuffmanCode()
{
	Huffmancode_node elem;
	Huffmancode.push_back(elem);
	if (H_number == 1)
	{
		Huffmancode.push_back(elem);
		Huffmancode[1].name =HuffmanTree[1].name;
		Huffmancode[1].code.push_back(1);
	}
	for (int i = 1; i <= H_number; ++i)
	{
		Huffmancode.push_back(elem);
		Huffmancode[i].name = HuffmanTree[i].name;
		int temp_p = HuffmanTree[i].p;
		int temp_nodeindex = HuffmanTree[i].Hnodeindex;
		auto p = Huffmancode[i].code.end();
		while (temp_p != 0)
		{
			if (HuffmanTree[temp_p].lchild == temp_nodeindex)
			{
				p=Huffmancode[i].code.insert(p, 0);
			}
			else
			{
				p=Huffmancode[i].code.insert(p, 1);
			}
			temp_nodeindex = HuffmanTree[temp_p].Hnodeindex;
			temp_p = HuffmanTree[temp_p].p;
		}
	}
}

//构造哈夫曼树的函数
void ComAndEx::CreateHuffmanTree()
{
	if (H_number == 1)
	{
		HuffmanTree[1].lchild = HuffmanTree[1].rchild = 1;
	}
	int mark = H_number + 1;
	int x = 0, y = 0;
	int min = 999999999, secmin = 0;
	while (mark < 2 * H_number)
	{
		for (int j = 1; j < mark; ++j)
		{
			if (HuffmanTree[j].p == 0 && HuffmanTree[j].weight<min)
			{
				y = x;
				secmin = min;
				min = HuffmanTree[j].weight;
				x = HuffmanTree[j].Hnodeindex;
			}
			else if (HuffmanTree[j].p == 0 && HuffmanTree[j].weight < secmin)
			{
				secmin = HuffmanTree[j].weight;
				y = HuffmanTree[j].Hnodeindex;
			}
			else;
		}
		HuffmanTree[mark].weight = HuffmanTree[x].weight + HuffmanTree[y].weight;
		HuffmanTree[mark].lchild = x;
		HuffmanTree[mark].rchild = y;
		HuffmanTree[x].p = mark;
		HuffmanTree[y].p = mark;
		mark++;
		x = y = 0;
		min = 99999999999, secmin = 0;
	}
	HuffmanTree[2 * H_number - 1].p = 0;
}

//扫描原文件中字符的种类及频度的函数
string ComAndEx::ScanCharacter()
{
	cout << "请输入要压缩的文件的文件名:";
	string filename;
	cin >> filename;
	ifstream in(filename,ios::binary);//以二进制的格式打开源文件
	if (!in)                          //检查指定的源文件是否存在          
	{
		cout << "没有发现该文件.." << endl<<endl;
		return "NULL";
	}
	unsigned char temp = '\0';  //用一个8位的无符号变量来依次读取源文件的信息
	Hnode *temparry = new Hnode[256];  //创建一个临时数组，用于统计字符的种类及频度
	while (true)           //判断是否到达文件的末尾
	{
		temp = in.get();
		if (in.eof())break;
		temparry[temp].name = temp;
		temparry[temp].weight++;
	}
	in.close();  //关闭in与源文件的关联
	Hnode elem;
	HuffmanTree.push_back(elem);//使得哈夫曼有效的存储是从HuffmanTree[1]开始
	H_number = 0;
	for (int i = 0; i < 256; i++)//选择出频度大于0的字符存储到哈夫曼数组中
	{
		if (temparry[i].weight != 0)
		{
			H_number++;//统计字符的种类数
			HuffmanTree.push_back(elem);
			*(HuffmanTree.end()-1) = temparry[i];
		}
	}
	if (HuffmanTree.size()== 1)
	{
		cout << "您输入的文本为空文本.." << endl << endl;
		return "NULL";
	}
	delete[]temparry;//统计完毕以后释放临时数组的内存
	for (int i = 1; i <= (H_number - 1); i++)//为叶节点的双亲节点开辟内存
	{
		HuffmanTree.push_back(elem);
	}
	for (int i = 0; i < 2 * H_number; i++)//为哈夫曼数组的所有元素建立索引
	{
		HuffmanTree[i].Hnodeindex = i;
	}
	return filename;
}
void ComAndEx::Extract()
{
	cout << "请输入您要解压的文件的名字:";
	string filename;
	cin >> filename;
	ifstream in(filename, ios::binary);
	if (!in)
	{
		cout << "文件打开失败.." << endl;
		return;
	}
	in.read((char*)&H_number,sizeof(int));//读取字符种类数
	Hnode elem;
	HuffmanTree.push_back(elem);
	for (int i = 1; i <= H_number; ++i)//读取字符的种类及频度
	{
		HuffmanTree.push_back(elem);
		in.read((char*)&(HuffmanTree[i].name), sizeof(char));
		in.read((char*)&(HuffmanTree[i].weight), sizeof(int));
	}
	for(int i=1;i<=H_number-1;++i)//建立完整的哈夫曼数组
		HuffmanTree.push_back(elem);
	for (int i = 0; i < 2 * H_number; i++)//为哈夫曼数组元素建立索引
	{
		HuffmanTree[i].Hnodeindex = i;
	}
	CreateHuffmanTree();//生成哈夫曼树
	unsigned  char temp_char = '\0';//用来读取哈夫曼编码的单位
	unsigned long long length = 0;
	for (int i = 1; i <= H_number; ++i)//计算原文件的字符的个数
		length = length + HuffmanTree[i].weight;
	int top = HuffmanTree[2 * H_number - 1].Hnodeindex;//top存储的是结点的索引，初始化时根节点的索引
	cout << "请输入解压以后的文件的名字:";
	string extractedname;
	cin >> extractedname;
	ofstream out(extractedname,ios::binary);
	if (!out)
	{
		cout << "该文件打开失败.." << endl<<endl;
		return;
	}
    while(length)
	{
		temp_char = in.get();//
		for (int i = 0; i<8;++i)
		{
			if (temp_char & 128)//如果最高位为1
				top = HuffmanTree[top].rchild;//top存储结点的右孩子的索引
			else
				top = HuffmanTree[top].lchild;//如果最高位为0，top存储结点的左孩子的索引
			if (top <=H_number)//如果从根节点到达叶节点
			{
				out <<HuffmanTree[top].name;//输出字符
				length--;//更新原文件中未输出的字符的个数
				if (length ==0 )break;//如果原文件中未输出的个数为0，表明原文件的字符已经全部输出，循环不需要再继续
				top = HuffmanTree[2 * H_number - 1].Hnodeindex;//否则top复位为根节点的索引
			}
			temp_char = temp_char << 1;//如果没到达叶节点，那么temp_char左移一位，继续循环
		}
	}
	in.close();
	out.close();
	//压缩文件完成，将哈夫曼树的数组和存储哈夫曼编码的数组的内存释放掉，并将字符的种类数置为0
	HuffmanTree.resize(0);
	Huffmancode.resize(0);
	H_number = 0;
}
