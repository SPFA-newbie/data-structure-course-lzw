#pragma once
#ifndef HELP_STRING
#define HELP_STRING
#ifndef NULL
#define NULL 0
#endif

//暂不启用 
//const char* initString="这是您首次使用程序，您可以选择：\n";
//const char* settingString="1 - 将程序添加至system32文件夹\n2 - 将当前文件夹添加至Path\n3- 直接开始使用\n";
//const char* initEndString="您可以使用\"-help\"命令呼出帮助列表，或使用\"-set\"命令更改设定\n";

const char* initString="程序将被移动至C:\\Windows\\system32，并命名为\"lzwZip.exe\"，此后您可以使用命令提示符窗口使用"; 

const char* helpString_1="用法：\nlzwZip -c [-set info] [-tar [-a] target] file1 [file2 [... [fileN]]] 或\nlzwZip -d [-p path] file\n\n";
const char* helpString_2="-c\t进行文件压缩操作\n-d\t进行文件解压操作\n-set\t对压缩参数进行设置，注意，在使用-a后设置将无效\n-tar\t指定压缩后文件名\n";
const char* helpString_3="\t当不指定压缩后文件名时，只能进行单个文件的压缩且压缩后文件名称与原文件相同\n-a\t在原文件上追加\n-p\t指定解压路径\n";
const char* helpString_4="info\t一个三位数字\n\t三位分别为欲设置的navDataLen、mapCodeLen和docSizeLen\nfileN\t进行压缩的第n个文件，需要提供完整路径,\n";
const char* helpString_5="path\t给定的解压路径\nfile\t欲解压文件的路径\n\n";

const char* errorString="命令语法错误，您可以使用\"-help\"命令呼出帮助列表\n";
#endif
