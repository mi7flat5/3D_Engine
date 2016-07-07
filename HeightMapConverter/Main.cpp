#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <fstream>
#include<iomanip>


using namespace cv;
using namespace std;

void Compress(uchar *szOut, const char *szMessage);
void Uncompress(char *szOut, const unsigned char *szCompressed, unsigned nCompressedLen);

int main()
{
	
	ofstream file;
	file.open("HeightMap.hm",ios::binary);
	
	Mat image;
	image = imread("hm_n.jpg", CV_LOAD_IMAGE_COLOR);   // Read the file

	if (!image.data)                              // Check for invalid input
	{
		cout << "Could not open or find the image" << std::endl;
		system("pause");
		return -1;
	}
	Point3_<uchar>* p = image.ptr<Point3_<uchar> >(0, 0);
	int a = p->x;
	int b = p->y;
	int c = p->z;
	
	file << image.rows << '\n'<<image.cols<<'\n';
	std::cout << "\nPixels "<< std::setprecision(2) <<(float)1.0/255*a << ", " << (float)1.0 / 255 * b<<", "<< (float)1.0 / 255 * c;
	for (int i = 0; i < image.rows; i++) {
		
		
		for (int j = 0; j < image.cols; j++) {
		
			p = image.ptr<Point3_<uchar>>(i, j);
				/*unsigned short int in = p->z;
				in = (float)1.0 / 255 * in * 100;
				
				
				if (in < 10)
				{
					file << 0;
				}*/
				uchar q = p->z;
				
				file << q;
				
		}
	}
	file.close();
	//ifstream infile("poop.hm", ios::binary);
	//std::stringstream buffer;
	//buffer << infile.rdbuf();
	//std::string contents(buffer.str());
	//std::string compressed;
	//unsigned char out;
	//for (auto s : contents)
	//{
	//	Compress(&out, (char)contents[s]);
	//	compressed.push_back((char)out);
	//}
	//
	//infile.close();
	system("pause");
	return 0;
}


void Compress(uchar *szOut, char *szMessage) {
	unsigned long long nBuffer = 0; //This is enough to store 8 uncompressed characters or 9 compressed. We will only use 8 tho.
	char nBitsInBuffer = 0;
	while (*szMessage != 0) {
		nBuffer |= (unsigned long long)(*szMessage++ & 0x7F) << nBitsInBuffer;
		nBitsInBuffer += 7;
		if (nBitsInBuffer == 7 * 8) { //We have 8 chars in the buffer, dump them
			while (nBitsInBuffer > 0) {
				*szOut++ = nBuffer & 0xFF;
				nBuffer >>= 8;
				nBitsInBuffer -= 8;
			}
			//The following should be redundant, but just to be safe
			nBitsInBuffer = 0;
			nBuffer = 0;
		}
	}
	//Write out whatever is left in the buffer
	while (nBitsInBuffer > 0) {
		*szOut++ = nBuffer & 0xFF;
		nBuffer >>= 8;
		nBitsInBuffer -= 8;
	}
}

void Uncompress(char *szOut, const unsigned char *szCompressed, unsigned nCompressedLen) {
	unsigned long long nBuffer = 0; //This is enough to store 8 uncompressed characters or 9 compressed. We will only use 8 tho.
	char nBitsInBuffer = 0;
	while (nCompressedLen) {
		while (nCompressedLen && nBitsInBuffer < 7 * 8) {
			nBuffer |= (unsigned long long)*szCompressed++ << nBitsInBuffer;
			nBitsInBuffer += 8;
			--nCompressedLen;
		}
		while (nBitsInBuffer > 0) {
			*szOut++ = nBuffer & 0x7F;
			nBuffer >>= 7;
			nBitsInBuffer -= 7;
		}
		//The following should be redundant, but just to be safe
		nBitsInBuffer = 0;
		nBuffer = 0;
	}
}