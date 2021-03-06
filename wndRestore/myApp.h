// C++ logic

#pragma once
#pragma comment(lib, "User32.lib")
#pragma comment(lib, "SHELL32.LIB")

#include <windows.h>
#include <fstream>
#include <sstream>
#include <string.h>
#include <vector>
#include <algorithm>



namespace myApplication {

	BOOL CALLBACK enumWindowsProc(HWND, LPARAM);

	#define FILTER_TXT "Enter filter text here..."

	typedef
		#ifndef UNICODE 
			std::string myString;
		#else 
			std::wstring myString;
		#endif
	typedef std::string std_str;


	static bool	 initDone	   = false;
	static bool	 isSilent      = false;
	static bool	 isAdmin	   = false;
	static short cBoxColumnNum = 3;
	static short defaultCoord  = -12345;

	// -----------------------------------------------------------------------------------------------------------------------

	struct wndData
	{
		wndData() : 
			xNew(defaultCoord), yNew(defaultCoord), wNew(defaultCoord), hNew(defaultCoord),
			isChecked(false), isVisible(true), isIconic(false),
			customTitle(false), customClass(false), customPath(false), customInst(false),
			instanceNo(-1)
		{
			;
		}

		myString	windowTitle, windowClass, fullExeName, shortExeName, windowTitleOrig, windowClassOrig, fullExeNameOrig, profile;
		HWND		hWnd;
		int			X, Y, W, H, index, xNew, yNew, wNew, hNew, instanceNo;
		bool		isChecked, isVisible, isIconic, customTitle, customClass, customPath, customInst, isChanged;
		FILETIME	fileTime;
	};

	// -----------------------------------------------------------------------------------------------------------------------

	struct iniData
	{
		iniData() :
			isIconic(false), customTitle(false), customClass(false), customPath(false), customInst(false)
		{
			;
		}

		void reset()
		{
			Title	= L"";
			Path	= Title;
			Class	= Title;
			exeName = Title;
			X = Y = W = H = defaultCoord;
			isFound		 = false;
			isChecked    = false;
			isIconic     = false;
			customTitle  = false;
			customClass  = false;
			customPath   = false;
			customInst	 = false;
		}

		myString	Title, Path, Class, exeName, profile;
		int			X, Y, W, H, instanceNo;
		bool	    isChecked, isFound, isIconic, customTitle, customClass, customPath, customInst;
	};

	// -----------------------------------------------------------------------------------------------------------------------

	extern std::vector<wndData>		vec_data;
	extern std::vector<iniData>		vec_ini;
	extern std::vector<myString>	vec_profiles;
	extern myString					str_profile;

	// -----------------------------------------------------------------------------------------------------------------------

	class appMain
	{
	 public:
		appMain();
	   ~appMain();

static	void		getWindowInfo		(wndData &, char = 0);				// Get additional window information
		void		getWindows			();									// Get sorted list of currently active windows with the info about them
		int			read_ini_file		(bool = false);						// Read data from the .ini-file
		void		compare_wnd_and_ini ();									// Compare existing windows list with a list from .ini file
		int			repositionWindows	(const int = -1);					// Move selected windows to their corresponding positions
		bool		save_ini			();									// Save data to ini-file
		void		ViewWindow			(int);
		void		setShowVisible		(bool);								// Set _showVisible flag to true/false
		bool		regExpCompare		(std_str  &, std_str  &);			// Check if the passed string matches the provided regExp expression
		bool		regExpCompare		(myString &, myString &);			// Check if the passed string matches the provided regExp expression
		std_str*	get_exe_path		();									// Get full path to this program's .exe file
		std_str*	get_ini_path		();									// Get full path to this program's .ini file
		void		getChildProcessList (HWND, std::vector<std_str> &);
		void		getStr_fromSystem	(System::String^, myString &);
		myString	getMyStr			(const std_str  &s) { return myString(&s[0], &s[s.length()]); }
		std_str		getStr				(const myString &s) { return  std_str(&s[0], &s[s.length()]); }
		std_str		getLastError		()					{ return _error;						  }
		void		clearLastError		()					{ _error.clear();						  }
		bool		isProfileFound		(const myString &);					// Checks if the profile is found in the list of known profiles
		void		replace_ini_profile (const myString &);					// Replace profile name for all vec_ini entries
		
	 private:
		std_str*	get_file_path		(const char file);					// Get .exe or .ini fileName

	 private:
		 bool		_showVisible;
		 std_str	_error;
	};

}
