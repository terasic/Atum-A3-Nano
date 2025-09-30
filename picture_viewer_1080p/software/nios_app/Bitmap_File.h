/*
 * Bitmap_File.h
 *
 *  Created on: 2025年6月7日
 *      Author: User
 */

#ifndef BITMAP_FILE_H_
#define BITMAP_FILE_H_

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif



#pragma pack(push, 1) // Ensure no padding is added
typedef struct {
	uint16_t bfType;		// File type, must be 'BM' (0x4D42)
	uint32_t bfSize;		// Size of the file in bytes
	uint16_t bfReserved1;	// Reserved, must be 0
	uint16_t bfReserved2;	// Reserved, must be 0
	uint32_t bfOffBits;		// Offset to start of pixel data
} BITMAP_FILE_HEADER;


typedef struct {
	uint32_t biSize;
	uint32_t biWidth;
	uint32_t biHeight;
	uint16_t biPlanes;
	uint16_t biBitCount;
	uint32_t biCompression;
	uint32_t biSizeImage;
	int32_t  biXPelsPerMeter;
	int32_t  biYPelsPerMeter;
	uint32_t biClrUsed;
	uint32_t biClrImportant;
} BITMAP_INFO_HEADER;

bool BMP_find_image_file(char szFileList[][FF_MAX_LFN], int nBufCnt, int *pFileCnt, const int nWidth, const int nHeight);
bool BMP_draw(char *pFilename, void *pMem, const int nWidth, const int nHeight);

#ifdef __cplusplus
}
#endif

#pragma pack(pop)


#endif /* BITMAP_FILE_H_ */
