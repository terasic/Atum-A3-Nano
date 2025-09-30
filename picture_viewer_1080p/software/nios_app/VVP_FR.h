/*
 * VVP_FB.h
 *
 *  Created on: Aug 23, 2024
 *      Author: User
 */

#ifndef VVP_FB_H_
#define VVP_FB_H_

#include <stdint.h>
#include <stdbool.h>

#include "intel_vvp_vfr_regs.h"
#include "intel_vvp_vfr.h"
#include "VVP_Core.h"

// https://www.intel.com/content/www/us/en/docs/programmable/683329/23-4/video-frame-reader-ip-functional-description.html

#define FB_IMG_INFO_WIDTH			0x0120
#define FB_IMG_INFO_HEIGHT			0x0124
#define FB_IMG_INFO_INTERLACE		0x0128
#define FB_IMG_INFO_COLORSPACE		0x0130
#define FB_IMG_INFO_SUBSAMPLING		0x0134
#define FB_IMG_INFO_COSITING		0x0138
#define FB_IMG_INFO_FIELD_COUNT		0x013c // Read only
#define FB_INPUT_STATUS				0x0140
#define FB_NUM_INPUT_FIELDS			0x0144
#define FB_NUM_DROPPED_FIELDS		0x0148
#define FB_NUM_INVALID_FIELDS		0x014c
#define FB_OUTPUT_STATUS			0x0150
#define FB_NUM_OUTPUT_FIELDS		0x0154
#define FB_NUM_REPEATED_FIELDS		0x0158
#define FB_OUTPUT_CONTROL			0x015c


class VVP_FR : public VVP_CORE {
protected:
	intel_vvp_vfr_instance  m_instance;
	bool InitFrame(uint32_t BufferSet_BaseAddr, uint8_t buffer_set, int nWidth=1270, int nHeight = 720);
public:
	VVP_FR(uint32_t BaseAddr);
	virtual ~VVP_FR();

	void ShowStatus();
	bool Config(uint32_t BuffsetSet0_BaseAddr, uint32_t BuffsetSet1_BaseAddr, uint8_t Active_Buffer_Set, int nWidth=1280, int nHeight = 720);
	bool Run(bool Start = true);
	bool Switch(uint8_t buffset_set);
	bool IsLiteMode();
};

#endif /* VVP_FB_H_ */
