#include <iostream>
#include <iomanip>
#pragma warning (disable : 4996)
using namespace std;
char rawdata[8000];//存储json的所有数据
void findpicname(char* read, char* p);
int getnumber(char* read);//获取该json含有的标签数
void getbox(char* read,char* p,int j);//将BoundingBox以字符串读入，j代表当前读取的是json中的第几项
void getlabelname(char* read, char* p, int j);//获取标签名

//void rightmove(char* p, int length);

struct element
{
	char label[50];
	float xmin, ymin, xmax, ymax;
}element[100];
int main()
{
	FILE* fp;
	int fileno = 0;
	char filename[15];
	cout << "请输入起始的文件序号：";
	cin >> fileno;
	char picturename[50];//存储该json对应的图片文件名
	char savename[50];//保存的文件名
	int labelcontain = 0;//存储该json含有的标签数量
	char box[20];

	int failure = 0;//文件中断计数器

	for (int i = fileno; ; i++)//打开文件
	{
		sprintf(filename, "%04d", i);
		//rightmove(filename, strlen(filename));
		sprintf(filename, "%s.json", filename);
		//cout << filename << endl;
		
		if ((fp = fopen(filename, "r")) == NULL)//打开文件
		{
			if (failure > 100)//判断之前是否中断过100个以上文件
			{
				
				break;
			}
			else
			{
				failure++;
				continue;
			}
		}
		else
		{
			failure = 0;//读取文件成功后为failure清零
		}

		fgets(rawdata, 8000, fp);//将json的所有内容全部装到rawdata里

		char* read ;//开始解析读取到的内容
		read = rawdata;
		findpicname(read, picturename);//将picturename装入

		labelcontain = getnumber(read);

		int j = 1;//读取文件内标签计数器
		for (j = 1; j <= labelcontain; j++)//读取各个标签内容
		{
			getbox(read, box, j);
			sscanf(box, "%f, %f, %f, %f", &element[j].xmin, &element[j].ymin, &element[j].xmax, &element[j].ymax);
			getlabelname(read, element[j].label, j);
			//cout <<picturename<<":"<< element[j].label <<": "<< element[j].xmin <<","<< element[j].ymin <<","<< element[j].xmax <<","<< element[j].ymax << endl;
			
		}
		fclose(fp);
		
		sprintf(savename, "%s.txt", picturename);
		fp = fopen(savename, "w");
		for (j = 1; j <= labelcontain; j++)
		{
			fprintf(fp,"%s 0.00 0 0.0 %.2f %.2f %.2f %.2f 0.0 0.0 0.0 0.0 0.0 0.0 0.0", element[j].label, element[j].xmin, element[j].ymin, element[j].xmax, element[j].ymax);
			if (j < labelcontain)
			{
				fprintf(fp, "\n");
			}
		}


		
		fclose(fp);
		cout << savename << "转换完成！" << endl;
		
		

	}
	system("pause");
	exit(0);
}

void findpicname(char* read, char* p)
{
	read = strstr(read, "FileName");//定位filename标签
	while (1)
	{
		if (*read == ':')//指针向前移动，直到读取到第一个 :
		{
			while (1)
			{
				if (*read == '\"')//指针继续移动，直到读取到 "
				{
					read++;
					goto readname;
				}
				read++;
			}
		}
		read++;
	}

	readname:
	while (*read != '.')//将read指向的字符装入*p指向的字符串，直到读取到 "
	{
		*p = *read;
		p++;
		read++;
	}
	*p = 0;//给字符串结尾
}

int getnumber(char* read)
{
	char num[5];
	int intnum = 0;
	char* p;
	p = num;
	read = strstr(read, "NumOfAnno");//定位NumOfAnno标签
	while (1)
	{
		if (*read == ':')//指针向前移动，直到读取到第一个 :
		{
			while (1)
			{
				if (*read == ' ')//指针继续移动，直到读取到 空格
				{
					read++;
					goto readname;
				}
				read++;
			}
		}
		read++;
	}

readname:
	while (*read != ',')//将read指向的字符装入*p指向的字符串，直到读取到 ,
	{
		*p = *read;
		p++;
		read++;
	}
	*p = 0;//给字符串结尾

	sscanf(num, "%d", &intnum);
	return intnum;

}

void getbox(char* read,char* p,int j)
{
	read = strstr(read, "BoundingBox");//定位classname标签
	for (int i = 1; i < j; i++)
	{

		read += strlen("BoundingBox");//略过已经读取的标签
		read = strstr(read, "BoundingBox");//定位classname标签

	}
	
	while (1)
	{
		if (*read == ':')//指针向前移动，直到读取到第一个 :
		{
			while (1)
			{
				if (*read == '[')//指针继续移动，直到读取到 [
				{
					read++;
					goto readname;
				}
				read++;
			}
		}
		read++;
	}

readname:
	while (*read != ']')//将read指向的字符装入*p指向的字符串，直到读取到 ]
	{
		*p = *read;
		p++;
		read++;
	}
	*p = 0;//给字符串结尾
}

void getlabelname(char* read, char* p,int j)
{
	read = strstr(read, "classname");//定位classname标签
	for (int i = 1; i < j; i++)
	{
		
		read += strlen("classname");//略过已经读取的标签
		read = strstr(read, "classname");//定位classname标签
		
	}

	while (1)
	{
		if (*read == ':')//指针向前移动，直到读取到第一个 :
		{
			while (1)
			{
				if (*read == '\"')//指针继续移动，直到读取到 "
				{
					read++;
					goto readname;
				}
				read++;
			}
		}
		read++;
	}

readname:
	while (*read != '\"')//将read指向的字符装入*p指向的字符串，直到读取到 "
	{
		*p = *read;
		p++;
		read++;
	}
	*p = 0;//给字符串结尾
}