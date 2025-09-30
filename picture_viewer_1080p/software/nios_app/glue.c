/*
 * diskio_mmc.c
 *
 *  Created on: Nov 16, 2024
 *      Author: User
 */


/*-----------------------------------------------------------------------*/
/* Low level disk I/O module SKELETON for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"			/* Obtains integer types */
#include "diskio.h"		/* Declarations of disk functions */

#include <sys/alt_alarm.h>
#include <sys/times.h>
#include <stdbool.h>
#include ".\terasic_sdcard\terasic_includes.h"
#include ".\terasic_sdcard\sd_lib.h"

//http://elm-chan.org/fsw/ff/doc/fattime.html
DWORD get_fattime (void){	/* Get current time */
#if 1
	return 0;
#else
    struct  timeval    tv;
    struct  timezone   tz;
    gettimeofday(&tv,&tz);


	struct tms stm;
	clock_t t;
	DWORD value32 = 0;

	//https://www.intel.com/content/www/us/en/docs/programmable/683282/current/times.html
	t = times (&stm);
	if (t == 0){
		//no system clock available
	}else{
		value32 =  (DWORD)(stm.tm_year - 80) << 25 |
	           (DWORD)(stm.tm_mon + 1) << 21 |
	           (DWORD)stm.tm_mday << 16 |
	           (DWORD)stm.tm_hour << 11 |
	           (DWORD)stm.tm_min << 5 |
	           (DWORD)stm.tm_sec >> 1;
	}

	return value32;
#endif

}

DWORD systimer (void){
	DWORD time_ms;
	time_ms = alt_nticks()/alt_ticks_per_second()*1000; // ms

	return time_ms;
}



static DSTATUS gMMC_status = STA_NOINIT;


//http://elm-chan.org/fsw/ff/doc/dstat.html
DSTATUS MMC_disk_status(void){
	return gMMC_status;
}

//http://elm-chan.org/fsw/ff/doc/dinit.html
DSTATUS MMC_disk_initialize(void){
	bool bSuccess;

	bSuccess = SDLIB_Init();
	if (bSuccess)
		gMMC_status = 0; // success
	else
		gMMC_status = STA_NOINIT;

	return gMMC_status;
}

//http://elm-chan.org/fsw/ff/doc/dread.html
DRESULT MMC_disk_read (
	BYTE *buff,		/* Data buffer to store read data */
	LBA_t sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
){
	bool bSuccess=true;
	int i;

	for(i=0;i<count && bSuccess;i++){
		bSuccess = SDLIB_ReadBlock512(sector+i, buff + 512*i);
	}

	return bSuccess?RES_OK:RES_ERROR;
}

//http://elm-chan.org/fsw/ff/doc/dwrite.html
DRESULT MMC_disk_write (
	const BYTE *buff,	/* Data to be written */
	LBA_t sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
){
	bool bSuccess=true;
	int i;

	for(i=0;i<count && bSuccess;i++){
		bSuccess = SDLIB_WriteBlock512(sector+i, buff + 512*i);
	}

	return bSuccess?RES_OK:RES_ERROR;
}

DSTATUS RAM_disk_status(void){

}

DSTATUS RAM_disk_initialize(void){

}

DRESULT RAM_disk_read (
	BYTE *buff,		/* Data buffer to store read data */
	LBA_t sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
){

}

DRESULT RAM_disk_write (
	const BYTE *buff,	/* Data to be written */
	LBA_t sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
){

}


DSTATUS USB_disk_status(void){

}

DSTATUS USB_disk_initialize(void){

}

DRESULT USB_disk_read (
	BYTE *buff,		/* Data buffer to store read data */
	LBA_t sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
){

}

DRESULT USB_disk_write (
	const BYTE *buff,	/* Data to be written */
	LBA_t sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
){

}




