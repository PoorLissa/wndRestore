#pragma once

#include "myApp.h"

extern myApplication::appMain cpp_app;

namespace Project1
{
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	public ref class profileForm : public System::Windows::Forms::Form
	{
		public:
			profileForm(void)
			{
				InitializeComponent();
			}

		protected:
			~profileForm()
			{
				if (components)
				{
					delete components;
				}
			}

		private:
			System::ComponentModel::Container	^ components;
			System::Windows::Forms::Button		^ button1;
			System::Windows::Forms::TextBox		^ textBox1;
			System::String						^ buttonText;

#pragma region Windows Form Designer generated code
			/// <summary>
			/// Required method for Designer support - do not modify
			/// the contents of this method with the code editor.
			/// </summary>
			void InitializeComponent(void)
			{
				this->button1 = (gcnew System::Windows::Forms::Button());
				this->textBox1 = (gcnew System::Windows::Forms::TextBox());
				this->SuspendLayout();
				// 
				// button1
				// 
				this->button1->Font = (gcnew System::Drawing::Font(L"Tahoma", 20, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
					static_cast<System::Byte>(204)));
				this->button1->Location = System::Drawing::Point(8, 43);
				this->button1->Name = L"button1";
				this->button1->Size = System::Drawing::Size(550, 114);
				this->button1->TabIndex = 0;
				this->button1->Text = L"...";
				this->button1->UseVisualStyleBackColor = true;
				this->button1->Click += gcnew System::EventHandler(this, &profileForm::button1_Click);
				// 
				// textBox1
				// 
				this->textBox1->Font = (gcnew System::Drawing::Font(L"Tahoma", 13, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
					static_cast<System::Byte>(204)));
				this->textBox1->Location = System::Drawing::Point(8, 9);
				this->textBox1->Name = L"textBox1";
				this->textBox1->Size = System::Drawing::Size(550, 28);
				this->textBox1->TabIndex = 1;
				this->textBox1->TextChanged += gcnew System::EventHandler(this, &profileForm::textBox1_TextChanged);
				this->textBox1->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &profileForm::textBox1_KeyPress);
				// 
				// profileForm
				// 
				this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
				this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
				this->ClientSize = System::Drawing::Size(565, 165);
				this->Controls->Add(this->textBox1);
				this->Controls->Add(this->button1);
				this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
				this->Name = L"profileForm";
				this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
				this->Text = L"...";
				this->Shown += gcnew System::EventHandler(this, &profileForm::profileForm_Shown);
				this->ResumeLayout(false);
				this->PerformLayout();

			}
#pragma endregion
	
		private:
			
			// Button click
			System::Void button1_Click(System::Object^ sender, System::EventArgs^ e)
			{
				// Add new profile
				if( Tag->ToString() == "form_tag_1_add" )
				{
					myApplication::myString str;

					cpp_app.getStr_fromSystem(textBox1->Text, str);
					myApplication::vec_profiles.push_back(str);

					textBox1->Clear();
				}

				// Rename current profile
				if( Tag->ToString() == "form_tag_2_rename" )
				{
					Tag = textBox1->Text;
					Close();
				}
			}
	
			// Check validity of the name
			System::Void textBox1_TextChanged(System::Object^ sender, System::EventArgs^ e)
			{
				if( !textBox1->Text->Length )
				{
					button1->Text = buttonText;
					button1->Enabled = false;
				}
				else
				{
					bool found = false;
					auto *vec = &myApplication::vec_profiles;
					myApplication::myString str;
					cpp_app.getStr_fromSystem(textBox1->Text, str);

					for(size_t i = 0; i < vec->size(); i++)
					{
						if( str == vec->at(i) )
						{
							found = true;
							break;
						}
					}

					button1->Enabled = !found;
					button1->Text = found ? "The name already exists" : buttonText;
				}

				return;
			}
	
			// On form display event
			System::Void profileForm_Shown(System::Object^ sender, System::EventArgs^ e)
			{
				if( Tag->ToString() == "form_tag_1_add" )
				{
					buttonText = "Add";
					button1->Text = buttonText;
					Text = "Add new Profile";
				}

				if( Tag->ToString() == "form_tag_2_rename" )
				{
					buttonText = "Rename";
					button1->Text = buttonText;
					Text = "Rename Profile";
				}

				// Focus on textbox input
				textBox1->Focus();
			}

			// Form KeyPress event
			System::Void textBox1_KeyPress(System::Object^ sender, System::Windows::Forms::KeyPressEventArgs^ e)
			{
				// Close form if ESC was hit
				if( e->KeyChar == char(Keys::Escape) )
					Close();

				// Add profile if ENTER was hit
				if( e->KeyChar == char(Keys::Enter) )
				{
					if( button1->Enabled )
						button1_Click(nullptr, nullptr);
				}
			}
	};
}
