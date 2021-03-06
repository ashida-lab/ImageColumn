#include "ImageObsProperty.h"
#include "yaCommon.h"
#include "yaString.h"

using namespace cv;
using namespace std;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::Drawing::Imaging;
using namespace System::Collections::Generic;
using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;

ImageObsProperty::ImageObsProperty()
{
}

//データのセット
void ImageObsProperty::setData(
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
	cli::array<double>^ adfGeoTransform)
{
	this->rasterFilePath = rasterFilePath;
	this->rasterFileName = rasterFileName;
	this->width = width;
	this->height = height;
	this->dataType = dataType;
	this->depth = depth;
	this->channel = channel;
	this->adfMinMax = adfMinMax;
	this->lat = lat;
	this->lon = lon;
	this->dLat = dLat;
	this->dLon = dLon;
	this->adfGeoTransform = adfGeoTransform;
}

int ImageObsProperty::readRasterFile(System::String^ fname)
{
	GDALDataset *paDataset;
	cv::Mat src_img;
	cv::Mat dst_img;

	GDALAllRegister();

	paDataset = (GDALDataset*)GDALOpen(yaString::systemStringToChar(fname), GA_ReadOnly);

	//バンド情報→OpenCV
	GDALRasterBand  *paBand;
	int             nBlockXSize, nBlockYSize;
	int             bGotMin, bGotMax;

	paBand = paDataset->GetRasterBand(1);
	paBand->GetBlockSize(&nBlockXSize, &nBlockYSize);
	printf("Block=%dx%d Type=%s, ColorInterp=%s\n", nBlockXSize, nBlockYSize,
		GDALGetDataTypeName(paBand->GetRasterDataType()),
		GDALGetColorInterpretationName(paBand->GetColorInterpretation()));

	adfMinMax[0] = paBand->GetMinimum(&bGotMin);
	adfMinMax[1] = paBand->GetMaximum(&bGotMax);
	if (!(bGotMin && bGotMax)){
		pin_ptr<double>pp_adfMinMax = &adfMinMax[0];

		GDALComputeRasterMinMax((GDALRasterBandH)paBand, TRUE, pp_adfMinMax);
		printf("Min=%.3fd, Max=%.3f\n", adfMinMax[0], adfMinMax[1]);
	}
	if (paBand->GetOverviewCount() > 0){
		printf("Band has %d overviews.\n", paBand->GetOverviewCount());
	}
	if (paBand->GetColorTable() != NULL){
		printf("Band has a color table with %d entries.\n", paBand->GetColorTable()->GetColorEntryCount());
	}

	width = paDataset->GetRasterXSize();
	height = paDataset->GetRasterYSize();

	//幾何情報
	pin_ptr<double>pp_adfGeoTransform = &adfGeoTransform[0];
	if (paDataset->GetGeoTransform(pp_adfGeoTransform) == CE_None ||
		GDALGCPsToGeoTransform(paDataset->GetGCPCount(), paDataset->GetGCPs(), pp_adfGeoTransform, TRUE)){
		printf("Origin is (%.6f,%.6f)\n",
			adfGeoTransform[0], adfGeoTransform[3]);

		printf("Pixel size is (%.6f,%.6f)\n",
			adfGeoTransform[1], adfGeoTransform[5]);

		/*
		y         tl ---tr           |       (0,0)     (w,0)
		^          |    |            |
		|-> +x    bl----br           |
		--------------------equator  |       (0,h)     (w,h)
		*/

		//tl
		lat[0] = adfGeoTransform[0] + 0 * adfGeoTransform[1] + 0 * adfGeoTransform[2];
		lon[0] = adfGeoTransform[3] + 0 * adfGeoTransform[4] + 0 * adfGeoTransform[5];

		//tr
		lat[1] = adfGeoTransform[0] + (width - 1) * adfGeoTransform[1] + 0 * adfGeoTransform[2];
		lon[1] = adfGeoTransform[3] + (width - 1) * adfGeoTransform[4] + 0 * adfGeoTransform[5];

		//br
		lat[2] = adfGeoTransform[0] + (width - 1) * adfGeoTransform[1] + (height - 1) * adfGeoTransform[2];
		lon[2] = adfGeoTransform[3] + (width - 1) * adfGeoTransform[4] + (height - 1) * adfGeoTransform[5];

		//bl
		lat[3] = adfGeoTransform[0] + 0 * adfGeoTransform[1] + (height - 1) * adfGeoTransform[2];
		lon[3] = adfGeoTransform[3] + 0 * adfGeoTransform[4] + (height - 1) * adfGeoTransform[5];

		//c
		lat[4] = adfGeoTransform[0] + (width / 2 - 1) * adfGeoTransform[1] + (height / 2 - 1) * adfGeoTransform[2];
		lon[4] = adfGeoTransform[3] + (width / 2 - 1) * adfGeoTransform[4] + (height / 2 - 1) * adfGeoTransform[5];

		char* Wkt = (char *)paDataset->GetProjectionRef();
		printf("%s\n", Wkt);
		OGRSpatialReference oSourceSRS, oTargetSRS;
		OGRCoordinateTransformation *poCT;
		double                  x, y;

		oSourceSRS.importFromWkt(&Wkt);
		//importFromEPSG(32653);
		oTargetSRS.importFromEPSG(4326);

		for (int i = 0; i < 5; i++){
			poCT = OGRCreateCoordinateTransformation(&oSourceSRS,
				&oTargetSRS);
			x = lat[i];
			y = lon[i];

			if (poCT == NULL || !poCT->Transform(1, &x, &y))
				printf("Transformation failed.\n");
			else
				printf("(%f,%f) -> (%f,%f)\n",
				lat[i],
				lon[i],
				x, y);
			lat[i] = y;
			lon[i] = x;
		}

		int nGCP = 5;

		GDAL_GCP *pasGCPs = (GDAL_GCP*)CPLMalloc(nGCP*sizeof(GDAL_GCP));
		GDALInitGCPs(nGCP, pasGCPs);

		pasGCPs[0].dfGCPPixel = 0;
		pasGCPs[0].dfGCPLine = 0;
		pasGCPs[0].dfGCPX = lon[0];
		pasGCPs[0].dfGCPY = lat[0];

		pasGCPs[1].dfGCPPixel = (width - 1);
		pasGCPs[1].dfGCPLine = 0;
		pasGCPs[1].dfGCPX = lon[1];
		pasGCPs[1].dfGCPY = lat[1];

		pasGCPs[2].dfGCPPixel = (width - 1);
		pasGCPs[2].dfGCPLine = (height - 1);
		pasGCPs[2].dfGCPX = lon[2];
		pasGCPs[2].dfGCPY = lat[2];

		pasGCPs[3].dfGCPPixel = 0;
		pasGCPs[3].dfGCPLine = (height - 1);
		pasGCPs[3].dfGCPX = lon[3];
		pasGCPs[3].dfGCPY = lat[3];

		pasGCPs[4].dfGCPPixel = (width / 2 - 1);
		pasGCPs[4].dfGCPLine = (height / 2 - 1);
		pasGCPs[4].dfGCPX = lon[4];
		pasGCPs[4].dfGCPY = lat[4];

		GDALGCPsToGeoTransform(nGCP, pasGCPs, pp_adfGeoTransform, TRUE);

		printf("projection\n (%f,%f)\n (%f,%f)\n (%f,%f)\n (%f,%f)\n (%f,%f)\n", lat[0], lon[0], lat[1], lon[1], lat[2], lon[2], lat[3], lon[3], lat[4], lon[4]);

		//tl
		lat[0] = adfGeoTransform[0] + 0 * adfGeoTransform[1] + 0 * adfGeoTransform[2];
		lon[0] = adfGeoTransform[3] + 0 * adfGeoTransform[4] + 0 * adfGeoTransform[5];

		//tr
		lat[1] = adfGeoTransform[0] + (width - 1) * adfGeoTransform[1] + 0 * adfGeoTransform[2];
		lon[1] = adfGeoTransform[3] + (width - 1) * adfGeoTransform[4] + 0 * adfGeoTransform[5];

		//br
		lat[2] = adfGeoTransform[0] + (width - 1) * adfGeoTransform[1] + (height - 1) * adfGeoTransform[2];
		lon[2] = adfGeoTransform[3] + (width - 1) * adfGeoTransform[4] + (height - 1) * adfGeoTransform[5];

		//bl
		lat[3] = adfGeoTransform[0] + 0 * adfGeoTransform[1] + (height - 1) * adfGeoTransform[2];
		lon[3] = adfGeoTransform[3] + 0 * adfGeoTransform[4] + (height - 1) * adfGeoTransform[5];

		//c
		lat[4] = adfGeoTransform[0] + (width / 2 - 1) * adfGeoTransform[1] + (height / 2 - 1) * adfGeoTransform[2];
		lon[4] = adfGeoTransform[3] + (width / 2 - 1) * adfGeoTransform[4] + (height / 2 - 1) * adfGeoTransform[5];

		printf("projection\n (%f,%f)\n (%f,%f)\n (%f,%f)\n (%f,%f)\n (%f,%f)\n", lat[0], lon[0], lat[1], lon[1], lat[2], lon[2], lat[3], lon[3], lat[4], lon[4]);
	}
	else{
		printf("No geo info\n");
	}

	// フルパスを分解
	char szDir[_MAX_DIR];
	char szDrive[_MAX_DRIVE];
	char szFName[_MAX_FNAME];
	char szExt[_MAX_EXT];

	_splitpath(yaString::systemStringToChar(fname), szDrive, szDir, szFName, szExt);

	// フォルダ、ファイル名取得
	rasterFilePath = System::String::Concat(yaString::charToSystemString(szDrive), yaString::charToSystemString(szDir));
	rasterFileName = System::String::Concat(yaString::charToSystemString(szFName), yaString::charToSystemString(szExt));

	printf("%s\n", rasterFileName);

	switch (paBand->GetRasterDataType()){
	case GDT_Byte:
		dataType = CV_8U;
		depth = 8;
		channel = 1;
		break;
	case GDT_UInt16:
		dataType = CV_16U;
		depth = 16;
		channel = 1;
		break;
	case GDT_Int16:
		dataType = CV_16S;
		depth = 16;
		channel = 1;
		break;
	case GDT_UInt32://OpenCVに該当するデータタイプなし
		dataType = 0;
		depth = 32;
		channel = 1;
		break;
	case GDT_Int32:
		dataType = CV_32S;
		depth = 32;
		channel = 1;
		break;
	case GDT_Float32:
		dataType = CV_32F;
		depth = 32;
		channel = 1;
		break;
	case GDT_Float64:
		dataType = CV_64F;
		depth = 64;
		channel = 1;
		break;
	case GDT_CInt16:
		dataType = CV_16SC2;
		depth = 16;
		channel = 2;
		break;
	case GDT_CInt32:
		dataType = CV_32SC2;
		depth = 32;
		channel = 2;
		break;
	case GDT_CFloat32:
		dataType = CV_32FC2;
		depth = 32;
		channel = 2;
		break;
	case GDT_CFloat64:
		dataType = CV_64FC2;
		depth = 64;
		channel = 2;
		break;
	}

	cv::Rect roi(0, 0, width, height);

	cv::Mat orgMat = cv::Mat(roi.size(), dataType);

	paBand->RasterIO(GF_Read, roi.x, roi.y, roi.width, roi.height, orgMat.data,
		roi.width, roi.height, paBand->GetRasterDataType(), 0, 0);

	//GPUのメモリに載らないときはCPUで計算
	if ((double)width*(double)height*(double)depth*(double)channel / 8. > 2e9){
		printf("[Loading with CPU]\n");
		orgMat = convertToMagnitude(orgMat, channel, depth, MAX(abs(adfMinMax[0]), abs(adfMinMax[1])));
	}
	else{
		printf("[Loading with GPU]\n");
		orgMat = convertToMagnitude_gpu(orgMat, channel, depth, MAX(abs(adfMinMax[0]), abs(adfMinMax[1])));
	}

	cv::Mat browseMat;
	cv::resize(orgMat, browseMat, cv::Size(4 * floor(orgMat.cols*0.01), 4 * floor(orgMat.rows*0.01)));
	cv::Mat in[] = { browseMat, browseMat, browseMat };
	cv::merge(in, 3, browseMat);//3バンドの色合成

	_mkdir("./browse");
	cv::imwrite(yaString::systemStringToChar(System::String::Concat("./browse/", rasterFileName, ".jpg")), browseMat);

	return 1;
}
cv::Mat ImageObsProperty::convertToMagnitude(cv::Mat input, int channel, int depth, float scale)
{
	cv::Mat output;
	cv::Mat src_img, dst_img;
	float new_max, new_min;

	if (channel == 2){
		//実部と虚部に分離
		vector<Mat> mv;
		split(input, mv);

		if (depth == 64){
			//倍精度浮動小数点に変換
			mv[0].convertTo(src_img, CV_64F);
			mv[1].convertTo(dst_img, CV_64F);
		}
		else{
			//単精度浮動小数点に変換
			mv[0].convertTo(src_img, CV_32F);
			mv[1].convertTo(dst_img, CV_32F);
		}

		//絶対値
		magnitude(src_img, dst_img, src_img);

		//float new_scale = viewHistogram(src_img, scale);
		viewHistogram(src_img, scale, &new_max, &new_min);

		//8ビット整数に変換
		dst_img = 255. / (new_max - new_min) * (src_img - new_min);
		dst_img.convertTo(output, CV_8UC1);

		//equalizeHist(output, output);
	}
	else if (channel == 3){
		dst_img = 255. / scale * input;
		dst_img.convertTo(output, CV_8UC3);
	}
	else{

		//float new_scale = viewHistogram(input, scale);
		viewHistogram(input, scale, &new_max, &new_min);

		//dst_img = 255. / new_scale * input;
		dst_img = 255. / (new_max - new_min) * (input - new_min);
		dst_img.convertTo(output, CV_8UC1);
	}

	return output;
}

cv::Mat ImageObsProperty::convertToMagnitude_gpu(cv::Mat input, int channel, int depth, float scale)
{
	cv::cuda::GpuMat input_gpu;
	cv::Mat output;
	cv::cuda::GpuMat output_gpu;
	cv::cuda::GpuMat src_img, dst_img;
	input_gpu.upload(input);
	float new_max, new_min;

	if (channel == 2){
		//実部と虚部に分離
		vector<cv::cuda::GpuMat> mv;
		cv::cuda::split(input_gpu, mv);
		input_gpu.release();

		if (depth == 64){
			//倍精度浮動小数点に変換
			mv[0].convertTo(src_img, CV_64F);
			mv[0].release();
			mv[1].convertTo(dst_img, CV_64F);
			mv[1].release();
		}
		else{
			//単精度浮動小数点に変換
			mv[0].convertTo(src_img, CV_32F);
			mv[0].release();
			mv[1].convertTo(dst_img, CV_32F);
			mv[1].release();
		}

		//絶対値
		cv::cuda::magnitude(src_img, dst_img, src_img);

		src_img.download(output);
		viewHistogram(output, scale, &new_max, &new_min);


		//cv::cuda::equalizeHist(src_img, src_img);

		//8ビット整数に変換
		cv::cuda::add(src_img, -new_min, src_img);
		cv::cuda::multiply(255. / (new_max - new_min), src_img, dst_img);
		//dst_img.convertTo(dst_img, dst_img.type(), max_val ? 200. / max_val : 0., 0);
		src_img.release();
		dst_img.convertTo(output_gpu, CV_8UC1);

		//cv::cuda::equalizeHist(output_gpu, output_gpu);

		dst_img.release();
	}
	else if (channel == 3){
		cv::cuda::multiply(255. / scale, input_gpu, dst_img);
		input_gpu.release();
		dst_img.convertTo(output_gpu, CV_8UC3);
		dst_img.release();
	}
	else{
		input_gpu.download(output);
		output.convertTo(output, CV_32F);
		viewHistogram(output, scale, &new_max, &new_min);

		cv::cuda::add(input_gpu, -new_min, input_gpu);
		cv::cuda::multiply(255. / (new_max - new_min), input_gpu, dst_img);
		input_gpu.release();
		dst_img.convertTo(output_gpu, CV_8UC1);
		dst_img.release();
	}

	output_gpu.download(output);
	output_gpu.release();

	return output;
}

cv::Mat ImageObsProperty::convertToMagnitude_gpu_f(cv::Mat input, int channel, int depth, float scale)
{
	cv::cuda::GpuMat input_gpu;
	cv::Mat output;
	cv::cuda::GpuMat output_gpu;
	cv::cuda::GpuMat src_img, dst_img;
	input_gpu.upload(input);
	double minVal, maxVal;
	float new_max, new_min;

	if (channel == 2){
		//実部と虚部に分離
		vector<cv::cuda::GpuMat> mv;
		cv::cuda::split(input_gpu, mv);
		input_gpu.release();

		if (depth == 64){
			//倍精度浮動小数点に変換
			mv[0].convertTo(src_img, CV_64F);
			mv[0].release();
			mv[1].convertTo(dst_img, CV_64F);
			mv[1].release();
		}
		else{
			//単精度浮動小数点に変換
			mv[0].convertTo(src_img, CV_32F);
			mv[0].release();
			mv[1].convertTo(dst_img, CV_32F);
			mv[1].release();
		}

		//絶対値
		cv::cuda::magnitude(src_img, dst_img, src_img);

		src_img.download(output);

		minMaxLoc(output, &minVal, &maxVal, NULL, NULL, Mat());

		printf("%f %f\n", minVal, maxVal);

		viewHistogram(output, maxVal, &new_max, &new_min);

		cv::cuda::add(src_img, -new_min, src_img);
		cv::cuda::multiply(255. / (new_max - new_min), src_img, output_gpu);

		src_img.release();
		dst_img.release();
	}
	else if (channel == 3){
		cv::cuda::multiply(255. / scale, input_gpu, output_gpu);
		input_gpu.release();
		dst_img.release();
	}
	else{
		input_gpu.download(output);
		output.convertTo(output, CV_32F);
		viewHistogram(output, scale, &new_max, &new_min);

		cv::cuda::add(input_gpu, -new_min, input_gpu);
		cv::cuda::multiply(255. / (new_max - new_min), input_gpu, output_gpu);
		input_gpu.release();
		dst_img.release();
	}

	output_gpu.download(output);
	output_gpu.release();

	return output;
}

void ImageObsProperty::viewHistogram(cv::Mat input, float scale, float *new_max, float *new_min)
{

	// (2)allocate Mat to draw a histogram image
	const int ch_width = 1028;
	const int sch = input.channels();
	Mat hist_img(cv::Size(ch_width * sch, 200), CV_8UC3, Scalar::all(255));

	vector<MatND> hist(3);
	const int hist_size = 1024;
	const int hdims[] = { hist_size };
	const float hranges[] = { 0, scale };
	const float* ranges[] = { hranges };
	double max_val = .0;

	if (sch == 1) {
		// (3a)if the source image has single-channel, calculate its histogram
		calcHist(&input, 1, 0, Mat(), hist[0], 1, hdims, ranges, true, false);
		minMaxLoc(hist[0], 0, &max_val);
	}
	else {
		// (3b)if the souce image has multi-channel, calculate histogram of each plane
		for (int i = 0; i < sch; ++i) {
			calcHist(&input, 1, &i, Mat(), hist[i], 1, hdims, ranges, true, false);
			double tmp_val;
			minMaxLoc(hist[i], 0, &tmp_val);
			max_val = max_val < tmp_val ? tmp_val : max_val;
		}
	}


	int j;
	float hist_sum[3] = { 0, 0, 0 };
	for (int i = 0; i < sch; i++) {
		if (sch == 1){
			for (j = 0; j < hist_size; j++){
				hist_sum[0] += hist[0].at<float>(j);

				if (hist_sum[0] > 0.95*(float)input.cols*(float)input.rows){
					break;
				}
			}
		}
	}

	*new_max = j*scale / 1024.;
	printf("scale=%f/%f, %f %d %d\n", *new_max, scale, hist_sum[0], input.cols, input.rows);

	hist_sum[0] = 0;
	for (int i = 0; i < sch; i++) {
		if (sch == 1){
			for (j = 0; j < hist_size; j++){
				hist_sum[0] += hist[0].at<float>(j);

				if (hist_sum[0] > 0.05*(float)input.cols*(float)input.rows){
					break;
				}
			}
		}
	}

	*new_min = j*scale / 1024.;
	printf("scale=%f/%f, %f %d %d\n", *new_min, scale, hist_sum[0], input.cols, input.rows);

	// (4)scale and draw the histogram(s)
	Scalar color = Scalar::all(100);
	for (int i = 0; i<sch; i++) {
		if (sch == 3){
			color = Scalar((0xaa << i * 8) & 0x0000ff, (0xaa << i * 8) & 0x00ff00, (0xaa << i * 8) & 0xff0000, 0);
		}
		hist[i].convertTo(hist[i], hist[i].type(), max_val ? 200. / max_val : 0., 0);
		for (int j = 0; j<hist_size; ++j) {
			int bin_w = saturate_cast<int>((double)ch_width / hist_size);
			rectangle(hist_img,
				cv::Point(j*bin_w + (i*ch_width), hist_img.rows),
				cv::Point((j + 1)*bin_w + (i*ch_width), hist_img.rows - saturate_cast<int>(hist[i].at<float>(j))),
				color, -1);
		}
	}

	// (5)show the histogram iamge, and quit when any key pressed
	namedWindow("Histogram", CV_WINDOW_AUTOSIZE);
	imshow("Histogram", hist_img);

	//return new_scale;
}