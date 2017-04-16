#include"ComAndEx.h"
ComAndEx::ComAndEx()
{
	H_number = 0;
	mark = 0;
}
void ComAndEx::Compress()
{
	string outfilename=ScanCharacter(); //扫描源文件种字符的种类及个数，返回源文件的文件名并用string类型变量保存
	if (outfilename == "NULL") return;
	CreateHuffmanTree(); //构造哈夫曼树
	CreateHuffmanCode(); //生成哈夫曼编码
	cout << "请输入被压缩后的文件的名字:";
	string filename;
	cin >> filename;
	ofstream out(filename, ios::binary);//向压缩文件中写入源文件的信息
	if (!out)
	{
		cout << "创建压缩文件失败.." << endl << endl;
		return;
	}
	out << H_number<<'s';//向压缩文件中输入字符的种类数
	for (int i = 1; i <= H_number; ++i)//依次输入字符
	{
		out << Huffmancode[i].name;
	}
	for (int i = 1; i <= H_number; ++i)//依次输入字符对应的频度
	{
		out << HuffmanTree[i].weight<<'|';
	}
	out << 'f';
	ifstream in(outfilename, ios::binary);//第二遍扫描源文件
	if (!in)
	{
		cout << "打开源文件失败" << endl<<endl;
		return;
	}
	unsigned char char_temp = '\0';//用8位的无符号变量作为读写文件的单元
	string unit;//存储哈夫曼编码，满八位就和char_temp进行|操作
	while (true)
	{
		char_temp=in.get();
		if (in.eof()) break;
		//in >>noskipws>>char_temp;
		for (int i = 1; i <= H_number; ++i)
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
		while (unit.length() >= 8)
		{
			char_temp = '\0';
			for (int i = 0; i < 8; ++i)
			{
				char_temp =char_temp<< 1;
				if (unit[i] == '1')
					char_temp = char_temp | 1;
			}
			auto p = unit.begin();
			unit.erase(p, p + 8);//清空前八位
			out << char_temp;
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
		out << char_temp;
	}
	//out << 'e';
	in.close();
	out.close();
	HuffmanTree.resize(0);//将哈夫曼树的容器置为0
	Huffmancode.resize(0);//将哈夫曼编码的容器置为0
	H_number = 0;         //字符的种类数置为0
}
void ComAndEx::CreateHuffmanCode()
{
	Huffmancode_node elem;
	Huffmancode.push_back(elem);
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
void ComAndEx::CreateHuffmanTree()
{
    mark = H_number + 1;//HuffmanTree[mark]为第一个空的节点
	int x = 0, y = 0;
	while (mark < 2 * H_number)
	{
		select(x, y);
		HuffmanTree[mark].weight = HuffmanTree[x].weight + HuffmanTree[y].weight;
		HuffmanTree[mark].lchild = x;
		HuffmanTree[mark].rchild = y;
		HuffmanTree[x].p = mark;
		HuffmanTree[y].p = mark;
		mark++;
	}
	HuffmanTree[2 * H_number - 1].p = 0;
}
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
		//in >> noskipws >> temp;
		if (in.eof())break;
		temparry[temp].name = temp;
		temparry[temp].weight++;
	}
	in.close();  //关闭in与源文件的关联
	Hnode elem;
	HuffmanTree.push_back(elem);//使得哈夫曼有效的存储是从HuffmanTree[1]开始
	for (int i = 0; i < 256; i++)//选择出频度大于0的字符存储到哈夫曼数组中
	{
		if (temparry[i].weight != 0)
		{
			H_number++;//统计字符的种类数
			HuffmanTree.push_back(elem);
			*(HuffmanTree.end()-1) = temparry[i];
		}
	}
	if (HuffmanTree.size()== 2)
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
	test2();
	return filename;
}
void ComAndEx::select(int&min, int&secmin)
{
	Hnode temp;
	for (int i = 1; i < mark; ++i)
	{
		if (HuffmanTree[i].p == 0)
		{
			temp = HuffmanTree[i];
			break;
		}
	}
	for (int i = 1; i < mark; ++i)
	{
		if (HuffmanTree[i].p == 0)
		{
			if (HuffmanTree[i].weight <= temp.weight)
				temp = HuffmanTree[i];
		}
	}
	min = temp.Hnodeindex;
	for (int i = 1; i < mark; ++i)
	{
		if (HuffmanTree[i].p == 0 && i != min)
		{
			temp = HuffmanTree[i];
			break;
		}
	}
	for (int i = 1; i < mark; ++i)
	{
		if (HuffmanTree[i].p == 0 && i != min)
		{
			if (HuffmanTree[i].weight <= temp.weight)
				temp = HuffmanTree[i];
		}
	}
	secmin = temp.Hnodeindex;
}
void ComAndEx::test1()
{
	for (int i = 1; i <= H_number; ++i)
	{
		cout << Huffmancode[i].name << ":";
		for (auto elem : Huffmancode[i].code)
		{
			cout << elem;
		}
			cout << endl;
	}
}
void ComAndEx::test2()
{
	for (int i = 1; i <= H_number; ++i)
	{
		cout << HuffmanTree[i].name << ":" << HuffmanTree[i].weight << endl;
	}
}
void ComAndEx::Extract()
{
	cout << "请输入您要解压的文件的名字:";
	string filename;
	cin >> filename;
	ifstream in(filename, ios::binary);
	if (!in)
	{
		cout << "没有找到该文件.." << endl<<endl;
		return;
	}
	unsigned char flag = '\0';
	in >> noskipws >> H_number;
	//flag=in.get();
	in >> noskipws >> flag;
	if (flag != 's')
	{
		cout << "没正常扫描到s,所以字符不能正确还原.." << endl<<endl;
		return;
	}
	Hnode elem;
	HuffmanTree.push_back(elem);
	for (int i = 1; i <= H_number; ++i)
	{
		HuffmanTree.push_back(elem);
		//HuffmanTree[i].name=in.get();
		in >> noskipws >> HuffmanTree[i].name;
	}
	for (int i = 1; i <= H_number; ++i)
	{
		in >> noskipws >> HuffmanTree[i].weight;
		//flag=in.get(); 
		in >> noskipws >> flag;
	}
	//flag = in.get();
	in >> noskipws >> flag;
	if (flag != 'f')
	{
		cout << "源文件信息未得到正确扫描.." << endl<<endl;
		return;
	}
	for(int i=1;i<=H_number-1;++i)
		HuffmanTree.push_back(elem);
	for (int i = 0; i < 2 * H_number; i++)
	{
		HuffmanTree[i].Hnodeindex = i;
	}
	CreateHuffmanTree();
	unsigned  char temp_char = '\0';
	unsigned long long Length = 0;
	for (int i = 1; i <= H_number; ++i)
		Length = Length + HuffmanTree[i].weight;
	unsigned long long length = 0;
	int top = HuffmanTree[2 * H_number - 1].Hnodeindex;
	cout << "请输入解压以后的文件的名字:";
	string extractedname;
	cin >> extractedname;
	ofstream out(extractedname,ios::binary);
	if (!out)
	{
		cout << "该文件打开失败.." << endl<<endl;
		return;
	}
    while(true)
	{
		//temp_char=in.get();
		in >> noskipws >> temp_char;
		if (in.eof())break;
		for (int i = 0; i<8;++i)
		{
			if (temp_char & 128)
				top = HuffmanTree[top].rchild;
			else
				top = HuffmanTree[top].lchild;
			if (top <=H_number)
			{
				out << HuffmanTree[top].name;
				length++;
				if (length ==Length )break;
				top = HuffmanTree[2 * H_number - 1].Hnodeindex;
			}
			temp_char = temp_char << 1;
		}
	}
	in.close();
	out.close();
	HuffmanTree.resize(0);
	Huffmancode.resize(0);
	H_number = 0;
}
