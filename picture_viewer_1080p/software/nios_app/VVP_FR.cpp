/*
 * VVP_FR.cpp
 *
 *  Created on: Sep 24, 2024
 *      Author: Richard
 */
#include <stdio.h>
#include <stdlib.h> // malloc, free
#include <unistd.h>  // usleep (unix standard?)
#include <string.h>
#include "system.h"
#include "io.h"
#include "VVP_FR.h"

#define DEBUG_MODE 0
#define VERBOSE_PRINTF if (DEBUG_MODE) printf




VVP_FR::VVP_FR(uint32_t BaseAddr):
VVP_CORE(BaseAddr){
	// TODO Auto-generated destructor stub
	int result;
	result =  intel_vvp_vfr_init(&m_instance, (intel_vvp_core_base)BaseAddr);
	if (result != 0){
		printf("intel_vvp_vfb_init failed\r\n");
	}else{
		m_bValidInstance = true;
	}
}

VVP_FR::~VVP_FR() {
	// TODO Auto-generated constructor stub

}

void VVP_FR::ShowStatus() {
	// TODO Auto-generated destructor stub

	int result;
	bool bYes;
	uint32_t  status, value32;
	uint8_t value8;

	if (!m_bValidInstance){
		printf("FB invalid instance\r\n");
		return;
	}
	printf("=========== FR Status =============\r\n");

		bYes = intel_vvp_vfr_get_lite_mode(&m_instance);
		printf("FR lite mode: %s\r\n", bYes?"Yes":"No");

		bYes = intel_vvp_vfr_get_debug_enabled(&m_instance);
		printf("FR debug mode: %s\r\n", bYes?"Yes":"No");

		//result =  intel_vvp_vfb_get_bits_per_sample(&m_instance);
		//printf("FB bit per sample: %d\r\n", result);

		result =  intel_vvp_vfr_get_max_buffer_sets(&m_instance);
		printf("FR maximum number of supported buffer sets: %d\r\n", result);

		result =  intel_vvp_vfr_get_num_buffer_sets(&m_instance);
		printf("FR number of buffer sets in use: %d\r\n", result);

		result =  intel_vvp_vfr_get_run_mode(&m_instance);
		printf("FR run mode: %d\r\n", result);

		result =  intel_vvp_vfr_get_fsync_mode(&m_instance);
		printf("FR fsync mode: %d\r\n", result);

		result =  intel_vvp_vfr_get_bufset_height(&m_instance, 0);
		printf("FR height of fields in a set 0: %d\r\n", result);

		result =  intel_vvp_vfr_get_bufset_width(&m_instance, 0);
		printf("FR width of fields in a set 0: %d\r\n", result);

		result =  intel_vvp_vfr_get_bufset_height(&m_instance, 1);
		printf("FR height of fields in a set 1: %d\r\n", result);

		result =  intel_vvp_vfr_get_bufset_width(&m_instance, 1);
		printf("FR width of fields in a set 1: %d\r\n", result);


		value8 =  intel_vvp_vfr_get_number_of_color_planes(&m_instance);
		printf("FR number of color plane: %d\r\n", value8);

		value8 =  intel_vvp_vfr_get_pixels_in_parallel(&m_instance);
		printf("FR pixel in parallel: %d\r\n", value8);

		bYes = intel_vvp_vfr_is_running(&m_instance);
		printf("FR running: %s\r\n", bYes?"Yes":"No");

		printf("\r\n");

}

bool VVP_FR::InitFrame(uint32_t BufferSet_BaseAddr, uint8_t buffer_set, int width, int height){
    //const int width = 1920*2;
   // const int height = 1080*2;
    int error;

    //if (error == 0)
    //printf("InitFrame, base=%xh, id=%d, width=%d, height=%d\r\n", BufferSet_BaseAddr,buffer_set, width, height);

    error = intel_vvp_vfr_set_bufset_base_addr(&m_instance, buffer_set, BufferSet_BaseAddr);// buffer set  [0..MAX_BUFFER_SETS] ???

    if (error == 0)
        error = intel_vvp_vfr_set_bufset_num_buffers(&m_instance, buffer_set, 1); // one buffer in this buffer set
    if (error == 0)
        error = intel_vvp_vfr_set_bufset_height(&m_instance, buffer_set, height);
    if (error == 0)
        error = intel_vvp_vfr_set_bufset_width(&m_instance, buffer_set, width);
    if (error == 0)
        error = intel_vvp_vfr_set_bufset_colorspace(&m_instance, buffer_set, 0); // 0: RGB. https://www.intel.com/content/www/us/en/docs/programmable/683397/current/image-information-packets.html
    if (error == 0)
        error = intel_vvp_vfr_set_bufset_interlace(&m_instance, buffer_set, 0); //0: Progressive frame
    if (error == 0)
        error = intel_vvp_vfr_set_bufset_subsampling(&m_instance, buffer_set,3); //3: 444
    if (error == 0)
        error = intel_vvp_vfr_set_bufset_cositing(&m_instance, buffer_set, 0); //0: top and left
    if (error == 0)
        error = intel_vvp_vfr_set_bufset_field_count(&m_instance, buffer_set, 0); //
    if (error == 0)
        error = intel_vvp_vfr_set_bufset_bps(&m_instance, buffer_set, 8);

    if (error == 0) // With only one buffer in each set, the BUFFER_N_INTER_BUFFER_OFFSET register is unused
        error = intel_vvp_vfr_set_bufset_inter_buffer_offset(&m_instance, buffer_set, width*height*3);
    if (error == 0)
        error = intel_vvp_vfr_set_bufset_inter_line_offset(&m_instance, buffer_set, width*3);

    return (error == 0)?true:false;

}

// see 'Double-buffer applications' in https://www.intel.com/content/www/us/en/docs/programmable/683329/23-4/video-frame-reader-ip-functional-description.html
bool VVP_FR::Config(uint32_t BuffsetSet0_BaseAddr, uint32_t BuffsetSet1_BaseAddr, uint8_t Active_Buffer_Set, int width, int height){


    int error;

    VERBOSE_PRINTF("Frame Reader init ...\r\n");

    // clear frame buffer
    memset((void *)BuffsetSet0_BaseAddr, 0xFF, width*height*3);
    memset((void *)BuffsetSet1_BaseAddr, 0x7F, width*height*3);
    //vvp_vrf_change_test_pattern();


    // SOP see
    // in https://www.intel.com/content/www/us/en/docs/programmable/683329/23-3/video-frame-reader-ip-functional-description.html

    //////////////////////
    // config buffer set
    // Configure the IP with the number of buffer sets you want
    //if (error == 0)
       error = intel_vvp_vfr_set_num_buffer_sets(&m_instance, 2); // two buffer set (one buffer in one buffer set)

    ///////////////////////////
    // config buffer set 0 and 1
    //  For each set, program the number of buffers, the base address and buffer offsets, the buffer dimensions and colorspace information into the buffer set registers.
    InitFrame(BuffsetSet0_BaseAddr, 0, width, height);
    InitFrame(BuffsetSet1_BaseAddr, 1, width, height);

    // configure

    // Set the csr_buffer_mode register to multiple buffer sets mode,
    // where the IP automatically reads buffers from the next set
    // when the IP produces the last buffer from the current set.
    // Otherwise, the IP reads buffers from the current set only.
    // my choice: IP reads buffers from the current set only

    // CSR_BUFFER_MODE = 0 (to prevent the IP automatically advancing to the buffer in set 1 before the host writes the new frame)
    if (error == 0)
    	error = intel_vvp_vfr_set_buffer_mode(&m_instance, kIntelVvpVfrSingleSet   ); //  A single set of buffers is used, 0 = Read buffers in STARTING_BUFFER_SET only.

    // Program the csr_starting_buffer_set register and
    // set the LSB of the csr_run register and
    // the IP starts producing output frames as soon as you write to the csr_commit register.
    if (error == 0)
        error = intel_vvp_vfr_set_starting_buffer_set(&m_instance, Active_Buffer_Set); // selects the first active buffer set when a read starts

    // CSR_RUN = 1 (the IP continuously produces all frames from the current set, in this case it is just one frame)
    if (error == 0)
        error = intel_vvp_vfr_set_run_mode(&m_instance, kIntelVvpVfrFreeRunning); // RUN=1, free run. process through the buffer sets and the buffers within the buffer sets in a round-robin fashion.

    if (error == 0)
        error = intel_vvp_vfr_commit_writes(&m_instance);

    if (error == 0){
    	VERBOSE_PRINTF("Frame Reader go with buffer_set %d... \r\n", Active_Buffer_Set);
    }else{
    	VERBOSE_PRINTF("Frame Reader configure failed!!!\r\n");
    }

    return (error == 0)?true:false;


}



bool VVP_FR::Run(bool bRun){
	int error;

	if (!m_bValidInstance){
		printf("FR invalid instance\r\n");
		return false;
	}

	error = intel_vvp_vfr_set_run_mode(&m_instance, bRun?kIntelVvpVfrFreeRunning:kIntelVvpVfrStop);

    return (error == 0)?true:false;

}

bool VVP_FR::IsLiteMode(){
	bool bYes;

	if (!m_bValidInstance){
		printf("FB invalid instance\r\n");
		return false;
	}
	bYes = intel_vvp_vfr_get_lite_mode(&m_instance);
	return bYes;
}

//// https://www.intel.com/content/www/us/en/docs/programmable/683329/23-4/video-frame-reader-ip-functional-description.html
bool VVP_FR::Switch(uint8_t buffset_set){
    int error;



    // CSR_BUFFER_MODE = 0 (to prevent the IP automatically advancing to the buffer in set 1 before the host writes the new frame)
   // if (error == 0)
  //      error = intel_vvp_vfr_set_buffer_mode(&instance, kIntelVvpVfrSingleSet   ); //  A single set of buffers is used, 0 = Read buffers in STARTING_BUFFER_SET only.

    //if (error == 0)
    error = intel_vvp_vfr_set_starting_buffer_set(&m_instance, buffset_set); // selects the first active buffer set when a read starts

    // must set !!! (missed in user manual)
    // CSR_RUN = 1 (the IP continuously produces all frames from the current set, in this case it is just one frame)
    if (error == 0)
        error = intel_vvp_vfr_set_run_mode(&m_instance, kIntelVvpVfrFreeRunning); // RUN=1, free run. process through the buffer sets and the buffers within the buffer sets in a round-robin fashion.

    if (error == 0)
        error = intel_vvp_vfr_commit_writes(&m_instance);

    if (error == 0){
        VERBOSE_PRINTF("Switch buffset_set to %d \r\n", buffset_set);
    }else{
        VERBOSE_PRINTF("failed to switch buffer_set!!!\r\n");
    }

    return (error == 0)?true:false;

}
