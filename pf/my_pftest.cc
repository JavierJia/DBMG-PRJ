#include <iostream>
#include "pf.h"

#define BLACK	"\e[30;1m"
#define RED		"\e[31;1m"
#define GREEN	"\e[32;1m"
#define YELLOW	"\e[33;1m"
#define BLUE	"\e[34;1m"
#define DEFAULT	"\e[0m"

using namespace std;

namespace test {

	void success(string str = "[ok]"){
		cout << GREEN << str << DEFAULT;
	}

	void fail(string str = "[fail]"){
		cout << RED << str << DEFAULT;
	}

	void assert_true(bool t, string msg = "Assert true"){
		cout << msg;
		for(int i = (73-msg.size()); i > 0; i--){
			cout << " ";
		}
		if(t) { success(); } else { fail(); }
		cout << endl;
	}
}

using namespace std;

void testSingleton()
{
  PF_Manager *pf1 = PF_Manager::Instance();
  PF_Manager *pf2 = PF_Manager::Instance();
  test::assert_true(pf1==pf2, "Only one instance of PF_Manager can exist");	
}

void testCreateFile()
{
  PF_Manager *pf = PF_Manager::Instance();
  RC status = pf->CreateFile("new_file.data");
  test::assert_true(status==0, "CreateFile() should return 0");
}

void testCreateFilePreventIllegalFileName()
{
  PF_Manager *pf = PF_Manager::Instance();
  RC status = 0;
  status += (0 != pf->CreateFile(""));
  status += (0 != pf->CreateFile("new file"));
  status += (0 != pf->CreateFile("new/file"));
  status += (0 != pf->CreateFile("new\\file"));
  status += (0 != pf->CreateFile("new@file"));
  test::assert_true(status==5, "CreateFile() prevent illegal file names");
}

void testCreateFilePreventFileOverwriting()
{
  PF_Manager *pf = PF_Manager::Instance();
  RC status = pf->CreateFile("new_file.data");
  test::assert_true(status!=0, "CreateFile() should prevent file overwriting");
}

void testDestroyFile()
{
  PF_Manager *pf = PF_Manager::Instance();
  RC status = pf->DestroyFile("new_file.data");
  test::assert_true(status==0, "DestroyFile() should return 0");
}

void testDestroyFilePreventIllegalFileName()
{
  PF_Manager *pf = PF_Manager::Instance();
  RC status = 0;
  status += (0 != pf->DestroyFile("new file"));
  status += (0 != pf->DestroyFile("../newfile"));
  status += (0 != pf->DestroyFile("new\\file"));
  status += (0 != pf->DestroyFile("new@file"));
  test::assert_true(status==4, "DestroyFile() prevent illegal file names");
}

void testDestroyFileCheckIfFileExists()
{
  PF_Manager *pf = PF_Manager::Instance();
  RC status = pf->DestroyFile("new_file.data");
  test::assert_true(status!=0, "DestroyFile() should check if file exists");
}

void testOpenFileCheckHandleStatus()
{
  PF_Manager *pf = PF_Manager::Instance();
  pf->CreateFile("new_file.data");
  PF_FileHandle fh;
  RC status = pf->OpenFile("new_file.data", fh);
  test::assert_true(status==0, "OpenFile() should work if handle is close");
  status = pf->OpenFile("new_file.data", fh);
  test::assert_true(status!=0, "OpenFile() should NOT work if handle is open");
  pf->DestroyFile("new_file.data");
}

void testCloseFileCheckHandleStatus()
{
  PF_Manager *pf = PF_Manager::Instance();
  pf->CreateFile("new_file.data");
  PF_FileHandle fh;
  pf->OpenFile("new_file.data", fh);
  RC status = pf->CloseFile(fh);
  test::assert_true(status==0, "CloseFile() should work if handle is open");
  status = pf->CloseFile(fh);
  test::assert_true(status!=0, "CloseFile() should NOT work if handle is close");
  pf->DestroyFile("new_file.data");
}

void testAppendPage()
{
  PF_Manager *pf = PF_Manager::Instance();
  pf->CreateFile("new_file.data");
  PF_FileHandle fh;
  pf->OpenFile("new_file.data", fh);
  test::assert_true(fh.GetNumberOfPages()==0, "New file should contain 0 pages");
  char* buffer = new char[PF_PAGE_SIZE];
  test::assert_true(fh.AppendPage(buffer)==0, "AppendPage should return 0");
  test::assert_true(fh.GetNumberOfPages()==1, "After 1 append there should be 1 page");
  pf->CloseFile(fh);
  pf->DestroyFile("new_file.data");
  delete buffer;
}

void test1000AppendPage()
{
  PF_Manager *pf = PF_Manager::Instance();
  pf->CreateFile("new_file.data");
  PF_FileHandle fh;
  pf->OpenFile("new_file.data", fh);
  test::assert_true(fh.GetNumberOfPages()==0, "New file should contain 0 pages");
  char* buffer = new char[PF_PAGE_SIZE];
  for(int i =0 ; i < 1000; i++) fh.AppendPage(buffer);
  test::assert_true(fh.GetNumberOfPages()==1000, "After 1000 append there should be 1000 pages");
  pf->CloseFile(fh);
  pf->DestroyFile("new_file.data");
  delete buffer;
}

int main() 
{
  cout << "##### PF Test Suite" << endl;
  for ( int i = 0; i < 100; i++){
  testSingleton();
  testCreateFile();
  testCreateFilePreventIllegalFileName();
  testCreateFilePreventFileOverwriting();
  testDestroyFile();
  testDestroyFilePreventIllegalFileName();
  testDestroyFileCheckIfFileExists();
  testOpenFileCheckHandleStatus();
  testCloseFileCheckHandleStatus();
  testAppendPage();
  test1000AppendPage();
  }
}
