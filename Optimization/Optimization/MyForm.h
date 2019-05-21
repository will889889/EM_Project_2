#pragma once
#include "DataManager.h"
#include "DotNetUtilities.h"
#include "UIHandler.h"

namespace Optimization {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// MyForm 的摘要
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		UIHandler * m_UIHandler;
	public:
		MyForm(void)
		{
			InitializeComponent();
			dataManager = new DataManager();
			m_UIHandler = new UIHandler();
			//
			//TODO:  在此加入建構函式程式碼
			//
		}
	private: System::Windows::Forms::ComboBox^  comboBox1;
	public:
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::Button^  button3;

	protected:
		/// <summary>
		DataManager * dataManager;
		/// 清除任何使用中的資源。
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel1;
	protected:
	private: System::Windows::Forms::FlowLayoutPanel^  flowLayoutPanel1;
	private: System::Windows::Forms::Label^  InputText;
	private: System::Windows::Forms::TextBox^  Input;
	private: System::Windows::Forms::Label^  OutputText;
	private: System::Windows::Forms::TextBox^  Output;
	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  loadEquationsToolStripMenuItem;
	private: System::Windows::Forms::OpenFileDialog^  openFileDialog1;

	private:
		/// <summary>
		/// 設計工具所需的變數。
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// 此為設計工具支援所需的方法 - 請勿使用程式碼編輯器修改
		/// 這個方法的內容。
		/// </summary>
		void InitializeComponent(void)
		{
			this->tableLayoutPanel1 = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->flowLayoutPanel1 = (gcnew System::Windows::Forms::FlowLayoutPanel());
			this->InputText = (gcnew System::Windows::Forms::Label());
			this->comboBox1 = (gcnew System::Windows::Forms::ComboBox());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->Input = (gcnew System::Windows::Forms::TextBox());
			this->OutputText = (gcnew System::Windows::Forms::Label());
			this->Output = (gcnew System::Windows::Forms::TextBox());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->loadEquationsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->tableLayoutPanel1->SuspendLayout();
			this->flowLayoutPanel1->SuspendLayout();
			this->menuStrip1->SuspendLayout();
			this->SuspendLayout();
			// 
			// tableLayoutPanel1
			// 
			this->tableLayoutPanel1->ColumnCount = 1;
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				100)));
			this->tableLayoutPanel1->Controls->Add(this->flowLayoutPanel1, 0, 1);
			this->tableLayoutPanel1->Controls->Add(this->menuStrip1, 0, 0);
			this->tableLayoutPanel1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->tableLayoutPanel1->Location = System::Drawing::Point(0, 0);
			this->tableLayoutPanel1->Margin = System::Windows::Forms::Padding(4);
			this->tableLayoutPanel1->Name = L"tableLayoutPanel1";
			this->tableLayoutPanel1->RowCount = 2;
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 5)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 95)));
			this->tableLayoutPanel1->Size = System::Drawing::Size(726, 853);
			this->tableLayoutPanel1->TabIndex = 0;
			// 
			// flowLayoutPanel1
			// 
			this->flowLayoutPanel1->Controls->Add(this->InputText);
			this->flowLayoutPanel1->Controls->Add(this->comboBox1);
			this->flowLayoutPanel1->Controls->Add(this->button1);
			this->flowLayoutPanel1->Controls->Add(this->button2);
			this->flowLayoutPanel1->Controls->Add(this->button3);
			this->flowLayoutPanel1->Controls->Add(this->Input);
			this->flowLayoutPanel1->Controls->Add(this->OutputText);
			this->flowLayoutPanel1->Controls->Add(this->Output);
			this->flowLayoutPanel1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->flowLayoutPanel1->Location = System::Drawing::Point(4, 46);
			this->flowLayoutPanel1->Margin = System::Windows::Forms::Padding(4);
			this->flowLayoutPanel1->Name = L"flowLayoutPanel1";
			this->flowLayoutPanel1->Size = System::Drawing::Size(718, 803);
			this->flowLayoutPanel1->TabIndex = 0;
			// 
			// InputText
			// 
			this->InputText->AutoSize = true;
			this->InputText->Location = System::Drawing::Point(4, 0);
			this->InputText->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->InputText->Name = L"InputText";
			this->InputText->Size = System::Drawing::Size(37, 15);
			this->InputText->TabIndex = 0;
			this->InputText->Text = L"Input";
			// 
			// comboBox1
			// 
			this->comboBox1->FormattingEnabled = true;
			this->comboBox1->Items->AddRange(gcnew cli::array< System::Object^  >(5) {
				L"Powell", L"Newton", L"SteepDescent", L"QuasiNewton",
					L"ConjugateGradient"
			});
			this->comboBox1->Location = System::Drawing::Point(48, 3);
			this->comboBox1->Name = L"comboBox1";
			this->comboBox1->Size = System::Drawing::Size(121, 23);
			this->comboBox1->TabIndex = 4;
			this->comboBox1->Text = L"Powell";
			this->comboBox1->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::comboBox1_SelectedIndexChanged);
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(175, 3);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 5;
			this->button1->Text = L"Calculate";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &MyForm::button1_Click);
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(256, 3);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(75, 23);
			this->button2->TabIndex = 6;
			this->button2->Text = L"Clear";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &MyForm::button2_Click);
			// 
			// Input
			// 
			this->Input->Location = System::Drawing::Point(4, 33);
			this->Input->Margin = System::Windows::Forms::Padding(4);
			this->Input->Multiline = true;
			this->Input->Name = L"Input";
			this->Input->Size = System::Drawing::Size(705, 97);
			this->Input->TabIndex = 1;
			this->Input->TextChanged += gcnew System::EventHandler(this, &MyForm::Input_TextChanged);
			// 
			// OutputText
			// 
			this->OutputText->AutoSize = true;
			this->OutputText->Location = System::Drawing::Point(4, 134);
			this->OutputText->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->OutputText->Name = L"OutputText";
			this->OutputText->Size = System::Drawing::Size(46, 15);
			this->OutputText->TabIndex = 2;
			this->OutputText->Text = L"Output";
			// 
			// Output
			// 
			this->Output->Location = System::Drawing::Point(4, 153);
			this->Output->Margin = System::Windows::Forms::Padding(4);
			this->Output->Multiline = true;
			this->Output->Name = L"Output";
			this->Output->ScrollBars = System::Windows::Forms::ScrollBars::Both;
			this->Output->Size = System::Drawing::Size(705, 641);
			this->Output->TabIndex = 3;
			// 
			// menuStrip1
			// 
			this->menuStrip1->ImageScalingSize = System::Drawing::Size(20, 20);
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->fileToolStripMenuItem });
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Padding = System::Windows::Forms::Padding(8, 2, 0, 2);
			this->menuStrip1->Size = System::Drawing::Size(726, 27);
			this->menuStrip1->TabIndex = 1;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->loadEquationsToolStripMenuItem });
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(45, 23);
			this->fileToolStripMenuItem->Text = L"File";
			// 
			// loadEquationsToolStripMenuItem
			// 
			this->loadEquationsToolStripMenuItem->Name = L"loadEquationsToolStripMenuItem";
			this->loadEquationsToolStripMenuItem->Size = System::Drawing::Size(192, 26);
			this->loadEquationsToolStripMenuItem->Text = L"Load Equations";
			this->loadEquationsToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::loadEquationsToolStripMenuItem_Click);
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->FileName = L"openFileDialog1";
			this->openFileDialog1->FileOk += gcnew System::ComponentModel::CancelEventHandler(this, &MyForm::openFileDialog1_FileOk);
			// 
			// button3
			// 
			this->button3->Location = System::Drawing::Point(337, 3);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(75, 23);
			this->button3->TabIndex = 7;
			this->button3->Text = L"Fix";
			this->button3->UseVisualStyleBackColor = true;
			this->button3->Click += gcnew System::EventHandler(this, &MyForm::button3_Click);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 15);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(726, 853);
			this->Controls->Add(this->tableLayoutPanel1);
			this->MainMenuStrip = this->menuStrip1;
			this->Margin = System::Windows::Forms::Padding(4);
			this->Name = L"MyForm";
			this->Text = L"Optimization";
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			this->tableLayoutPanel1->ResumeLayout(false);
			this->tableLayoutPanel1->PerformLayout();
			this->flowLayoutPanel1->ResumeLayout(false);
			this->flowLayoutPanel1->PerformLayout();
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void MyForm_Load(System::Object^  sender, System::EventArgs^  e) {
	}
	private: System::Void loadEquationsToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
		openFileDialog1->ShowDialog();
	}
	private: System::Void openFileDialog1_FileOk(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e) {
		//在Dialog按下OK便會進入此函式
		//字串轉制string^ to string
		std::string tempFileName;
		MarshalString(openFileDialog1->FileName, tempFileName);
		//將檔案路徑名稱傳入dataManager
		dataManager->SetFileName(tempFileName);

		/*Output->Text = "";
		Output->Multiline = true;
		Output->Text += gcnew String(tempFileName.c_str());
		return;*/

		//從讀取讀取向量資料
		if (dataManager->LoadEquationData())
		{
			/*std::vector<std::string> equations = dataManager->GetEquations();
			m_UIHandler->equations = equations;*/
			m_UIHandler->equations.clear();
			m_UIHandler->equations = dataManager->GetEquations();

			Output->Text = "";
			Output->Multiline = true;
			for (unsigned int i = 0; i < m_UIHandler->equations.size(); i++)
			{
				///std::cout << std::to_string(i) << " : " << m_UIHandler->equations[i] << "\n";
				Output->Text += gcnew String(i.ToString() + " : ");
				Output->Text += gcnew String(m_UIHandler->equations[i].c_str());
				Output->Text += Environment::NewLine;
			}
		}
	}
	private: System::Void comboBox1_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
	{
		ComboBox^ comboBox = (ComboBox^)sender;

		std::string tempStr;
		//	to string
		MarshalString((String^)comboBox->SelectedItem, tempStr);
		std::cout << tempStr << " \n";

		if (tempStr == "Powell")
		{
			m_UIHandler->m_Method = Powell;
		}
		else if (tempStr == "Newton")
		{
			m_UIHandler->m_Method = Newton;
		}
		else if (tempStr == "SteepDescent")
		{
			m_UIHandler->m_Method = SteepDescent;
		}
		else if (tempStr == "QuasiNewton")
		{
			m_UIHandler->m_Method = QuasiNewton;
		}
		else if (tempStr == "ConjugateGradient")
		{
			m_UIHandler->m_Method = ConjugateGradient;
		}
		else
		{
			std::cout << "comboBox->SelectedItem ERROR \n";
		}

		/*Output->Multiline = true;
		Output->Text += comboBox->SelectedItem;
		Output->Text += Environment::NewLine;*/
	}
	
	private: System::Void Input_TextChanged(System::Object^  sender, System::EventArgs^  e)
	{
		TextBox^ textBox = (TextBox^)sender;

		//	to string
		std::string tempStr;
		MarshalString(textBox->Text, tempStr);

		m_UIHandler->m_Input = tempStr;
	}
	//	[Calculate]
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e)
	{
		std::vector<std::string> answers;

		//	call m_UIHandler
		///answers = m_UIHandler->DoMath();
		answers = m_UIHandler->TEST();

		Output->Multiline = true;

		Output->Text = "";
		for (unsigned int i = 0; i < answers.size(); i++)
		{
			Output->Text += gcnew String(answers[i].c_str());
			Output->Text += Environment::NewLine;
		}
	}
	//	[Clear]
	private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e)
	{
		Output->Multiline = true;
		
		Output->Text = "";

		for (unsigned int i = 0; i < m_UIHandler->equations.size(); i++)
		{
			Output->Text += gcnew String(i.ToString() + " : ");
			Output->Text += gcnew String(m_UIHandler->equations[i].c_str());
			Output->Text += Environment::NewLine;
		}
	}
	//	[Fix]
	private: System::Void button3_Click(System::Object^  sender, System::EventArgs^  e) 
	{
		//	fix equation
		m_UIHandler->FixEquation();
		//	output
		Output->Multiline = true;
		Output->Text = "";
		for (unsigned int i = 0; i < m_UIHandler->Answer.size(); i++)
		{
			Output->Text += gcnew String(m_UIHandler->Answer[i].c_str());
			Output->Text += Environment::NewLine;
		}
	}
};
}
