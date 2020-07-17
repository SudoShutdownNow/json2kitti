#include <iostream>
#include <iomanip>
#pragma warning (disable : 4996)
using namespace std;
char rawdata[8000];//�洢json����������
void findpicname(char* read, char* p);
int getnumber(char* read);//��ȡ��json���еı�ǩ��
void getbox(char* read,char* p,int j);//��BoundingBox���ַ������룬j����ǰ��ȡ����json�еĵڼ���
void getlabelname(char* read, char* p, int j);//��ȡ��ǩ��

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
	cout << "��������ʼ���ļ���ţ�";
	cin >> fileno;
	char picturename[50];//�洢��json��Ӧ��ͼƬ�ļ���
	char savename[50];//������ļ���
	int labelcontain = 0;//�洢��json���еı�ǩ����
	char box[20];

	int failure = 0;//�ļ��жϼ�����

	for (int i = fileno; ; i++)//���ļ�
	{
		sprintf(filename, "%04d", i);
		//rightmove(filename, strlen(filename));
		sprintf(filename, "%s.json", filename);
		//cout << filename << endl;
		
		if ((fp = fopen(filename, "r")) == NULL)//���ļ�
		{
			if (failure > 100)//�ж�֮ǰ�Ƿ��жϹ�100�������ļ�
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
			failure = 0;//��ȡ�ļ��ɹ���Ϊfailure����
		}

		fgets(rawdata, 8000, fp);//��json����������ȫ��װ��rawdata��

		char* read ;//��ʼ������ȡ��������
		read = rawdata;
		findpicname(read, picturename);//��picturenameװ��

		labelcontain = getnumber(read);

		int j = 1;//��ȡ�ļ��ڱ�ǩ������
		for (j = 1; j <= labelcontain; j++)//��ȡ������ǩ����
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
		cout << savename << "ת����ɣ�" << endl;
		
		

	}
	system("pause");
	exit(0);
}

void findpicname(char* read, char* p)
{
	read = strstr(read, "FileName");//��λfilename��ǩ
	while (1)
	{
		if (*read == ':')//ָ����ǰ�ƶ���ֱ����ȡ����һ�� :
		{
			while (1)
			{
				if (*read == '\"')//ָ������ƶ���ֱ����ȡ�� "
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
	while (*read != '.')//��readָ����ַ�װ��*pָ����ַ�����ֱ����ȡ�� "
	{
		*p = *read;
		p++;
		read++;
	}
	*p = 0;//���ַ�����β
}

int getnumber(char* read)
{
	char num[5];
	int intnum = 0;
	char* p;
	p = num;
	read = strstr(read, "NumOfAnno");//��λNumOfAnno��ǩ
	while (1)
	{
		if (*read == ':')//ָ����ǰ�ƶ���ֱ����ȡ����һ�� :
		{
			while (1)
			{
				if (*read == ' ')//ָ������ƶ���ֱ����ȡ�� �ո�
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
	while (*read != ',')//��readָ����ַ�װ��*pָ����ַ�����ֱ����ȡ�� ,
	{
		*p = *read;
		p++;
		read++;
	}
	*p = 0;//���ַ�����β

	sscanf(num, "%d", &intnum);
	return intnum;

}

void getbox(char* read,char* p,int j)
{
	read = strstr(read, "BoundingBox");//��λclassname��ǩ
	for (int i = 1; i < j; i++)
	{

		read += strlen("BoundingBox");//�Թ��Ѿ���ȡ�ı�ǩ
		read = strstr(read, "BoundingBox");//��λclassname��ǩ

	}
	
	while (1)
	{
		if (*read == ':')//ָ����ǰ�ƶ���ֱ����ȡ����һ�� :
		{
			while (1)
			{
				if (*read == '[')//ָ������ƶ���ֱ����ȡ�� [
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
	while (*read != ']')//��readָ����ַ�װ��*pָ����ַ�����ֱ����ȡ�� ]
	{
		*p = *read;
		p++;
		read++;
	}
	*p = 0;//���ַ�����β
}

void getlabelname(char* read, char* p,int j)
{
	read = strstr(read, "classname");//��λclassname��ǩ
	for (int i = 1; i < j; i++)
	{
		
		read += strlen("classname");//�Թ��Ѿ���ȡ�ı�ǩ
		read = strstr(read, "classname");//��λclassname��ǩ
		
	}

	while (1)
	{
		if (*read == ':')//ָ����ǰ�ƶ���ֱ����ȡ����һ�� :
		{
			while (1)
			{
				if (*read == '\"')//ָ������ƶ���ֱ����ȡ�� "
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
	while (*read != '\"')//��readָ����ַ�װ��*pָ����ַ�����ֱ����ȡ�� "
	{
		*p = *read;
		p++;
		read++;
	}
	*p = 0;//���ַ�����β
}