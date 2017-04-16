#include"ComAndEx.h"
int main()
{
	ComAndEx *comandex = new ComAndEx;
	while (1)
	{
		cout << "1 " << "压缩文件" << endl;
		cout << "2 " << "解压文件" << endl;
		cout << "3 " << "退出程序" << endl;
		cout << "请选择要进行的操作:";
		int select = 0;
		cin >> select;
		if (select == 1)
		{
			comandex->Compress();
		}
		else if (select == 2)
		{
			comandex->Extract();
		}
		else if (select == 3)
		{
			break;
		}
		else
		{
			cout << "请输入正确的操作索引号.." << endl;
		}
	}
	return 0;
}
