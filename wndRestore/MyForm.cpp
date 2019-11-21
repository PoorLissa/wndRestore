#include "MyForm.h"

using namespace System;
using namespace System::Windows::Forms;

using namespace System::Security::Principal;
using namespace System::Diagnostics;

// =======================================================================================================================

[STAThread]
void Main(array<String^>^ args)
{
	int len = args->Length;

	for(int i = 0; i < len; i++)
	{
		String^ str = args[i];

		if( str == "/silent" )
		{
			myApplication::isSilent = true;
		}
	}

	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	// Check if the application is running in administrator mode
	WindowsIdentity  ^identity  = WindowsIdentity::GetCurrent();
	WindowsPrincipal ^principal = gcnew WindowsPrincipal(identity);

	myApplication::isAdmin = principal->IsInRole(WindowsBuiltInRole::Administrator);

	Project1::MyForm form;

	if( myApplication::isAdmin )
	{
		form.Text = form.Text + " -- Administrator";
	}

	Application::Run(%form);
}

// =======================================================================================================================

namespace Project1
{
	// Initialization for MyForm Constructor
	void MyForm::init_MyForm()
	{
		myApplication::initDone = false;

		// Add checkbox row to our DataGridView
		{
			addCheckBox_toTheGrid(dataGridView1, 1, myApplication::cBoxColumnNum, "Selected ");
			addCheckBox_toTheGrid(dataGridView2, 1, myApplication::cBoxColumnNum,	  "Keep ");

			defaultColor	  = tB_Title->BackColor;
			changedColor	  = Color::Orange;
			changedColor	  = Color::FromArgb(255, 190,  90);
			selColor		  = Color::FromArgb(200, 235, 150);
			dirtyColor		  = Color::FromArgb(250, 100,  40);
			alternateRowColor = Color::FromArgb(233, 233, 233);
		}

		cpp_app.setShowVisible(cB_Show_Invis->Checked);

		// Get current windows
		button2_Click(nullptr, nullptr);

		activeGrid = -1;

		dataGridView1->DefaultCellStyle->SelectionBackColor = Color::FromArgb(255, 111, 3);
		dataGridView2->DefaultCellStyle->SelectionBackColor = Color::FromArgb(255, 111, 3);

		setGridColors(dataGridView1, dataGridView2);

		tB_Filter1->Font = (gcnew System::Drawing::Font(L"Calibri", 9, FontStyle::Regular, GraphicsUnit::Point, static_cast<System::Byte>(204)));
		tB_Filter1->Text = FILTER_TXT;

		myApplication::initDone = true;

		return;
	}
	// ----------------------------------------------------------------------------------------------------------------

	// Restart application with elevated privileges
	void MyForm::restartAsAdmin()
	{
		if( !myApplication::isAdmin )
		{
			ProcessStartInfo ^proc = gcnew ProcessStartInfo();

            proc->UseShellExecute  = true;
            proc->WorkingDirectory = Environment::CurrentDirectory;
            proc->FileName		   = Application::ExecutablePath;
            proc->Verb			   = "runas";

            try
            {
				Process::Start(proc);
				Application::Exit();
			}
            catch(...)
            {
				// The user refused the elevation: do nothing
				MessageBox::Show("You have refused the elevation", "Nothing changed");
			}
		}

		return;
	}
	// ----------------------------------------------------------------------------------------------------------------

	// Get pointer to the active grid
	DataGridView^ MyForm::getActiveGrid()
	{
		DataGridView^ grid = nullptr;

		switch( activeGrid )
		{
			case 0:
				grid = dataGridView1;
				break;

			case 1:
				grid = dataGridView2;
				break;
		}

		return grid;
	}
	// ----------------------------------------------------------------------------------------------------------------

	// Get index for a selected item in the grid
	int MyForm::getItemIndex(int &tag)
	{
		int res = -1;

		DataGridView^ grid = getActiveGrid();

		if( grid )
		{
			int idx  = grid->CurrentCell->RowIndex;
				tag  = Convert::ToInt32(grid->Tag);
			int size = (tag == 0) ? myApplication::vec_data.size() : myApplication::vec_ini.size();

			if( idx >= 0 && idx < size )
			{
				try {
				
					res = Convert::ToInt32(grid->Rows[idx]->Cells[0]->Value->ToString());

				}
				catch(...)
				{
					res = -1;
				}
			}
		}

		return res;
	}
	// ----------------------------------------------------------------------------------------------------------------

	// Set colors for active and inactive grids
	void MyForm::setGridColors(DataGridView^ active, DataGridView^ inactive)
	{
		if( active && inactive )
		{
			active->AlternatingRowsDefaultCellStyle->BackColor = alternateRowColor;
			active->DefaultCellStyle			   ->BackColor = defaultColor;
			active->DefaultCellStyle			   ->ForeColor = Color::Black;

			inactive->ClearSelection();

			inactive->AlternatingRowsDefaultCellStyle->BackColor = defaultColor;
			inactive->DefaultCellStyle				 ->BackColor = defaultColor;
			inactive->DefaultCellStyle				 ->ForeColor = Color::LightGray;
		}

		return;
	}
	// ----------------------------------------------------------------------------------------------------------------

	// Set all text data for a selected grid item
	void MyForm::setTextData(int tag, int index)
	{
		Color tb_title_color = defaultColor,
			  tb_class_color = defaultColor,
			  tb_path__color = defaultColor;

		cB_isIconic->CheckedChanged -= gcnew EventHandler(this, &MyForm::cB_isIconic_CheckedChanged);
		   cB_Title->CheckedChanged -= gcnew EventHandler(this, &MyForm::cB_CustomText_Changed);
		   cB_Class->CheckedChanged -= gcnew EventHandler(this, &MyForm::cB_CustomText_Changed);
		    cB_Path->CheckedChanged -= gcnew EventHandler(this, &MyForm::cB_CustomText_Changed);

		if( index >= 0 )
		{
			switch( tag )
			{
				// for grid1
				case 0: {

						if( index < static_cast<int>(myApplication::vec_data.size()) )
						{
							myApplication::wndData *data = &(myApplication::vec_data[index]);

							tB_Title->Text = gcnew String(data->windowTitle.c_str());
							tB_Class->Text = gcnew String(data->windowClass.c_str());
 							 tB_Path->Text = gcnew String(data->fullExeName.c_str());
					   tB_InstanceNo->Text = gcnew String(data->instanceNo.ToString());

							numUpD_OldX->Text = gcnew String(data->X.ToString());
							numUpD_OldY->Text = gcnew String(data->Y.ToString());
							numUpD_OldW->Text = gcnew String(data->W.ToString());
							numUpD_OldH->Text = gcnew String(data->H.ToString());

							numUpD_NewX->Text = data->xNew == myApplication::defaultCoord ? "" : gcnew String(data->xNew.ToString());
							numUpD_NewY->Text = data->yNew == myApplication::defaultCoord ? "" : gcnew String(data->yNew.ToString());
							numUpD_NewW->Text = data->wNew == myApplication::defaultCoord ? "" : gcnew String(data->wNew.ToString());
							numUpD_NewH->Text = data->hNew == myApplication::defaultCoord ? "" : gcnew String(data->hNew.ToString());

							cB_isIconic->Checked = data->isIconic;
							   cB_Title->Checked = data->customTitle;
							   cB_Class->Checked = data->customClass;
								cB_Path->Checked = data->customPath;
							cB_Instance->Checked = data->customInst;

							if( data->windowTitle != data->windowTitleOrig )
								tb_title_color = changedColor;

							if( data->windowClass != data->windowClassOrig )
								tb_class_color = changedColor;

							if( data->fullExeName != data->fullExeNameOrig )
								tb_path__color = changedColor;

							// Here we can get the list of all child processes
							{
#if 0
								std::vector<std::string> vec;
								cpp_app.getChildProcessList(data->hWnd, vec);
#endif
							}
						}

					}
					break;

				// for grid2
				case 1: {

						if( index < static_cast<int>(myApplication::vec_ini.size()) )
						{
							myApplication::iniData *ini = &(myApplication::vec_ini[index]);

							tB_Title->Text = gcnew String(ini->Title.c_str());
							tB_Class->Text = gcnew String(ini->Class.c_str());
 							 tB_Path->Text = gcnew String(ini->Path .c_str());
					   tB_InstanceNo->Text = gcnew String(ini->instanceNo.ToString());

							numUpD_NewX->Text = gcnew String(ini->X.ToString());
							numUpD_NewY->Text = gcnew String(ini->Y.ToString());
							numUpD_NewW->Text = gcnew String(ini->W.ToString());
							numUpD_NewH->Text = gcnew String(ini->H.ToString());

							array<NumericUpDown ^>^ arr = { numUpD_OldX, numUpD_OldY, numUpD_OldW, numUpD_OldH };

							for(int i = 0; i < arr->Length; i++)
								arr[i]->Text = "";

							cB_isIconic->Checked = ini->isIconic;
							   cB_Title->Checked = ini->customTitle;
							   cB_Class->Checked = ini->customClass;
								cB_Path->Checked = ini->customPath;
							cB_Instance->Checked = ini->customInst;
						}

					}
					break;
			}
		}
		else
		{
			array<TextBox^       >^ arr1 = { tB_Title, tB_Class, tB_Path };
			array<NumericUpDown ^>^ arr2 = { numUpD_OldX, numUpD_OldY, numUpD_OldW, numUpD_OldH, numUpD_NewX, numUpD_NewY, numUpD_NewW, numUpD_NewH };

			for(int i = 0; i < arr1->Length; i++)
				arr1[i]->Text = "";

			for(int i = 0; i < arr2->Length; i++)
				arr2[i]->Text = "";

			cB_isIconic->Checked = false;
		       cB_Title->Checked = false;
		       cB_Class->Checked = false;
		        cB_Path->Checked = false;
 		}

		tB_Title->BackColor = tb_title_color;
		tB_Class->BackColor = tb_class_color;
		 tB_Path->BackColor = tb_path__color;

		cB_isIconic->CheckedChanged += gcnew EventHandler(this, &MyForm::cB_isIconic_CheckedChanged);
		   cB_Title->CheckedChanged += gcnew EventHandler(this, &MyForm::cB_CustomText_Changed);
		   cB_Class->CheckedChanged += gcnew EventHandler(this, &MyForm::cB_CustomText_Changed);
		    cB_Path->CheckedChanged += gcnew EventHandler(this, &MyForm::cB_CustomText_Changed);
		cB_Instance->CheckedChanged += gcnew EventHandler(this, &MyForm::cB_CustomText_Changed);

		return;
	}
	// ----------------------------------------------------------------------------------------------------------------

	// Reposition selected windows
	Void MyForm::button1_Click(Object^ sender, EventArgs^ e)
	{
		if( !cpp_app.repositionWindows() )
		{
			// In case main window gets covered by other windows, make it visible again
			HWND hWnd = static_cast<HWND>(Handle.ToPointer());
			SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		}
		else
		{
			restartAsAdmin();
		}

		return;
	}
	// ----------------------------------------------------------------------------------------------------------------

	// Reposition single selected window (only in grid1)
	Void MyForm::button6_Click(Object^ sender, EventArgs^ e)
	{
		int tag, idx = getItemIndex(tag);

		if( tag == 0 )
		{
			cpp_app.repositionWindows(idx);

			// In case main window gets covered by other windows, make it visible again
			HWND hWnd = static_cast<HWND>(Handle.ToPointer());
			SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		}

		return;
	}
	// ----------------------------------------------------------------------------------------------------------------

	// Set active grid
	void MyForm::setActiveGrid(DataGridView^ grid, int &tag, int &size)
	{
		tag = Convert::ToInt32(grid->Tag);

		dataGridView1->SelectionChanged -= gcnew System::EventHandler(this, &MyForm::dgv1_SelectionChanged);
		dataGridView2->SelectionChanged -= gcnew System::EventHandler(this, &MyForm::dgv1_SelectionChanged);

			switch( tag )
			{
				// grid1
				case 0:
					activeGrid = 0;
					setGridColors(dataGridView1, dataGridView2);
					size = myApplication::vec_data.size();
					break;

				// grid2
				case 1:
					activeGrid = 1;
					setGridColors(dataGridView2, dataGridView1);
					size = myApplication::vec_ini.size();
					break;

				default:
					activeGrid = -1;
					size = 0;
			}

			// Enable/Disable control buttons
			{
				array<Button ^>^ buttons = { button4, button5, button6 };

				bool enabled = (activeGrid == 0 && grid->SelectedRows->Count > 0);
				for(int i = 0; i < buttons->Length; i++)
					buttons[i]->Enabled = enabled;
			}

		dataGridView1->SelectionChanged += gcnew System::EventHandler(this, &MyForm::dgv1_SelectionChanged);
		dataGridView2->SelectionChanged += gcnew System::EventHandler(this, &MyForm::dgv1_SelectionChanged);

		return;
	}
	// ----------------------------------------------------------------------------------------------------------------

	// Move through the Grid
	Void MyForm::dgv1_SelectionChanged(Object^ sender, EventArgs^ e)
	{
		if( myApplication::initDone )
		{
			DataGridView^ grid = static_cast<DataGridView^>(sender); 
			int tag, size;

			setActiveGrid(grid, tag, size);

			if( size )
			{
				bool init_done = myApplication::initDone;

				myApplication::initDone = false;

				int idx = getItemIndex(tag);

				setTextData(tag, idx);

				myApplication::initDone = init_done;
			}
		}

		return;
	}
	// ----------------------------------------------------------------------------------------------------------------

	// Change value of a checkbox in grid's row. Set inner 'checked' value in the vector's element.
	template<class T>
	void MyForm::setGridCheckbox(DataGridView^ grid, T& vec, int i, int idx)
	{
		auto data = &(vec[i]);
		auto cell = static_cast<DataGridViewCheckBoxCell^>(grid->Rows[idx]->Cells[myApplication::cBoxColumnNum]);

		cell->Value     = (cell->Value == "1") ? "0" : "1";
		data->isChecked = (cell->Value == "1");

		int num = 0;

		for(size_t i = 0; i < vec.size(); i++)
		{
			if( vec[i].isChecked )
				num++;
		}
		
		String^ s = grid->Columns[myApplication::cBoxColumnNum]->HeaderText;
		grid->Columns[myApplication::cBoxColumnNum]->HeaderText = s->Substring(0, s->IndexOf(' ') + 1) + "(" + num + ")";

		return;
	}
	// ----------------------------------------------------------------------------------------------------------------

	// Grid helper1 -- checkBox selection
	Void MyForm::dgv1_CellContentClick(Object^ sender, DataGridViewCellEventArgs^ e)
	{
		if( myApplication::initDone )
		{
			DataGridView^ grid = static_cast<DataGridView^>(sender);
			int tag, size;

			setActiveGrid(grid, tag, size);

			if( grid->CurrentCell->ColumnIndex == myApplication::cBoxColumnNum )
			{
				int idx = grid->CurrentCell->RowIndex,
					  i = Convert::ToInt32(grid->Rows[idx]->Cells[0]->Value);

				switch( activeGrid )
				{
					case 0:
						setGridCheckbox(grid, myApplication::vec_data, i, idx);
						break;

					case 1:
						setGridCheckbox(grid, myApplication::vec_ini, i, idx);
						break;

					default:
						;
				}
			}
		}

		return;
	}
	// ----------------------------------------------------------------------------------------------------------------

	// Grid helper2 -- checkBox selection
	Void MyForm::dgv1_KeyDown(Object^ sender, KeyEventArgs^ e)
	{
		if( myApplication::initDone )
		{
			if( e->KeyCode == Keys::Space )
			{
				DataGridView^ grid = static_cast<DataGridView^>(sender);\
				int tag, size;

				setActiveGrid(grid, tag, size);

				int idx = grid->CurrentCell->RowIndex,
					  i = Convert::ToInt32(grid->Rows[idx]->Cells[0]->Value->ToString());

				// set active column 1, because if the 3rd column is active, we experience some visual artefacts
				grid->CurrentCell = grid[1, idx];

				switch( activeGrid )
				{
					case 0:
						setGridCheckbox(grid, myApplication::vec_data, i, idx);
						break;

					case 1:
						setGridCheckbox(grid, myApplication::vec_ini, i, idx);
						break;

					default:
						;
				}
			}
		}

		return;
	}
	// ----------------------------------------------------------------------------------------------------------------

	// Grid helper3 -- checkBox selection
	Void MyForm::dgv1_CellDoubleClick(Object^ sender, DataGridViewCellEventArgs^ e)
	{
		if( myApplication::initDone )
		{
			DataGridView^ grid = static_cast<DataGridView^>(sender);

			int tag = Convert::ToInt32(grid->Tag);

			switch( tag )
			{
				// grid1
				case 0:
					activeGrid = 0;
					dataGridView2->ClearSelection();
					break;

				// grid2
				case 1:
					activeGrid = 1;
					dataGridView1->ClearSelection();
					break;

				default:
					activeGrid = -1;
			}
			
			int idx = grid->CurrentCell->RowIndex,
				  i = Convert::ToInt32(grid->Rows[idx]->Cells[0]->Value->ToString());

			switch( activeGrid )
			{
				case 0:
					setGridCheckbox(grid, myApplication::vec_data, i, idx);
					break;

				case 1:
					setGridCheckbox(grid, myApplication::vec_ini, i, idx);
					break;

				default:
					;
			}
		}

		return;
	}
	// ----------------------------------------------------------------------------------------------------------------

	// Grid helper4 -- grid click with additional key pressed
	Void MyForm::dgv1_CellMouseClick(Object^ sender, DataGridViewCellMouseEventArgs^ e)
	{
		if( myApplication::initDone )
		{
			if( e->Button == System::Windows::Forms::MouseButtons::Left )
			{
				if( Control::ModifierKeys == Keys::Control )
				{
					int tag, idx = getItemIndex(tag);

					if( tag == 0 && idx >= 0 )
					{
						myApplication::wndData *data = &myApplication::vec_data[idx];

						std::vector<std::string> vec;
						cpp_app.getChildProcessList(data->hWnd, vec);

						if( vec.size() )
						{
							std::string str;

							for(size_t i = 0; i < vec.size(); i++)
								str += std::to_string(i+1) + ": " + vec[i] + "\n\n";

							MessageBox::Show(gcnew String(str.c_str()), "Child Process List");
						}
					}
				}
			}
		}

		return;
	}
	// ----------------------------------------------------------------------------------------------------------------

	// Get all the data from Windows and from ini-file
	void MyForm::renewPhysicalData()
	{
		cpp_app.getWindows();
		cpp_app.read_ini_file();
		cpp_app.compare_wnd_and_ini();


		// Check if there were errors of comparison
		std::string last_error = cpp_app.getLastError();
		if( !last_error.empty() )
		{
			MessageBox::Show(gcnew String(last_error.c_str()));
			cpp_app.clearLastError();
		}

		return;
	}
	// ----------------------------------------------------------------------------------------------------------------

	// Clear the grids and put the data in there again
	void MyForm::redrawGridData()
	{
		myApplication::initDone = false;

		int num1 = 0, num2 = 0, num3 = 0, num4 = 0, h;

		tB_Filter1->Text = "";
		tB_Filter1_Leave(tB_Filter1, nullptr);

		// Clear all our grid data
		{
			dataGridView1->Rows->Clear();
			dataGridView2->Rows->Clear();
			dataGridView1->Refresh();
			dataGridView2->Refresh();
		}

		// Fill grid1 with data
		for(UINT i = 0; i < myApplication::vec_data.size(); i++)
		{
			auto data = &(myApplication::vec_data[i]);

			if( cB_Show_Invis->Checked || data->isVisible )
			{
				num1++;

				if( data->isChecked )
					num3++;

				String^ wndTitle    = gcnew String(data->windowTitleOrig.c_str());
				String^ wndClass    = gcnew String(data->windowClassOrig.c_str());
				String^ wndExeShort = gcnew String(data->shortExeName   .c_str());
				String^ wndExeFull  = gcnew String(data->fullExeNameOrig.c_str());

				int rowNo = dataGridView1->Rows->Add(data->index, wndExeShort, wndTitle, data->isChecked ? "1" : "0");

				if( data->isChecked )
					dataGridView1->Rows[rowNo]->DefaultCellStyle->BackColor = selColor;
			}
		}

		// Fill grid2 with data
		for(UINT i = 0; i < myApplication::vec_ini.size(); i++)
		{
			auto ini = &(myApplication::vec_ini[i]);

			if( !ini->isFound )
			{
				num2++;
				num4++;

				ini->isChecked = true;

				String^ wndTitle = gcnew String(ini->Title  .c_str());
				String^ wndClass = gcnew String(ini->Class  .c_str());
				String^ wndExe   = gcnew String(ini->exeName.c_str());

				dataGridView2->Rows->Add(i, wndExe, wndTitle, "1");
			}
		}

		dataGridView1->ClearSelection();
		dataGridView2->ClearSelection();

		dataGridView1->Columns[1]->HeaderText = "Executable (" + num1 + ")";
		dataGridView2->Columns[1]->HeaderText = "Executable (" + num2 + ")";

		String^ s;
		s = dataGridView1->Columns[myApplication::cBoxColumnNum]->HeaderText;
		dataGridView1->Columns[myApplication::cBoxColumnNum]->HeaderText = s->Substring(0, s->IndexOf(' ') + 1) + "(" + num3 + ")";

		s = dataGridView2->Columns[myApplication::cBoxColumnNum]->HeaderText;
		dataGridView2->Columns[myApplication::cBoxColumnNum]->HeaderText = s->Substring(0, s->IndexOf(' ') + 1) + "(" + num4 + ")";

		// Set splitter distance so that grid2 had the minimal height
		{
			h = dataGridView2->ColumnHeadersHeight + splitContainer1->SplitterWidth;

			int cnt = dataGridView2->Rows->Count;

			if( cnt )
				for(int i = 0; i < dataGridView2->Rows->Count; i++)
					h += dataGridView2->Rows[i]->Height + 1;
			else
				h += 10;

			h = (h > splitContainer1->Height/3) ? splitContainer1->Height/3 : h;

			splitContainer1->SplitterDistance = splitContainer1->Height - h;
			splitContainer1->Panel2MinSize	  = h - splitContainer1->SplitterWidth;
		}

		setTextData(-1, -1);

		// here h acts as a tmp parameter that is not needed
		setActiveGrid(dataGridView1, h, h);

		myApplication::initDone = true;
	
		return;
	}
	// ----------------------------------------------------------------------------------------------------------------

	// Get currently active windows (refresh)
	Void MyForm::button2_Click(Object^ sender, EventArgs^ e)
	{
		renewPhysicalData();

		redrawGridData();

		return;
	}
	// ----------------------------------------------------------------------------------------------------------------

	// Change of a Coordinate Numeric Box
	Void MyForm::numericUpDown_ValChanged(Object^ sender, EventArgs^ e)
	{
		if( myApplication::initDone )
		{
			int tag, idx = getItemIndex(tag), num;

			NumericUpDown^ numUpD = static_cast<NumericUpDown^>(sender);

			try {

				num = Convert::ToInt32(numUpD->Value);

			}
			catch(...)
			{
				idx = -1;
			}

			if( idx > -1 )
			{
				int tag = Convert::ToInt32(numUpD->Tag);

				auto *data = &(myApplication::vec_data[idx]);

				switch( tag )
				{
					case 0:
						data->xNew = num;
						break;

					case 1:
						data->yNew = num;
						break;

					case 2:
						data->wNew = num;
						break;

					case 3:
						data->hNew = num;
						break;

					default:
						;
				}

				data->isChanged = true;
			}
		}

		return;
	}
	// ----------------------------------------------------------------------------------------------------------------

	// Save data to ini-file
	Void MyForm::button3_Click(Object^ sender, EventArgs^ e)
	{
		if( myApplication::initDone )
		{
			std::string str("");
			std::vector<myApplication::wndData> *vec = &(myApplication::vec_data);

			for(size_t i = 0; i < vec->size(); i++)
			{
				if( vec->at(i).isChanged && !vec->at(i).isChecked )
					str += "[" + cpp_app.getStr(vec->at(i).windowTitleOrig) + "]\n";
			}

			if( !str.empty() )
			{
				str = "Changes made to these unchecked items won't be saved:\n\n" + str;

				MessageBox::Show(gcnew String(str.c_str()), "Attention!");
			}

			cpp_app.save_ini();
		}

		return;
	}
	// ----------------------------------------------------------------------------------------------------------------

	// Replace new coordinates with the current ones for a celected window
	Void MyForm::button4_Click(Object^ sender, EventArgs^ e)
	{
		DataGridView^ grid = getActiveGrid();

		if( grid )
		{
			int tag, idx = getItemIndex(tag);

			if( idx > -1 )
			{
				myApplication::wndData *data = &(myApplication::vec_data[idx]);

				// get only current coordinates
				myApplication::appMain::getWindowInfo(*data, 1);

#if 0
				{
					std::vector<myApplication::wndData> vec;

					myApplication::enumWindowsProc(data->hWnd, reinterpret_cast<LPARAM>(&vec));

					data->windowTitle = vec[0].windowTitle;
				}
#endif
				setTextData(tag, idx);
			}
		}

		return;
	}
	// ----------------------------------------------------------------------------------------------------------------

	// View selected window
	Void MyForm::button5_Click(Object^ sender, EventArgs^ e)
	{
		int tag, idx = getItemIndex(tag);

		cpp_app.ViewWindow(idx);

		return;
	}
	// ----------------------------------------------------------------------------------------------------------------

	// Change Title for a selected grid item
	Void MyForm::tB_Title_KeyUp(Object^ sender, KeyEventArgs^ e)
	{
		int		 gridTag, idx = getItemIndex(gridTag);
		TextBox^ tb = static_cast<TextBox^>(sender);
		Color	 color = defaultColor;

		if( tb && idx >= 0 )
		{
			int tb_tag = Convert::ToInt32(tb->Tag);
			myApplication::myString new_value, *old_value, orig_value;

			cpp_app.getStr_fromSystem(tb->Text, new_value);

			switch( tb_tag )
			{
				case 0:
					old_value  = &(gridTag ? myApplication::vec_ini[idx].Title : myApplication::vec_data[idx].windowTitle);
					orig_value = gridTag ? L"" : myApplication::vec_data[idx].windowTitleOrig;
					break;

				case 1:
					old_value  = &(gridTag ? myApplication::vec_ini[idx].Class : myApplication::vec_data[idx].windowClass);
					orig_value = gridTag ? L"" : myApplication::vec_data[idx].windowClassOrig;
					break;

				case 2:
					old_value  = &(gridTag ? myApplication::vec_ini[idx].Path  : myApplication::vec_data[idx].fullExeName);
					orig_value = gridTag ? L"" : myApplication::vec_data[idx].fullExeNameOrig;
					break;
			}

			if( !gridTag )
			{
				if( new_value != orig_value )
					color = changedColor;
			}

			if( *old_value != new_value )
				color = dirtyColor;
		}

		tb->BackColor = color;

		return;
	}
	// ----------------------------------------------------------------------------------------------------------------

	// Press 'Enter' for a selected grid item's Text Property
	Void MyForm::tB_Title_KeyPress(Object^ sender, KeyPressEventArgs^ e)
	{
		// Handle pressing 'Enter' key:
		if( e->KeyChar == (char)Keys::Enter )
		{
			int		 gridTag, idx = getItemIndex(gridTag);
			TextBox^ tb = static_cast<TextBox^>(sender);

			if( tb && idx >= 0 )
			{
				int tb_tag = Convert::ToInt32(tb->Tag);
				myApplication::myString new_value, *old_value, orig_value;

				cpp_app.getStr_fromSystem(tb->Text, new_value);

				switch( tb_tag )
				{
					case 0:
						old_value  = &(gridTag ? myApplication::vec_ini[idx].Title : myApplication::vec_data[idx].windowTitle);
						orig_value = gridTag ? L"" : myApplication::vec_data[idx].windowTitleOrig;
						break;

					case 1:
						old_value  = &(gridTag ? myApplication::vec_ini[idx].Class : myApplication::vec_data[idx].windowClass);
						orig_value = gridTag ? L"" : myApplication::vec_data[idx].windowClassOrig;
						break;

					case 2:
						old_value  = &(gridTag ? myApplication::vec_ini[idx].Path  : myApplication::vec_data[idx].fullExeName);
						orig_value = gridTag ? L"" : myApplication::vec_data[idx].fullExeNameOrig;
						break;				
				}

			   *old_value = new_value;

				if( gridTag )
				{
					tb->BackColor = defaultColor;
				}
				else
				{
					tb->BackColor = (new_value == orig_value) ? defaultColor : changedColor;

					myApplication::vec_data[idx].isChanged = true;
				}
			}
		}

		return;
	}
	// ----------------------------------------------------------------------------------------------------------------

	// Checkbox 'Show Invisible' onChange method
	Void MyForm::cB_Show_Invis_CheckedChanged(Object^ sender, EventArgs^ e)
	{
		cpp_app.setShowVisible(cB_Show_Invis->Checked);

		button2_Click(nullptr, nullptr);

		return;
	}
	// ----------------------------------------------------------------------------------------------------------------

	// Checkbox 'Window is Minimized' onChange method
	Void MyForm::cB_isIconic_CheckedChanged(Object^ sender, EventArgs^ e)
	{
		int tag, idx = getItemIndex(tag);

		if( tag == 0 )
		{
			myApplication::vec_data[idx].isIconic  = cB_isIconic->Checked;
			myApplication::vec_data[idx].isChanged = true;
		}
		else
		{
			myApplication::vec_ini[idx].isIconic = cB_isIconic->Checked;
		}

		return;
	}
	// ----------------------------------------------------------------------------------------------------------------

	// Add another column with chackbox to the grid
	void MyForm::addCheckBox_toTheGrid(DataGridView^ grid, UINT width, UINT num, const char *text)
	{
		DataGridViewCheckBoxColumn^ cBox = gcnew DataGridViewCheckBoxColumn(true);

		cBox->Width		 = width;
		cBox->HeaderText = gcnew String(text);
		cBox->FalseValue = "0";
		cBox->TrueValue  = "1";
		cBox->SortMode   = DataGridViewColumnSortMode::Automatic;

		grid->Columns->Insert(num, cBox);

		return;
	}
	// ----------------------------------------------------------------------------------------------------------------

	// Event: application window is shown for the 1st time
	Void MyForm::MyForm_Shown(Object^ sender, EventArgs^ e)
	{
		std::string error = cpp_app.getLastError();

		if( !error.empty() )
		{
			MessageBox::Show(gcnew String(error.c_str()) + "\n\nApplication will be closed.");
			Application::Exit();
		}

		// If the app was started with /silent modifier, just restore all checked windows position and quit
		if( myApplication::isSilent )
		{
			cpp_app.repositionWindows();
			Application::Exit();
		}

		// Resize checkbox columns in both grids
		dgv1_Resize(nullptr, nullptr);

		// Set focus on the Reposition Button
		button1->Focus();

		return;
	}
	// ----------------------------------------------------------------------------------------------------------------

	// Checkbox for custom text onChange method
	Void MyForm::cB_CustomText_Changed(Object^ sender, EventArgs^ e)
	{
		bool	   *value = nullptr;
		int			gridTag, idx = getItemIndex(gridTag);
		CheckBox^   cBox = static_cast<CheckBox^>(sender);

		if( cBox && idx >= 0 )
		{
			int cB_Tag = Convert::ToInt32(cBox->Tag);

			switch( cB_Tag )
			{
				case 0:
					value = &(gridTag ? myApplication::vec_ini[idx].customTitle : myApplication::vec_data[idx].customTitle);
					break;

				case 1:
					value = &(gridTag ? myApplication::vec_ini[idx].customClass : myApplication::vec_data[idx].customClass);
					break;

				case 2:
					value = &(gridTag ? myApplication::vec_ini[idx].customPath  : myApplication::vec_data[idx].customPath );
					break;

				case 3:
					value = &(gridTag ? myApplication::vec_ini[idx].customInst  : myApplication::vec_data[idx].customInst );
					break;
			}

			if( value )
			{
				*value = cBox->Checked;
			}

			if( !gridTag )
				myApplication::vec_data[idx].isChanged = true;
		}
	
		return;
	}
	// ----------------------------------------------------------------------------------------------------------------

	// Resize checkbox columns in both grids
	Void MyForm::dgv1_Resize(Object^ sender, EventArgs^ e)
	{
		bool vScrollBarVisible1 = false;
		bool vScrollBarVisible2 = false;

		int w1 = 100, w2 = 100, width1 = 0, width2 = 0;

		// Check if Vertical Scroll Bar is visible
		for(int i = 0; i < dataGridView1->Controls->Count; i++ )
		{
			if( dataGridView1->Controls[i]->GetType()->ToString() == "System.Windows.Forms.VScrollBar" )
			{
				vScrollBarVisible1 = dataGridView1->Controls[i]->Visible;
				width1 = dataGridView1->Controls[i]->Width;
				break;
			}
		}

		// Check if Vertical Scroll Bar is visible
		for(int i = 0; i < dataGridView2->Controls->Count; i++ )
		{
			if( dataGridView2->Controls[i]->GetType()->ToString() == "System.Windows.Forms.VScrollBar" )
			{
				vScrollBarVisible2 = dataGridView2->Controls[i]->Visible;
				width2 = dataGridView2->Controls[i]->Width;
				break;
			}
		}

		if( vScrollBarVisible1 )
		{
			if( !vScrollBarVisible2 )
				w2 += width2;
		}
		else
		{
			if( vScrollBarVisible2 )
				w1 += width1;
		}

		dataGridView1->Columns[myApplication::cBoxColumnNum]->Width = w1;
		dataGridView2->Columns[myApplication::cBoxColumnNum]->Width = w2;

		tB_Filter1->Width = dataGridView1->Columns[1]->Width + dataGridView1->Columns[2]->Width - tB_Filter1->Left - 2;

		return;	
	}
	// ----------------------------------------------------------------------------------------------------------------

	// Custom Draw for Split Container
	Void MyForm::splitContainer1_Paint(Object^ sender, PaintEventArgs^ e)
	{
		SplitContainer^ splCont = static_cast<SplitContainer^>(sender);

		if( splCont != nullptr )
		{
			int y1 = splCont->SplitterDistance + splCont->SplitterWidth/2 - 1,
				y2 = splCont->SplitterDistance + splCont->SplitterWidth/2 + 1,
				y3 = splCont->Panel1->Height + 3,
				x1 = splCont->Width / 2 - 33,
				x2 = splCont->Width / 2 + 33;

			e->Graphics->DrawLine(Pens::DarkGray, x1, y1, x2, y1);
			e->Graphics->DrawLine(Pens::DarkGray, x1, y2, x2, y2);
		}	

		return;
	}
	// ----------------------------------------------------------------------------------------------------------------

	// Split Container Splitter Move event
	Void MyForm::splitContainer1_SplitterMoved(Object^ sender, SplitterEventArgs^ e)
	{
		DataGridView^ grid;

		switch( activeGrid )
		{
			case 0:
				grid = dataGridView1;
				break;

			case 1:
				grid = dataGridView2;
				break;

			default:
				grid = dataGridView1;
		}

		grid->Focus();

		return;
	}
	// ----------------------------------------------------------------------------------------------------------------

	// Main form resize event
	Void MyForm::MyForm_Resize(Object^ sender, EventArgs^ e)
	{
		// Refresh SplitSontainer in order to properly draw additional graphics in its Paint event
		splitContainer1->Refresh();
		return;
	}
	// ----------------------------------------------------------------------------------------------------------------

	// Filtering grid1
	Void MyForm::tB_Filter_TextChanged(Object^ sender, EventArgs^ e)
	{
		if( myApplication::initDone )
		{
			TextBox ^tb = static_cast<TextBox ^>(sender);

			if( tb->Tag == "1" )
			{
				int num1 = 0, num3 = 0, h;

				// Clear all our grid data
				dataGridView1->Rows->Clear();

				myApplication::myString str0;
				cpp_app.getStr_fromSystem(tB_Filter1->Text, str0);
				std::string str2 = cpp_app.getStr(str0);
				std::transform(str2.begin(), str2.end(), str2.begin(), ::tolower);

				myApplication::initDone = false;

				// Fill grid1 with data
				for(UINT i = 0; i < myApplication::vec_data.size(); i++)
				{
					auto data = &(myApplication::vec_data[i]);

					if( cB_Show_Invis->Checked || data->isVisible )
					{
						std::string str1 = cpp_app.getStr(data->windowTitleOrig);
						std::transform(str1.begin(), str1.end(), str1.begin(), ::tolower);

						if( cpp_app.regExpCompare(str1, str2) )
						{
							num1++;

							if( data->isChecked )
								num3++;

							String^ wndTitle    = gcnew String(data->windowTitleOrig.c_str());
							String^ wndClass    = gcnew String(data->windowClassOrig.c_str());
							String^ wndExeShort = gcnew String(data->shortExeName   .c_str());
							String^ wndExeFull  = gcnew String(data->fullExeNameOrig.c_str());

							int rowNo = dataGridView1->Rows->Add(data->index, wndExeShort, wndTitle, data->isChecked ? "1" : "0");

							if( data->isChecked )
								dataGridView1->Rows[rowNo]->DefaultCellStyle->BackColor = selColor;
						}
					}
				}

				dataGridView1->ClearSelection();
				dataGridView1->Columns[1]->HeaderText = "Executable (" + num1 + ")";

				String^ sss;
				sss = dataGridView1->Columns[myApplication::cBoxColumnNum]->HeaderText;
				dataGridView1->Columns[myApplication::cBoxColumnNum]->HeaderText = sss->Substring(0, sss->IndexOf(' ') + 1) + "(" + num3 + ")";

				setTextData(-1, -1);

				// here h acts as a tmp parameter that is not needed
				setActiveGrid(dataGridView1, h, h);

				// The number of rows in the grid will be changing, so we need to update the width of checkbox column
				dgv1_Resize(nullptr, nullptr);

				myApplication::initDone = true;
			}
		}

		return;
	}
	// ----------------------------------------------------------------------------------------------------------------

	// Enter Event for a Filter Text Box
	Void MyForm::tB_Filter1_Enter(Object^ sender, EventArgs^ e)
	{
		FilterTextBox(sender, 'E');
	}
	// ----------------------------------------------------------------------------------------------------------------

	// Leave Event for a Filter Text Box
	Void MyForm::tB_Filter1_Leave(Object^ sender, EventArgs^ e)
	{
		FilterTextBox(sender, 'L');
	}
	// ----------------------------------------------------------------------------------------------------------------

	// Enter / Leave Event for a Filter Text Box
	void MyForm::FilterTextBox(Object^ sender, const char mode)
	{
		TextBox ^tb = static_cast<TextBox ^>(sender);

		tb->TextChanged -= gcnew System::EventHandler(this, &MyForm::tB_Filter_TextChanged);

		switch( mode )
		{
			case 'E'/*ENTER*/:
			
				if( tb->Text == FILTER_TXT )
				{
					tb->ForeColor = Color::Black;
					tb->Text = "";
					tb->Tag = "1";
				}
				break;

			case 'L'/*LEAVE*/:
			
				if( tb->Text == "" )
				{
					tb->ForeColor = Color::FromArgb(190, 190, 190);
					tb->Text = FILTER_TXT;
					tb->Tag = "0";
				}
				break;
		}

		tb->TextChanged += gcnew System::EventHandler(this, &MyForm::tB_Filter_TextChanged);

		return;
	}
	// ----------------------------------------------------------------------------------------------------------------

	// Mouse hover event
	Void MyForm::control_MouseHover(Object^ sender, EventArgs^ e)
	{
		String ^str("");

		// Get control's Description
		do {
			
			Type ^t = sender->GetType();
			
			if( t == Button::typeid )
			{
				str = static_cast<Button^>(sender)->AccessibleDescription;
				break;
			}

			if( t == TextBox::typeid )
			{
				str = static_cast<TextBox^>(sender)->AccessibleDescription;
				break;
			}

			if( t == CheckBox::typeid )
			{
				str = static_cast<CheckBox^>(sender)->AccessibleDescription;
				break;
			}

			if( t == DataGridView::typeid )
			{
				str = static_cast<DataGridView^>(sender)->AccessibleDescription;
				break;
			}
			
		}
		while( false );

		toolStripStatusLabel1->Text = str;

		return;
	}
	// ----------------------------------------------------------------------------------------------------------------

	// Mouse leave event
	Void MyForm::control_MouseLeave(Object^ sender, EventArgs^ e)
	{
		toolStripStatusLabel1->Text = "";
	}

}; // namespace Project1

// =======================================================================================================================
