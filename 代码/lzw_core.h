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
//			char* info;//ԭ�ļ���ѹ���ļ����ֽڴ� 
//			unsigned long long len;//�ֽڴ����� 
//		};
		static char fileTag;//ϵͳ��ʹ�õ��ļ��ָ��� 
		
		char navDataLen;//ԭ�����ֽڿ��
		char mapCodeLen;//ӳ�������ֽڿ��
		char docSizeLen;//�ļ���С�ֽڿ��

		unsigned long long dicLen;//�ֵ䳤��
		
		//data��ԭ���ݣ�code�Ǳ��������
		std::map<std::string, std::string> dic;//�ֵ䣬����ʱΪ��data, code��������ʱΪ��code, data�� 

		void makeNavDic(std::vector<char>& data);//�������ʱ��ʼ�ֵ�
		void makeNavTDic(std::vector<char>& data);//�������ʱ��ʼ�ֵ�

		std::string makeCodeString(unsigned long long num,char len);//���õ��ı���ת����string��ʽ�������ļ����� 
		
		inline void insertNewMap(std::string data)//����ʱ����µ�ӳ����
		{
			dic.insert(std::pair<std::string,std::string>(data,makeCodeString(dicLen,mapCodeLen)));//(data, code)
			dicLen++; 
		}
		inline void insertNewTMap(std::string data)//����ʱ����µ�ӳ���� 
		{
			dic.insert(std::pair<std::string,std::string>(makeCodeString(dicLen,mapCodeLen),data));//(code, data)
			dicLen++;
		}
		inline void refreshDic()//��ʼʱ���ֵ䳤�ȴﵽ����ʱ���¹��죨���룩 
		{
			dic.clear();
			dicLen=0;
			std::vector<char> temp;
			makeNavDic(temp);
		} 
		inline void refreshTDic()//��ʼʱ���ֵ䳤�ȴﵽ����ʱ���¹��죨���룩 
		{
			dic.clear();
			dicLen=0;
			std::vector<char> temp;
			makeNavTDic(temp);
		} 
		
		std::string makeFileHeader();//�����ļ�ͷ 
		void catchFileHeader(const char* info);//��ȡ�ļ�ͷ��������� 
		
		std::string makeString(char* str,unsigned long long len);//�����ַ������ַ����п��ܰ���'\0' 
		
		std::string getRealFileName(const char* src);//���ַ����л�ȡ�ļ�����������·����
		std::string getPathOfFile(const char* src);//���ַ�����ȡ·�����������ļ����� 
		std::string getFileNameInDoc(std::ifstream& in);//���ļ��л�ȡ�ļ��� 
		
		void compression(std::string data,std::string& now,std::string& out);//ѹ�����ķ���
		void decompression(std::string code,std::string& last,std::string& out);//��ѹ���ķ��� 
		
		FILE* makeCompressTempFile(unsigned long long& fileLen,std::ifstream& in);//����һ������ѹ�������ݵ���ʱ�ļ� 
		
		void makeDecompressFile(unsigned long long& fileLen,const char* navDoc,std::ifstream& in);//��ѹһ���ļ� 
		
//		void multithreadingCore(void* info);//������ɶ��߳�ѹ������ 
		
		void debugTool(std::string name,unsigned long long info);//���Թ���
		void debugTool(std::string name,std::string info);//���Թ��� 
		
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
		
		void compressFile(const char* src);//�����ļ�ѹ�������ṩѹ������ļ����� 
		void compressFile(const char* src,const char* target);//�����ļ�ѹ�� ���ṩѹ������ļ����� 
		void compressFileGroup(unsigned long long fileNum,const char** src,const char* target);//�ļ���ѹ�� 
		void compressFileAdding(const char* src,const char* target);//׷�ӵ����ļ�ѹ��
		void compressFileAdding(unsigned long long fileNum,const char** src,const char* target);//׷���ļ���ѹ�� 
		
		void decompressFile(const char* src);//��ѹ�ļ� 
		void decompressFile(const char* src,const char* path);//��ѹ�ļ���ָ��·�� 
		void decompressFileMember(const char* src,const char* target);//��ѹѹ���ļ��е�ĳһ���ļ� ,δ����ʵ�� 

};


#endif
