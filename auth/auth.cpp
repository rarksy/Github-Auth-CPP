#include <iostream>
#include <string>
#include "func.hpp"
#include "md5.h"
#include <vector>
#include <io.h>
#include <fcntl.h>
#include <Windows.h>
#include <WMIUtils.h>
#include <chrono>
#include <thread>
#include <cctype>
#include <future>
using std::string;
#define backspace "\x1B[1D" << "\x1B[1P"
//i intend to remove goto:label entirely (maybe with do:while)

void printASCII()
{
	_setmode(_fileno(stdout), _O_U16TEXT);
	func::set_col(termcolor::green);
	std::wcout << L" ██████╗ ██╗████████╗██╗  ██╗██╗   ██╗██████╗      █████╗ ██╗   ██╗████████╗██╗  ██╗\n";
	std::wcout << L"██╔════╝ ██║╚══██╔══╝██║  ██║██║   ██║██╔══██╗    ██╔══██╗██║   ██║╚══██╔══╝██║  ██║\n";
	std::wcout << L"██║  ███╗██║   ██║   ███████║██║   ██║██████╔╝    ███████║██║   ██║   ██║   ███████║\n";
	std::wcout << L"██║   ██║██║   ██║   ██╔══██║██║   ██║██╔══██╗    ██╔══██║██║   ██║   ██║   ██╔══██║\n";
	std::wcout << L"╚██████╔╝██║   ██║   ██║  ██║╚██████╔╝██████╔╝    ██║  ██║╚██████╔╝   ██║   ██║  ██║\n";
	std::wcout << L" ╚═════╝ ╚═╝   ╚═╝   ╚═╝  ╚═╝ ╚═════╝ ╚═════╝     ╚═╝  ╚═╝ ╚═════╝    ╚═╝   ╚═╝  ╚═╝\n";
	_setmode(_fileno(stdout), _O_TEXT);
	func::set_col(termcolor::white);
}

void main()
{
	printASCII();
	func::spacer();
	func::spacer();
	std::cout << "\n[" << termcolor::magenta << 1 << termcolor::white << "] Login";
	std::cout << "\n[" << termcolor::magenta << 2 << termcolor::white << "] Register";

	while (true)
	{
		switch (int key = _getch())
		{
		case 0x31:
			func::login();
			break;
		case 0x32:
			func::Register();
			break;
		default:
			continue;
		}
	}
	
} 

void func::Register()
{
	string user,pass;
	DWORD VSN = 0;
	GetVolumeInformation("c:\\", NULL, NULL, &VSN, NULL, NULL, NULL, NULL);
EnterUsername:
	input("Enter A Username: ", user);
	string user_list = DownloadString("https://raw.githubusercontent.com/rarksyy/gh_auth_test/main/cppauth.auth");
	if (user_list.find(user+md5(user)) != string::npos) {
		set_col(termcolor::red);
		log("Username Already In Use. Contact Support To Reset HWID");
		set_col(termcolor::white);
		goto EnterUsername;
	}
	else {
		input("Enter A Password: ", pass, true);
		string CPUID = func::GetCPUID(),
			CSerial = std::to_string(VSN),
			PCUser = GetPCUser();
		string md5info = md5(user) + md5(reversestring(CPUID + CSerial + PCUser)) + md5(pass);
		setClipboardText(user.append(md5info).c_str()); //username+hashed_username+hashed & reversed CPUID + CSerial + PCUsername

		set_col(termcolor::green);
		log("\nAuth Info Copied To Clipboard, Contact Owner / Support");
		std::cout << ("Exiting In 6");
		for (int i = 5; i >= 0; i--) {
			std::this_thread::sleep_for(std::chrono::seconds(1));
			std::cout << backspace << i;
		}
	}
}

void func::login() 
{
EnterUsername:
	string user;
	input("Enter Username: ", user);
	string pass, user_list = DownloadString("https://raw.githubusercontent.com/rarksyy/gh_auth_test/main/cppauth.auth"), hwInfo;
	DWORD VSN = 0; GetVolumeInformation("c:\\", NULL, NULL, &VSN, NULL, NULL, NULL, NULL);
	if (user_list.find(user + md5(user)) != string::npos) {
EnterPassword:
		input("Enter Password: ", pass, true);
		hwInfo = user + md5(user) + md5(reversestring(GetCPUID() + std::to_string(VSN) + GetPCUser()));
		if (user_list.find(hwInfo) != string::npos) {
			if (user_list.find(hwInfo + md5(pass)) != string::npos) {
				Access();
			}
			else {
				set_col(termcolor::red);
				log("Incorrect Password");
				set_col(termcolor::white);
				goto EnterPassword;
			}
		}
		else {
			set_col(termcolor::red);
			log("Incorrect HWID");
			set_col(termcolor::white);
			goto EnterUsername;
		}
	}
	else {
		set_col(termcolor::red);
		log("Invalid username");
		set_col(termcolor::white);
		goto EnterUsername;
	}
}

void func::Access()
{
	log("success");
	std::this_thread::sleep_for(std::chrono::seconds(10));
}