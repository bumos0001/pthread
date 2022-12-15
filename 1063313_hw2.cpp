#include<iostream>
#include<string>
#include<pthread.h>
#include<fstream>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<vector>
#include<ctime>
#include<algorithm>
#include<sys/time.h>
using namespace std;
class my_data					//class�ΨӶǻ���� 
{
	public:
	vector <char> v;
	long TID1,TID2;
	string infix="";
	string postfix="";
	string prefix="";
	double t1_time;
	double t2_time;
};
int Rank(char a)				//�ƶ��� 
{
	switch(a)
	{
		case'/':return 2;
		case'*':return 2;
		case'+':return 1;
		case'-':return 1;
		default:return 0;
	}
}
string InToPre(string infix)	//infix�নprefix 
{
	vector<char>a;
	string prefix="";
	reverse(infix.begin(),infix.end());
	for(int i = 0; i< infix.size() ; i++)
	{
		if(infix[i] == '(')
		infix[i]=')';
		
		else if(infix[i]==')')
		infix[i]='(';
	}
	for(int i = 0 ; i < infix.size();i++)
	{
		if(infix[i] >= '0' && infix[i] <='9')
		prefix += infix[i];
		
		else if (infix[i] == '(' )
		a.push_back(infix[i]);
		else if (infix[i] == ')')
		{
			while((a.back()!='(' ) && (!a.empty()))
			{
				prefix += a.back();
				a.pop_back();
			} 
			if(a.back() =='(')
			a.pop_back();
		}
		else
		{
			if(a.empty())
			a.push_back(infix[i]);
			
			else
			{
				if(Rank(infix[i]) > Rank(a.back()))
				a.push_back(infix[i]);
				
				else if (Rank(infix[i]) ==Rank(a.back()))
				a.push_back(infix[i]);
				else
				{
					while((!a.empty()) && (Rank(infix[i]) < Rank(a.back())))
					{
						prefix += a.back();
						a.pop_back();
					} 
					a.push_back(infix[i]);
				}
			}
		}
	}
	while (!a.empty())
	{
		prefix += a.back();
		a.pop_back();
	}
	reverse (prefix.begin(),prefix.end());
	return prefix;
}
int cal_pre(string str)		//�p��prefix 
{
	vector<char>bb;
	
	for(int i= str.size()-1 ; i>=0 ;i--)
	{
		if(str[i]>='0' && str[i]<='9')
		bb.push_back(str[i]-'0');
		
		else
		{
			int a = bb.back();
			bb.pop_back();
			int b = bb.back();
			bb.pop_back();
		
			switch(str[i])
			{
				case '+':bb.push_back(a+b);
					break;
				case '-':bb.push_back(a-b);
					break;
				case '*':bb.push_back(a*b);
					break;
				case '/':bb.push_back(a/b);
					break;
			}
		}
		
	}
	int result = bb.back();
	return result;
}
string InToPost(string infix)	//infix�নpostfix 
{
	vector<char>a;
	string postfix="";
	for(int i = 0 ; i<infix.size();i++)
	{
		if(infix[i]>='0'&&infix[i]<='9')
			postfix+=infix[i];
		else if(infix[i]=='(')
		{
			a.push_back(infix[i]);
		}
		else if (infix [i]==')')
		{
			while((a.back()!='(')&&!(a.empty()))
			{
				char b=a.back();
				postfix+=b;
				a.pop_back();
			}
			if(a.back()=='(')
			{
				a.pop_back();
			}
		}
		else
		{
			if(a.empty())
			{
				a.push_back(infix[i]);
			}
			else
			{
				if(Rank(infix[i])>Rank(a.back()))
				{
					a.push_back(infix[i]);
				}
				else if((Rank(infix[i])==Rank(a.back()))&&(infix[i]=='^'))
				{
					a.push_back(infix[i]);
				}
				else
				{
					while(!a.empty()&&(Rank(infix[i])<=Rank(a.back())))
					{
						postfix +=a.back();
						a.pop_back();
					}
					a.push_back(infix[i]);
				}
			}
			
		}
	}
	while(!a.empty())
	{
		postfix +=a.back();
		a.pop_back();
	}
	return postfix;
}
int cal_post(string str)	//�p��postfix 
{
	
	vector<char>aa;
	for(int i= 0 ; i<str.size();i++)
	{
		if(str[i]>='0' && str[i]<='9')
		aa.push_back(str[i]-'0');
		
		else
		{
			int a = aa.back();
			aa.pop_back();
			int b = aa.back();
			aa.pop_back();
		
			switch(str[i])
			{
				case '+':aa.push_back(b+a);
					break;
				case '-':aa.push_back(b-a);
					break;
				case '*':aa.push_back(b*a);
					break;
				case '/':aa.push_back(b/a);
					break;
			}
		}
	}
	
	int result = aa.back();
	return result;
	
}
void* thread1(void* arg)
{
	struct timeval begin , end;		//�p��ɶ� 
	gettimeofday(&begin,0);			//�}�l�p�� 
	my_data *data = (my_data *)arg;
	
	long tid = gettid();			//���otid 
	string infix = data ->infix;	//���oinfix��
	string prefix = InToPre(infix); //�i��prefix�ഫ 
	data->TID1=tid;					//tid�Ǩ�class 
	cout<<"[Child 1]";
	
	for(int i = 0 ; i < prefix.size() ;i++)
		cout<<prefix[i]<<" ";
	cout<<"= ";
	int result = cal_pre(prefix);	//�i��prefix�p�� 
	cout<<result<<" ";
	
	gettimeofday(&end,0);			//�����p��	
	long double microseconds = end.tv_usec - begin.tv_usec;
	cout<<microseconds/1000<<"ms"<<endl;
	data->t1_time=microseconds/1000;
	return 0;
}
void* thread2(void* arg)
{
	struct timeval begin , end;		//�p��ɶ� 
	gettimeofday(&begin,0);			//�}�l�p�� 
	
	my_data *data = (my_data *)arg;
	long tid = gettid();			//���otid 
	data->TID2=tid;					//tid�Ǩ�class 
	string infix = data->infix;		//���oinfix�� 
	string postfix = InToPost(infix);//�i��postfix�ഫ 
	cout<<"[Child 2]";
	
	for(int i = 0 ; i <postfix.size() ;i++)
		cout<<postfix[i]<<" ";

	cout<<"= ";
	cout<<cal_post(postfix)<<" ";	//�p��postfix���G 
	
	gettimeofday(&end,0);				//�����p�� 
	long double microseconds = end.tv_usec - begin.tv_usec;
	cout<<microseconds/1000<<"ms"<<endl;
	data->t2_time=microseconds/1000;	//thread2����ɶ��ǵ�class 
	return 0;
}


int main()
{
	struct timeval begin , end;			//�p��ɶ� 
	gettimeofday(&begin,0);				//�}�l�p�� 
	
	string input_data;
	string data_name="";
	cin>>data_name;						//��J�ɦW 
	ifstream text;
	text.open(data_name);
	
	getline(text,input_data);			//Ū����� 
	my_data data;
	pthread_t t1;
	pthread_t t2;
	string check = input_data;
	int temp=1;
	int ltemp=0;
	int rtemp=0;
	for(int i = 0 ; i < check.size() ; i++)		//�ˬd�r��O�_�ŦX�W�h
	{
		if(check[i]=='*'||check[i]=='+'||check[i]=='*'||check[i]=='/')
		check[i]='x';
	}
	if(check[0]=='x'||check[check.size()-1]=='x')
	temp=0;
	for(int i = 0 ; i < check.size() ; i++)
	{
		if(check[i]=='(')
		ltemp++;
		if(check[i]==')')
		rtemp++;
	}
	if(ltemp!=rtemp)
	temp=0;
	for(int i = 0 ; i < check.size() ; i++)
	{
		if(check[i]=='x' &&check[i+1]=='x')
		{
			temp=0;
			break;
		}
	}
	
	if(temp==0)
	cout<<"Wrong input! "<<endl;
	else if(temp ==1)
	{
	cout<<"The infix input: "<<input_data<<endl;
	int PID_t1,PID_t2;
	data.infix = input_data;
	PID_t1=pthread_create(&t1,0,thread1, &data);		//�إ�thread1 
	pthread_join(t1,0);
	
	PID_t2=pthread_create(&t2,0,thread2, &data);		//�إ�thread2 
	pthread_join(t2,0);
	
	cout<<"[Child 1 tid="<<data.TID1<<"] "<<data.t1_time<<"ms"<<endl;
	cout<<"[Child 2 tid="<<data.TID2<<"] "<<data.t2_time<<"ms"<<endl;
	
	gettimeofday(&end,0);				//�����p�� 
	long double diff = end.tv_usec - begin.tv_usec;	//�p��ɮt 
	cout<<"[Main thread] "<<diff/1000<<"ms"<<endl;
	 }
}
