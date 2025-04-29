#pragma once
#include<iostream>
using namespace std;
namespace img {
	enum TypeCountr{Sobelx,Sobely, Sobelxy, Prefitx, Prefity, Prefytxy};
	enum TypeCountrInversion { InNull,InL,InR};
	struct SizeImg {
		int WidthPixel;
		int HeightPixel;
	};
	struct RGB {
		int RGB[3];
		
		

	};
	struct imageData {
		int OffsetData;
		SizeImg size;
	
		int BytesRast;
		RGB** rgb=0;
		unsigned char** MonochromDat = 0;
	};

	
class Image
{
private:
	imageData data;
	double funP(double x, double x1, double x2, double y1, double y2);
public:
	Image(const char str[]);
	void ZoomImage(int xPos, int yPos);
	void ScaleImage(int cov);
	void ScaleImage(double pixelX, double pixelY);
	void WriteImage(const char str[]);
	void BilinenInterpolation(double zoom);
	void GreyFilter();
	void BillatiraelFilter(double gs, double gr,int val);
	void BilinenInterpolation(double pixelX, double pixelY);
	void contourBlack(TypeCountr type, TypeCountrInversion typeIn);
	void contourRGB(TypeCountr type, TypeCountrInversion typeIn);
	void contourRGBD(TypeCountr type, TypeCountrInversion typeIn);

	void Sharpness(double k);
	void SmoothFilt();
	~Image();

};

}

