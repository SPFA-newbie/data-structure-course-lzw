#include "lzw_core.h"
#include "help_string.h"
#include "comm_string.h"

#include<string>
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<iostream>
using namespace std;

const char** info;
LzwCore core;

inline string getPath(const char* path)
{
	string ret="";
	int pos=0;
	while(path[pos]!='\0')pos++;
	while(pos>=0&&path[pos]!='\\')pos--;
	pos++;
	ret.insert(0,path);
	ret=ret.substr(0,pos);
	return ret;
}

inline void helpCase()
{
	cout<<helpString_1<<helpString_2<<helpString_3<<helpString_4<<helpString_5<<endl;
	return;
}

inline void compressCase(int argc)
{
	int base=2;
	if(strcmp(info[base],"-set")==0)//change setting
	{
		base++;
		core.setNavDataLen(info[base][0]-'0');
		core.setMapCodeLen(info[base][1]-'0');
		core.setDocSizeLen(info[base][2]-'0');
		base++; 
	}
	
	if(strcmp(info[base],"-tar")==0)//Have target 
	{
		base++;
		if(strcmp(info[base],"-a")==0)//Adding
		{
			base++;
			core.compressFileAdding(argc-base-1,info+base+1,info[base]);	
		}else//New
		 core.compressFileGroup(argc-base-1,info+base+1,info[base]);
	}else//Don`t have target
	{
		core.compressFile(info[base]);
	}
	return; 
}

inline void decompressCase(int argc)
{
	if(argc==2)//the way of open file
	{
		cout<<getPath(info[1]).c_str()<<endl;
		core.decompressFile(info[1]);
		return;
	}
	
	if(strcmp(info[2],"-p")==0)//Have path
	{
		if(argc!=5)//argv is not enough
		{
			cout<<errorString<<endl;
			return;
		}
		core.decompressFile(info[4],info[3]);
	}else//No path 
	{
		if(argc!=3)//argv is not enough
		{
			cout<<errorString<<endl;
			return;
		}
		core.decompressFile(info[2],getPath(info[2]).c_str());
	}
	return;
}

int main(int argc,char** argv)
{
//	for(int i=0;i<argc;i++)
//	 cout<<argv[i]<<endl;
	
	//Move to system path
	string self="";
	self.insert(0,argv[0]);
	if(self!="lzwZip" && self!="C:\\Windows\\System32\\lzwZip.exe" && self!="C:\\Windows\\system32\\lzwZip.exe")
	{
		cout<<initString<<endl;
		system("pause");
		string comm="";
		comm.insert(0," C:\\Windows\\System32\\lzwZip.exe 1> nul 2>nul");
		comm.insert(0,self);
		comm.insert(0,"copy /B ");
		system(comm.c_str());
		return 0;
	}
	
	//Pack all argv
	info=new const char*[argc];
	string* tmp=new string[argc];
	for(int i=0;i<argc;i++)
	{
		tmp[i]="";
		tmp[i].insert(0,argv[i]);
		info[i]=tmp[i].c_str();	
	}
	
	if(argc==1||strcmp(info[1],"-help")==0)helpCase();else//help
 	if(strcmp(info[1],"-c")==0)compressCase(argc);else//compress
	if(strcmp(info[1],"-d")==0)decompressCase(argc);else//decopress
	if(argc==2)decompressCase(argc);else//the way of open file
	 cout<<errorString<<endl;//error
	
	delete[] info;
	return 0;
}
 
