/*this creates a yaml or xml list of files from the command line args
 */

#include "opencv2/core.hpp"
#include <string>
#include <iostream>

#include <stdio.h>  
#include <io.h>  
#include <direct.h>
#include <algorithm>

using namespace std;

void getFiles(string path, vector<string>& files, string postfix = "*")
{
	if (postfix != "*") {
		postfix.insert(0, "*.");
	}

	//文件句柄      
	long   hFile = 0;
	//文件信息      
	struct _finddata_t fileinfo;
	string p;

	if ((hFile = _findfirst(p.assign(path).append("\\").append(postfix).c_str(), &fileinfo)) != -1)
	{
		do
		{
			//如果是目录,迭代之      
			//如果不是,加入列表 
			if ((fileinfo.attrib &  _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					getFiles(p.assign(path).append("\\").append(fileinfo.name), files, postfix);
			}
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}

//Predicate, 比较算法的“谓词”
bool sortByfileNamePredicate(const string &s1, const string &s2) {
	if (s1.length() == s2.length()) {
		return s1 < s2;
	}
	else{
		for (size_t ii = 0; ii < s2.length() && ii < s1.length(); ++ii) {
			if (isdigit(s1[ii]) && isdigit(s2[ii])) {
				string subs1, subs2;
				size_t jj = 0;
				do {
					subs1.append(s1, ii + jj, 1);					
				} while (isdigit(s1[ii+(++jj)]) && (ii+jj)<s1.length());
				jj = 0;
				do {
					subs2.append(s2, ii + jj, 1);
				} while (isdigit(s2[ii +(++jj)]) && (ii + jj)<s2.length());
				if (subs1 == subs2) {
					ii += jj;
				}
				else {
					return stoi(subs1) < stoi(subs2);
				}
			}
		}
	}
	return s1 < s2;
}

int main(int argc, char** argv)
{
	//get the current directory
	char cwd[1000];
	_getcwd(cwd, 1000);
	string path = string(begin(cwd), begin(cwd) + strlen(cwd));	

	string postfix;
	vector<string> files;
	string outputFileName;
	cout << "Please input the file postfix which you want to record. " << endl;
	cout << "  If you want record all the files, please input'*'. " << endl;
	cout << "  If you input Enter, the postfix will be the default 'bmp'. " << endl;
	char c = cin.get();
	if (c == '\n') {
		postfix = "bmp";
	}
	else {
		cin >> postfix;
		postfix.insert(0, &c);
		c = cin.get();
	}	

	getFiles(path, files, postfix);
	sort(files.begin(), files.end(), sortByfileNamePredicate);

	cout << "Please input the file name which you want to record to. For example:'ImageList'" << endl;
	cout << "  The file's extension is '.xml'. " << endl;
	cout << "  If you input Enter, the file name will be the default 'ImageList.xml'." << endl;
	c = cin.get();
	if (c == '\n') {
		outputFileName = "ImageList.xml";
	}
	else {
		cin >> outputFileName;
		outputFileName.insert(0, &c);
		outputFileName = outputFileName + ".xml";
	}
	
	cv::FileStorage fs(outputFileName, cv::FileStorage::WRITE);
	fs << "ImageListPaths" << "[";
	for (size_t i = 0; i < files.size(); i++) {
		fs << files[i];
	}
	fs << "]";
	
	cout << "Have record " << files.size() << " files" << endl;

	system("pause");
	return 0;
}





