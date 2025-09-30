/*
 * VPP_CORE.h
 *
 *  Created on: Aug 23, 2024
 *      Author: Richard
 */

#ifndef VVP_CORE_H_
#define VVP_CORE_H_

#include <stdint.h>
#include <stdbool.h>
#include "intel_vvp_core_regs.h"
#include "intel_vvp_core.h"

class VVP_CORE {
protected:
	uint32_t m_BaseAddr;
	bool m_bValidInstance;

	typedef struct{
		uint32_t RegisterAddr;
		uint32_t ValueMask;
		char szHint[256];
	}REG_INFO;
	void DumpRegister(REG_INFO szRegInfo[], const int nItemNum);
public:
	VVP_CORE(uint32_t BaseAddr);
	virtual ~VVP_CORE();


	virtual bool IsLiteMode() = 0;
	virtual void ShowStatus() = 0;
};

#endif /* VVP_CORE_H_ */
