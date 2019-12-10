// C++ logic

#include "Globals.h"
#include "myApp.h"
#include <regex>
#include <tlhelp32.h>

// =======================================================================================================================

namespace myApplication
{
	// Global storage for program data
	std::vector<wndData>	vec_data;
	std::vector<iniData>	vec_ini;
	std::vector<myString>	vec_profiles;
	myString				str_profile;

	// ----------------------------------------------------------------------------------------------------------------

	// Global LESS operator for FILETIME
	inline bool operator < (const FILETIME &a, const FILETIME &b)
	{
		if( a.dwHighDateTime != b.dwHighDateTime )
		{
			return (a.dwHighDateTime < b.dwHighDateTime);
		}
		else
		{
			return (a.dwLowDateTime < b.dwLowDateTime);
		}
	}

	// Global callback function for a EnumWindows()
	// Made it global, since it was impossible to make it a member of a class
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
								data.isChanged       = false;
								data.windowTitleOrig = data.windowTitle;
								data.windowClassOrig = data.windowClass;
								data.fullExeNameOrig = data.fullExeName;
								data.profile		 = str_profile;

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

	// Get sorted list of currently existing proper windows (both visible and invisible)
	void appMain::getWindows()
	{
		auto *vec = &vec_data;

		vec->clear();

		EnumWindows(enumWindowsProc, reinterpret_cast<LPARAM>(vec));

		// Sort the list by: 1) exe name
		//					 2) process start time (which will give us the instance number later)
		//					 3) process visibility (visible goes first)
		std::sort(vec->begin(), vec->end(), [](const wndData &a, const wndData &b) {

												bool bTime		= a.fileTime	 < b.fileTime;
												bool bShortName = a.shortExeName < b.shortExeName;

												return (a.shortExeName == b.shortExeName)
															? (
																a.isVisible == b.isVisible ? bTime : a.isVisible
															  )
															: bShortName;
											}
		);

		// Add unique index to each entry
		for(size_t i = 0; i < vec->size(); i++)
		{
			wndData *iter = &vec->at(i);
			iter->instanceNo = 1;
			iter->index		 = i;

			if( i > 0 )
			{
				wndData *prev = &vec->at(i-1);

				// If exe file is present more than once, set InstanceNo depending on the running time of the process:
				// the longer it has been running, the lower instNo
				if( iter->shortExeName == prev->shortExeName )
					iter->instanceNo = prev->instanceNo + 1;
			}
		}

		return;
	}
	// ----------------------------------------------------------------------------------------------------------------

	// Read data from the .ini-file
	int appMain::read_ini_file(bool doReadProfiles /*default=false*/)
	{
		auto *vec = &vec_ini;
		auto *prf = &vec_profiles;

		vec->clear();

		if( doReadProfiles )
			prf->clear();

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
				std::string		line, str;
				int				lineNo = 0, pos;
				bool			isRecord = false;
				iniData			ini;

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

					// { marks the beginning of the global record
					if( line[0] == '{' && doReadProfiles )
					{
						pos = line.find_first_of('}') + 1;

						std::string lineName = line.substr(0, pos);
						std::string lineData = line.substr(pos, len - pos);

						// switch by string
						SWITCH()
						{
							CASE( lineName == "{Profiles}" )
								prf->push_back(getMyStr(lineData));
								BREAK;

						} SWITCH_END;

						if( str_profile.empty() )
							str_profile = prf->back();

						continue;
					}

					// [ marks the beginning of the record
					if( line[0] == '[' )
					{
						isRecord = true;

						pos = line.find_first_of(']') + 1;

						std::string lineName = line.substr(0, pos);
						std::string lineData = line.substr(pos, len - pos);

						// switch by string
						SWITCH()
						{
							CASE( lineName == "[Profile]" )
								ini.profile = getMyStr(lineData);
								BREAK;

							CASE( lineName == "[Title]" )
								ini.Title = getMyStr(lineData);
								BREAK;

							CASE( lineName == "[Path]" )
								ini.Path = getMyStr(lineData);
								pos = ini.Path.find_last_of('\\') + 1;
								ini.exeName = ini.Path.substr(pos, ini.Path.length() - pos);
								BREAK;

							CASE( lineName == "[Class]" )
								ini.Class = getMyStr(lineData);
								BREAK;

							CASE( lineName == "[Coords]" )
								std::istringstream iss(lineData);
								iss >> ini.X >> ini.Y >> ini.W >> ini.H;
								BREAK;

							CASE( lineName == "[Iconic]" )
								std::istringstream iss(lineData);
								ini.isIconic = (lineData == "Yes");
								BREAK;

							CASE( lineName == "[InstanceNo]" )
								std::istringstream iss(lineData);
								iss >> ini.instanceNo;
								BREAK;

							CASE( lineName == "[customTitle]" )
								std::istringstream iss(lineData);
								ini.customTitle = (lineData == "Yes");
								BREAK;

							CASE( lineName == "[customClass]" )
								std::istringstream iss(lineData);
								ini.customClass = (lineData == "Yes");
								BREAK;

							CASE( lineName == "[customPath]" )
								std::istringstream iss(lineData);
								ini.customPath = (lineData == "Yes");
								BREAK;

							CASE( lineName == "[customInst]" )
								std::istringstream iss(lineData);
								ini.customInst = (lineData == "Yes");
								BREAK;

						} SWITCH_END;
					}
				}

				// In case no profiles were found
				if( str_profile.empty() )
				{
					str_profile = L"default";
					prf->clear();
					prf->push_back(str_profile);
					replace_ini_profile(str_profile);
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


		for(size_t i = 0; i < vec_ini.size(); i++)
		{
			ini = &vec_ini[i];

			for(size_t j = 0; j < vec_data.size(); j++)
			{
				dat = &vec_data[j];

				// Skip [This App's] process
				if( dat->fullExeName == exe )
				{
					dat->isChecked = false;
					dat->isVisible = false;
					continue;
				}


				if( !dat->isChecked )
				{
					// check profile
					bool found = (dat->profile == ini->profile);

					if( found )
					{
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
							found = (dat->windowTitle == ini->Title);
						}
					}

					// check Instance Number (only needs checking if the according checkbox is selected)
					if( found )
					{
						if( ini->customInst )
						{
							found = (dat->instanceNo == ini->instanceNo);
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
						dat->customInst	 = ini->customInst;
						dat->windowTitle = ini->Title;
						dat->windowClass = ini->Class;
						dat->fullExeName = ini->Path;

						ini->isFound = true;

						// break out of for( .. vec_data ..)
						break;
					}
				}
			}
		}

		return;
	}
	// ----------------------------------------------------------------------------------------------------------------

	// Move selected windows to their desired positions
	// On success returns 0
	// Otherwise returns the number of windows that failed to reposition. In this case the caller should request elevation of privileges or suffer the consequences
	int appMain::repositionWindows(const int id /*default=-1*/)
	{
		int res = 0;

		auto isMaximized = [](wndData *data) {

			WINDOWPLACEMENT plc;
			plc.length = sizeof(WINDOWPLACEMENT);
			GetWindowPlacement(data->hWnd, &plc);

			return plc.showCmd == SW_SHOWMAXIMIZED;
		};


		for(UINT i = 0; i < vec_data.size(); i++)
		{
			wndData *dat = &vec_data[i];

			if( id < 0 || id == dat->index )
			{
				if( dat->isChecked || id == dat->index )
				{
					if( IsWindow(dat->hWnd) )
					{
						// Do nothing if the window is already at its desired coordinates
						if( dat->X != dat->xNew || dat->Y != dat->yNew || dat->H != dat->hNew || dat->W != dat->wNew )
						{
							SWITCH()
							{
								// Iconic window must be reduced to (tray) icon
								CASE( dat->isIconic )

									ShowWindow(dat->hWnd, SW_HIDE);
									SetWindowPos(dat->hWnd, HWND_TOP, dat->xNew, dat->yNew, dat->wNew, dat->hNew, 0);
									BREAK;

								// Window [1, 1, 1, 1] will be maximized on the primary monitor
								CASE( dat->xNew == 1 && dat->yNew == 1 && dat->wNew == 1 && dat->hNew == 1 )

									if( !isMaximized(dat) )
									{
										SetWindowPos(dat->hWnd, HWND_TOP, 100, 100, 800, 600, 0);
										ShowWindow(dat->hWnd, SW_SHOWMAXIMIZED);
									}
									BREAK;

								// Window [-1, 1, 1, 1] will be maximized on the left monitor
								CASE( dat->xNew == -1 && dat->yNew == 1 && dat->wNew == 1 && dat->hNew == 1 )

									if( !isMaximized(dat) )
									{
										SetWindowPos(dat->hWnd, HWND_TOP, -850, 100, 800, 600, 0);
										ShowWindow(dat->hWnd, SW_SHOWMAXIMIZED);
									}
									BREAK;

								// Other windows will be shown as they are
								DEFAULT

									ShowWindow(dat->hWnd, SW_RESTORE);
									BOOL isOk = SetWindowPos(dat->hWnd, HWND_TOP, dat->xNew, dat->yNew, dat->wNew, dat->hNew, 0);

									// See if SetWindowPos failed due to the insufficiend privileges
									if ( !isOk && !isAdmin && GetLastError() == ERROR_ACCESS_DENIED )
										res++;
									BREAK;

							} SWITCH_END;
						}
					}
				}
			}
		}

		return res;
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
				auto putStr2    = [&file, this]	(const char *paramName, myString &data)				{ file << "{" << paramName << "}"  << getStr(data)							<< std::endl; };
				auto putBool    = [&file]		(const char *paramName, bool	 &data)				{ file << "[" << paramName << "]"  << (data ? "Yes" : "No") 				<< std::endl; };
				auto putStrExt  = [&file]		(const char *paramName, int x, int y, int w, int h)	{ file << "[" << paramName << "] " << x << " " << y << " " << w << " " << h << std::endl; };
				auto putInt		= [&file]		(const char *paramName, int i)						{ file << "[" << paramName << "] " << i										<< std::endl; };


				// profiles
				{
					auto *vec = &vec_profiles;

					putComment(getStr(L"Profiles"));

					if( str_profile.length() )
						putStr2("Profiles", str_profile);

					for(size_t i = 0; i < vec->size(); i++)
					{
						myApplication::myString *str = &(vec->at(i));

						if( str->length() && *str != str_profile )
							putStr2("Profiles", *str);
					}

					file << std::endl;
				}

				// data -- current selected windows
				{
					auto *vec = &vec_data;

					for(size_t i = 0; i < vec->size(); i++)
					{
						wndData *data = &vec->at(i);

						if( data->isChecked )
						{
							putComment(getStr(data->shortExeName));

							putStr   ("Profile",	   str_profile									 );
							putStr   ("Title",		   data->windowTitle							 );
							putStr   ("Class",		   data->windowClass							 );
							putBool  ("Iconic",		   data->isIconic								 );
							putStr   ("Path",		   data->fullExeName							 );
							putBool  ("customTitle",   data->customTitle							 );
							putBool  ("customClass",   data->customClass							 );
							putBool	 ("customPath",	   data->customPath								 );
							putBool	 ("customInst",	   data->customInst								 );
							putStrExt("Coords",		   data->xNew, data->yNew, data->wNew, data->hNew);
							putInt	 ("InstanceNo",	   data->instanceNo								 );

							file << std::endl;
						}
					}
				}

				// ini -- windows from ini file
				{
					auto *vec = &vec_ini;

					for(size_t i = 0; i < vec->size(); i++)
					{
						iniData *ini = &vec->at(i);

						bool isIniChecked = ini->isChecked;
						bool isOtherExistingProfile = (ini->profile != str_profile) && (isProfileFound(ini->profile));

						if( isIniChecked || isOtherExistingProfile )
						{
							putComment(getStr(ini->Title));

							putStr   ("Profile",	   ini->profile					 );
							putStr	 ("Title",		   ini->Title					 );
							putStr	 ("Class",		   ini->Class					 );
							putBool	 ("Iconic",		   ini->isIconic				 );
							putStr	 ("Path",		   ini->Path					 );
							putBool	 ("customTitle",   ini->customTitle				 );
							putBool	 ("customClass",   ini->customClass				 );
							putBool	 ("customPath",	   ini->customPath				 );
							putBool	 ("customInst",	   ini->customInst				 );
							putStrExt("Coords",		   ini->X, ini->Y, ini->W, ini->H);
							putInt	 ("InstanceNo",	   ini->instanceNo);

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
	void appMain::getWindowInfo(wndData &data, char mode /*default=0*/)
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

					data.fullExeName  = myString(&exe_name[0], &exe_name[pos1]);

					std::transform(data.fullExeName.begin(), data.fullExeName.end(), data.fullExeName.begin(), ::tolower);

					data.shortExeName = myString(&exe_name[pos2], &exe_name[pos1]);
					data.windowClass  = wndClass;

					// Remember process creation time
					{
						FILETIME fProcessTime, ftExit, ftKernel, ftUser;							// this variables for get process start time and etc
						GetProcessTimes(handle, &fProcessTime, &ftExit, &ftKernel, &ftUser);		// Get process time
						data.fileTime = fProcessTime;

						data.instanceNo = fProcessTime.dwLowDateTime / 10;
					}

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
						// For a maximized window, check its left restored coordinate.
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

			// Mode == 1 means we must obtain only window coordinates
			if( mode == 1 )
			{
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
						// For a maximized window, check its left restored coordinate.
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
		// We will store 'Expr' and 'rx' as statics, so we don't need to rebuild them every time. They will be rebuilt only when 'expr' changes to something else.
		static std::string Expr(""), old("");
		static std::regex  rx;
		bool   res(true), reInit(false);

		if( !expr.empty() )
		{
			// eliminate continuous 'Expr' calculations for the same value
			if( old != expr )
			{
				old = expr;
				Expr.clear();
				reInit = true;

				const char  oldChar[] = {   '*',    '\\',   '[',   ']',   '{',   '}' };
				const char* newStr [] = { "(.*)", "\\\\", "\\[", "\\]", "\\{", "\\}" };

				// Replace certain symbols for the needs or regex
				for(size_t i = 0; i < expr.length(); i++)
				{
					for(size_t j = 0; j < sizeof(oldChar)/sizeof(oldChar[0]); j++)
					{
						if( expr[i] == oldChar[j] )
						{
							Expr += newStr[j];
							goto continue_main_loop;		// never done this before... let's try it out!!!
						}
					}

					Expr += expr[i];

					continue_main_loop:
						continue;
				}
			}

			// Some untreated characters in Expr will cause exception ('*', '\\', '[', ']' etc)
			// Just add them to oldChar/newStr arrays above
			try
			{
				if( reInit )
					rx.assign(Expr);

				res = regex_search(text.begin(), text.end(), rx);
			}
			catch(...)
			{
				res = false;

			   _error  = "Function regExpCompare() caused Exception for the :\n\n";
			   _error += " - text: " + text + "\n";
			   _error += " - expr: " + Expr + "\n";
			}
		}

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
	// ----------------------------------------------------------------------------------------------------------------

	// Checks if the profile is found in the list of known profiles
	bool appMain::isProfileFound(const myString &profile)
	{
		for(size_t i = 0; i < vec_profiles.size(); i++)
		{
			if( vec_profiles[i] == profile )
				return true;
		}

		return false;
	}
	// ----------------------------------------------------------------------------------------------------------------

	// Replace profile name for all vec_ini entries
	void appMain::replace_ini_profile(const myString &new_profile)
	{
		for(size_t i = 0; i < vec_ini.size(); i++)
			vec_ini[i].profile = new_profile;

		return;
	}
	// ----------------------------------------------------------------------------------------------------------------

}; // namespace Project1

// =======================================================================================================================
