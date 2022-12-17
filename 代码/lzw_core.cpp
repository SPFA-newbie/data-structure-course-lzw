#include "lzw_core.h"
#include "lzw_exception.h"

#include<cstdio> 
#include<vector>
#include<fstream>

//#include<iostream>
//#define DEBUG_TOOL

using namespace std;

char LzwCore::fileTag='\\';
//---------------------------------------

void LzwCore::makeNavDic(std::vector<char>& data)
{
	if(data.size()==this->navDataLen)
	{
		string key="";
		char* temp=new char[this->navDataLen+1];
		temp[this->navDataLen]='\0';
		for (int i=0;i<this->navDataLen;i++)
		 temp[i]=data[i];
		key=this->makeString(temp,this->navDataLen);
		delete[] temp;

		this->insertNewMap(key);
//		this->debugTool("data",key);
//		this->debugTool("code",this->dicLen-1);
//		this->debugTool("-----------------------------","");
	}else
	{
		for (int i=0;i<256;i++)
		{
			data.push_back(i);
			this->makeNavDic(data);
			data.pop_back();
		}
	}
	return;
}

void LzwCore::makeNavTDic(std::vector<char>& data)
{
	if(data.size()==this->navDataLen)
	{
		string key="";
		char* temp=new char[this->navDataLen+1];
		temp[this->navDataLen]='\0';
		for (int i=0;i<this->navDataLen;i++)
			temp[i]=data[i];
		key=this->makeString(temp,this->navDataLen);
		delete[] temp;

		this->insertNewTMap(key);
//		this->debugTool("data",key);
//		this->debugTool("code",this->dicLen-1);
//		this->debugTool("-----------------------------","");
	}else
	{
		for (int i=0;i<256;i++)
		{
			data.push_back(i);
			this->makeNavTDic(data);
			data.pop_back();
		}
	}
	return;
}
//------------------

std::string LzwCore::makeCodeString(unsigned long long num,char len)
{
	std::string code = "";
	for (int i = 0; i < len; i++)
	{
		std::string temp = "0";
		temp[0] = ((char)(num % 256));
		num = (num >> 8);
		code.insert(code.length(), temp);
	}
	return code;
}

//------------------

std::string LzwCore::makeFileHeader()
{
	string ret="0000";
	ret[0]='W';
	ret[1]=this->navDataLen;
	ret[2]=this->mapCodeLen;
	ret[3]=this->docSizeLen;
	return ret;
}

void LzwCore::catchFileHeader(const char* info)
{
	try
	{
		if(info[0]!='W')throw InvalidFileHeader();
		this->setNavDataLen(info[1]);
		this->setMapCodeLen(info[2]);
		this->setDocSizeLen(info[3]);
	}catch(LzwCoreException e)
	{
		throw InvalidFileHeader();
	}
	return;
}
//---------------------

std::string LzwCore::makeString(char* str,unsigned long long len)
{
	string ret="";
	string temp="0";
	for(unsigned long long i=0;i<len;i++)
	 ret.insert(0,temp);
	for(unsigned long long i=0;i<len;i++)
	 ret[i]=str[i];
	return ret;
}
//-----------------------

std::string LzwCore::getRealFileName(const char* src)
{
	string ret="";
	int pos=0;
	while(src[pos]!='\0')pos++;
	while(pos>=0&&src[pos]!=fileTag)pos--;
	pos++;
	ret.insert(0,src+pos);
	return ret;
}

std::string LzwCore::getPathOfFile(const char* src)
{
	string ret="";
	int pos=0;
	while(src[pos]!='\0')pos++;
	while(pos>=0&&src[pos]!=fileTag)pos--;
	pos++;
	ret.insert(0,src);
	ret=ret.substr(0,pos);
	return ret;
}

std::string LzwCore::getFileNameInDoc(std::ifstream& in)
{
	unsigned nameLen;
	in.read((char*)&nameLen,4);
	if(!in)return "";
	string name="";
	char* middle=new char[nameLen+1];
	in.read(middle,nameLen);
	middle[nameLen]='\0';
	name=this->makeString(middle,nameLen);
	delete[] middle;
	return name;
} 
//---------------------

void LzwCore::compression(std::string data,std::string& now,std::string& out)
{
	string temp=now;
	temp.insert(temp.length(),data);
	if(dic.find(temp)!=dic.end())
	 now=temp;
	 else
	 {
	 	out.insert(out.length(),dic[now]);
//	 	this->debugTool("the data",data);
//	 	this->debugTool("insert data",temp);
//	 	this->debugTool("the code",this->dicLen);
//	 	this->debugTool("----------------------------","");
	 	this->insertNewMap(temp);
	 	now="";
	 	now.insert(now.length(),data);
	 }
	 return;
}

void LzwCore::decompression(std::string code,std::string& last,std::string& out)
{
	if(dic.find(code)!=dic.end())
	{
		string now=dic[code];
		out.insert(out.length(),now);
		last.insert(last.length(),now.substr(0,this->navDataLen));
		this->insertNewTMap(last);
		last=now;
	}else
	{
		last.insert(last.length(),last.substr(0,this->navDataLen));
		out.insert(out.length(),last);
		this->insertNewTMap(last);
	}
	return;
}
//-----------------

FILE* LzwCore::makeCompressTempFile(unsigned long long& fileLen,std::ifstream& in)
{
	FILE* tempFile=tmpfile();//使用临时文件存储压缩后的内容 
	string code="";
	string now="";
	string data="";
	this->refreshDic();
	while(in)
	{
		char* middle=new char[this->navDataLen+1];
		in.read(middle,this->navDataLen);
		middle[this->navDataLen]='\0';
		data=this->makeString(middle,in.gcount());//!!!!!!!
//		this->debugTool("get number",in.gcount());
		delete[] middle;
		this->compression(data,now,code);
		data="";
		if(code.length()!=0)//compression已经给出输出 
		{
			fwrite(code.data(),1,this->mapCodeLen,tempFile);
			fileLen+=code.length();
			code="";
		}
		
		//满字典处理 
		if(this->mapCodeLen==8&&this->dicLen+1<this->dicLen)
		{
			fwrite((this->dic[now]).data(),1,this->mapCodeLen,tempFile);
			fileLen+=((this->dic[now]).length());
			this->refreshDic();
		}else if(this->dicLen+1==(((unsigned long long)1)<<(this->mapCodeLen*8)))
		{
			fwrite((this->dic[now]).data(),1,this->mapCodeLen,tempFile);
			fileLen+=((this->dic[now]).length());
			this->refreshDic();
//			debugTool("Clear",0);
		}
	}
//	debugTool("Done",0); 
//	debugTool("last",now);
//	debugTool("last map",dic[now]);
//	this->debugTool("the last one is",now);
//	this->debugTool("the last one code is",this->dic[now]);
	fwrite((this->dic[now]).data(),1,this->mapCodeLen,tempFile);
	fileLen+=((this->dic[now]).length());
	
	rewind(tempFile);//重置指针
	return tempFile; 
} 
//------------------------------

void LzwCore::makeDecompressFile(unsigned long long& fileLen,const char* navDoc,std::ifstream& in)
{
	ofstream out(navDoc,ios::binary);
	this->debugTool("file length",fileLen);
	
	string code="";
	string last="";
	string outString="";
	unsigned long long pos=0;
	bool refreshTag=true;
	this->refreshTDic();
	
	while(pos<fileLen)
	{
		char* middle=new char[this->mapCodeLen+1];
		in.read(middle,this->mapCodeLen);
		pos+=this->mapCodeLen;
		middle[this->mapCodeLen]='\0';
		code.clear();
		code=this->makeString(middle,this->mapCodeLen);
		delete[] middle;
		this->debugTool("Now position is",pos);
		
		if(refreshTag)//初始时或重置字典完成标志 
		{
			refreshTag=false;
			last.clear();
			last.insert(0,dic[code]);
			outString=this->dic[code];
			out.write(outString.data(),outString.length());
			outString.clear();
		}else//正常状态下 
		{
			this->decompression(code,last,outString);
			out.write(outString.data(),outString.length());
//			this->debugTool("Out",outString);
			outString.clear();
			
			//满字典处理 
			if(this->mapCodeLen==8&&this->dicLen+1<this->dicLen)
			{
				this->refreshTDic();
				refreshTag=true;
			}else if(this->dicLen+1==(((unsigned long long)1)<<(this->mapCodeLen*8)))
			{
				this->refreshTDic();
				refreshTag=true;				
			}
		}
//		this->debugTool("code is",code);
//		this->debugTool("code map is",this->dic[code]);
//		this->debugTool("last string is",last);
//		this->debugTool("---------------------------------------","");
	}
//	this->debugTool("exit","");
	out.close();
	return;	
}
//------------------------------

//由于字典问题暂时放弃多线程压缩 
//void LzwCore::multithreadingCore(void* info)
//{
//	struct ThreadInfo
//	{
//		const char* src;
//		unsigned long long fileLen; 
//		FILE* tempFile;
//	};
//	ThreadInfo* infoLink=(ThreadInfo*)info;
//	
//	return;
//}
//--------------------------------

void LzwCore::debugTool(std::string name,unsigned long long info)
{
	#ifdef DEBUG_TOOL
	cout<<name<<" : "<<info<<endl;
	#endif
	return;
}

void LzwCore::debugTool(std::string name,std::string info)
{
	#ifdef DEBUG_TOOL
	cout<<name<<" : ";
	for(int i=0;i<info.length();i++)
	 cout<<hex<<(int)info[i]<<" ";
	cout<<endl;
	#endif
	return;
}
//----------------


LzwCore::LzwCore()
{
	this->navDataLen=1;
	this->mapCodeLen=4;
	this->docSizeLen=8;
}

LzwCore::LzwCore(char navDataLen,char mapCodeLen,char docSizeLen)
{
	this->navDataLen=navDataLen;
	this->mapCodeLen=mapCodeLen;
	this->docSizeLen=docSizeLen;
}

LzwCore::~LzwCore()
{
	dic.clear();
}
//------------------------

void LzwCore::setFileTag(char tag)
{
	fileTag=tag;
}


void LzwCore::setNavDataLen(char len)
{
	if(len<=0||len>=8)throw InvalidNavDataLen();
	if(len>=this->mapCodeLen)throw InvalidNavDataLen();
	this->navDataLen=len;
	return;
}

void LzwCore::setMapCodeLen(char len)
{
	if(len<=0||len>8)throw InvalidMapCodeLen();
	if(len<=this->navDataLen)throw InvalidMapCodeLen();
	this->mapCodeLen=len;
	return;
}

void LzwCore::setDocSizeLen(char len)
{
	if(len<=0||len>8)throw InvalidDocSizeLen();
	this->docSizeLen=len;
	return;
}
//-------------------------

void LzwCore::compressFile(const char* src)
{
//	string defaultTarget=this->getRealFileName(src);
	string defaultTarget="";
	defaultTarget.insert(0,src);
	
	string fileSuffix=".lzwf";
	defaultTarget.insert(defaultTarget.length(),fileSuffix);
	this->compressFile(src,defaultTarget.c_str());
	return;
} 

void LzwCore::compressFile(const char* src,const char* target)
{
	ifstream in(src,ios::binary);
	ofstream out(target,ios::binary);
	
//	cout<<target<<endl; 
	
	out.write(this->makeFileHeader().data(),4);//写文件头
	
	string fileName=this->getRealFileName(src);
	unsigned nameLen=fileName.length();
	out.write((char*)&nameLen,4);//写文件名长
	out.write(fileName.data(),nameLen);//写文件名
	
	unsigned long long fileLen=0;
	FILE* tempFile=this->makeCompressTempFile(fileLen,in);
	
	out.write(this->makeCodeString(fileLen,this->docSizeLen).data(),this->docSizeLen);
	for(unsigned long long i=0;i<fileLen;i++)
	{
		char bride[2];
		fgets(bride,2,tempFile);
		out.write(bride,1);
	}
	fclose(tempFile);
	in.close();
	out.close();
	return;	
} 

void LzwCore::compressFileGroup(unsigned long long fileNum,const char** src,const char* target)
{
	if(fileNum==0)throw InvalidFileNumber();
	this->compressFile(src[0],target);
	if(fileNum>1)
	 this->compressFileAdding(fileNum-1,src+1,target);
	return;
}

void LzwCore::compressFileAdding(const char* src,const char* target)
{
	//读文件头 
	ifstream headReader(target,ios::binary);
	char head[5]="0000";
	headReader.read(head,4);
	this->catchFileHeader(head);
	headReader.close();
	
	ifstream in(src,ios::binary);
	ofstream out(target,ios::app | ios::binary);
	
	string fileName=this->getRealFileName(src);
	unsigned nameLen=fileName.length();
	out.write((char*)&nameLen,4);//写文件名长
	out.write(fileName.data(),nameLen);//写文件名
	
	unsigned long long fileLen=0;
	FILE* tempFile=this->makeCompressTempFile(fileLen,in);
	
	out.write(this->makeCodeString(fileLen,this->docSizeLen).data(),this->docSizeLen);
	for(unsigned long long i=0;i<fileLen;i++)
	{
		char bride[2];
		fgets(bride,2,tempFile);
		out.write(bride,1);
	}
	fclose(tempFile);
	in.close();
	out.close();
	return;	
}

void LzwCore::compressFileAdding(unsigned long long fileNum,const char** src,const char* target)
{
	if(fileNum==0)throw InvalidFileNumber();
	
	for(int i=0;i<fileNum;i++)
	 this->compressFileAdding(src[i],target);
	return;
}
//---------------------------------

void LzwCore::decompressFile(const char* src)
{
	ifstream in(src,ios::binary);
	char info[5]="0000";
	in.read(info,4);
	this->catchFileHeader(info);
	
	while(1)
	{
		string name=this->getFileNameInDoc(in);
		
//		this->debugTool("Done a File","");
		if(!in)break;//结束条件 
		
		name.insert(0,this->getPathOfFile(src));
		unsigned long long fileLen=0;
		in.read((char*)&fileLen,this->docSizeLen);
		this->makeDecompressFile(fileLen,name.c_str(),in);
	}
	in.close();
	return;
}

void LzwCore::decompressFile(const char* src,const char* path)
{
	ifstream in(src,ios::binary);
	char info[5]="0000";
	in.read(info,4);
	this->catchFileHeader(info);
	
	//判断路径最后是否含有分隔符
	bool hasPathTag=false; 
	int pathPos=0;
	while(path[pathPos]!='\0')pathPos++;
	pathPos--;
	if(pathPos>=0&&path[pathPos]==fileTag)
	 hasPathTag=true;
	
	while(1)
	{
		string name=this->getFileNameInDoc(in);
		
//		this->debugTool("Done a File","");
		if(!in)break;//结束条件 
		
		if(!hasPathTag)
		{
			name.insert(0,"0");
			name[0]=fileTag;
		}
		name.insert(0,path);
		
		unsigned long long fileLen=0;
		in.read((char*)&fileLen,this->docSizeLen);
		this->makeDecompressFile(fileLen,name.c_str(),in);
	}
	in.close();
	return;
}

/*----------------------------------------------------------------------------
	由于类的设计最初未考虑多线程，这导致要实现多线程需要对类进行大量的改造工作
	因此，带有多线程的文件组压缩被暂时弃用
	考虑在lzw_plus中重新启用，但因时间有限未完成设计 
------------------------------------------------------------------------------*/
 
//void LzwCore::compressFileAdding(unsigned long long fileNum,const char** src,const char* target)
//{
//	if(fileNum==0)throw InvalidFileNumber();
//	
//	//读文件头 
//	ifstream headReader(target,ios::binary);
//	char head[5]="0000";
//	headReader.read(head,4);
//	this->catchFileHeader(head);
//	headReader.close();
//	
//	//多线程压缩 
//	struct ThreadInfo
//	{
//		const char* src;
//		unsigned long long fileLen; 
//		FILE* tempFile;
//	};
//	ThreadInfo* info=new ThreadInfo[fileNum];
//	vector<thread> threads;
//	for(int i=0;i<fileNum;i++)
//	{
//		info[i].src=src[i];
//		info[i].fileLen=0;
//		threads.push_back(thread(&LzwCore::multithreadingCore,this,(void*)(info+i)));
//	}
//	for(vector<thread>::iterator it=threads.begin();it!=threads.end();it++)
//	 it->join();
//	
//	//从各个临时文件中复制 
//	ofstream out(target,ios::app | ios::binary);
//	for(int i=0;i<fileNum;i++)
//	{
//		out.write(this->makeCodeString(info[i].fileLen,this->docSizeLen).data(),this->docSizeLen);
//		for(unsigned long long i=0;i<info[i].fileLen;i++)
//		{
//			char bride[2];
//			fgets(bride,2,info[i].tempFile);
//			out.write(bride,1);
//		}
//		fclose(info[i].tempFile);
//	}
//	out.close();
//	return;
//}


/*------------------------------------------------
	初始版本的压缩方法，具有严重的问题，目前已废弃 
--------------------------------------------------*/

//LzwCore::InfoPack LzwCore::compression(LzwCore::InfoPack pack)
//{
//	this->refreshDic();
//	std::string out="";
//	std::string now="";
//	for(unsigned long long i=0;i<pack.len;i+=this->navDataLen)
//	{
//		std::string temp=now;
//		temp.insert(temp.length(),pack.info[i],this->navDataLen);
//		if(this->dic.find(temp)!=this->dic.end())
//		 now=temp;
//		 else
//		 {
//		 	out.insert(out.length(),dic[now]);
//		 	this->insertNewMap(temp);
//		 	now="";
//		 	now.insert(now.length(),pack.info[i],this->navDataLen);
//		 }
//		
//		if(this->mapCodeLen==8&&this->dicLen+1<this->dicLen)//字典宽度为8B的情况，需要舍弃2^64-1 
//		{
//			this->refreshDic();
//			now="";
//		}else if(this->dicLen==(((unsigned long long)1)<<(this->mapCodeLen*8)))//小于8B的情况 
//		{
//			this->refreshDic();
//			now="";
//		}
//	}
//	
//	this->refreshDic();
//	
//	InfoPack ret;
//	ret.info=new char[out.length()+1];
//	out.copy()
//	ret.len=out.length();
//	return ret;
//} 
