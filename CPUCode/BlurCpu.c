double filter[3][3] =
{		{1, 0, 0},
		{0,1, 0},
		{0, 0, 1}
};

double factor = 1.0/1.0;
double bias = 0.0;

#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

int main(void)
{
	printf("Loading image.\n");
	//int32_t *inImage;
	int width = 0, height = 0;
	unsigned char* bitmapBits = NULL;
	unsigned char* bitmapBitsRes = NULL;
	extractPixels("photo3.bmp", &bitmapBits, &width, &height);

	bitmapBitsRes = malloc(width*height*3*sizeof(char));

	//loadImage("lena.ppm", &inImage, &width, &height,0);
	int32_t* array1 = (int32_t*)malloc(width*height*sizeof(int));
	int32_t* array2 = (int32_t*)malloc(width*height*sizeof(int));
	int32_t* array3 = (int32_t*)malloc(width*height*sizeof(int));

	int32_t* outImage1 = (int32_t*)malloc(width*height*sizeof(int));
	int32_t* outImage2 = (int32_t*)malloc(width*height*sizeof(int));
	int32_t* outImage3 = (int32_t*)malloc(width*height*sizeof(int));

		int cnt = 0;

		for (int i = 0; i < width*height*(BITS_PER_PIXEL/8); i++)
		  if (i%3 == 0)
		    array1[cnt] = (int32_t)bitmapBits[i];
		  else
		    if (i%3 == 1)
		      array2[cnt] = (int32_t)bitmapBits[i];
		    else
		      array3[cnt++] = (int32_t)bitmapBits[i];

		  BlurKernel(array1, array2, array3, outImage1,outImage2,outImage3);

		  for(int x = 0; x < width; x++)
		    for(int y = 0; y < height; y++)
		    {


		        double red = 0.0, green = 0.0, blue = 0.0;

		        //multiply every value of the filter with corresponding image pixel
		        for(int filterX = 0; filterX < 3; filterX++)
		        for(int filterY = 0; filterY < 3; filterY++)
		        {
		            int imageX = (x - 1 + filterX + width) % width;
		            int imageY = (y - 1 + filterY + height) % height;

		             char pixelRed = array1[imageX* width+ imageY];
		             char pixelGreen = array2[imageX* width+ imageY];
		             char pixelBlue = array3[imageX* width+ imageY];

		            red += pixelRed * filter[filterX][filterY];
		            green += pixelGreen * filter[filterX][filterY];
		            blue += pixelBlue * filter[filterX][filterY];
		        }

		        //truncate values smaller than zero and larger than 255
		        char r = min(max(((int)factor * red),0), 255);
		        char g = min(max(((int)factor * green ),0), 255);
		        char b = min(max(((int)factor * blue ),0), 255);

//		        char r = 255;
//				char g =  255;
//				char b =  255;

//		        int tmp = 0x0;
//		        tmp = (tmp | (int)(r & 0xFF)) << 8;
//		        tmp = (tmp |(int) (g & 0xFF)) << 8;
//		        tmp = (tmp | (int)(b & 0xFF));

		        outImage1[x*width + y] =r;
		        outImage2[x*width + y] = g;
		        outImage3[x*width + y] = b;

		    }
		  cnt =0;
		  for (int i = 0; i < width*height*(BITS_PER_PIXEL/8); i++)
		        if (i %3  == 0)
		  	    bitmapBitsRes[i] = (char)outImage1[cnt];
		        else
		        	if (i % 3 == 1)
		        		bitmapBitsRes[i] = (char)outImage2[cnt];
		        	else
		        		bitmapBitsRes[i] = (char)outImage3[cnt++];

	FILE* out = fopen("resul.bpm", "wb");

	write_BITMAPFILEHEADER(out, width, height);

	write_BITMAPINFOHEADER(out, width, height);

	fwrite(bitmapBitsRes, 1, width*height*(BITS_PER_PIXEL/8), out);


//	int dataSize = width * height * sizeof(int);
//	// Allocate a buffer for the output image
//	int32_t *outImage = malloc(dataSize);
//
//	printf("Running Kernel.\n");
//	//Sobel(width * height, inImage, outImage);
//
//	//printf("Saving image.\n");
//	//writeImage("lena_sobel.ppm", outImage, width, height, 0);
//
//	  for(int x = 2; x < 255; x++)
//	    for(int y = 2; y < 255; y++)
//	    {
//
//	        double red = 0.0, green = 0.0, blue = 0.0;
//
//	        //multiply every value of the filter with corresponding image pixel
//	        for(int filterX = 0; filterX < 3; filterX++)
//	        for(int filterY = 0; filterY < 3; filterY++)
//	        {
//	            int imageX = (x - 3 / 2 + filterX + 256) % 256;
//	            int imageY = (y - 3 / 2 + filterY + 256) % 256;
//
//	             char pixelRed = (char)((inImage[imageX* 256+ imageY] >> 16) & 0xFF);
//	             char pixelGreen = (char)((inImage[imageX* 256+ imageY] >> 8) & 0xFF);
//	             char pixelBlue = (char)((inImage[imageX* 256+ imageY]) & 0xFF);
//
//	            red += pixelRed * filter[filterX][filterY];
//	            green += pixelGreen * filter[filterX][filterY];
//	            blue += pixelBlue * filter[filterX][filterY];
//	        }
//
//	        //truncate values smaller than zero and larger than 255
//	        char r = min(max((int)(factor * red + bias), 0), 255);
//	        char g = min(max((int)(factor * green + bias), 0), 255);
//	        char b = min(max((int)(factor * blue + bias), 0), 255);
//
////	        char r = (int)min(max( red , 0), 255);
////			char g = (int)min(max( green , 0), 255);
////			char b = (int)min(max( blue , 0), 255);
//
//	        int tmp = 0x0;
//	        tmp = (tmp | (int)(r & 0xFF)) << 8;
//	        tmp = (tmp |(int) (g & 0xFF)) << 8;
//	        tmp = (tmp | (int)(b & 0xFF));
//	        outImage[x*256 + y] = tmp;
//	    }
//
//
//
//
//	printf("Saving image.\n");
//		writeImage("lena_sobel33.ppm", outImage, width, height, 0);
	printf("Exiting\n");
	return 0;
}