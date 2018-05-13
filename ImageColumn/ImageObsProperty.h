#pragma once

#include "yaCommon.h"

using namespace System;

public ref class ImageObsProperty
{

private:
	//ファイル情報
	System::String^ rasterFilePath;
	System::String^ rasterFileName;

	//画像の基本情報
	int width = -1;//cols x
	int height = -1;//rows y
	int dataType = 0;//Opencvのデータ型
	int depth = -1;
	int channel = -1;

	//最小値・最大値
	cli::array<double>^ adfMinMax = gcnew cli::array<double>(2);

	//幾何情報
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

	//rasterFilePathのプロパティ
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

	//rasterFileNameのプロパティ
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

	//widthのプロパティ
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

	//heightのプロパティ
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

	//dataTypeのプロパティ
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

	//depthのプロパティ
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

	//channelのプロパティ
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

	//adfMinMaxのプロパティ
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

	//latのプロパティ
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

	//lonのプロパティ
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

	//dLatのプロパティ
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

	//dLonのプロパティ
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

	//adfGeoTransformのプロパティ
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

	//データのセット
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

