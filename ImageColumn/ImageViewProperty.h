#pragma once

#include "yaCommon.h"
#include "ImageObsProperty.h"

using namespace System;

public ref class ImageViewProperty
{

private:

	int iw=0;
	int ih=0;
	int layer=0;
	int sw=0;
	int sh=0;
	bool clicked = FALSE;

public:
	ImageViewProperty();
	
	IplImage* browseImg;

	//iw�̃v���p�e�B
	property int Iw
	{
		int get()
		{
			return this->iw;
		}

		void set(int iw)
		{
			this->iw = iw;
		}
	}

	//ih�̃v���p�e�B
	property int Ih
	{
		int get()
		{
			return this->ih;
		}

		void set(int ih)
		{
			this->ih = ih;
		}
	}

	//layer�̃v���p�e�B
	property int Layer
	{
		int get()
		{
			return this->layer;
		}

		void set(int layer)
		{
			this->layer = layer;
		}
	}

	//sw�̃v���p�e�B
	property int Sw
	{
		int get()
		{
			return this->sw;
		}

		void set(int sw)
		{
			this->sw = sw;
		}
	}

	//sh�̃v���p�e�B
	property int Sh
	{
		int get()
		{
			return this->sh;
		}

		void set(int sh)
		{
			this->sh = sh;
		}
	}

	//clicked�̃v���p�e�B
	property bool Clicked
	{
		bool get()
		{
			return this->clicked;
		}

		void set(bool clicked)
		{
			this->clicked =clicked;
		}
	}
	
	//�f�[�^�̃Z�b�g
	void setData(ImageObsProperty^ newImageObsProperty,int load_image_count);

};

