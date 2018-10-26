#include "MyForm.h"

using namespace System;
using namespace System::Windows::Forms;

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

	Project1::MyForm form;
	Application::Run(%form);
}

// =======================================================================================================================

namespace Project1
{
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
		Color tb_title_color = defaultColor;

		cB_isIconic->CheckedChanged -= gcnew System::EventHandler(this, &MyForm::cB_isIconic_CheckedChanged);
		   cB_Title->CheckedChanged -= gcnew System::EventHandler(this, &MyForm::cB_CustomText_Changed);
		   cB_Class->CheckedChanged -= gcnew System::EventHandler(this, &MyForm::cB_CustomText_Changed);
		    cB_Path->CheckedChanged -= gcnew System::EventHandler(this, &MyForm::cB_CustomText_Changed);

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

							 tB_oldX->Text = gcnew String(data->X.ToString());
							 tB_oldY->Text = gcnew String(data->Y.ToString());
							 tB_oldW->Text = gcnew String(data->W.ToString());
							 tB_oldH->Text = gcnew String(data->H.ToString());

							 tB_NewX->Text = data->xNew == myApplication::defaultCoord ? "" : gcnew String(data->xNew.ToString());
							 tB_NewY->Text = data->yNew == myApplication::defaultCoord ? "" : gcnew String(data->yNew.ToString());
							 tB_NewW->Text = data->wNew == myApplication::defaultCoord ? "" : gcnew String(data->wNew.ToString());
							 tB_NewH->Text = data->hNew == myApplication::defaultCoord ? "" : gcnew String(data->hNew.ToString());

							cB_isIconic->Checked = data->isIconic;
							   cB_Title->Checked = data->customTitle;
							   cB_Class->Checked = data->customClass;
								cB_Path->Checked = data->customPath;

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

							 tB_NewX->Text = gcnew String(ini->X.ToString());
							 tB_NewY->Text = gcnew String(ini->Y.ToString());
							 tB_NewW->Text = gcnew String(ini->W.ToString());
							 tB_NewH->Text = gcnew String(ini->H.ToString());

							array<System::Windows::Forms::TextBox^>^ arr = { tB_oldX, tB_oldY, tB_oldW, tB_oldH };

							for(int i = 0; i < arr->Length; i++)
								arr[i]->Text = "";

							cB_isIconic->Checked = ini->isIconic;
							   cB_Title->Checked = ini->customTitle;
							   cB_Class->Checked = ini->customClass;
								cB_Path->Checked = ini->customPath;
						}

					}
					break;
			}
		}
		else
		{
			array<System::Windows::Forms::TextBox^>^ arr = { tB_Title, tB_Class, tB_Path, tB_oldX, tB_oldY, tB_oldW, tB_oldH, tB_NewX, tB_NewY, tB_NewW, tB_NewH };

			for(int i = 0; i < arr->Length; i++)
				arr[i]->Text = "";

			cB_isIconic->Checked = false;
		       cB_Title->Checked = false;
		       cB_Class->Checked = false;
		        cB_Path->Checked = false;
 		}

		tB_Title->BackColor = tb_title_color;

		cB_isIconic->CheckedChanged += gcnew System::EventHandler(this, &MyForm::cB_isIconic_CheckedChanged);
		   cB_Title->CheckedChanged += gcnew System::EventHandler(this, &MyForm::cB_CustomText_Changed);
		   cB_Class->CheckedChanged += gcnew System::EventHandler(this, &MyForm::cB_CustomText_Changed);
		    cB_Path->CheckedChanged += gcnew System::EventHandler(this, &MyForm::cB_CustomText_Changed);

		return;
	}
	// ----------------------------------------------------------------------------------------------------------------

	// Reposition selected windows
	Void MyForm::button1_Click(Object^ sender, EventArgs^ e)
	{
		cpp_app.repositionWindows();

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
		
		String^ sss = grid->Columns[myApplication::cBoxColumnNum]->HeaderText;
		grid->Columns[myApplication::cBoxColumnNum]->HeaderText = sss->Substring(0, sss->IndexOf(' ') + 1) + "(" + num + ")";

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

	// Get currently active windows (refresh)
	Void MyForm::button2_Click(Object^ sender, EventArgs^ e)
	{
		int num1 = 0, num2 = 0, num3 = 0, num4 = 0, h;

		myApplication::initDone = false;

		// Clear all our grid data
		{
			dataGridView1->Rows->Clear();
			dataGridView2->Rows->Clear();
			dataGridView1->Refresh();
			dataGridView2->Refresh();
		}


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

				dataGridView1->Rows->Add(data->index, wndExeShort, wndTitle, data->isChecked ? "1" : "0");
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

		String^ sss;
		sss = dataGridView1->Columns[myApplication::cBoxColumnNum]->HeaderText;
		dataGridView1->Columns[myApplication::cBoxColumnNum]->HeaderText = sss->Substring(0, sss->IndexOf(' ') + 1) + "(" + num3 + ")";

		sss = dataGridView2->Columns[myApplication::cBoxColumnNum]->HeaderText;
		dataGridView2->Columns[myApplication::cBoxColumnNum]->HeaderText = sss->Substring(0, sss->IndexOf(' ') + 1) + "(" + num4 + ")";

		// Set splitter distance so that grid2 has the minimal height
		{
			h = dataGridView2->ColumnHeadersHeight + splitContainer1->SplitterWidth;

			for(int i = 0; i < dataGridView2->Rows->Count; i++)
				h += dataGridView2->Rows[i]->Height + 1;

			h = (h > splitContainer1->Height/3) ? splitContainer1->Height/3 : h;

			splitContainer1->SplitterDistance = splitContainer1->Height - h;
		}

		setTextData(-1, -1);

		// here h acts as a tmp parameter that is not needed
		setActiveGrid(dataGridView1, h, h);

		myApplication::initDone = true;

		return;
	}
	// ----------------------------------------------------------------------------------------------------------------

	// Change of Coordinate Text Box 
	Void MyForm::tB_New_TextChanged(Object^ sender, EventArgs^ e)
	{
		if( myApplication::initDone )
		{
			int tag, idx = getItemIndex(tag), num;

			TextBox^ tb = static_cast<TextBox^>(sender);

			try {
				
				num = Convert::ToInt32(tb->Text);

			}
			catch(...)
			{
				idx = -1;
			}

			if( idx > -1 )
			{
				int tag = Convert::ToInt32(tb->Tag);

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

				myApplication::appMain::getWindowInfo(*data);

				{
					std::vector<myApplication::wndData> vec;

					myApplication::enumWindowsProc(data->hWnd, reinterpret_cast<LPARAM>(&vec));

					data->windowTitle = vec[0].windowTitle;
				}

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
			myApplication::myString new_value, *old_value;

			cpp_app.getStr_fromSystem(tb->Text, new_value);

			switch( tb_tag )
			{
				case 0:
					old_value = &(gridTag ? myApplication::vec_ini[idx].Title : myApplication::vec_data[idx].windowTitle);
					break;

				case 1:
					old_value = &(gridTag ? myApplication::vec_ini[idx].Class : myApplication::vec_data[idx].windowClass);
					break;

				case 2:
					old_value = &(gridTag ? myApplication::vec_ini[idx].Path  : myApplication::vec_data[idx].fullExeName);
					break;
			}

			if( *old_value != new_value )
				color = dirtyColor;
		}

		tb->BackColor = color;

		return;
	}
	// ----------------------------------------------------------------------------------------------------------------

	// Press Enter for a selected grid item's Text Property
	Void MyForm::tB_Title_KeyPress(Object^ sender, KeyPressEventArgs^ e)
	{
		if( e->KeyChar == (char)Keys::Enter )
		{
			int		 gridTag, idx = getItemIndex(gridTag);
			TextBox^ tb = static_cast<TextBox^>(sender);

			if( tb && idx >= 0 )
			{
				int tb_tag = Convert::ToInt32(tb->Tag);
				myApplication::myString new_value, *old_value;

				cpp_app.getStr_fromSystem(tb->Text, new_value);

				switch( tb_tag )
				{
					case 0:
						old_value = &(gridTag ? myApplication::vec_ini[idx].Title : myApplication::vec_data[idx].windowTitle);
						break;

					case 1:
						old_value = &(gridTag ? myApplication::vec_ini[idx].Class : myApplication::vec_data[idx].windowClass);
						break;

					case 2:
						old_value = &(gridTag ? myApplication::vec_ini[idx].Path  : myApplication::vec_data[idx].fullExeName);
						break;				
				}

			   *old_value = new_value;

				tb->BackColor = defaultColor;
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
			myApplication::vec_data[idx].isIconic = cB_isIconic->Checked;
		else
			myApplication::vec_ini[idx].isIconic = cB_isIconic->Checked;

		return;
	}
	// ----------------------------------------------------------------------------------------------------------------

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
			}

			if( value )
			{
				*value = cBox->Checked;
			}
		}
	
		return;
	}
	// ----------------------------------------------------------------------------------------------------------------

	// Resize checkbox columns in both grids
	Void MyForm::dgv1_Resize(Object^ sender, EventArgs^ e)
	{
		bool vScrollBarVisible1 = false;
		bool vScrollBarVisible2 = false;

		int w1 = 100, w2 = 100, width1, width2;

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

		return;	
	}

}; // namespace Project1

// =======================================================================================================================
