#pragma once

#include "lalala.h"

// =======================================================================================================================

myApplication::appMain cpp_app;

// =======================================================================================================================

namespace Project1
{
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public Form
	{
	 public:
		MyForm(void)
		{
			myApplication::initDone = false;

			InitializeComponent();

			// Add checkbox row to our DataGridView
			{
				addCheckBox_toTheGrid(dataGridView1, 1, myApplication::cBoxColumnNum, "Selected ");
				addCheckBox_toTheGrid(dataGridView2, 1, myApplication::cBoxColumnNum,	  "Keep ");

				defaultColor	  = tB_Title->BackColor;
				dirtyColor		  = Color::FromArgb(250, 100,  40);
				changedColor	  = Color::FromArgb(240, 210,  60);
				alternateRowColor = Color::FromArgb(233, 233, 233);
			}

			cpp_app.setShowVisible(cB_Show_Invis->Checked);

			// Get current windows
			button2_Click(nullptr, nullptr);

			activeGrid = -1;

			dataGridView1->DefaultCellStyle->SelectionBackColor = Color::FromArgb(255, 111, 3);
			dataGridView2->DefaultCellStyle->SelectionBackColor = Color::FromArgb(255, 111, 3);

			setGridColors(dataGridView1, dataGridView2);

//			splitContainer1->BackColor = Color::LightGray;

			myApplication::initDone = true;
		}

	 protected:
		// Clean up any resources being used
		~MyForm()
		{
			if( components )
				delete components;
		}

	 private:
		Color		  defaultColor, dirtyColor, changedColor, alternateRowColor;
		int			  activeGrid;

		Button		 ^button1, ^button2, ^button3, ^button4, ^button5;
		TextBox		 ^tB_Title, ^tB_Class, ^tB_Path;
		TextBox		 ^tB_oldX, ^tB_oldY, ^tB_oldW, ^tB_oldH, ^tB_NewH, ^tB_NewW, ^tB_NewY, ^tB_NewX;
		Panel		 ^panel1, ^panel2;

		System::Windows::Forms::Label^	label1;
		System::Windows::Forms::Label^	label2;
		System::Windows::Forms::Label^	label3;
		System::Windows::Forms::Label^	label4;
		System::Windows::Forms::Label^	label5;
		System::Windows::Forms::Label^	label6;
		System::Windows::Forms::Label^	label7;
		System::Windows::Forms::Label^	label8;
		System::Windows::Forms::Label^	label9;
		System::Windows::Forms::Label^  label10;

		CheckBox						^cB_Show_Invis, ^cB_isIconic, ^cB_Path, ^cB_Class, ^cB_Title;
		SplitContainer					^splitContainer1;
		DataGridView					^dataGridView1, ^dataGridView2;
		DataGridViewTextBoxColumn		^id, ^Short, ^Long, ^id2, ^dataGridViewTextBoxColumn2, ^dataGridViewTextBoxColumn3;

		System::ComponentModel::Container^	components;
	
#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle1 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
			System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle2 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(MyForm::typeid));
			System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle3 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
			System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle4 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->tB_Title = (gcnew System::Windows::Forms::TextBox());
			this->tB_Class = (gcnew System::Windows::Forms::TextBox());
			this->tB_Path = (gcnew System::Windows::Forms::TextBox());
			this->dataGridView1 = (gcnew System::Windows::Forms::DataGridView());
			this->id = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Short = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Long = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->tB_oldX = (gcnew System::Windows::Forms::TextBox());
			this->tB_oldY = (gcnew System::Windows::Forms::TextBox());
			this->tB_oldW = (gcnew System::Windows::Forms::TextBox());
			this->tB_oldH = (gcnew System::Windows::Forms::TextBox());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->tB_NewH = (gcnew System::Windows::Forms::TextBox());
			this->tB_NewW = (gcnew System::Windows::Forms::TextBox());
			this->tB_NewY = (gcnew System::Windows::Forms::TextBox());
			this->tB_NewX = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->button4 = (gcnew System::Windows::Forms::Button());
			this->button5 = (gcnew System::Windows::Forms::Button());
			this->splitContainer1 = (gcnew System::Windows::Forms::SplitContainer());
			this->dataGridView2 = (gcnew System::Windows::Forms::DataGridView());
			this->id2 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->dataGridViewTextBoxColumn2 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->dataGridViewTextBoxColumn3 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->panel2 = (gcnew System::Windows::Forms::Panel());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->cB_Path = (gcnew System::Windows::Forms::CheckBox());
			this->cB_Class = (gcnew System::Windows::Forms::CheckBox());
			this->cB_Title = (gcnew System::Windows::Forms::CheckBox());
			this->cB_isIconic = (gcnew System::Windows::Forms::CheckBox());
			this->cB_Show_Invis = (gcnew System::Windows::Forms::CheckBox());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->splitContainer1))->BeginInit();
			this->splitContainer1->Panel1->SuspendLayout();
			this->splitContainer1->Panel2->SuspendLayout();
			this->splitContainer1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView2))->BeginInit();
			this->panel1->SuspendLayout();
			this->panel2->SuspendLayout();
			this->SuspendLayout();
			// 
			// button1
			// 
			this->button1->Font = (gcnew System::Drawing::Font(L"Tahoma", 20, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->button1->Location = System::Drawing::Point(588, 36);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(332, 102);
			this->button1->TabIndex = 0;
			this->button1->Text = L"Reposition Windows";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &MyForm::button1_Click);
			// 
			// tB_Title
			// 
			this->tB_Title->Font = (gcnew System::Drawing::Font(L"Tahoma", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->tB_Title->Location = System::Drawing::Point(51, 175);
			this->tB_Title->Name = L"tB_Title";
			this->tB_Title->Size = System::Drawing::Size(977, 24);
			this->tB_Title->TabIndex = 3;
			this->tB_Title->Tag = L"0";
			this->tB_Title->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm::tB_Title_KeyPress);
			this->tB_Title->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &MyForm::tB_Title_KeyUp);
			// 
			// tB_Class
			// 
			this->tB_Class->Font = (gcnew System::Drawing::Font(L"Tahoma", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->tB_Class->Location = System::Drawing::Point(51, 201);
			this->tB_Class->Name = L"tB_Class";
			this->tB_Class->Size = System::Drawing::Size(977, 24);
			this->tB_Class->TabIndex = 4;
			this->tB_Class->Tag = L"1";
			this->tB_Class->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm::tB_Title_KeyPress);
			this->tB_Class->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &MyForm::tB_Title_KeyUp);
			// 
			// tB_Path
			// 
			this->tB_Path->Font = (gcnew System::Drawing::Font(L"Tahoma", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->tB_Path->Location = System::Drawing::Point(51, 227);
			this->tB_Path->Name = L"tB_Path";
			this->tB_Path->Size = System::Drawing::Size(977, 24);
			this->tB_Path->TabIndex = 5;
			this->tB_Path->Tag = L"2";
			this->tB_Path->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm::tB_Title_KeyPress);
			this->tB_Path->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &MyForm::tB_Title_KeyUp);
			// 
			// dataGridView1
			// 
			this->dataGridView1->AllowUserToAddRows = false;
			this->dataGridView1->AllowUserToDeleteRows = false;
			this->dataGridView1->AllowUserToResizeRows = false;
			dataGridViewCellStyle1->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(233)), 
				static_cast<System::Int32>(static_cast<System::Byte>(233)), static_cast<System::Int32>(static_cast<System::Byte>(233)));
			this->dataGridView1->AlternatingRowsDefaultCellStyle = dataGridViewCellStyle1;
			this->dataGridView1->BackgroundColor = System::Drawing::SystemColors::ButtonHighlight;
			dataGridViewCellStyle2->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleCenter;
			dataGridViewCellStyle2->BackColor = System::Drawing::SystemColors::Control;
			dataGridViewCellStyle2->Font = (gcnew System::Drawing::Font(L"Tahoma", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			dataGridViewCellStyle2->ForeColor = System::Drawing::SystemColors::WindowText;
			dataGridViewCellStyle2->SelectionBackColor = System::Drawing::SystemColors::Highlight;
			dataGridViewCellStyle2->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
			dataGridViewCellStyle2->WrapMode = System::Windows::Forms::DataGridViewTriState::True;
			this->dataGridView1->ColumnHeadersDefaultCellStyle = dataGridViewCellStyle2;
			this->dataGridView1->ColumnHeadersHeight = 25;
			this->dataGridView1->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::DisableResizing;
			this->dataGridView1->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(3) {this->id, this->Short, 
				this->Long});
			this->dataGridView1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->dataGridView1->Font = (gcnew System::Drawing::Font(L"Tahoma", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->dataGridView1->GridColor = System::Drawing::SystemColors::ButtonShadow;
			this->dataGridView1->Location = System::Drawing::Point(0, 0);
			this->dataGridView1->MultiSelect = false;
			this->dataGridView1->Name = L"dataGridView1";
			this->dataGridView1->ReadOnly = true;
			this->dataGridView1->RowHeadersVisible = false;
			this->dataGridView1->RowTemplate->ReadOnly = true;
			this->dataGridView1->RowTemplate->Resizable = System::Windows::Forms::DataGridViewTriState::False;
			this->dataGridView1->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
			this->dataGridView1->ShowRowErrors = false;
			this->dataGridView1->Size = System::Drawing::Size(1086, 290);
			this->dataGridView1->TabIndex = 7;
			this->dataGridView1->Tag = L"0";
			this->dataGridView1->CellContentClick += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &MyForm::dgv1_CellContentClick);
			this->dataGridView1->CellDoubleClick += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &MyForm::dgv1_CellDoubleClick);
			this->dataGridView1->CellMouseClick += gcnew System::Windows::Forms::DataGridViewCellMouseEventHandler(this, &MyForm::dgv1_CellMouseClick);
			this->dataGridView1->SelectionChanged += gcnew System::EventHandler(this, &MyForm::dgv1_SelectionChanged);
			this->dataGridView1->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &MyForm::dgv1_KeyDown);
			this->dataGridView1->Resize += gcnew System::EventHandler(this, &MyForm::dgv1_Resize);
			// 
			// id
			// 
			this->id->HeaderText = L"id";
			this->id->Name = L"id";
			this->id->ReadOnly = true;
			this->id->Visible = false;
			// 
			// Short
			// 
			this->Short->HeaderText = L"Executable";
			this->Short->MinimumWidth = 150;
			this->Short->Name = L"Short";
			this->Short->ReadOnly = true;
			this->Short->Width = 200;
			// 
			// Long
			// 
			this->Long->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::Fill;
			this->Long->HeaderText = L"Window Title -- Currently Active Windows";
			this->Long->MinimumWidth = 200;
			this->Long->Name = L"Long";
			this->Long->ReadOnly = true;
			this->Long->Resizable = System::Windows::Forms::DataGridViewTriState::False;
			// 
			// tB_oldX
			// 
			this->tB_oldX->Font = (gcnew System::Drawing::Font(L"Tahoma", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->tB_oldX->Location = System::Drawing::Point(51, 36);
			this->tB_oldX->Name = L"tB_oldX";
			this->tB_oldX->ReadOnly = true;
			this->tB_oldX->Size = System::Drawing::Size(100, 24);
			this->tB_oldX->TabIndex = 8;
			// 
			// tB_oldY
			// 
			this->tB_oldY->Font = (gcnew System::Drawing::Font(L"Tahoma", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->tB_oldY->Location = System::Drawing::Point(51, 62);
			this->tB_oldY->Name = L"tB_oldY";
			this->tB_oldY->ReadOnly = true;
			this->tB_oldY->Size = System::Drawing::Size(100, 24);
			this->tB_oldY->TabIndex = 9;
			// 
			// tB_oldW
			// 
			this->tB_oldW->Font = (gcnew System::Drawing::Font(L"Tahoma", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->tB_oldW->Location = System::Drawing::Point(51, 88);
			this->tB_oldW->Name = L"tB_oldW";
			this->tB_oldW->ReadOnly = true;
			this->tB_oldW->Size = System::Drawing::Size(100, 24);
			this->tB_oldW->TabIndex = 10;
			// 
			// tB_oldH
			// 
			this->tB_oldH->Font = (gcnew System::Drawing::Font(L"Tahoma", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->tB_oldH->Location = System::Drawing::Point(51, 114);
			this->tB_oldH->Name = L"tB_oldH";
			this->tB_oldH->ReadOnly = true;
			this->tB_oldH->Size = System::Drawing::Size(100, 24);
			this->tB_oldH->TabIndex = 11;
			// 
			// button2
			// 
			this->button2->BackColor = System::Drawing::SystemColors::Control;
			this->button2->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"button2.BackgroundImage")));
			this->button2->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->button2->Location = System::Drawing::Point(480, 36);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(102, 102);
			this->button2->TabIndex = 12;
			this->button2->UseVisualStyleBackColor = false;
			this->button2->Click += gcnew System::EventHandler(this, &MyForm::button2_Click);
			// 
			// tB_NewH
			// 
			this->tB_NewH->Font = (gcnew System::Drawing::Font(L"Tahoma", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->tB_NewH->Location = System::Drawing::Point(157, 114);
			this->tB_NewH->Name = L"tB_NewH";
			this->tB_NewH->Size = System::Drawing::Size(100, 24);
			this->tB_NewH->TabIndex = 16;
			this->tB_NewH->Tag = L"3";
			this->tB_NewH->TextChanged += gcnew System::EventHandler(this, &MyForm::tB_New_TextChanged);
			// 
			// tB_NewW
			// 
			this->tB_NewW->Font = (gcnew System::Drawing::Font(L"Tahoma", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->tB_NewW->Location = System::Drawing::Point(157, 88);
			this->tB_NewW->Name = L"tB_NewW";
			this->tB_NewW->Size = System::Drawing::Size(100, 24);
			this->tB_NewW->TabIndex = 15;
			this->tB_NewW->Tag = L"2";
			this->tB_NewW->TextChanged += gcnew System::EventHandler(this, &MyForm::tB_New_TextChanged);
			// 
			// tB_NewY
			// 
			this->tB_NewY->Font = (gcnew System::Drawing::Font(L"Tahoma", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->tB_NewY->Location = System::Drawing::Point(157, 62);
			this->tB_NewY->Name = L"tB_NewY";
			this->tB_NewY->Size = System::Drawing::Size(100, 24);
			this->tB_NewY->TabIndex = 14;
			this->tB_NewY->Tag = L"1";
			this->tB_NewY->TextChanged += gcnew System::EventHandler(this, &MyForm::tB_New_TextChanged);
			// 
			// tB_NewX
			// 
			this->tB_NewX->Font = (gcnew System::Drawing::Font(L"Tahoma", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->tB_NewX->Location = System::Drawing::Point(157, 35);
			this->tB_NewX->Name = L"tB_NewX";
			this->tB_NewX->Size = System::Drawing::Size(100, 24);
			this->tB_NewX->TabIndex = 13;
			this->tB_NewX->Tag = L"0";
			this->tB_NewX->TextChanged += gcnew System::EventHandler(this, &MyForm::tB_New_TextChanged);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(28, 40);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(17, 13);
			this->label1->TabIndex = 17;
			this->label1->Text = L"X:";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(28, 67);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(17, 13);
			this->label2->TabIndex = 18;
			this->label2->Text = L"Y:";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(7, 93);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(39, 13);
			this->label3->TabIndex = 19;
			this->label3->Text = L"Width:";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(4, 119);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(42, 13);
			this->label4->TabIndex = 20;
			this->label4->Text = L"Height:";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(83, 16);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(44, 13);
			this->label5->TabIndex = 21;
			this->label5->Text = L"Current";
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(193, 16);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(28, 13);
			this->label6->TabIndex = 22;
			this->label6->Text = L"New";
			// 
			// button3
			// 
			this->button3->Font = (gcnew System::Drawing::Font(L"Tahoma", 20, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->button3->Location = System::Drawing::Point(926, 36);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(154, 102);
			this->button3->TabIndex = 23;
			this->button3->Text = L"Save .ini";
			this->button3->UseVisualStyleBackColor = true;
			this->button3->Click += gcnew System::EventHandler(this, &MyForm::button3_Click);
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(15, 180);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(31, 13);
			this->label7->TabIndex = 24;
			this->label7->Text = L"Title:";
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(10, 206);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(36, 13);
			this->label8->TabIndex = 25;
			this->label8->Text = L"Class:";
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(13, 232);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(33, 13);
			this->label9->TabIndex = 26;
			this->label9->Text = L"Path:";
			// 
			// button4
			// 
			this->button4->Font = (gcnew System::Drawing::Font(L"Tahoma", 20, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->button4->Location = System::Drawing::Point(263, 35);
			this->button4->Name = L"button4";
			this->button4->Size = System::Drawing::Size(171, 51);
			this->button4->TabIndex = 28;
			this->button4->Text = L"Use Current";
			this->button4->UseVisualStyleBackColor = true;
			this->button4->Click += gcnew System::EventHandler(this, &MyForm::button4_Click);
			// 
			// button5
			// 
			this->button5->Font = (gcnew System::Drawing::Font(L"Tahoma", 20, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->button5->Location = System::Drawing::Point(263, 88);
			this->button5->Name = L"button5";
			this->button5->Size = System::Drawing::Size(171, 50);
			this->button5->TabIndex = 29;
			this->button5->Text = L"View";
			this->button5->UseVisualStyleBackColor = true;
			this->button5->Click += gcnew System::EventHandler(this, &MyForm::button5_Click);
			// 
			// splitContainer1
			// 
			this->splitContainer1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainer1->Location = System::Drawing::Point(3, 3);
			this->splitContainer1->Name = L"splitContainer1";
			this->splitContainer1->Orientation = System::Windows::Forms::Orientation::Horizontal;
			// 
			// splitContainer1.Panel1
			// 
			this->splitContainer1->Panel1->Controls->Add(this->dataGridView1);
			this->splitContainer1->Panel1MinSize = 100;
			// 
			// splitContainer1.Panel2
			// 
			this->splitContainer1->Panel2->Controls->Add(this->dataGridView2);
			this->splitContainer1->Panel2MinSize = 100;
			this->splitContainer1->Size = System::Drawing::Size(1086, 431);
			this->splitContainer1->SplitterDistance = 290;
			this->splitContainer1->SplitterWidth = 7;
			this->splitContainer1->TabIndex = 30;
			this->splitContainer1->SplitterMoved += gcnew System::Windows::Forms::SplitterEventHandler(this, &MyForm::splitContainer1_SplitterMoved);
			this->splitContainer1->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MyForm::splitContainer1_Paint);
			// 
			// dataGridView2
			// 
			this->dataGridView2->AllowUserToAddRows = false;
			this->dataGridView2->AllowUserToDeleteRows = false;
			this->dataGridView2->AllowUserToResizeRows = false;
			dataGridViewCellStyle3->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(233)), 
				static_cast<System::Int32>(static_cast<System::Byte>(233)), static_cast<System::Int32>(static_cast<System::Byte>(233)));
			this->dataGridView2->AlternatingRowsDefaultCellStyle = dataGridViewCellStyle3;
			this->dataGridView2->BackgroundColor = System::Drawing::SystemColors::ButtonHighlight;
			dataGridViewCellStyle4->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleCenter;
			dataGridViewCellStyle4->BackColor = System::Drawing::SystemColors::Control;
			dataGridViewCellStyle4->Font = (gcnew System::Drawing::Font(L"Tahoma", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			dataGridViewCellStyle4->ForeColor = System::Drawing::SystemColors::WindowText;
			dataGridViewCellStyle4->SelectionBackColor = System::Drawing::SystemColors::Highlight;
			dataGridViewCellStyle4->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
			dataGridViewCellStyle4->WrapMode = System::Windows::Forms::DataGridViewTriState::True;
			this->dataGridView2->ColumnHeadersDefaultCellStyle = dataGridViewCellStyle4;
			this->dataGridView2->ColumnHeadersHeight = 25;
			this->dataGridView2->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::DisableResizing;
			this->dataGridView2->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(3) {this->id2, this->dataGridViewTextBoxColumn2, 
				this->dataGridViewTextBoxColumn3});
			this->dataGridView2->Dock = System::Windows::Forms::DockStyle::Fill;
			this->dataGridView2->Font = (gcnew System::Drawing::Font(L"Tahoma", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->dataGridView2->GridColor = System::Drawing::SystemColors::ButtonShadow;
			this->dataGridView2->Location = System::Drawing::Point(0, 0);
			this->dataGridView2->MultiSelect = false;
			this->dataGridView2->Name = L"dataGridView2";
			this->dataGridView2->ReadOnly = true;
			this->dataGridView2->RowHeadersVisible = false;
			this->dataGridView2->RowTemplate->ReadOnly = true;
			this->dataGridView2->RowTemplate->Resizable = System::Windows::Forms::DataGridViewTriState::False;
			this->dataGridView2->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->dataGridView2->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
			this->dataGridView2->ShowRowErrors = false;
			this->dataGridView2->Size = System::Drawing::Size(1086, 134);
			this->dataGridView2->TabIndex = 8;
			this->dataGridView2->Tag = L"1";
			this->dataGridView2->CellContentClick += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &MyForm::dgv1_CellContentClick);
			this->dataGridView2->CellDoubleClick += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &MyForm::dgv1_CellDoubleClick);
			this->dataGridView2->SelectionChanged += gcnew System::EventHandler(this, &MyForm::dgv1_SelectionChanged);
			this->dataGridView2->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &MyForm::dgv1_KeyDown);
			// 
			// id2
			// 
			this->id2->HeaderText = L"id";
			this->id2->Name = L"id2";
			this->id2->ReadOnly = true;
			this->id2->Visible = false;
			// 
			// dataGridViewTextBoxColumn2
			// 
			this->dataGridViewTextBoxColumn2->HeaderText = L"Executable";
			this->dataGridViewTextBoxColumn2->MinimumWidth = 150;
			this->dataGridViewTextBoxColumn2->Name = L"dataGridViewTextBoxColumn2";
			this->dataGridViewTextBoxColumn2->ReadOnly = true;
			this->dataGridViewTextBoxColumn2->Width = 200;
			// 
			// dataGridViewTextBoxColumn3
			// 
			this->dataGridViewTextBoxColumn3->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::Fill;
			this->dataGridViewTextBoxColumn3->HeaderText = L"Window Title -- Inactive Windows from *.ini file";
			this->dataGridViewTextBoxColumn3->MinimumWidth = 200;
			this->dataGridViewTextBoxColumn3->Name = L"dataGridViewTextBoxColumn3";
			this->dataGridViewTextBoxColumn3->ReadOnly = true;
			this->dataGridViewTextBoxColumn3->Resizable = System::Windows::Forms::DataGridViewTriState::False;
			// 
			// panel1
			// 
			this->panel1->CausesValidation = false;
			this->panel1->Controls->Add(this->splitContainer1);
			this->panel1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->panel1->Location = System::Drawing::Point(0, 0);
			this->panel1->Name = L"panel1";
			this->panel1->Padding = System::Windows::Forms::Padding(3);
			this->panel1->Size = System::Drawing::Size(1092, 437);
			this->panel1->TabIndex = 31;
			// 
			// panel2
			// 
			this->panel2->Controls->Add(this->label10);
			this->panel2->Controls->Add(this->cB_Path);
			this->panel2->Controls->Add(this->cB_Class);
			this->panel2->Controls->Add(this->cB_Title);
			this->panel2->Controls->Add(this->cB_isIconic);
			this->panel2->Controls->Add(this->cB_Show_Invis);
			this->panel2->Controls->Add(this->button3);
			this->panel2->Controls->Add(this->button1);
			this->panel2->Controls->Add(this->label9);
			this->panel2->Controls->Add(this->button5);
			this->panel2->Controls->Add(this->label8);
			this->panel2->Controls->Add(this->tB_oldX);
			this->panel2->Controls->Add(this->label7);
			this->panel2->Controls->Add(this->button4);
			this->panel2->Controls->Add(this->tB_Path);
			this->panel2->Controls->Add(this->tB_oldY);
			this->panel2->Controls->Add(this->tB_Class);
			this->panel2->Controls->Add(this->tB_oldW);
			this->panel2->Controls->Add(this->tB_Title);
			this->panel2->Controls->Add(this->tB_oldH);
			this->panel2->Controls->Add(this->button2);
			this->panel2->Controls->Add(this->tB_NewX);
			this->panel2->Controls->Add(this->label6);
			this->panel2->Controls->Add(this->tB_NewY);
			this->panel2->Controls->Add(this->label5);
			this->panel2->Controls->Add(this->tB_NewW);
			this->panel2->Controls->Add(this->label4);
			this->panel2->Controls->Add(this->tB_NewH);
			this->panel2->Controls->Add(this->label3);
			this->panel2->Controls->Add(this->label1);
			this->panel2->Controls->Add(this->label2);
			this->panel2->Dock = System::Windows::Forms::DockStyle::Bottom;
			this->panel2->Font = (gcnew System::Drawing::Font(L"Tahoma", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->panel2->Location = System::Drawing::Point(0, 437);
			this->panel2->Name = L"panel2";
			this->panel2->Size = System::Drawing::Size(1092, 265);
			this->panel2->TabIndex = 32;
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Font = (gcnew System::Drawing::Font(L"Tahoma", 8.25F, System::Drawing::FontStyle::Underline, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->label10->Location = System::Drawing::Point(1037, 157);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(43, 13);
			this->label10->TabIndex = 35;
			this->label10->Text = L"Custom";
			// 
			// cB_Path
			// 
			this->cB_Path->Location = System::Drawing::Point(1052, 232);
			this->cB_Path->Name = L"cB_Path";
			this->cB_Path->Size = System::Drawing::Size(15, 15);
			this->cB_Path->TabIndex = 34;
			this->cB_Path->Tag = L"2";
			this->cB_Path->UseVisualStyleBackColor = true;
			this->cB_Path->CheckedChanged += gcnew System::EventHandler(this, &MyForm::cB_CustomText_Changed);
			// 
			// cB_Class
			// 
			this->cB_Class->Location = System::Drawing::Point(1052, 206);
			this->cB_Class->Name = L"cB_Class";
			this->cB_Class->Size = System::Drawing::Size(15, 15);
			this->cB_Class->TabIndex = 33;
			this->cB_Class->Tag = L"1";
			this->cB_Class->UseVisualStyleBackColor = true;
			this->cB_Class->CheckedChanged += gcnew System::EventHandler(this, &MyForm::cB_CustomText_Changed);
			// 
			// cB_Title
			// 
			this->cB_Title->Location = System::Drawing::Point(1052, 180);
			this->cB_Title->Name = L"cB_Title";
			this->cB_Title->Size = System::Drawing::Size(15, 15);
			this->cB_Title->TabIndex = 32;
			this->cB_Title->Tag = L"0";
			this->cB_Title->UseVisualStyleBackColor = true;
			this->cB_Title->CheckedChanged += gcnew System::EventHandler(this, &MyForm::cB_CustomText_Changed);
			// 
			// cB_isIconic
			// 
			this->cB_isIconic->AutoSize = true;
			this->cB_isIconic->Location = System::Drawing::Point(51, 156);
			this->cB_isIconic->Name = L"cB_isIconic";
			this->cB_isIconic->Size = System::Drawing::Size(122, 17);
			this->cB_isIconic->TabIndex = 31;
			this->cB_isIconic->Text = L"Window is Minimized";
			this->cB_isIconic->UseVisualStyleBackColor = true;
			this->cB_isIconic->CheckedChanged += gcnew System::EventHandler(this, &MyForm::cB_isIconic_CheckedChanged);
			// 
			// cB_Show_Invis
			// 
			this->cB_Show_Invis->Location = System::Drawing::Point(926, 6);
			this->cB_Show_Invis->Name = L"cB_Show_Invis";
			this->cB_Show_Invis->Size = System::Drawing::Size(154, 24);
			this->cB_Show_Invis->TabIndex = 30;
			this->cB_Show_Invis->Text = L"Show Invisible Windows";
			this->cB_Show_Invis->UseVisualStyleBackColor = true;
			this->cB_Show_Invis->CheckedChanged += gcnew System::EventHandler(this, &MyForm::cB_Show_Invis_CheckedChanged);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1092, 702);
			this->Controls->Add(this->panel1);
			this->Controls->Add(this->panel2);
			this->Font = (gcnew System::Drawing::Font(L"Tahoma", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->MaximizeBox = false;
			this->MaximumSize = System::Drawing::Size(1108, 2000);
			this->MinimizeBox = false;
			this->MinimumSize = System::Drawing::Size(1108, 666);
			this->Name = L"MyForm";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"wndRestore 2.0";
			this->Shown += gcnew System::EventHandler(this, &MyForm::MyForm_Shown);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView1))->EndInit();
			this->splitContainer1->Panel1->ResumeLayout(false);
			this->splitContainer1->Panel2->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->splitContainer1))->EndInit();
			this->splitContainer1->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView2))->EndInit();
			this->panel1->ResumeLayout(false);
			this->panel2->ResumeLayout(false);
			this->panel2->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion

	private:

		typedef DataGridView					DGV;
		typedef DataGridViewCellEventArgs		dgv_CellEvtArgs;
		typedef DataGridViewCellMouseEventArgs	dgv_CellMsEvtArgs;

		static void addCheckBox_toTheGrid		(DGV^, UINT, UINT, const char *);

		template<class T>
		void	setGridCheckbox					(DataGridView^, T&, int, int );

		int		getItemIndex					(int &						 );	// Get index for a selected item in the grid
		void	setTextData						(int, int					 );	// Set all text data for a selected grid item
		void	setActiveGrid					(DGV ^, int &, int &		 );	// Set active grid
		void	setGridColors					(DGV ^, DGV ^				 );	// Set colors for active and inactive grids
		DGV^	getActiveGrid					();								// Get pointer to the active grid
		Void	MyForm_Shown					(Object^, EventArgs^		 );	// Event: application window is shown for the 1st time
		Void	button1_Click					(Object^, EventArgs^		 );	// Reposition selected windows
		Void	button2_Click					(Object^, EventArgs^		 );	// Get currently active windows (refresh)
		Void	button3_Click					(Object^, EventArgs^		 );	// Save data to ini-file
		Void	button4_Click					(Object^, EventArgs^		 );	// Replace new coordinates with the current ones for a celected window
		Void	button5_Click					(Object^, EventArgs^		 );	// View selected window
		Void	dgv1_Resize						(Object^, EventArgs^		 );	// Resize checkbox columns in both grids
		Void	dgv1_SelectionChanged			(Object^, EventArgs^		 );	// Move through the Grid
		Void	dgv1_CellContentClick			(Object^, dgv_CellEvtArgs^   );	// Grid helper1 -- checkBox selection
		Void	dgv1_KeyDown					(Object^, KeyEventArgs^		 );	// Grid helper2 -- checkBox selection
		Void	dgv1_CellDoubleClick			(Object^, dgv_CellEvtArgs^   );	// Grid helper3 -- checkBox selection
		Void	dgv1_CellMouseClick				(Object^, dgv_CellMsEvtArgs^ );	// Grid helper4 -- grid click with additional key pressed
		Void	tB_New_TextChanged				(Object^, EventArgs^		 );	// Change of Coordinate Text Box 
		Void	tB_Title_KeyUp					(Object^, KeyEventArgs^		 );	// Change Title for a selected grid item
		Void	tB_Title_KeyPress				(Object^, KeyPressEventArgs^ );	// Press Enter for a selected grid item's Text Property
		Void	cB_Show_Invis_CheckedChanged	(Object^, EventArgs^		 );	// Checkbox 'Show Invisible' onChange method
		Void	cB_isIconic_CheckedChanged		(Object^, EventArgs^		 );	// Checkbox 'Window is Minimized' onChange method
		Void	cB_CustomText_Changed			(Object^, EventArgs^		 );	// Checkbox for custom text onChange method
		Void	splitContainer1_Paint			(Object^, PaintEventArgs^	 );	// Custom Draw for Split Container
		Void	splitContainer1_SplitterMoved	(Object^, SplitterEventArgs^ );	// Split Container Splitter Move event
	};
}
