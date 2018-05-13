#pragma once

#include "yaCommon.h"
#include "yaString.h"
#include "ImageObsProperty.h"
#include "ImageViewProperty.h"

namespace ImageColumn {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	using namespace System::Drawing::Imaging;
	using namespace System::Collections::Generic;

	using namespace System::Xml::Serialization;
	using namespace System::IO;

	int load_image_count = 0;//ì«Ç›çûÇ‹ÇÍÇƒÇ¢ÇÈâÊëúêî
	cv::Mat layerMat;

	/// <summary>
	/// Summary for MainForm
	/// </summary>
	public ref class MainForm : public System::Windows::Forms::Form
	{
	public:
		MainForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//

			listImageObsProperty = gcnew BindingList<ImageObsProperty^>();
			listImageViewProperty = gcnew BindingList<ImageViewProperty^>();

			imageDataGridView->DataSource = listImageObsProperty;

			this->Text = "Image Column";
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MainForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::PictureBox^  mainPictureBox;
	protected:

	protected:
	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  openToolStripMenuItem;
	private: System::Windows::Forms::DataGridView^  imageDataGridView;
	private: System::Windows::Forms::Panel^  mainPanel;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Button^  removeButton;




	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->mainPictureBox = (gcnew System::Windows::Forms::PictureBox());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->imageDataGridView = (gcnew System::Windows::Forms::DataGridView());
			this->mainPanel = (gcnew System::Windows::Forms::Panel());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->removeButton = (gcnew System::Windows::Forms::Button());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->mainPictureBox))->BeginInit();
			this->menuStrip1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->imageDataGridView))->BeginInit();
			this->mainPanel->SuspendLayout();
			this->SuspendLayout();
			// 
			// mainPictureBox
			// 
			this->mainPictureBox->BackColor = System::Drawing::SystemColors::ActiveCaptionText;
			this->mainPictureBox->Location = System::Drawing::Point(3, 3);
			this->mainPictureBox->Name = L"mainPictureBox";
			this->mainPictureBox->Size = System::Drawing::Size(540, 328);
			this->mainPictureBox->SizeMode = System::Windows::Forms::PictureBoxSizeMode::AutoSize;
			this->mainPictureBox->TabIndex = 0;
			this->mainPictureBox->TabStop = false;
			this->mainPictureBox->Click += gcnew System::EventHandler(this, &MainForm::mainPictureBox_Click);
			this->mainPictureBox->MouseDoubleClick += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::mainPictureBox_MouseDoubleClick);
			// 
			// menuStrip1
			// 
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->fileToolStripMenuItem });
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(744, 24);
			this->menuStrip1->TabIndex = 1;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->openToolStripMenuItem });
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(39, 20);
			this->fileToolStripMenuItem->Text = L"File";
			// 
			// openToolStripMenuItem
			// 
			this->openToolStripMenuItem->Name = L"openToolStripMenuItem";
			this->openToolStripMenuItem->Size = System::Drawing::Size(104, 22);
			this->openToolStripMenuItem->Text = L"Open";
			this->openToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::openToolStripMenuItem_Click);
			// 
			// imageDataGridView
			// 
			this->imageDataGridView->AllowUserToAddRows = false;
			this->imageDataGridView->AllowUserToDeleteRows = false;
			this->imageDataGridView->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Right));
			this->imageDataGridView->AutoSizeColumnsMode = System::Windows::Forms::DataGridViewAutoSizeColumnsMode::ColumnHeader;
			this->imageDataGridView->Location = System::Drawing::Point(565, 64);
			this->imageDataGridView->Name = L"imageDataGridView";
			this->imageDataGridView->ReadOnly = true;
			this->imageDataGridView->RightToLeft = System::Windows::Forms::RightToLeft::Yes;
			this->imageDataGridView->RowTemplate->Height = 21;
			this->imageDataGridView->Size = System::Drawing::Size(167, 279);
			this->imageDataGridView->TabIndex = 2;
			// 
			// mainPanel
			// 
			this->mainPanel->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->mainPanel->AutoScroll = true;
			this->mainPanel->BackColor = System::Drawing::SystemColors::ActiveCaptionText;
			this->mainPanel->Controls->Add(this->mainPictureBox);
			this->mainPanel->Location = System::Drawing::Point(13, 41);
			this->mainPanel->Name = L"mainPanel";
			this->mainPanel->Size = System::Drawing::Size(546, 334);
			this->mainPanel->TabIndex = 3;
			// 
			// label1
			// 
			this->label1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(566, 41);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(55, 12);
			this->label1->TabIndex = 4;
			this->label1->Text = L"Image list";
			// 
			// removeButton
			// 
			this->removeButton->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->removeButton->Location = System::Drawing::Point(657, 349);
			this->removeButton->Name = L"removeButton";
			this->removeButton->Size = System::Drawing::Size(75, 23);
			this->removeButton->TabIndex = 5;
			this->removeButton->Text = L"Remove";
			this->removeButton->UseVisualStyleBackColor = true;
			this->removeButton->Click += gcnew System::EventHandler(this, &MainForm::removeButton_Click);
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(744, 387);
			this->Controls->Add(this->removeButton);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->mainPanel);
			this->Controls->Add(this->imageDataGridView);
			this->Controls->Add(this->menuStrip1);
			this->MainMenuStrip = this->menuStrip1;
			this->MinimizeBox = false;
			this->Name = L"MainForm";
			this->Text = L"MainForm";
			this->Load += gcnew System::EventHandler(this, &MainForm::MainForm_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->mainPictureBox))->EndInit();
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->imageDataGridView))->EndInit();
			this->mainPanel->ResumeLayout(false);
			this->mainPanel->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	public: BindingList<ImageObsProperty^>^ listImageObsProperty;
	public: BindingList<ImageViewProperty^>^ listImageViewProperty;

	private: System::Void openToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
	{
		OpenFileDialog^ dlg = gcnew OpenFileDialog;
		dlg->Filter = "âÊëúÉtÉ@ÉCÉã|*.*";
		if (dlg->ShowDialog() == System::Windows::Forms::DialogResult::Cancel){
			return;
		}
		this->Cursor = System::Windows::Forms::Cursors::WaitCursor;

		System::String^ fname = dlg->FileName;

		ImageObsProperty^ newImageObsProperty = gcnew ImageObsProperty();
		ImageViewProperty^ newImageViewProperty = gcnew ImageViewProperty();

		this->Cursor = System::Windows::Forms::Cursors::WaitCursor;

		newImageObsProperty->readRasterFile(fname);
		newImageViewProperty->setData(newImageObsProperty, load_image_count);

		System::String^ browseName = System::String::Concat("./browse/", newImageObsProperty->RasterFileName, ".jpg");
		newImageViewProperty->browseImg = cvLoadImage(yaString::systemStringToChar(browseName));

		listImageObsProperty->Add(newImageObsProperty);
		listImageViewProperty->Add(newImageViewProperty);

		load_image_count++;

		this->Cursor = System::Windows::Forms::Cursors::Default;

		XmlSerializer^ xmlSerializer = gcnew XmlSerializer(BindingList<ImageObsProperty^>::typeid);

		FileStream^ fileStream = File::Open("default.xml", FileMode::Create);

		xmlSerializer->Serialize(fileStream, listImageObsProperty);

		fileStream->Close();

		redrawView();
	}

	private: System::Void MainForm_Load(System::Object^  sender, System::EventArgs^  e)
	{
		XmlSerializer^ xmlSerializer = gcnew XmlSerializer(BindingList<ImageObsProperty^>::typeid);
		FileStream^ fileStream = File::Open("default.xml", FileMode::Open, FileAccess::Read);

		listImageObsProperty->Clear();

		listImageObsProperty = (BindingList<ImageObsProperty^>^)xmlSerializer->Deserialize(fileStream);

		imageDataGridView->DataSource = listImageObsProperty;

		fileStream->Close();

		ImageObsProperty^ selectedImageObsProperty = gcnew ImageObsProperty();

		for (int i = 0; i < imageDataGridView->Rows->Count; i++){

			ImageObsProperty^ selectedImageObsProperty = listImageObsProperty[i];
			ImageViewProperty^ newImageViewProperty = gcnew ImageViewProperty();

			newImageViewProperty->setData(selectedImageObsProperty, load_image_count);

			System::String^ browseName = System::String::Concat("./browse/", selectedImageObsProperty->RasterFileName, ".jpg");
			newImageViewProperty->browseImg = cvLoadImage(yaString::systemStringToChar(browseName), 1);
			newImageViewProperty->Layer = i;

			listImageViewProperty->Add(newImageViewProperty);

			load_image_count++;
		}

		redrawView();

		mainPanel->SizeChanged += gcnew System::EventHandler(this, &MainForm::mainPanel_SizeChanged);
	}

	private: System::Void redrawView()
	{
		int mw = 20;
		int mh = 20;
		int iw = 200;

		int N_column = floor((double)(mainPanel->Width - mw-4) / (double)(mw + iw));
		int N_row = ceil((double)load_image_count / (double)N_column);

		int ih_max[256];
		for (int i = 0; i < 256; i++){
			ih_max[i] = 0;
		}

		for (int i = 0; i < N_row; i++){
			for (int j = 0; j < N_column; j++){
				if (i*N_column + j > load_image_count - 1){
					break;
				}

				ImageViewProperty^ selectedImageViewProperty = listImageViewProperty[i*N_column + j];

				selectedImageViewProperty->Sw = j*(mw + iw) + mw;
				selectedImageViewProperty->Sh = 0;

				for (int k = 0; k < i; k++){
					selectedImageViewProperty->Sh += (ih_max[k] + mh);
				}
				selectedImageViewProperty->Sh += mh;

				ih_max[i] = max(ih_max[i], selectedImageViewProperty->Ih);
			}
		}

		int image_width = (mainPanel->Width - mw);
		int image_height = 0;

		for (int i = 0; i < N_row; i++){
			image_height += ih_max[i] + mh;

			printf("max height %d\n", ih_max[i]);
		}

		image_height += mh;

		cv::Mat listMat = cv::Mat::zeros(cv::Size(image_width, image_height), CV_8UC3);
		layerMat = cv::Mat::zeros(cv::Size(image_width, image_height), CV_8UC1);

		printf("(w,h)=%d %d\n", image_width, image_height);

		for (int i = 0; i < load_image_count; i++){

			ImageViewProperty^ selectedImageViewProperty = listImageViewProperty[i];

			cv::Mat src = cvarrToMat(selectedImageViewProperty->browseImg);
			resize(src, src, cv::Size(selectedImageViewProperty->Iw, selectedImageViewProperty->Ih));

			printf("(Sw,Sh)=%d %d, Layer=%d\n", selectedImageViewProperty->Sw, selectedImageViewProperty->Sh, selectedImageViewProperty->Layer);

			if (selectedImageViewProperty->Clicked == TRUE){
				cv::Mat roi_mat1 = cv::Mat(listMat, cv::Rect(selectedImageViewProperty->Sw-2, selectedImageViewProperty->Sh-2,
					selectedImageViewProperty->Iw+4, selectedImageViewProperty->Ih+4));

				cv::Mat selected = cv::Mat(cv::Size(selectedImageViewProperty->Iw+4, selectedImageViewProperty->Ih+4), CV_8UC3,cv::Scalar(0,0,1)) * 128;

				selected.copyTo(roi_mat1);
			}

			cv::Mat roi_mat1 = cv::Mat(listMat, cv::Rect(selectedImageViewProperty->Sw, selectedImageViewProperty->Sh,
				selectedImageViewProperty->Iw, selectedImageViewProperty->Ih));
			cv::Mat roi_mat2 = cv::Mat(layerMat, cv::Rect(selectedImageViewProperty->Sw, selectedImageViewProperty->Sh,
				selectedImageViewProperty->Iw, selectedImageViewProperty->Ih));

			src.copyTo(roi_mat1);

			cv::Mat src2 = cv::Mat::ones(cv::Size(selectedImageViewProperty->Iw, selectedImageViewProperty->Ih), CV_8UC1)*(selectedImageViewProperty->Layer+1);//0ÇÕîwåi
			src2.copyTo(roi_mat2);
		}
		mainPictureBox->Image = mat2bmp(listMat);
		imshow("Layer", layerMat);

	}

			 // cv::Mat to Bitmap
	private: Bitmap^ mat2bmp(cv::Mat img)
	{
		cv::Mat out;
		if (img.channels() == 1){

			cv::Mat in[] = { img, img, img };//R G B
			out = cv::Mat(img.cols, img.rows, CV_8UC3);

			cv::merge(in, 3, out);
		}
		else{
			out = img.clone();
		}

		const int aligneCol = static_cast<int>(cv::alignSize(out.cols, 4));
		cv::Mat t_mat = cv::Mat(out.rows, aligneCol, out.type()); //row colÇÃèá

		cv::Mat roi_mat2 = cv::Mat(t_mat, cv::Rect(0, 0, out.cols, out.rows));//col rowÇÃèá

		out.copyTo(roi_mat2);
		Bitmap^ inter_bmp = gcnew Bitmap(roi_mat2.cols, roi_mat2.rows, roi_mat2.step,
			PixelFormat::Format24bppRgb, IntPtr(roi_mat2.ptr()));

		Bitmap^ bmp = gcnew Bitmap(inter_bmp);

		return bmp;
	}


	private: System::Void mainPictureBox_Click(System::Object^  sender, System::EventArgs^  e)
	{
		mainPanel->Focus();
	}

	private: System::Void removeButton_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if (imageDataGridView->RowCount < 1){
			return;
		}

		int index = imageDataGridView->CurrentRow->Index;
		listImageObsProperty->RemoveAt(index);
		listImageViewProperty->RemoveAt(index);

		printf("index %d\n", index);

		XmlSerializer^ xmlSerializer = gcnew XmlSerializer(BindingList<ImageObsProperty^>::typeid);

		FileStream^ fileStream = File::Open("default.xml", FileMode::Create);

		xmlSerializer->Serialize(fileStream, listImageObsProperty);

		fileStream->Close();

		load_image_count--;

		for (int i = 0; i < imageDataGridView->Rows->Count; i++){

			ImageViewProperty^ selectedImageViewProperty = listImageViewProperty[i];

			selectedImageViewProperty->Layer = i;
		}

		redrawView();
	}

	private: System::Void mainPanel_SizeChanged(System::Object^  sender, System::EventArgs^  e)
	{
		redrawView();
	}

	private: System::Void mainPictureBox_MouseDoubleClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	{

		if (e->Button != System::Windows::Forms::MouseButtons::Left){
			return;
		}

		Control^ control = dynamic_cast<Control^>(sender);

		System::Drawing::Point clickedPoint = System::Drawing::Point(e->X, e->Y);

		printf("[Mouce double click] %d,%d\n", e->X, e->Y);

		int layer = layerMat.at<Byte>(e->Y, e->X);

		printf("Clicked Layer %d\n", layer-1);//0ÇÕîwåi

		if (layer > 0){
			ImageViewProperty^ selectedImageViewProperty = listImageViewProperty[layer - 1];

			selectedImageViewProperty->Clicked = !selectedImageViewProperty->Clicked;
		}

		redrawView();

	}
	};
}
