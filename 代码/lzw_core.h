#pragma once
#ifndef LZW_CORE
#define LZW_CORE
#ifndef NULL
#define NULL 0
#endif

#include<map>
#include<cstdio>
#include<vector>
#include<string>
#include<fstream> 

class LzwCore
{
	private:
//		struct InfoPack
//		{
//			char* info;//原文件、压缩文件的字节串 
//			unsigned long long len;//字节串长度 
//		};
		static char fileTag;//系统所使用的文件分隔符 
		
		char navDataLen;//原数据字节宽度
		char mapCodeLen;//映射数据字节宽度
		char docSizeLen;//文件大小字节宽度

		unsigned long long dicLen;//字典长度
		
		//data是原数据，code是编码后数据
		std::map<std::string, std::string> dic;//字典，编码时为（data, code），解码时为（code, data） 

		void makeNavDic(std::vector<char>& data);//构造编码时初始字典
		void makeNavTDic(std::vector<char>& data);//构造解码时初始字典

		std::string makeCodeString(unsigned long long num,char len);//将得到的编码转换成string格式，方便文件操作 
		
		inline void insertNewMap(std::string data)//编码时添加新的映射项
		{
			dic.insert(std::pair<std::string,std::string>(data,makeCodeString(dicLen,mapCodeLen)));//(data, code)
			dicLen++; 
		}
		inline void insertNewTMap(std::string data)//解码时添加新的映射项 
		{
			dic.insert(std::pair<std::string,std::string>(makeCodeString(dicLen,mapCodeLen),data));//(code, data)
			dicLen++;
		}
		inline void refreshDic()//初始时或字典长度达到极限时重新构造（编码） 
		{
			dic.clear();
			dicLen=0;
			std::vector<char> temp;
			makeNavDic(temp);
		} 
		inline void refreshTDic()//初始时或字典长度达到极限时重新构造（解码） 
		{
			dic.clear();
			dicLen=0;
			std::vector<char> temp;
			makeNavTDic(temp);
		} 
		
		std::string makeFileHeader();//生成文件头 
		void catchFileHeader(const char* info);//获取文件头并完成设置 
		
		std::string makeString(char* str,unsigned long long len);//构造字符串，字符串中可能包含'\0' 
		
		std::string getRealFileName(const char* src);//从字符串中获取文件名（不包含路径）
		std::string getPathOfFile(const char* src);//从字符串获取路径（不包含文件名） 
		std::string getFileNameInDoc(std::ifstream& in);//从文件中获取文件名 
		
		void compression(std::string data,std::string& now,std::string& out);//压缩核心方法
		void decompression(std::string code,std::string& last,std::string& out);//解压核心方法 
		
		FILE* makeCompressTempFile(unsigned long long& fileLen,std::ifstream& in);//产生一个保存压缩后内容的临时文件 
		
		void makeDecompressFile(unsigned long long& fileLen,const char* navDoc,std::ifstream& in);//解压一个文件 
		
//		void multithreadingCore(void* info);//用来完成多线程压缩操作 
		
		void debugTool(std::string name,unsigned long long info);//调试工具
		void debugTool(std::string name,std::string info);//调试工具 
		
	public:
		LzwCore();
		LzwCore(char navDataLen,char mapCodeLen,char docSizeLen);
		~LzwCore();
		
		void setFileTag(char tag); 
		
		void setNavDataLen(char len);
		void setMapCodeLen(char len);
		void setDocSizeLen(char len);
		
		char getNavDataLen(){return this->navDataLen;}
		char getMapCodeLen(){return this->mapCodeLen;}
		char getDocSizeLen(){return this->docSizeLen;}
		
		void compressFile(const char* src);//单个文件压缩（不提供压缩后的文件名） 
		void compressFile(const char* src,const char* target);//单个文件压缩 （提供压缩后的文件名） 
		void compressFileGroup(unsigned long long fileNum,const char** src,const char* target);//文件组压缩 
		void compressFileAdding(const char* src,const char* target);//追加单个文件压缩
		void compressFileAdding(unsigned long long fileNum,const char** src,const char* target);//追加文件组压缩 
		
		void decompressFile(const char* src);//解压文件 
		void decompressFile(const char* src,const char* path);//解压文件并指定路径 
		void decompressFileMember(const char* src,const char* target);//解压压缩文件中的某一个文件 ,未进行实现 

};


#endif
