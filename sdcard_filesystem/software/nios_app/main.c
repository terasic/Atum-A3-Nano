#include <stdio.h>
#include <stdbool.h>
#include "ff.h"
#include "diskio.h"
#include ".\terasic_sdcard\terasic_includes.h"
#include ".\terasic_sdcard\sd_lib.h"

//http://elm-chan.org/fsw/ff/
//http://elm-chan.org/fsw/ff/res/app1.c
// http://elm-chan.org/fsw/ff/doc/mount.html
// http://elm-chan.org/fsw/ff/doc/open.html
bool ff_test_read(void){
	bool bSuccess = false;
    FRESULT fr;
    FATFS fs;
    FIL fil;
    char line[100];
    char szFilename[] = "readme.txt";

    /* Open or create a log file and ready to append */
    fr = f_mount(&fs, "", 0); /* Mount the default drive */

    if (fr == FR_OK){
    	fr = f_open(&fil, szFilename, FA_OPEN_EXISTING | FA_READ);

    	if (fr != FR_OK){
    		printf("failed to open file - %s\r\n", szFilename);
    	}else{
    		bSuccess = true;
    		printf("dump  file - %s\r\n", szFilename);

    	    /* Read every line and display it */
    	    while (f_gets(line, sizeof(line), &fil)) {
    	        printf("%s", line);
    	    }
    	    printf("\r\n");

        	/* Close the file */
        	f_close(&fil);

    	}


    	f_unmount("");                 /* Unmount the default drive */
    }

    return bSuccess;
}

//http://elm-chan.org/fsw/ff/
int ff_test_write(void){
	bool bSuccess = false;
    FRESULT fr;
    FATFS fs;
    FIL fil;
    char write_content[]="Write Test!";
    char szFilename[] = "create.txt";
    UINT len;

    /* Open or create a log file and ready to append */
    fr = f_mount(&fs, "", 0); /* Mount the default drive */

    if (fr == FR_OK){
    	fr = f_open(&fil, szFilename, FA_CREATE_ALWAYS | FA_WRITE);

    	if (fr != FR_OK){
    		printf("failed to create file - %s\r\n", szFilename);
    	}else{
    		fr = f_write(&fil, write_content, strlen(write_content), &len);
        	if (fr != FR_OK){
        		printf("failed to write file - %s\r\n", szFilename);
        	}else{
        		fr = f_sync (&fil);
        		if ((fr == FR_OK) && (strlen(write_content) == len))
        			bSuccess = true;

        	}

        	/* Close the file */
        	f_close(&fil);
    	}


    	f_unmount("");                 /* Unmount the default drive */
    }

    return bSuccess;
}





// http://elm-chan.org/fsw/ff/00index_e.html
//http://elm-chan.org/fsw/ff/res/app1.c
int main(void){
	bool bSuccess;

	printf("========== SDCARD File System Demo ==========\n");

	SDLIB_Init();


	printf("========== ff_test_read ==========\n");
	bSuccess = ff_test_read();
	printf("read test: %s\r\n", bSuccess?"pass":"ng");

	printf("========== ff_test_write ==========\n");
	bSuccess = ff_test_write();
	printf("write test: %s\r\n", bSuccess?"pass":"ng");



	return 0;

}
