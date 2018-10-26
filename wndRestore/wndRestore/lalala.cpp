#include "lalala.h"
#include <regex>
#include <tlhelp32.h>

// =======================================================================================================================

namespace myApplication
{
	// Global storage for program data
	std::vector<wndData> vec_data;
	std::vector<iniData> vec_ini;

	// ----------------------------------------------------------------------------------------------------------------

	// Global callback function for a EnumWindows(). Made it global, since it was impossible to make it a member of a class
	BOOL CALLBACK enumWindowsProc(HWND hWnd, LPARAM lParam)
	{
		if( IsWindow(hWnd) )
		{
			RECT rect;

			if( GetWindowRect(hWnd, &rect) )
			{
				int width  = rect.right  - rect.left;
				int height = rect.bottom - rect.top;

				// Skip, if the window does not have proper width and height
				if( width > 0 && height > 0 )
				{
					bool isVisible = (IsWindowVisible(hWnd) != 0);

					int length = GetWindowTextLength(hWnd);

					if( length )
					{
						length += 1;

						wndData		data;
						TCHAR*		buffer = new TCHAR[length];

						auto *vec = reinterpret_cast< std::vector<wndData>* >(lParam);

						memset(buffer, 0, length * sizeof(TCHAR));

						GetWindowText(hWnd, buffer, length);


						data.hWnd		 = hWnd;
						data.index		 = vec->size();
						data.windowTitle = myString(buffer);
						data.isVisible   = isVisible;


						appMain::getWindowInfo(data);


						// Skip processes with strange or improper paths
						if( data.fullExeName.length() > 7 )
						{
							if( data.fullExeName.find(':') != myString::npos && data.fullExeName.find('\\') != myString::npos && data.fullExeName.find('.') != myString::npos )
							{
								data.windowTitleOrig = data.windowTitle;
								data.windowClassOrig = data.windowClass;
								data.fullExeNameOrig = data.fullExeName;

								vec->push_back(data);
							}
						}

						delete[] buffer;
					}
				}
			}
		}

		return true;
	}
	// ----------------------------------------------------------------------------------------------------------------

	appMain::appMain(void)
	{
	}
	// ----------------------------------------------------------------------------------------------------------------

	appMain::~appMain(void)
	{
	}
	// ----------------------------------------------------------------------------------------------------------------

	// Get list of child processes for the given process
	void appMain::getChildProcessList(HWND hWnd, std::vector<std::string> &vec)
	{
		std::string		strProcessName;
		HANDLE			hProcessSnapshot;
		PROCESSENTRY32	processEntry32;

		vec.clear();

		DWORD processId, buffSize = 1024;

		GetWindowThreadProcessId(hWnd, &processId);
	
		hProcessSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

		if( hProcessSnapshot != INVALID_HANDLE_VALUE )
		{
			processEntry32.dwSize = sizeof(PROCESSENTRY32);

			if( Process32First(hProcessSnapshot, &processEntry32) )
			{
				do
				{
					if( processId == processEntry32.th32ParentProcessID )
					{
						CHAR  exe_name[1024];
						DWORD buffSize = 1024;

						HANDLE handle = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, false, processEntry32.th32ProcessID);

						QueryFullProcessImageNameA(handle, 0, exe_name, &buffSize);

						CloseHandle(handle);

						std::string sss = "PID: " + std::to_string(processEntry32.th32ParentProcessID) + "; EXE: " + exe_name;

						vec.push_back(sss);
					}
				}
				while( strProcessName.empty() && Process32Next(hProcessSnapshot, &processEntry32) );
			
				CloseHandle(hProcessSnapshot) ;
			}
		}

		return;
	}
	// ----------------------------------------------------------------------------------------------------------------

	// Get sorted list of currently existing proper windows
	void appMain::getWindows()
	{
		auto *vec = &vec_data;

		vec->clear();

		EnumWindows(enumWindowsProc, reinterpret_cast<LPARAM>(vec));

		// Sort the list by exe name
		std::sort(vec->begin(), vec->end(), [](const wndData &a, const wndData &b) { 
												return a.shortExeName < b.shortExeName;
											}
		);

		// Add unique index to each entry
		for(UINT i = 0; i < vec->size(); i++)
			vec->at(i).index = i;

		return;
	}
	// ----------------------------------------------------------------------------------------------------------------

	// Read data from the .ini-file
	int appMain::read_ini_file()
	{
		auto *vec = &vec_ini;

		vec->clear();

		int res = -1;

		std::string *fileName = get_ini_path();

		if( (*fileName).length() )
		{
			std::fstream file;
			std::string error("");

			// Make .ini NOT hidden, system or readOnly, because in Windows 7 fstream::open fails for hidden files:
			if( true )
			{
				DWORD attr = GetFileAttributesA(fileName->c_str());

				if( attr & FILE_ATTRIBUTE_HIDDEN || attr & FILE_ATTRIBUTE_SYSTEM || attr & FILE_ATTRIBUTE_READONLY )
				{
					DWORD removeAttr = FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_READONLY;

					SetFileAttributesA(fileName->c_str(), attr & ~removeAttr);
				}
			}

			// If the file exists, we read data from it. Otherwise, we create it.
			file.open(*fileName, std::fstream::in | std::fstream::app);


			if( file.is_open() )
			{
				std::string line, str;

				int lineNo = 0;

				bool isRecord = false;

				iniData ini;

				while( std::getline(file, line) && error.empty() )
				{
					lineNo++;

					int len = line.length();

					// Empty line or commentary indicate the end of the record
					if( !len || line[0] == '#' )
					{
						if( isRecord )
						{
							ini.isChecked = false;
							ini.isFound   = false;
							vec->push_back(ini);
							ini.reset();
						}

						isRecord = false;
						continue;
					}

					if( line[0] == '[' )
					{
						isRecord = true;

						int pos = line.find_first_of(']') + 1;

						std::string lineName = line.substr(0, pos);
						std::string lineData = line.substr(pos, len - pos);

						// switch by string
						do {
							
							if( lineName == "[Title]" )
							{
								ini.Title = getMyStr(lineData);
								break;
							}

							if( lineName == "[Path]" )
							{
								ini.Path = getMyStr(lineData);
								int pos = ini.Path.find_last_of('\\') + 1;
								ini.exeName = ini.Path.substr(pos, ini.Path.length() - pos);
								break;
							}

							if( lineName == "[Class]" )
							{
								ini.Class = getMyStr(lineData);
								break;
							}

							if( lineName == "[Coords]" )
							{
								std::istringstream iss(lineData);
								iss >> ini.X >> ini.Y >> ini.W >> ini.H;
								break;
							}

							if( lineName == "[Iconic]" )
							{
								std::istringstream iss(lineData);
								ini.isIconic = (lineData == "Yes");
								break;
							}

							if( lineName == "[customTitle]" )
							{
								std::istringstream iss(lineData);
								ini.customTitle = (lineData == "Yes");
								break;
							}

							if( lineName == "[customClass]" )
							{
								std::istringstream iss(lineData);
								ini.customClass = (lineData == "Yes");
								break;
							}

							if( lineName == "[customPath]" )
							{
								std::istringstream iss(lineData);
								ini.customPath = (lineData == "Yes");
								break;
							}

						} while(false);
					}
				}

				res = vec->size();

				file.close();

				// Make .ini hidden
				if( true )
				{
					DWORD attr = GetFileAttributesA(fileName->c_str());

					SetFileAttributesA(fileName->c_str(), attr | FILE_ATTRIBUTE_HIDDEN);
				}

				// Sort list by the Title
				std::sort(vec->begin(), vec->end(), [](const iniData &a, const iniData &b) { 
														return a.Title < b.Title;
													}
				);
			}
			else
			{
				error = "Could not open .ini file";
			}

			if( !error.empty() )
			{
				vec->clear();
			   _error = error;
				res = -1;
			}
		}

		return res;
	}
	// ----------------------------------------------------------------------------------------------------------------

	// Compare existing windows list with a list from .ini file
	void appMain::compare_wnd_and_ini()
	{
		iniData *ini;
		wndData *dat;

		// Get this app's .exe path
		myString exe = getMyStr(*get_exe_path());


		for(UINT i = 0; i < vec_ini.size(); i++)
		{
			ini = &vec_ini[i];

			for(UINT j = 0; j < vec_data.size(); j++)
			{
				dat = &vec_data[j];

				// Skip _this_app's_ process
				if( dat->fullExeName == exe )
				{
					dat->isChecked = false;
					dat->isVisible = false;
					continue;
				}


				if( !dat->isChecked )
				{
					bool found = false;

					// In order to compare, convert both strings to lower case
					std::string str1 = getStr(dat->fullExeName), str2 = getStr(ini->Path);

					// check path
					if( ini->customPath )
					{
						found = regExpCompare(str1, str2);
					}
					else
					{
						found = (str1 == str2);
					}

					// check Window Class
					if( found )
					{
						if( ini->customClass )
						{
							found = regExpCompare(dat->windowClass, ini->Class);
						}
						else
						{
							found = (dat->windowClass == ini->Class);
						}
					}

					// check Window Title
					if( found )
					{
						if( ini->customTitle )
						{
							found = regExpCompare(dat->windowTitle, ini->Title);
						}
						else
						{
							found = (dat->windowTitle== ini->Title);
						}
					}

					if( found )
					{
						dat->xNew = ini->X;
						dat->yNew = ini->Y;
						dat->wNew = ini->W;
						dat->hNew = ini->H;

						dat->isChecked	 = true;
						dat->isVisible	 = true;
						dat->isIconic	 = ini->isIconic;
						dat->customTitle = ini->customTitle;
						dat->customClass = ini->customClass;
						dat->customPath	 = ini->customPath;
						dat->windowTitle = ini->Title;
						dat->windowClass = ini->Class;
						dat->fullExeName = ini->Path;

						ini->isFound = true;

						break;
					}
				}
			}
		}

		return;
	}
	// ----------------------------------------------------------------------------------------------------------------

	// Move selected windows to their corresponding positions
	void appMain::repositionWindows()
	{
		auto isMaximized = [](wndData *data) {   
		
			WINDOWPLACEMENT plc;
			plc.length = sizeof(WINDOWPLACEMENT);
			GetWindowPlacement(data->hWnd, &plc);

			return plc.showCmd == SW_SHOWMAXIMIZED;
		};

		for(UINT i = 0; i < vec_data.size(); i++)
		{
			wndData *dat = &vec_data[i];

			if( dat->isChecked )
			{
				if( IsWindow(dat->hWnd) )
				{
					do
					{
						// Iconic window must be reduced to (tray) icon
						if( dat->isIconic )
						{
							ShowWindow(dat->hWnd, SW_HIDE);
							SetWindowPos(dat->hWnd, HWND_TOP, dat->xNew, dat->yNew, dat->wNew, dat->hNew, 0);
							break;
						}

						// Window [1, 1, 1, 1] will be maximized on the primary monitor
						if( dat->xNew == 1 && dat->yNew == 1 && dat->wNew == 1 && dat->hNew == 1 )
						{
							if( !isMaximized(dat) )
							{
								SetWindowPos(dat->hWnd, HWND_TOP, 100, 100, 800, 600, 0);
								ShowWindow(dat->hWnd, SW_SHOWMAXIMIZED);
							}

							break;
						}

						// Window [-1, 1, 1, 1] will be maximized on the left monitor
						if( dat->xNew == -1 && dat->yNew == 1 && dat->wNew == 1 && dat->hNew == 1 )
						{
							if( !isMaximized(dat) )
							{
								SetWindowPos(dat->hWnd, HWND_TOP, -850, 100, 800, 600, 0);
								ShowWindow(dat->hWnd, SW_SHOWMAXIMIZED);
							}

							break;
						}

						// Other windows will be shown as they are
						{
							ShowWindow(dat->hWnd, SW_RESTORE);
							SetWindowPos(dat->hWnd, HWND_TOP, dat->xNew, dat->yNew, dat->wNew, dat->hNew, 0);
							break;
						}

					} while(false);
				}
			}
		}
	
		return;
	}
	// ----------------------------------------------------------------------------------------------------------------

	// Get full path to this program's .exe file
	std::string* appMain::get_exe_path()
	{
		return get_file_path('e');
	}
	// ----------------------------------------------------------------------------------------------------------------

	// Get full path to this program's .ini file
	std::string* appMain::get_ini_path()
	{
		return get_file_path('i');
	}
	// ----------------------------------------------------------------------------------------------------------------

	// Get .exe or .ini fileName
	std::string* appMain::get_file_path(const char file)
	{
		std::string *res = nullptr;

		static std::string iniFileName("");
		static std::string exeFileName("");

		if( iniFileName.empty() && exeFileName.empty() )
		{
			TCHAR path[MAX_PATH];

			int len = GetModuleFileName(NULL, path, MAX_PATH);

			if( len )
			{
				exeFileName = std::string(&path[0], &path[len]);

				std::transform(exeFileName.begin(), exeFileName.end(), exeFileName.begin(), ::tolower);

				int pos = exeFileName.find_last_of('.');

				iniFileName = exeFileName.substr(0, pos+1) + "ini";
			}
		}

		switch( file )
		{
			case 'i':
				res = &iniFileName;
				break;

			case 'e':
				res = &exeFileName;
				break;
		}

		return res;
	}
	// ----------------------------------------------------------------------------------------------------------------

	// Save data to ini-file
	bool appMain::save_ini()
	{
		bool res = false;

		std::string *fileName = get_ini_path();

		if( (*fileName).length() )
		{
			std::fstream file;
			std::string error("");

			// Make .ini NOT hidden, system or readOnly, because in Windows 7 fstream::open fails for hidden files:
			if( true )
			{
				DWORD attr = GetFileAttributesA(fileName->c_str());

				if( attr & FILE_ATTRIBUTE_HIDDEN || attr & FILE_ATTRIBUTE_SYSTEM || attr & FILE_ATTRIBUTE_READONLY )
				{
					DWORD removeAttr = FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_READONLY;

					SetFileAttributesA(fileName->c_str(), attr & ~removeAttr);
				}
			}


			file.open(*fileName, std::fstream::out);


			if( file.is_open() )
			{
				auto putComment = [&file]		(const std::string				 &data)				{ file << "# --- "				   << data << " ---"						<< std::endl; };
				auto putStr     = [&file, this]	(const char *paramName, myString &data)				{ file << "[" << paramName << "]"  << getStr(data)							<< std::endl; };
				auto putBool    = [&file]		(const char *paramName, bool	 &data)				{ file << "[" << paramName << "]"  << (data ? "Yes" : "No") 				<< std::endl; };
				auto putStrExt  = [&file]		(const char *paramName, int x, int y, int w, int h)	{ file << "[" << paramName << "] " << x << " " << y << " " << w << " " << h << std::endl; };

				// data
				{
					auto *vec = &vec_data;

					for(UINT i = 0; i < vec->size(); i++)
					{
						wndData *data = &vec->at(i);

						if( data->isChecked )
						{
							putComment(getStr(data->shortExeName));

							putStr   ("Title",		   data->windowTitle							 );
							putStr   ("Class",		   data->windowClass							 );
							putBool  ("Iconic",		   data->isIconic								 );
							putStr   ("Path",		   data->fullExeName							 );
							putBool  ("customTitle",   data->customTitle							 );
							putBool  ("customClass",   data->customClass							 );
							putBool	 ("customPath",	   data->customPath								 );
							putStrExt("Coords",		   data->xNew, data->yNew, data->wNew, data->hNew);

							file << std::endl;
						}
					}
				}

				// ini
				{
					auto *vec = &vec_ini;

					for(UINT i = 0; i < vec->size(); i++)
					{
						iniData *ini = &vec->at(i);

						if( ini->isChecked )
						{
							putComment(getStr(ini->Title));

							putStr	 ("Title",		   ini->Title					 );
							putStr	 ("Class",		   ini->Class					 );
							putBool	 ("Iconic",		   ini->isIconic				 );
							putStr	 ("Path",		   ini->Path					 );
							putBool	 ("customTitle",   ini->customTitle				 );
							putBool	 ("customClass",   ini->customClass				 );
							putBool	 ("customPath",	   ini->customPath				 );
							putStrExt("Coords",		   ini->X, ini->Y, ini->W, ini->H);

							file << std::endl;
						}

					}
				}

				file.close();

				// Make .ini hidden again
				if( true )
				{
					DWORD attr = GetFileAttributesA(fileName->c_str());

					SetFileAttributesA(fileName->c_str(), attr | FILE_ATTRIBUTE_HIDDEN);
				}
			}
		}

		return res;
	}
	// ----------------------------------------------------------------------------------------------------------------

	// Get additional window information
	void appMain::getWindowInfo(wndData &data, char mode)
	{
		RECT rect;

		if( GetWindowRect(data.hWnd, &rect) )
		{
			// Mode == 0 means we must obtain all the additional info
			if( mode == 0 )
			{
				TCHAR wndClass[80];
				DWORD processId, buffSize = 1024;

				// Get window class name
				GetClassName(data.hWnd, wndClass, 80);

				// Get full .exe name
				{
					int pos1 = 0, pos2 = 0;
					CHAR exe_name[MAX_PATH];

					GetWindowThreadProcessId(data.hWnd, &processId);

					HANDLE handle = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, false, processId);

					QueryFullProcessImageNameA(handle, 0, exe_name, &buffSize);

					while( exe_name[pos1] != '\0' )
					{
						if( exe_name[pos1] == '\\' )
							pos2 = pos1 + 1;

						pos1++;
					}

					data.fullExeName  = myString(&exe_name[0],    &exe_name[pos1]);

					std::transform(data.fullExeName.begin(), data.fullExeName.end(), data.fullExeName.begin(), ::tolower);

					data.shortExeName = myString(&exe_name[pos2], &exe_name[pos1]);
					data.windowClass  = wndClass;

					CloseHandle(handle);
				}

				// Check if the window is maximized
				{
					WINDOWPLACEMENT plc;
					plc.length = sizeof(WINDOWPLACEMENT);
					GetWindowPlacement(data.hWnd, &plc);

					// The window is maximized if it is really maximized...
					//... or if it was maximized and then was minimized to taskbar
					if( plc.showCmd == SW_SHOWMINIMIZED && data.isVisible )
					{
						ShowWindow(data.hWnd, SW_RESTORE);
						GetWindowPlacement(data.hWnd, &plc);
						ShowWindow(data.hWnd, SW_SHOWMINIMIZED);
					}

					if( plc.showCmd == SW_SHOWMAXIMIZED )
					{
						// For a maximized window check it's left restored coordinate.
						// If it is < 0, then the window is originally located on the left monitor
						if( plc.rcNormalPosition.left < 0 )
						{
							rect.left   = -1;
							rect.right  =  0;
							rect.top    =  1;
							rect.bottom =  2;
						}
						else
						{
							rect.left   = 1;
							rect.right  = 2;
							rect.top    = 1;
							rect.bottom = 2;
						}
					}
				}
			}

			// Get original window position and coordinates
			data.X = data.xNew = rect.left;
			data.Y = data.yNew = rect.top;
			data.W = data.wNew = rect.right  - rect.left;
			data.H = data.hNew = rect.bottom - rect.top;
		}

		return;
	}
	// ----------------------------------------------------------------------------------------------------------------

	void appMain::ViewWindow(int index)
	{
		if( index >= 0 && index < static_cast<int>(vec_data.size()) )
		{
			wndData tmp;

			tmp.hWnd = (vec_data[index]).hWnd;

			if( IsWindow(tmp.hWnd) )
			{
				if( IsWindowVisible(tmp.hWnd) )
				{
					ShowWindow(tmp.hWnd, SW_HIDE);
				}
				else
				{
					getWindowInfo(tmp, 1);

					ShowWindow(tmp.hWnd, SW_RESTORE);
					SetWindowPos(tmp.hWnd, HWND_TOP, tmp.X, tmp.Y, tmp.W, tmp.H, 0);
				}
			}
		}

		return;
	}
	// ----------------------------------------------------------------------------------------------------------------

	// Set _showVisible flag to true/false
	void appMain::setShowVisible(bool doShow)
	{
	   _showVisible = doShow;
	}
	// ----------------------------------------------------------------------------------------------------------------

	// Check if the passed string matches the provided regExp expression
	bool appMain::regExpCompare(std::string& text, std::string &expr)
	{
		#define REPLACE_REGEX_SYMBOL(x, y)  \
			if( expr[i] == x ) {			\
				Expr += y;					\
				continue;					\
			}

		bool res = false;
		std::string Expr;

		// Replace certain symbols for the needs or regex
		for(size_t i = 0; i < expr.length(); i++)
		{
			REPLACE_REGEX_SYMBOL('*',	"(.*)");
			REPLACE_REGEX_SYMBOL('\\',	"\\\\");
			REPLACE_REGEX_SYMBOL('[',	"\\[" );
			REPLACE_REGEX_SYMBOL(']',	"\\]" );

			Expr += expr[i];
		}

		// Some untreated characters in Expr will cause exception ('*', '\\', '[', ']' etc)
		try
		{
			std::cmatch mr; 
			std::regex rx(Expr);

			res = regex_search(text.begin(), text.end(), rx);
		}
		catch(...)
		{
		   _error = "Function regExpCompare() caused Exception for the :\n\n";
		   _error += " - text: " + text + "\n";
		   _error += " - expr: " + Expr + "\n";
		}

		#undef REPLACE_REGEX_SYMBOL

		return res;
	}
	// ----------------------------------------------------------------------------------------------------------------

	// Check if the passed string matches the provided regExp expression --- overload
	bool appMain::regExpCompare(myString& text, myString& expr)
	{
		return regExpCompare(getStr(text), getStr(expr));
	}
	// ----------------------------------------------------------------------------------------------------------------

	void appMain::getStr_fromSystem(System::String ^sys, myString &str)
	{
		str.resize(sys->Length);

		for(int i = 0; i < sys->Length; i++)
			str[i] = sys[i];

		return;
	}

}; // namespace Project1

// =======================================================================================================================
