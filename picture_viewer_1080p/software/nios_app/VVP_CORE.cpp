/*
 * VPP_CORE.cpp
 *
 *  Created on: Aug 23, 2024
 *      Author: User
 */

#include <stdio.h>
#include <stdlib.h> // malloc, free
#include <unistd.h>  // usleep (unix standard?)
#include "VVP_CORE.h"



VVP_CORE::VVP_CORE(uint32_t BaseAddr):
m_BaseAddr(BaseAddr),
m_bValidInstance(false){
	// TODO Auto-generated destructor stub
}

VVP_CORE::~VVP_CORE() {
	// TODO Auto-generated destructor stub
}

void VVP_CORE::DumpRegister(REG_INFO szRegInfo[], const int nItemNum){
	int i;
	uint32_t value32;


	for(i=0;i<nItemNum;i++){
		value32 = IORD_32DIRECT(m_BaseAddr, szRegInfo[i].RegisterAddr);
		printf("%s: %d (%xh)\r\n", szRegInfo[i].szHint, value32 & szRegInfo[i].ValueMask, value32);
	}
}
