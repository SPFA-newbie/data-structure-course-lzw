#include "lzw_core.h"

#include<string>
using namespace std;

//int main()
//{
//	LzwCore lzw;
//	return 0;
//}

//	lzw.compressFile("E:\\try1.txt","E:\\try.lzwf");
//	lzw.decompressFile("E:\\try.lzwf");
//
//	const char** src=new const char*[3];
//	src[0]="E:\\mmb1.txt";
//	src[1]="E:\\mmb2.txt";
//	src[2]="E:\\mmb3.xlsx";
//	lzw.compressFileGroup(3,src,"res.lzwf");
//	lzw.decompressFile("E:\\res.lzwf");
//
//	lzw.compressFile("E:\\try1.bmp","E:\\res.lzwf");
//	lzw.decompressFile("E:\\res.lzwf");
//
//	const char* src="E:\\try2.txt";
//	const char* target="E:\\final.lzwf";
//	LzwCore lzw=LzwCore();
//	lzw.compressFile(src,target); 
//	lzw.compressFileAdding(src,target); 
//
//	typedef const char* t;
//	const char** src=new const char*[10];
//	for(int i=0;i<10;i++)
//	 src[i]="E:\\try1.txt";
//	LzwCore lzw;
//	lzw.compressFileGroup(10,src,"E:\\final.lzwf");
//	return 0;
