#pragma once
#ifndef HELP_STRING
#define HELP_STRING
#ifndef NULL
#define NULL 0
#endif

//�ݲ����� 
//const char* initString="�������״�ʹ�ó���������ѡ��\n";
//const char* settingString="1 - �����������system32�ļ���\n2 - ����ǰ�ļ��������Path\n3- ֱ�ӿ�ʼʹ��\n";
//const char* initEndString="������ʹ��\"-help\"������������б���ʹ��\"-set\"��������趨\n";

const char* initString="���򽫱��ƶ���C:\\Windows\\system32��������Ϊ\"lzwZip.exe\"���˺�������ʹ��������ʾ������ʹ��"; 

const char* helpString_1="�÷���\nlzwZip -c [-set info] [-tar [-a] target] file1 [file2 [... [fileN]]] ��\nlzwZip -d [-p path] file\n\n";
const char* helpString_2="-c\t�����ļ�ѹ������\n-d\t�����ļ���ѹ����\n-set\t��ѹ�������������ã�ע�⣬��ʹ��-a�����ý���Ч\n-tar\tָ��ѹ�����ļ���\n";
const char* helpString_3="\t����ָ��ѹ�����ļ���ʱ��ֻ�ܽ��е����ļ���ѹ����ѹ�����ļ�������ԭ�ļ���ͬ\n-a\t��ԭ�ļ���׷��\n-p\tָ����ѹ·��\n";
const char* helpString_4="info\tһ����λ����\n\t��λ�ֱ�Ϊ�����õ�navDataLen��mapCodeLen��docSizeLen\nfileN\t����ѹ���ĵ�n���ļ�����Ҫ�ṩ����·��,\n";
const char* helpString_5="path\t�����Ľ�ѹ·��\nfile\t����ѹ�ļ���·��\n\n";

const char* errorString="�����﷨����������ʹ��\"-help\"������������б�\n";
#endif
