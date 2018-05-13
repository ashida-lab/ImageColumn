#pragma once

#include "yaCommon.h"

using namespace System;

public ref class ImageObsProperty
{

private:
	//�t�@�C�����
	System::String^ rasterFilePath;
	System::String^ rasterFileName;

	//�摜�̊�{���
	int width = -1;//cols x
	int height = -1;//rows y
	int dataType = 0;//Opencv�̃f�[�^�^
	int depth = -1;
	int channel = -1;

	//�ŏ��l�E�ő�l
	cli::array<double>^ adfMinMax = gcnew cli::array<double>(2);

	//�􉽏��
	cli::array<double>^ lat = gcnew cli::array<double>(5);//WGS84 TL TR BR BL C
	cli::array<double>^ lon = gcnew cli::array<double>(5);
	double dLat;
	double dLon;
	cli::array<double>^ adfGeoTransform = gcnew cli::array<double>(6);

public:
	ImageObsProperty();
	int readRasterFile(System::String^ fname);
	cv::Mat convertToMagnitude(cv::Mat input, int channel, int depth, float scale);
	cv::Mat convertToMagnitude_gpu(cv::Mat input, int channel, int depth, float scale);
	cv::Mat convertToMagnitude_gpu_f(cv::Mat input, int channel, int depth, float scale);
	void viewHistogram(cv::Mat input, float scale, float *new_max, float *new_min);

	//rasterFilePath�̃v���p�e�B
	property System::String^ RasterFilePath
	{
		System::String^ get()
		{
			return this->rasterFilePath;
		}

		void set(System::String^ rasterFilePath)
		{
			this->rasterFilePath = rasterFilePath;
		}
	}

	//rasterFileName�̃v���p�e�B
	property System::String^ RasterFileName
	{
		System::String^ get()
		{
			return this->rasterFileName;
		}

		void set(System::String^ rasterFileName)
		{
			this->rasterFileName = rasterFileName;
		}
	}

	//width�̃v���p�e�B
	property int Width
	{
		int get()
		{
			return this->width;
		}

		void set(int width)
		{
			this->width = width;
		}
	}

	//height�̃v���p�e�B
	property int Height
	{
		int get()
		{
			return this->height;
		}

		void set(int height)
		{
			this->height = height;
		}
	}

	//dataType�̃v���p�e�B
	property int DataType
	{
		int get()
		{
			return this->dataType;
		}

		void set(int dataType)
		{
			this->dataType = dataType;
		}
	}

	//depth�̃v���p�e�B
	property int Depth
	{
		int get()
		{
			return this->depth;
		}

		void set(int depth)
		{
			this->depth = depth;
		}
	}

	//channel�̃v���p�e�B
	property int Channel
	{
		int get()
		{
			return this->channel;
		}

		void set(int channel)
		{
			this->channel = channel;
		}
	}

	//adfMinMax�̃v���p�e�B
	property cli::array<double>^ AdfMinMax
	{
		cli::array<double>^ get()
		{
			return this->adfMinMax;
		}

		void set(cli::array<double>^ adfMinMax)
		{
			this->adfMinMax = adfMinMax;
		}
	}

	//lat�̃v���p�e�B
	property cli::array<double>^ Lat
	{
		cli::array<double>^ get()
		{
			return this->lat;
		}

		void set(cli::array<double>^ lat)
		{
			this->lat = lat;
		}
	}

	//lon�̃v���p�e�B
	property cli::array<double>^ Lon
	{
		cli::array<double>^ get()
		{
			return this->lon;
		}

		void set(cli::array<double>^ lon)
		{
			this->lon = lon;
		}
	}

	//dLat�̃v���p�e�B
	property double DLat
	{
		double get()
		{
			return this->dLat;
		}

		void set(double dLat)
		{
			this->dLat = dLat;
		}
	}

	//dLon�̃v���p�e�B
	property double DLon
	{
		double get()
		{
			return this->dLon;
		}

		void set(double dLon)
		{
			this->dLon = dLon;
		}
	}

	//adfGeoTransform�̃v���p�e�B
	property cli::array<double>^ AdfGeoTransform
	{
		cli::array<double>^ get()
		{
			return this->adfGeoTransform;
		}

		void set(cli::array<double>^ adfGeoTransform)
		{
			this->adfGeoTransform = adfGeoTransform;
		}
	}

	//�f�[�^�̃Z�b�g
	void setData(
		System::String^ rasterFilePath,
		System::String^ rasterFileName,
		int width,
		int height,
		int dataType,
		int depth,
		int channel,
		cli::array<double>^ adfMinMax,
		cli::array<double>^ lat,
		cli::array<double>^ lon,
		double dLat,
		double dLon,
		cli::array<double>^ adfGeoTransform);

};

