#include "ImageViewProperty.h"
#include "yaCommon.h"
#include "yaString.h"

ImageViewProperty::ImageViewProperty()
{
}

//データのセット
void ImageViewProperty::setData(ImageObsProperty^ newImageObsProperty,int load_image_count)
{
	cli::array<double>^ lat=newImageObsProperty->Lat;
	cli::array<double>^ lon= newImageObsProperty->Lon;
	
	this->iw = 200;
	this->ih = (int)((double)newImageObsProperty->Height/(double)newImageObsProperty->Width*200.);
	this->layer = load_image_count;
}

