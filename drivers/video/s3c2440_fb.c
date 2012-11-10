/*
 * (C) Copyright 2006 by OpenMoko, Inc.
 * Author: Harald Welte <laforge@openmoko.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>

#if defined(CONFIG_VIDEO_S3C2440)

#include <video_fb.h>
#include "videomodes.h"
#include <s3c2440.h>
/*
 * Export Graphic Device
 */
GraphicDevice smi;

#define VIDEO_MEM_SIZE	0x25800; /* 240x320x16bit  */
extern void board_video_init(GraphicDevice *pGD);

/*******************************************************************************
 *
 * Init video chip with common Linux graphic modes (lilo)
 */
void *video_hw_init (void)
{
	S3C24X0_LCD * const lcd = S3C24X0_GetBase_LCD();
	GraphicDevice *pGD = (GraphicDevice *)&smi;

	/* Search for video chip */
	printf("Video: 240x320x16bit\n");

	/* statically configure settings */
	pGD->winSizeX = pGD->plnSizeX = 240;
	pGD->winSizeY = pGD->plnSizeY = 320;
	pGD->gdfBytesPP = 2;
	pGD->gdfIndex = GDF_16BIT_565RGB;

	pGD->frameAdrs = LCD_VIDEO_ADDR;
	pGD->memSize = VIDEO_MEM_SIZE;

	board_video_init(pGD);

	lcd->LCDSADDR1 = pGD->frameAdrs >> 1;

	/* This marks the end of the frame buffer. */
	lcd->LCDSADDR2 = (lcd->LCDSADDR1&0x1fffff) + (pGD->winSizeX+0) * pGD->winSizeY;
	lcd->LCDSADDR3 = pGD->winSizeX;

	/* Clear video memory */
	memset(pGD->frameAdrs, 0, pGD->memSize);

	/* Enable  Display  */
	lcd->LCDCON1 |= 0x01;	/* ENVID = 1 */

	return ((void*)&smi);
}

void
video_set_lut (unsigned int index,	/* color number */
	       unsigned char r,	/* red */
	       unsigned char g,	/* green */
	       unsigned char b	/* blue */
    )
{
}

#endif /* CONFIG_VIDEO_S3C2410 */
