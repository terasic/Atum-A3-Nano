/*
 * glue.h
 *
 *  Created on: May 22, 2025
 *      Author: User
 */

#ifndef GLUE_H_
#define GLUE_H_

DSTATUS MMC_disk_status(void);
DSTATUS MMC_disk_initialize(void);
DRESULT MMC_disk_read (
	BYTE *buff,		/* Data buffer to store read data */
	LBA_t sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
);
DRESULT MMC_disk_write (
	const BYTE *buff,	/* Data to be written */
	LBA_t sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
);

DSTATUS RAM_disk_status(void);
DSTATUS RAM_disk_initialize(void);

DRESULT RAM_disk_read (
	BYTE *buff,		/* Data buffer to store read data */
	LBA_t sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
);


DRESULT RAM_disk_write (
	const BYTE *buff,	/* Data to be written */
	LBA_t sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
);

DSTATUS USB_disk_status(void);
DSTATUS USB_disk_initialize(void);

DRESULT USB_disk_read (
	BYTE *buff,		/* Data buffer to store read data */
	LBA_t sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
);

DRESULT USB_disk_write (
	const BYTE *buff,	/* Data to be written */
	LBA_t sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
);




#endif /* GLUE_H_ */
