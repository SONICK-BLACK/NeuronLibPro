#include "Image.h"
using namespace img;
Image::Image(const char str[]) {
	FILE* file;
	fopen_s(&file, str, "rb");
	fseek(file, 10, SEEK_SET);
	fread(&data.OffsetData, 4, 1, file);
	fseek(file, 18, SEEK_SET);
	fread(&data.size.WidthPixel, 4, 1, file);
	fseek(file, 22, SEEK_SET);
	fread(&data.size.HeightPixel, 4, 1, file);
	fseek(file, 28, SEEK_SET);
	short Bits;
	fread(&Bits, 2, 1, file);
	int Bytes = Bits / 8;
	if (Bytes == 0) {
		data.BytesRast = 1;
		data.rgb = NULL;
	
		if (data.size.WidthPixel % 8) {

			data.MonochromDat = new unsigned char* [data.size.HeightPixel];
			for (int j = 0; j < data.size.HeightPixel; j++) {
				data.MonochromDat[j] = new unsigned char[data.size.WidthPixel / 8 + 1];
			}
			bool F = false;
			if ((data.size.WidthPixel / 8 + 1) % 4) {
				F = true;
			}
			for (int i = 0; i < data.size.HeightPixel; i++) {
				if (F) {
					fseek(file, data.OffsetData + ((data.size.WidthPixel / 8 + 1)+ 4 - ((data.size.WidthPixel / 8 + 1) % 4)) * i, SEEK_SET);
				}
				else {
					fseek(file, data.OffsetData + (data.size.WidthPixel / 8 + 1) * i, SEEK_SET);
				}
			
				
					fread(data.MonochromDat[i], 1, data.size.WidthPixel / 8 + 1, file);



			
			}
		}

		else {
			data.MonochromDat = new unsigned char*[data.size.HeightPixel];
			for (int j = 0; j < data.size.HeightPixel; j++) {
				data.MonochromDat[j] = new unsigned char [data.size.WidthPixel / 8];
			}
			bool F = false;
			if ((data.size.WidthPixel / 8) % 4) {
				F = true;
			}
			for (int i = 0; i < data.size.HeightPixel; i++) {
				if (F) {
					
					fseek(file, data.OffsetData + ((data.size.WidthPixel/8)+2) * i, SEEK_SET);
				}
				else {
					fseek(file, data.OffsetData + (data.size.WidthPixel / 8) * i, SEEK_SET);
				}
				
			
					fread(data.MonochromDat[i], 1, data.size.WidthPixel / 8, file);



				
			}
		}
		
		


	}
	else if (Bytes == 3) {
		data.BytesRast = 3;
		data.rgb = new RGB* [data.size.HeightPixel];
		for (int i = 0; i < data.size.HeightPixel; i++) {
			data.rgb[i] = new RGB[data.size.WidthPixel];
		}
		bool F = false;
		if ((data.size.WidthPixel) % 4) {
			F = true;
		}
		for (int i = 0; i < data.size.HeightPixel; i++) {
			if (F) {
			
				fseek(file, data.OffsetData + ((data.size.WidthPixel * data.BytesRast)+ (4 - (data.size.WidthPixel * data.BytesRast) % 4)) * i, SEEK_SET);
			
			}
			else {
				fseek(file, data.OffsetData + (data.size.WidthPixel * data.BytesRast) * i, SEEK_SET);
			}
			
			for (int j = 0; j < data.size.WidthPixel; j++) {
				for (int l = 0; l < 3; l++) {
					unsigned char val;
					fread(&val, 1, 1, file);
					data.rgb[i][j].RGB[l] = (int)val;
				}
				

			}

		}




	}
	else {
		cout << "error";
	}
	fclose(file);


}
void Image::WriteImage(const char str[]) {
	FILE* file;
	fopen_s(&file, str, "wb");
	char BM[2] = { 'B','M' };
	fwrite(BM, 1, 2, file);
	if (data.BytesRast == 3) {

		int sizeFile = 14 + 40 + data.size.WidthPixel * data.BytesRast * data.size.HeightPixel;
		fwrite(&sizeFile, 4, 1, file);
		int s = 0x0000;
		fwrite(&s, 4, 1, file);
		int dataOffset = 14 + 40;
		fwrite(&dataOffset, 4, 1, file);
		int Info = 40;
		fwrite(&Info, 4, 1, file);

		fwrite(&data.size.WidthPixel, 4, 1, file);
		fwrite(&data.size.HeightPixel, 4, 1, file);
		short plane = 1;
		fwrite(&plane, 2, 1, file);
		short BitsPixel = 24;
		fwrite(&BitsPixel, 2, 1, file);
		fwrite(&s, 4, 1, file);
		int pad1 = (data.size.WidthPixel / 8);
		int sizeImg = pad1 * (data.size.HeightPixel);
		fwrite(&sizeImg, 4, 1, file);
		int resolutionX = 11800; 
		int resolutionY = 11800;
		fwrite(&resolutionX, 4, 1, file);
		fwrite(&resolutionY, 4, 1, file);
		int usedColor = 0;
		fwrite(&usedColor, 4, 1, file);
		int ValColor = 0;
		fwrite(&ValColor, 4, 1, file);
		char h = (char)0;
		bool F = false;
		if ((data.size.WidthPixel) % 4) {
			F = true;
		}
		if (F) {
			for (int i = 0; i < data.size.HeightPixel; i++) {
				for (int j = 0; j < data.size.WidthPixel; j++) {
					for (int l = 0; l < 3; l++) {
						unsigned char val=data.rgb[i][j].RGB[l];
						fwrite(&val, 1, 1, file);
						
					}

				}
				
				for (int l = 0; l < 4 - (data.size.WidthPixel * data.BytesRast) % 4; l++) {
					fwrite(&h, 1, 1, file);
				}
			}
		}
		else {
			for (int i = 0; i < data.size.HeightPixel; i++) {
				for (int j = 0; j < data.size.WidthPixel; j++) {
					for (int l = 0; l < 3; l++) {
						unsigned char val = data.rgb[i][j].RGB[l];
						fwrite(&val, 1, 1, file);

					}

				}
				
			}

		}
		

	}
	else if (data.BytesRast == 1) {
		if (data.size.WidthPixel % 8) {
			int v = 0;
			bool F = false;
			if ((data.size.WidthPixel / 8 + 1) % 4) {
				F = true;
				v = 4 - ((data.size.WidthPixel / 8 + 1) % 4);
			}
			int sizeFile = 14 + 40 + (data.size.WidthPixel / 8 + 1+v) * data.size.HeightPixel;
			fwrite(&sizeFile, 4, 1, file);
			int s = 0x0000;
			fwrite(&s, 4, 1, file);
			int dataOffset = 14 + 40;
			fwrite(&dataOffset, 4, 1, file);
			int Info = 40;
			fwrite(&Info, 4, 1, file);

			fwrite(&data.size.WidthPixel, 4, 1, file);
			fwrite(&data.size.HeightPixel, 4, 1, file);
			short plane = 1;
			fwrite(&plane, 2, 1, file);
			short BitsPixel = 1;
			fwrite(&BitsPixel, 2, 1, file);
			fwrite(&s, 4, 1, file);

			int pad1 = (data.size.WidthPixel / 8+1+v);
			int sizeImg = pad1 * (data.size.HeightPixel);
			fwrite(&sizeImg, 4, 1, file);
			int resolutionX = 11800;
			int resolutionY = 11800; 
			fwrite(&resolutionX, 4, 1, file);
			fwrite(&resolutionY, 4, 1, file);
			int usedColor = 2;
			fwrite(&usedColor, 4, 1, file);
			int ValColor = 0;
			fwrite(&ValColor, 4, 1, file);
			char Palyt[8];
			Palyt[0] = (char)255;
			Palyt[1] = (char)255;
			Palyt[2] = (char)255;
			Palyt[3] = (char)255;

			Palyt[4] = (char)0;
			Palyt[5] = (char)0;
			Palyt[6] = (char)0;
			Palyt[7] = (char)255;
			fwrite(&Palyt, 1, 8, file);
			char h = 0;

			
			if (F) {
				for (int i = 0; i < data.size.HeightPixel; i++) {
			
						fwrite(data.MonochromDat[i], 1, data.size.WidthPixel / 8 + 1, file);

		
					for (int g = 0; g < v; g++) {
						fwrite(&h, 1, 1, file);
					}


				}

			}
			else {
				for (int i = 0; i < data.size.HeightPixel; i++) {

						fwrite(data.MonochromDat[i], 1, data.size.WidthPixel / 8 + 1, file);

			


				}

			}
		}
		else {
			bool F = false;
			int sizeFile;
			int v = 0;
			if ((data.size.WidthPixel / 8) % 4) {
				F = true;
				v = (4 - ((data.size.WidthPixel / 8) % 4));
			}
			
			 sizeFile = 14 + 40 + (data.size.WidthPixel / 8 + v) * data.size.HeightPixel;

			
				
			
	
			fwrite(&sizeFile, 4, 1, file);
			int s = 0x0000;
			fwrite(&s, 4, 1, file);
			int dataOffset = 14 + 40;
			fwrite(&dataOffset, 4, 1, file);
			int Info = 40;
			fwrite(&Info, 4, 1, file);

			fwrite(&data.size.WidthPixel, 4, 1, file);
			fwrite(&data.size.HeightPixel, 4, 1, file);
			short plane = 1;
			fwrite(&plane, 2, 1, file);
			short BitsPixel = 1;
			fwrite(&BitsPixel, 2, 1, file);
			fwrite(&s, 4, 1, file);

			int pad1 = (data.size.WidthPixel / 8+v);
			int sizeImg = pad1 * (data.size.HeightPixel);
			fwrite(&sizeImg, 4, 1, file);
			int resolutionX = 11800; //300 dpi
			int resolutionY = 11800; //300 dpi
			fwrite(&resolutionX, 4, 1, file);
			fwrite(&resolutionY, 4, 1, file);
			int usedColor = 2;
			fwrite(&usedColor, 4, 1, file);
			int ValColor = 0;
			fwrite(&ValColor, 4, 1, file);
			char Palyt[8];
			Palyt[0] = (char)255;
			Palyt[1] = (char)255;
			Palyt[2] = (char)255;
			Palyt[3] = (char)255;

			Palyt[4] = (char)0;
			Palyt[5] = (char)0;
			Palyt[6] = (char)0;
			Palyt[7] = (char)255;
			fwrite(&Palyt, 1, 8, file);
			char h = 0;
		
			if (F) {
				for (int i = 0; i < data.size.HeightPixel; i++) {
					
						fwrite(data.MonochromDat[i], 1, data.size.WidthPixel / 8, file);

					
					for (int g = 0; g < v; g++) {
						fwrite(&h, 1, 1, file);
					}

					
				}

			}
			else {
				for (int i = 0; i < data.size.HeightPixel; i++) {
				
						fwrite(data.MonochromDat[i], 1, data.size.WidthPixel / 8, file);

					

					
				}

			}
		
		}
		
	}
	else {
		cout << "error";

	}

	fclose(file);
	
	

	

	

}
void Image::ZoomImage(int pixelX, int pixelY) {
	if (data.BytesRast != 3) {
		cout << "error";
		return;
	}
	
	if (data.rgb != 0) {

		RGB** NewImage = new RGB * [data.size.HeightPixel - pixelY];
		for (int i = 0; i < data.size.HeightPixel - pixelY; i++) {
			NewImage[i] = new RGB[data.size.WidthPixel - pixelX];
		}

		for (int b = 0; b < data.size.HeightPixel - pixelY; b++) {
			for (int h = 0; h < data.size.WidthPixel - pixelX; h++) {

				NewImage[b][h] = data.rgb[pixelY + b][pixelX + h];





			}
		}


		data.size.HeightPixel = data.size.HeightPixel - pixelY;
		data.size.WidthPixel = data.size.WidthPixel - pixelX;

		for (int i = 0; i < data.size.HeightPixel; i++) {
			delete[] data.rgb[i];
		}
		delete[] data.rgb;
		data.rgb = NewImage;
	}
	else {
		cout << "error";
	}
}
void Image::ScaleImage(int cov) {
	if (data.BytesRast != 3) {
		cout << "error";
		return;
	}
	
	if (data.rgb != 0) {
		RGB** dat;
		dat = new RGB * [data.size.HeightPixel / (4*cov)];
		for (int i = 0; i < data.size.HeightPixel / (4*cov); i++) {
			dat[i] = new RGB[data.size.WidthPixel / (4*cov)];
		}
	



		RGB val;
		

		for (int t = 0; t < 3; t++) {
			val.RGB[t] = 0;
		}



		for (int b = 0; b < data.size.HeightPixel / (4*cov); b++) {
			for (int h = 0; h < data.size.WidthPixel / (4*cov); h++) {
				for (int i = 0; i < 4*cov; i++) {
					for (int j = 0; j < 4*cov; j++) {
						for (int t = 0; t < 3; t++) {
							val.RGB[t] += data.rgb[i + 4*cov *b][j + 4*cov *h].RGB[t];

						}
					}

				}
				for (int t = 0; t < 3; t++) {
					
					dat[b][h].RGB[t] = (int)val.RGB[t] / (4*cov * 4*cov);

				}
				for (int t = 0; t < 3; t++) {
					val.RGB[t] = 0;
				}
			}
		}
		

		for (int i = 0; i < data.size.HeightPixel; i++) {
			delete[] data.rgb[i];
		}

		delete[] data.rgb;
		data.rgb = dat;
		data.size.HeightPixel = data.size.HeightPixel / (4*cov);
		data.size.WidthPixel = data.size.WidthPixel / (4*cov);

	}
	else {
		cout << "error";
	}
}
void Image::BilinenInterpolation(double zoom)  {
	if (data.BytesRast != 3) {
		cout << "error";
		return;
	}
	if (data.rgb != 0) {
		imageData dat;
		dat.rgb = new RGB * [data.size.HeightPixel * zoom + data.size.HeightPixel];
		for (int i = 0; i < data.size.HeightPixel * zoom + data.size.HeightPixel; i++) {
			dat.rgb[i] = new RGB[data.size.WidthPixel * zoom + data.size.WidthPixel];
		}
		for (int i = 0; i < data.size.HeightPixel * zoom + data.size.HeightPixel; i++) {
			for (int j = 0; j < data.size.WidthPixel * zoom + data.size.WidthPixel; j++) {
				for (int t = 0; t < 3; t++) {
					dat.rgb[i][j].RGB[t] = 0;
				}

			}
		}

		for (int i = 0; i < data.size.HeightPixel; i++) {
			for (int j = 0; j < data.size.WidthPixel; j++) {
				for (int t = 0; t < 3; t++) {
					dat.rgb[(int)(zoom * i + i)][(int)(zoom * j + j)].RGB[t] = data.rgb[i][j].RGB[t];
				}

			}

		}

		for (int i = 0; i < data.size.HeightPixel; i++) {

			for (int j = 0; j < data.size.WidthPixel - 1; j++) {
				for (int h = 1; h < zoom + 1; h++) {
					if ((zoom * j + j) + h > data.size.WidthPixel * zoom + data.size.WidthPixel) {
						break;
					}
					for (int t = 0; t < 3; t++) {

						dat.rgb[(int)(zoom * i + i)][(int)((zoom * j + j) + h)].RGB[t] = funP((zoom * j + j) + h, (zoom * j + j), zoom * (j + 1) + (j + 1), dat.rgb[(int)(zoom * i + i)][(int)(zoom * j + j)].RGB[t], dat.rgb[(int)(zoom * i + i)][(int)(zoom * (j + 1) + (j + 1))].RGB[t]);

					}
				}
			}


		}




		for (int i = 0; i < data.size.HeightPixel - 1; i++) {
			for (int y = 1; y < zoom + 1; y++) {
				if ((zoom * i + i) + y > data.size.HeightPixel * zoom + data.size.HeightPixel) {
					break;
				}
				for (int j = 0; j < data.size.WidthPixel * zoom + data.size.WidthPixel; j++) {

					for (int t = 0; t < 3; t++) {

						dat.rgb[(int)((zoom * i + i) + y)][j].RGB[t] = funP((zoom * i + i) + y, (zoom * i + i), zoom * (i + 1) + (i + 1), dat.rgb[(int)((zoom * i + i))][j].RGB[t], dat.rgb[(int)(zoom * (i + 1) + (i + 1))][j].RGB[t]);


					}

				}

			}
		}




		for (int i = 0; i < data.size.HeightPixel; i++) {
			delete[] data.rgb[i];
		}


		delete[] data.rgb;
		data.rgb = dat.rgb;
		data.size.HeightPixel = data.size.HeightPixel * zoom + data.size.HeightPixel;
		data.size.WidthPixel = data.size.WidthPixel * zoom + data.size.WidthPixel;
	}
	else {
		cout << "error";
	}

}
double Image::funP(double x, double x1, double x2, double y1, double y2) {
	return abs((x * y2 - x * y1 - x1 * y2 + x1 * y1 + y1 * x2 - y1 * x1) / (x2 - x1));
}
void Image::GreyFilter() {
	if (data.BytesRast != 3) {
		cout << "error";
		return;
	}
	if (data.rgb != 0) {
		for (int i = 0; i < data.size.HeightPixel; i++) {
			for (int j = 0; j < data.size.WidthPixel; j++) {
				for (int t = 0; t < 3; t++) {
					data.rgb[i][j].RGB[t] = data.rgb[i][j].RGB[0] * 0.2989 + data.rgb[i][j].RGB[1] * 0.5870 + data.rgb[i][j].RGB[2] * 0.1140;
				}
			}
		}
	}
	else {
		cout << "error";
	}
}
void Image::contourBlack(TypeCountr type, TypeCountrInversion typeIn) {
	if (data.BytesRast != 3) {
		cout << "error";
		return;
	}
	if (data.rgb != 0) {
		int maskX[3][3];
		RGB** valrgb;
		switch (type)
		{
		case Sobelx:
			
			 maskX[0][0]=-1;
			 maskX[0][1] = -2;
			 maskX[0][2] = -1;
			 maskX[1][0] = 0;
			 maskX[1][1] = 0;
			 maskX[1][2] = 0;
			 maskX[2][0] = 1;
			 maskX[2][1] = 2;
			 maskX[2][2] = 1;
			 break;
		case Sobely:
			maskX[0][0] = -1;
			maskX[0][1] = 0;
			maskX[0][2] = 1;
			maskX[1][0] = -2;
			maskX[1][1] = 0;
			maskX[1][2] = 2;
			maskX[2][0] = -1;
			maskX[2][1] = 0;
			maskX[2][2] = 1;
			break;
		case Sobelxy:
			maskX[0][0] = -1;
			maskX[0][1] = -2;
			maskX[0][2] = 1;
			maskX[1][0] = -2;
			maskX[1][1] = 0;
			maskX[1][2] = 2;
			maskX[2][0] = -1;
			maskX[2][1] = 2;
			maskX[2][2] = 1;
			break;
		case Prefitx:
			maskX[0][0] = -1;
			maskX[0][1] = -1;
			maskX[0][2] = -1;
			maskX[1][0] = 0;
			maskX[1][1] = 0;
			maskX[1][2] = 0;
			maskX[2][0] = 1;
			maskX[2][1] = 1;
			maskX[2][2] = 1;
			break;
		case Prefity:
			maskX[0][0] = -1;
			maskX[0][1] = 0;
			maskX[0][2] = 1;
			maskX[1][0] = -1;
			maskX[1][1] = 0;
			maskX[1][2] = 1;
			maskX[2][0] = -1;
			maskX[2][1] = 0;
			maskX[2][2] = 1;
			break;

		case Prefytxy:
			maskX[0][0] = -1;
			maskX[0][1] = -1;
			maskX[0][2] = 1;
			maskX[1][0] = -1;
			maskX[1][1] = 0;
			maskX[1][2] = 1;
			maskX[2][0] = -1;
			maskX[2][1] = 1;
			maskX[2][2] = 1;
			break;

		default:
			break;
		}
		switch (typeIn)
		{
		case InNull:
		
			valrgb = new RGB * [data.size.HeightPixel + 2];
			for (int i = 0; i < data.size.HeightPixel + 2; i++) {
				valrgb[i] = new RGB[data.size.WidthPixel + 2];
			}
			

			for (int i = 0; i < data.size.HeightPixel; i++) {
				for (int j = 0; j < data.size.WidthPixel; j++) {
					int max = data.rgb[i][j].RGB[0];
					for (int t = 0; t < 2; t++) {
						if (max < data.rgb[i][j].RGB[t + 1]) {
							max = data.rgb[i][j].RGB[t + 1];
						}
					}
					for (int t = 0; t < 3; t++) {
						data.rgb[i][j].RGB[t] = max;
					}
				}
			}
			for (int i = 1; i < data.size.HeightPixel + 1; i++) {

				for (int t = 0; t < 3; t++) {
					valrgb[i][0].RGB[t] = 0;
				}
				for (int t = 0; t < 3; t++) {
					valrgb[i][data.size.WidthPixel + 1].RGB[t] = 0;
				}

				for (int j = 1; j < data.size.WidthPixel + 1; j++) {

					for (int t = 0; t < 3; t++) {
						valrgb[i][j].RGB[t] = data.rgb[i - 1][j - 1].RGB[t];
					}
				}

			}

		for (int j = 0; j < data.size.WidthPixel+2; j++) {
				for (int t = 0; t < 3; t++) {
					valrgb[0][j].RGB[t] = 0;
			}
				for (int t = 0; t < 3; t++) {
					valrgb[data.size.HeightPixel + 1][j].RGB[t] = 0;
				}
			}

			for (int i = 0; i < data.size.HeightPixel; i++) {
				for (int j = 0; j < data.size.WidthPixel; j++) {
					int val = 0;
					for (int u = 0; u < 3; u++) {
						for (int r = 0; r < 3; r++) {
							
							val += valrgb[i + u][j + r].RGB[0] * maskX[u][r];
							
						}
					}
					if (val > 250) {
						val = 250;
					}
					else if (abs(val) < 0) {
						val = 0;
					}
					for (int t = 0; t < 3; t++) {
						data.rgb[i][j].RGB[t] = abs(val);
					}

				}

			}
			for (int i = 0; i < data.size.HeightPixel + 2; i++) {
				delete[] valrgb[i];
			}
			delete[] valrgb;
			break;
		case InR:
			
			 valrgb = new RGB * [data.size.HeightPixel + 2];
			for (int i = 0; i < data.size.HeightPixel + 2; i++) {
				valrgb[i] = new RGB[data.size.WidthPixel + 2];
			}
		

			for (int i = 0; i < data.size.HeightPixel; i++) {
				for (int j = 0; j < data.size.WidthPixel; j++) {
					int max = data.rgb[i][j].RGB[0];
					for (int t = 0; t < 2; t++) {
						if (max < data.rgb[i][j].RGB[t + 1]) {
							max = data.rgb[i][j].RGB[t + 1];
						}
					}
					for (int t = 0; t < 3; t++) {
						data.rgb[i][j].RGB[t] = max;
					}
				}
			}
			for (int i = 1; i < data.size.HeightPixel + 1; i++) {

				for (int t = 0; t < 3; t++) {
					valrgb[i][0].RGB[t] = 0;
				}
				for (int t = 0; t < 3; t++) {
					valrgb[i][data.size.WidthPixel + 1].RGB[t] = 0;
				}

				for (int j = 1; j < data.size.WidthPixel + 1; j++) {

					for (int t = 0; t < 3; t++) {
						valrgb[i][j].RGB[t] = data.rgb[i - 1][j - 1].RGB[t];
					}
				}

			}
			for (int j = 0; j < data.size.WidthPixel + 2; j++) {
				for (int t = 0; t < 3; t++) {
					valrgb[0][j].RGB[t] = 0;
				}
				for (int t = 0; t < 3; t++) {
					valrgb[data.size.HeightPixel + 1][j].RGB[t] = 0;
				}
			}

			for (int i = 0; i < data.size.HeightPixel; i++) {
				for (int j = 0; j < data.size.WidthPixel; j++) {
					int val = 0;
					for (int u = 0; u < 3; u++) {
						for (int r = 0; r < 3; r++) {

							val += valrgb[i + u][j + r].RGB[0] * maskX[u][r];
						}
					}
					if (val > 250) {
						val = 250;
					}
					else if (val < 0) {
						val = 0;
					}
					for (int t = 0; t < 3; t++) {
						data.rgb[i][j].RGB[t] = val;
					}

				}

			}
			for (int i = 0; i < data.size.HeightPixel + 2; i++) {
				delete[] valrgb[i];
			}
			delete[] valrgb;
			break;
		case InL:
		
			 valrgb = new RGB * [data.size.HeightPixel + 2];
			for (int i = 0; i < data.size.HeightPixel + 2; i++) {
				valrgb[i] = new RGB[data.size.WidthPixel + 2];
			}
			

			for (int i = 0; i < data.size.HeightPixel; i++) {
				for (int j = 0; j < data.size.WidthPixel; j++) {
					int max = data.rgb[i][j].RGB[0];
					for (int t = 0; t < 2; t++) {
						if (max < data.rgb[i][j].RGB[t + 1]) {
							max = data.rgb[i][j].RGB[t + 1];
						}
					}
					for (int t = 0; t < 3; t++) {
						data.rgb[i][j].RGB[t] = max;
					}
				}
			}
			for (int i = 1; i < data.size.HeightPixel + 1; i++) {

				for (int t = 0; t < 3; t++) {
					valrgb[i][0].RGB[t] = 0;
				}
				for (int t = 0; t < 3; t++) {
					valrgb[i][data.size.WidthPixel + 1].RGB[t] = 0;
				}

				for (int j = 1; j < data.size.WidthPixel + 1; j++) {

					for (int t = 0; t < 3; t++) {
						valrgb[i][j].RGB[t] = data.rgb[i - 1][j - 1].RGB[t];
					}
				}

			}
			for (int j = 0; j < data.size.WidthPixel + 2; j++) {
				for (int t = 0; t < 3; t++) {
					valrgb[0][j].RGB[t] = 0;
				}
				for (int t = 0; t < 3; t++) {
					valrgb[data.size.HeightPixel + 1][j].RGB[t] = 0;
				}
			}

			for (int i = 0; i < data.size.HeightPixel; i++) {
				for (int j = 0; j < data.size.WidthPixel; j++) {
					int val = 0;
					for (int u = 0; u < 3; u++) {
						for (int r = 0; r < 3; r++) {

							val += valrgb[i + u][j + r].RGB[0] * maskX[u][r];
						}
					}
					val = -val;
					if (val > 250) {
						val = 250;
					}
					else if (val < 0) {
						val = 0;
					}
					for (int t = 0; t < 3; t++) {
						data.rgb[i][j].RGB[t] = val;
					}

				}

			}
			for (int i = 0; i < data.size.HeightPixel + 2; i++) {
				delete[] valrgb[i];
			}
			delete[] valrgb;
			break;
		default:
			break;
		}
	
	
	}
	else {
		cout << "error";
	}
}
void Image::contourRGB(TypeCountr type, TypeCountrInversion typeIn) {
	if (data.BytesRast != 3) {
		cout << "error";
		return;
	}
	if (data.rgb != 0) {
		int maskX[3][3];
		RGB** valrgb;
		switch (type)
		{
		case Sobelx:

			maskX[0][0] = -1;
			maskX[0][1] = -2;
			maskX[0][2] = -1;
			maskX[1][0] = 0;
			maskX[1][1] = 0;
			maskX[1][2] = 0;
			maskX[2][0] = 1;
			maskX[2][1] = 2;
			maskX[2][2] = 1;
			break;
		case Sobely:
			maskX[0][0] = -1;
			maskX[0][1] = 0;
			maskX[0][2] = 1;
			maskX[1][0] = -2;
			maskX[1][1] = 0;
			maskX[1][2] = 2;
			maskX[2][0] = -1;
			maskX[2][1] = 0;
			maskX[2][2] = 1;
			break;
		case Sobelxy:
			maskX[0][0] = -1;
			maskX[0][1] = -2;
			maskX[0][2] = 1;
			maskX[1][0] = -2;
			maskX[1][1] = 0;
			maskX[1][2] = 2;
			maskX[2][0] = -1;
			maskX[2][1] = 2;
			maskX[2][2] = 1;
			break;
		case Prefitx:
			maskX[0][0] = -1;
			maskX[0][1] = -1;
			maskX[0][2] = -1;
			maskX[1][0] = 0;
			maskX[1][1] = 0;
			maskX[1][2] = 0;
			maskX[2][0] = 1;
			maskX[2][1] = 1;
			maskX[2][2] = 1;
			break;
		case Prefity:
			maskX[0][0] = -1;
			maskX[0][1] = 0;
			maskX[0][2] = 1;
			maskX[1][0] = -1;
			maskX[1][1] = 0;
			maskX[1][2] = 1;
			maskX[2][0] = -1;
			maskX[2][1] = 0;
			maskX[2][2] = 1;
			break;

		case Prefytxy:
			maskX[0][0] = -1;
			maskX[0][1] = -1;
			maskX[0][2] = 1;
			maskX[1][0] = -1;
			maskX[1][1] = 0;
			maskX[1][2] = 1;
			maskX[2][0] = -1;
			maskX[2][1] = 1;
			maskX[2][2] = 1;
			break;

		default:
			break;
		}
		switch (typeIn)
		{
		case InNull:

			valrgb = new RGB * [data.size.HeightPixel + 2];
			for (int i = 0; i < data.size.HeightPixel + 2; i++) {
				valrgb[i] = new RGB[data.size.WidthPixel + 2];
			}


		

			for (int i = 1; i < data.size.HeightPixel + 1; i++) {

				for (int t = 0; t < 3; t++) {
					valrgb[i][0].RGB[t] = 0;
				}
				for (int t = 0; t < 3; t++) {
					valrgb[i][data.size.WidthPixel + 1].RGB[t] = 0;
				}

				for (int j = 1; j < data.size.WidthPixel + 1; j++) {

					for (int t = 0; t < 3; t++) {
						valrgb[i][j].RGB[t] = data.rgb[i - 1][j - 1].RGB[t];
					}
				}

			}
			
			for (int j = 0; j < data.size.WidthPixel + 2; j++) {
				for (int t = 0; t < 3; t++) {
					valrgb[0][j].RGB[t] = 0;
				}
				for (int t = 0; t < 3; t++) {
					valrgb[data.size.HeightPixel + 1][j].RGB[t] = 0;
				}
			}

			for (int i = 0; i < data.size.HeightPixel; i++) {
				for (int j = 0; j < data.size.WidthPixel; j++) {
					int val[3] = { 0,0,0 };
					for (int t = 0; t < 3; t++) {
						for (int u = 0; u < 3; u++) {
							for (int r = 0; r < 3; r++) {

								val[t] += valrgb[i + u][j + r].RGB[t] * maskX[u][r];
							}
						}
					
			
					if (val[t] > 250) {
						val[t] = 250;
					}
					else if (abs(val[t]) < 0) {
						val[t] = 0;
					}
					
						data.rgb[i][j].RGB[t] = abs(val[t]);
					}

				}

			}
			for (int i = 0; i < data.size.HeightPixel + 2; i++) {
				delete[] valrgb[i];
			}
			delete[] valrgb;
			break;
		case InR:

			valrgb = new RGB * [data.size.HeightPixel + 2];
			for (int i = 0; i < data.size.HeightPixel + 2; i++) {
				valrgb[i] = new RGB[data.size.WidthPixel + 2];
			}





			for (int i = 1; i < data.size.HeightPixel + 1; i++) {

				for (int t = 0; t < 3; t++) {
					valrgb[i][0].RGB[t] = 0;
				}
				for (int t = 0; t < 3; t++) {
					valrgb[i][data.size.WidthPixel + 1].RGB[t] = 0;
				}

				for (int j = 1; j < data.size.WidthPixel + 1; j++) {

					for (int t = 0; t < 3; t++) {
						valrgb[i][j].RGB[t] = data.rgb[i - 1][j - 1].RGB[t];
					}
				}

			}
			for (int j = 0; j < data.size.WidthPixel + 2; j++) {
				for (int t = 0; t < 3; t++) {
					valrgb[0][j].RGB[t] = 0;
				}
				for (int t = 0; t < 3; t++) {
					valrgb[data.size.HeightPixel + 1][j].RGB[t] = 0;
				}
			}

			for (int i = 0; i < data.size.HeightPixel; i++) {
				for (int j = 0; j < data.size.WidthPixel; j++) {
					int val[3] = { 0,0,0 };
					for (int t = 0; t < 3; t++) {
						for (int u = 0; u < 3; u++) {
							for (int r = 0; r < 3; r++) {

								val[t] += valrgb[i + u][j + r].RGB[0] * maskX[u][r];
							}

						}

						if (val[t] > 250) {
							val[t] = 250;
						}
						else if (val[t] < 0) {
							val[t] = 0;
						}

						data.rgb[i][j].RGB[t] = val[t];

					}
				}

			}
			for (int i = 0; i < data.size.HeightPixel + 2; i++) {
				delete[] valrgb[i];
			}
			delete[] valrgb;
			break;
		case InL:

			valrgb = new RGB * [data.size.HeightPixel + 2];
			for (int i = 0; i < data.size.HeightPixel + 2; i++) {
				valrgb[i] = new RGB[data.size.WidthPixel + 2];
			}




			for (int i = 1; i < data.size.HeightPixel + 1; i++) {

				for (int t = 0; t < 3; t++) {
					valrgb[i][0].RGB[t] = 0;
				}
				for (int t = 0; t < 3; t++) {
					valrgb[i][data.size.WidthPixel + 1].RGB[t] = 0;
				}

				for (int j = 1; j < data.size.WidthPixel + 1; j++) {

					for (int t = 0; t < 3; t++) {
						valrgb[i][j].RGB[t] = data.rgb[i - 1][j - 1].RGB[t];
					}
				}

			}
			for (int j = 0; j < data.size.WidthPixel + 2; j++) {
				for (int t = 0; t < 3; t++) {
					valrgb[0][j].RGB[t] = 0;
				}
				for (int t = 0; t < 3; t++) {
					valrgb[data.size.HeightPixel + 1][j].RGB[t] = 0;
				}
			}

			for (int i = 0; i < data.size.HeightPixel; i++) {
				for (int j = 0; j < data.size.WidthPixel; j++) {
					int val[3] = { 0,0,0 };
					for (int t = 0; t < 3; t++) {
					for (int u = 0; u < 3; u++) {
						for (int r = 0; r < 3; r++) {

							val[t] += valrgb[i + u][j + r].RGB[0] * maskX[u][r];
						}
					}
					val[t] = -val[t];
					if (val[t] > 250) {
						val[t] = 250;
					}
					else if (val[t] < 0) {
						val[t] = 0;
					}
					
						data.rgb[i][j].RGB[t] = val[t];
					}

				}

			}
			for (int i = 0; i < data.size.HeightPixel + 2; i++) {
				delete[] valrgb[i];
			}
			delete[] valrgb;
			break;
		default:
			break;
		}

	
	}
	else {
		cout << "error";
	}
}
void Image::contourRGBD(TypeCountr type, TypeCountrInversion typeIn) {
	if (data.BytesRast != 3) {
		cout << "error";
		return ;
	}
	if (data.rgb != 0) {
		int maskX[3][3];
		RGB** valrgb;
		switch (type)
		{
		case Sobelx:

			maskX[0][0] = -1;
			maskX[0][1] = -2;
			maskX[0][2] = -1;
			maskX[1][0] = 0;
			maskX[1][1] = 0;
			maskX[1][2] = 0;
			maskX[2][0] = 1;
			maskX[2][1] = 2;
			maskX[2][2] = 1;
			break;
		case Sobely:
			maskX[0][0] = -1;
			maskX[0][1] = 0;
			maskX[0][2] = 1;
			maskX[1][0] = -2;
			maskX[1][1] = 0;
			maskX[1][2] = 2;
			maskX[2][0] = -1;
			maskX[2][1] = 0;
			maskX[2][2] = 1;
			break;
		case Sobelxy:
			maskX[0][0] = -1;
			maskX[0][1] = -2;
			maskX[0][2] = 1;
			maskX[1][0] = -2;
			maskX[1][1] = 0;
			maskX[1][2] = 2;
			maskX[2][0] = -1;
			maskX[2][1] = 2;
			maskX[2][2] = 1;
			break;
		case Prefitx:
			maskX[0][0] = -1;
			maskX[0][1] = -1;
			maskX[0][2] = -1;
			maskX[1][0] = 0;
			maskX[1][1] = 0;
			maskX[1][2] = 0;
			maskX[2][0] = 1;
			maskX[2][1] = 1;
			maskX[2][2] = 1;
			break;
		case Prefity:
			maskX[0][0] = -1;
			maskX[0][1] = 0;
			maskX[0][2] = 1;
			maskX[1][0] = -1;
			maskX[1][1] = 0;
			maskX[1][2] = 1;
			maskX[2][0] = -1;
			maskX[2][1] = 0;
			maskX[2][2] = 1;
			break;

		case Prefytxy:
			maskX[0][0] = -1;
			maskX[0][1] = -1;
			maskX[0][2] = 1;
			maskX[1][0] = -1;
			maskX[1][1] = 0;
			maskX[1][2] = 1;
			maskX[2][0] = -1;
			maskX[2][1] = 1;
			maskX[2][2] = 1;
			break;

		default:
			break;
		}
		switch (typeIn)
		{
		case InNull:

			valrgb = new RGB * [data.size.HeightPixel + 2];
			for (int i = 0; i < data.size.HeightPixel + 2; i++) {
				valrgb[i] = new RGB[data.size.WidthPixel + 2];
			}


		

			for (int i = 1; i < data.size.HeightPixel + 1; i++) {

				for (int t = 0; t < 3; t++) {
					valrgb[i][0].RGB[t] = 0;
				}
				for (int t = 0; t < 3; t++) {
					valrgb[i][data.size.WidthPixel + 1].RGB[t] = 0;
				}

				for (int j = 1; j < data.size.WidthPixel + 1; j++) {

					int max = data.rgb[i-1][j-1].RGB[0];
					int k=0;
					for (int t = 0; t < 2; t++) {
						if (max < data.rgb[i-1][j-1].RGB[t + 1]) {
							max = data.rgb[i-1][j-1].RGB[t + 1];
							k = t + 1;
						}
					}
					
						valrgb[i][j].RGB[k] = max;
						for (int t = 0; t < 3; t++) {
							if (t != k) {
								valrgb[i][j].RGB[t] = 0;
							}

						}
					
				}

					
				}

			

			for (int j = 0; j < data.size.WidthPixel + 2; j++) {
				for (int t = 0; t < 3; t++) {
					valrgb[0][j].RGB[t] = 0;
				}
				for (int t = 0; t < 3; t++) {
					valrgb[data.size.HeightPixel + 1][j].RGB[t] = 0;
				}
			}

			for (int i = 0; i < data.size.HeightPixel; i++) {
				for (int j = 0; j < data.size.WidthPixel; j++) {
					int val = 0;
					int k=0;
					
						for (int u = 0; u < 3; u++) {
							for (int r = 0; r < 3; r++) {
								for (int t = 0; t < 3; t++) {
								
										if (valrgb[i + u][j + r].RGB[t] > 0) {
										
											k = t;
										}
							

								}
								val += valrgb[i + u][j + r].RGB[k] * maskX[u][r];
							}
						}


					if (val > 250) {
						val = 250;
					}
					else if (abs(val) < 0) {
						val = 0;
					}

					data.rgb[i][j].RGB[k] = abs(val);
					

				

				}

			}
			for (int i = 0; i < data.size.HeightPixel + 2; i++) {
				delete[] valrgb[i];
			}
			delete[] valrgb;
			break;
		case InR:

			valrgb = new RGB * [data.size.HeightPixel + 2];
			for (int i = 0; i < data.size.HeightPixel + 2; i++) {
				valrgb[i] = new RGB[data.size.WidthPixel + 2];
			}



			for (int i = 1; i < data.size.HeightPixel + 1; i++) {

				for (int t = 0; t < 3; t++) {
					valrgb[i][0].RGB[t] = 0;
				}
				for (int t = 0; t < 3; t++) {
					valrgb[i][data.size.WidthPixel + 1].RGB[t] = 0;
				}

				for (int j = 1; j < data.size.WidthPixel + 1; j++) {

					int max = data.rgb[i - 1][j - 1].RGB[0];
					int k = 0;
					for (int t = 0; t < 2; t++) {
						if (max < data.rgb[i - 1][j - 1].RGB[t + 1]) {
							max = data.rgb[i - 1][j - 1].RGB[t + 1];
							k = t + 1;
						}
					}

					valrgb[i][j].RGB[k] = max;
					for (int t = 0; t < 3; t++) {
						if (t != k) {
							valrgb[i][j].RGB[t] = 0;
						}

					}

				}


			}



			for (int j = 0; j < data.size.WidthPixel + 2; j++) {
				for (int t = 0; t < 3; t++) {
					valrgb[0][j].RGB[t] = 0;
				}
				for (int t = 0; t < 3; t++) {
					valrgb[data.size.HeightPixel + 1][j].RGB[t] = 0;
				}
			}

			for (int i = 0; i < data.size.HeightPixel; i++) {
				for (int j = 0; j < data.size.WidthPixel; j++) {
					int val = 0;
					int k = 0;

					for (int u = 0; u < 3; u++) {
						for (int r = 0; r < 3; r++) {
							for (int t = 0; t < 3; t++) {

								if (valrgb[i + u][j + r].RGB[t] > 0) {

									k = t;
								}


							}
							val += valrgb[i + u][j + r].RGB[k] * maskX[u][r];
						}
					}


					if (val > 250) {
						val = 250;
					}
					else if (val < 0) {
						val = 0;
					}

					data.rgb[i][j].RGB[k] = val;




				}

			}
			for (int i = 0; i < data.size.HeightPixel + 2; i++) {
				delete[] valrgb[i];
			}
			delete[] valrgb;
			break;
		case InL:

			valrgb = new RGB * [data.size.HeightPixel + 2];
			for (int i = 0; i < data.size.HeightPixel + 2; i++) {
				valrgb[i] = new RGB[data.size.WidthPixel + 2];
			}


			

			for (int i = 1; i < data.size.HeightPixel + 1; i++) {

				for (int t = 0; t < 3; t++) {
					valrgb[i][0].RGB[t] = 0;
				}
				for (int t = 0; t < 3; t++) {
					valrgb[i][data.size.WidthPixel + 1].RGB[t] = 0;
				}

				for (int j = 1; j < data.size.WidthPixel + 1; j++) {

					int max = data.rgb[i - 1][j - 1].RGB[0];
					int k = 0;
					for (int t = 0; t < 2; t++) {
						if (max < data.rgb[i - 1][j - 1].RGB[t + 1]) {
							max = data.rgb[i - 1][j - 1].RGB[t + 1];
							k = t + 1;
						}
					}

					valrgb[i][j].RGB[k] = max;
					for (int t = 0; t < 3; t++) {
						if (t != k) {
							valrgb[i][j].RGB[t] = 0;
						}

					}

				}


			}



			for (int j = 0; j < data.size.WidthPixel + 2; j++) {
				for (int t = 0; t < 3; t++) {
					valrgb[0][j].RGB[t] = 0;
				}
				for (int t = 0; t < 3; t++) {
					valrgb[data.size.HeightPixel + 1][j].RGB[t] = 0;
				}
			}

			for (int i = 0; i < data.size.HeightPixel; i++) {
				for (int j = 0; j < data.size.WidthPixel; j++) {
					int val = 0;
					int k = 0;

					for (int u = 0; u < 3; u++) {
						for (int r = 0; r < 3; r++) {
							for (int t = 0; t < 3; t++) {

								if (valrgb[i + u][j + r].RGB[t] > 0) {

									k = t;
								}


							}
							val += valrgb[i + u][j + r].RGB[k] * maskX[u][r];
						}
					}

					val = -val;
					if (val > 250) {
						val = 250;
					}
					else if (val < 0) {
						val = 0;
					}

					data.rgb[i][j].RGB[k] = val;




				}

			}
			for (int i = 0; i < data.size.HeightPixel + 2; i++) {
				delete[] valrgb[i];
			}
			delete[] valrgb;
			break;
		default:
			break;
		}


	}
	else {
		cout << "error";
	}
}
void Image::BillatiraelFilter(double gs, double gr, int val) {
	if (data.BytesRast != 3) {
		cout << "error";
		return;
	}
	if (data.rgb != 0) {

		RGB** valrgb = new RGB * [data.size.HeightPixel + (2*val)];
		for (int i = 0; i < data.size.HeightPixel + (2 * val); i++) {
			valrgb[i] = new RGB[data.size.WidthPixel + (2 * val)];
		}
		
		for (int t = 0; t < 3; t++) {
			for (int l = 0; l < val; l++) {
				for (int j = 0; j < val; j++) {
					valrgb[l][j].RGB[t] = data.rgb[0][0].RGB[t];
				}
			}
			for (int l = 0; l < val; l++) {
				for (int j = 0; j < val; j++) {
					valrgb[l][data.size.WidthPixel + 1 + j].RGB[t] = data.rgb[0][data.size.WidthPixel - 1].RGB[t];
				}
			}
			for (int l = 0; l < val; l++) {
				for (int j = 0; j < val; j++) {
					valrgb[data.size.HeightPixel + 1 + l][j].RGB[t] = data.rgb[data.size.HeightPixel - 1][0].RGB[t];
				}
			}
			for (int l = 0; l < val; l++) {
				for (int j = 0; j < val; j++) {
					valrgb[data.size.HeightPixel + 1 + l][data.size.WidthPixel + 1 + j].RGB[t] = data.rgb[data.size.HeightPixel - 1][data.size.WidthPixel - 1].RGB[t];
				}
			}
			}
		
		for (int j = 0; j < val; j++) {
			for (int i = val; i < data.size.WidthPixel + (2 * val)-val; i++) {
				for (int t = 0; t < 3; t++) {
					valrgb[j][i].RGB[t] = data.rgb[0][i].RGB[t];
					valrgb[data.size.HeightPixel + 1+j][i].RGB[t] = data.rgb[data.size.HeightPixel - 1][i].RGB[t];
				}

			}
		}


		for (int i = val; i < data.size.HeightPixel + (2 * val) - val; i++) {
			for (int y = 0; y < val; y++) {
				for (int t = 0; t < 3; t++) {
					valrgb[i][y].RGB[t] = data.rgb[i - val][0].RGB[t];
				}
				for (int t = 0; t < 3; t++) {
					valrgb[i][data.size.WidthPixel + y+val].RGB[t] = data.rgb[i -  val][data.size.WidthPixel - 1].RGB[t];
				}
			}
			for (int j = val; j < data.size.WidthPixel + (2 * val) - val; j++) {

				for (int t = 0; t < 3; t++) {

					valrgb[i][j].RGB[t] = data.rgb[i - val][j - val].RGB[t];
				}
			}

		}


		for (int i = 0; i < data.size.HeightPixel; i++) {
			for (int j = 0; j < data.size.WidthPixel; j++) {
				
				for (int t = 0; t < 3; t++) {
	
					double W = 0;
					double  w = 0;
					for (int u = 0; u < 3+(val-1); u++) {
						for (int r = 0; r < 3+(val-1); r++) {
							W += (1/((4* 9.8)*pow(gs,2)*pow(gr,2)))*exp(-(pow((i - (i + u)), 2) + pow((j - (j + r)), 2)) / (2 * pow(gs, 2)) - (pow((valrgb[i][j].RGB[t] - valrgb[i + u][j + r].RGB[t]), 2)) / (2 * pow(gr, 2))) * valrgb[i + u][j + r].RGB[t];
							w += (1/((4* 9.8)*pow(gs,2)*pow(gr,2)))*exp(-(pow((i - (i + u)), 2) + pow((j - (j + r)), 2)) / (2 * pow(gs, 2)) - (pow((valrgb[i][j].RGB[t] - valrgb[i + u][j + r].RGB[t]), 2)) / (2 * pow(gr, 2)));
						
						}
					}
				
					if (W/w > 255) {
						
						data.rgb[i][j].RGB[t] = 255;
					}
					else{
						data.rgb[i][j].RGB[t] = W/w;
					}

					
				}

			}

		}
		for (int i = 0; i < data.size.HeightPixel + (2 * val); i++) {
			delete[] valrgb[i];
		}
		delete[] valrgb;
	}
	else {
		cout << "error";
	}







}
void Image::Sharpness(double k) {
	if (data.BytesRast != 3) {
		cout << "error";
		return;
	}
	if (data.rgb != 0) {
		double mask[3][3] = { {-k/ (double)8,-k / (double)8,-k / (double)8},{ -k / (double)8, k+1.0,-k / (double)8},{ -k / (double)8,-k / (double)8,-k / (double)8} };
	
		RGB** valrgb = new RGB * [data.size.HeightPixel + 2];
		for (int i = 0; i < data.size.HeightPixel + 2; i++) {
			valrgb[i] = new RGB[data.size.WidthPixel + 2];
		}
		for (int t = 0; t < 3; t++) {
			valrgb[0][0].RGB[t] = data.rgb[0][0].RGB[t];
			valrgb[0][data.size.WidthPixel + 1].RGB[t] = data.rgb[0][data.size.WidthPixel - 1].RGB[t];
			valrgb[data.size.HeightPixel + 1][0].RGB[t] = data.rgb[data.size.HeightPixel - 1][0].RGB[t];
			valrgb[data.size.HeightPixel + 1][data.size.WidthPixel + 1].RGB[t] = data.rgb[data.size.HeightPixel - 1][data.size.WidthPixel - 1].RGB[t];
		}

		for (int i = 1; i < data.size.WidthPixel + 1; i++) {
			for (int t = 0; t < 3; t++) {
				valrgb[0][i].RGB[t] = data.rgb[0][i].RGB[t];
				valrgb[data.size.HeightPixel + 1][i].RGB[t] = data.rgb[data.size.HeightPixel - 1][i].RGB[t];
			}

		}


		for (int i = 1; i < data.size.HeightPixel + 1; i++) {

			for (int t = 0; t < 3; t++) {
				valrgb[i][0].RGB[t] = data.rgb[i - 1][0].RGB[t];
			}
			for (int t = 0; t < 3; t++) {
				valrgb[i][data.size.WidthPixel + 1].RGB[t] = data.rgb[i - 1][data.size.WidthPixel - 1].RGB[t];
			}

			for (int j = 1; j < data.size.WidthPixel + 1; j++) {

				for (int t = 0; t < 3; t++) {

					valrgb[i][j].RGB[t] = data.rgb[i - 1][j - 1].RGB[t];
				}
			}

		}


		for (int i = 0; i < data.size.HeightPixel; i++) {
			for (int j = 0; j < data.size.WidthPixel; j++) {
				double val = 0;
				for (int t = 0; t < 3; t++) {
					int val = 0;
					for (int u = 0; u < 3; u++) {
						for (int r = 0; r < 3; r++) {

							val += (double)valrgb[i + u][j + r].RGB[t] * mask[u][r];
						}
					}
					if (val > 255) {
						val = 255;
					}
					else if (val < 0) {
						val = 0;
					}

					data.rgb[i][j].RGB[t] = val;
				}

			}

		}
		for (int i = 0; i < data.size.HeightPixel + 2; i++) {
			delete[] valrgb[i];
		}
		delete[] valrgb;
	}
	else {
		cout << "error";
	}
}
void Image::SmoothFilt() {
	if (data.BytesRast != 3) {
		cout << "error";
		return;
	}
	if (data.rgb != 0) {
		double mask[3][3] = { {1/ 9,1 / 9,1 / 9},{ 1 / 9,1 / 9,1 / 9},{ 1 / 9,1 / 9,1 / 9} };
		RGB** valrgb = new RGB * [data.size.HeightPixel + 2];
		for (int i = 0; i < data.size.HeightPixel + 2; i++) {
			valrgb[i] = new RGB[data.size.WidthPixel + 2];
		}
		for (int t = 0; t < 3; t++) {
			valrgb[0][0].RGB[t] = data.rgb[0][0].RGB[t];
			valrgb[0][data.size.WidthPixel + 1].RGB[t] = data.rgb[0][data.size.WidthPixel - 1].RGB[t];
			valrgb[data.size.HeightPixel + 1][0].RGB[t] = data.rgb[data.size.HeightPixel - 1][0].RGB[t];
			valrgb[data.size.HeightPixel + 1][data.size.WidthPixel + 1].RGB[t] = data.rgb[data.size.HeightPixel - 1][data.size.WidthPixel - 1].RGB[t];
		}

		for (int i = 1; i < data.size.WidthPixel + 1; i++) {
			for (int t = 0; t < 3; t++) {
				valrgb[0][i].RGB[t] = data.rgb[0][i].RGB[t];
				valrgb[data.size.HeightPixel + 1][i].RGB[t] = data.rgb[data.size.HeightPixel - 1][i].RGB[t];
			}

		}


		for (int i = 1; i < data.size.HeightPixel + 1; i++) {

			for (int t = 0; t < 3; t++) {
				valrgb[i][0].RGB[t] = data.rgb[i - 1][0].RGB[t];
			}
			for (int t = 0; t < 3; t++) {
				valrgb[i][data.size.WidthPixel + 1].RGB[t] = data.rgb[i - 1][data.size.WidthPixel - 1].RGB[t];
			}

			for (int j = 1; j < data.size.WidthPixel + 1; j++) {

				for (int t = 0; t < 3; t++) {

					valrgb[i][j].RGB[t] = data.rgb[i - 1][j - 1].RGB[t];
				}
			}

		}


		for (int i = 0; i < data.size.HeightPixel; i++) {
			for (int j = 0; j < data.size.WidthPixel; j++) {
				double val = 0;
				for (int t = 0; t < 3; t++) {
					int val = 0;
					for (int u = 0; u < 3; u++) {
						for (int r = 0; r < 3; r++) {

							val += (double)valrgb[i + u][j + r].RGB[t] * mask[u][r];
						}
					}
					if (val > 255) {
						val = 255;
					}
					else if (val < 0) {
						val = 0;
					}

					data.rgb[i][j].RGB[t] = val;
				}

			}

		}
		for (int i = 0; i < data.size.HeightPixel + 2; i++) {
			delete[] valrgb[i];
		}
		delete[] valrgb;
	}
	else {
		cout << "error";
	}
}
void Image::BilinenInterpolation(double pixelX, double pixelY) {
	if (data.BytesRast != 3) {
		cout << "error";
		return;
	}
	if (data.rgb != 0) {

		RGB** NewImage = new RGB * [pixelY];
		for (int i = 0; i < pixelY; i++) {
			NewImage[i] = new RGB[pixelX];
		}
		double covX = data.size.WidthPixel / pixelX;
		double covY = data.size.HeightPixel / pixelY;
	
		for (int i = 0; i < pixelY; i++) {
			for (int j = 0; j < pixelX; j++) {
				for (int t = 0; t < 3; t++) {
					NewImage[i][j].RGB[t] = funP((j * covX)-1, j, (j * covX), data.rgb[i][j].RGB[t], data.rgb[((int)(i * covY))][((int)(j * covX))].RGB[t]);
				}
				



			}
		}
		for (int i = 0; i < data.size.HeightPixel; i++) {
			delete[] data.rgb[i];
		}
		delete[] data.rgb;
		data.size.HeightPixel = pixelY;
		data.size.WidthPixel = pixelX;


		data.rgb = NewImage;
	}
	else {
		cout << "error";
	}
}
void Image::ScaleImage(double pixelX, double pixelY) {
	if (data.BytesRast != 3) {
		cout << "error";
		return;
	}
	if (data.rgb != 0) {
	
		RGB** NewImage = new RGB * [pixelY];
		for (int i = 0; i < pixelY; i++) {
			NewImage[i] = new RGB[pixelX];
		}
		double covX = data.size.WidthPixel / pixelX;
		double covY = data.size.HeightPixel / pixelY;

		for (int i = 0; i < pixelY; i++) {
			for (int j = 0; j < pixelX; j++) {

				NewImage[i][j] = data.rgb[((int)(i * covY))][((int)(j * covX))];
				
			}
		}
		for (int i = 0; i < data.size.HeightPixel; i++) {
			delete[] data.rgb[i];
		}
		delete[] data.rgb;
		data.size.HeightPixel = pixelY;
		data.size.WidthPixel = pixelX;
		
			
			data.rgb = NewImage;
	}
	else {
		cout << "error";
	}
}
Image::~Image(){
	if (data.MonochromDat != 0) {
	
		
		for (int i = 0; i < data.size.HeightPixel;i++) {
				delete[] data.MonochromDat[i];
			}
			delete[] data.MonochromDat;
		
	}
	else if (data.rgb != 0) {
		for (int j = 0; j < data.size.HeightPixel; j++) {
			delete[] data.rgb[j];
		}
		delete[] data.rgb;
	}

}
