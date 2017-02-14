/**
 * bandwidth.S
 *
 * History:
 *    2016/5/25 - Cao Rongrong
 *
 * Copyright (c) 2016 Ambarella, Inc.
 *
 * This file and its contents ("Software") are protected by intellectual
 * property rights including, without limitation, U.S. and/or foreign
 * copyrights. This Software is also the confidential and proprietary
 * information of Ambarella, Inc. and its licensors. You may not use, reproduce,
 * disclose, distribute, modify, or otherwise prepare derivative works of this
 * Software or any portion thereof except pursuant to a signed license agreement
 * or nondisclosure agreement with Ambarella, Inc. or its authorized affiliates.
 * In the absence of such an agreement, you agree to promptly notify and return
 * this Software to Ambarella, Inc.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF NON-INFRINGEMENT,
 * MERCHANTABILITY, AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL AMBARELLA, INC. OR ITS AFFILIATES BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; COMPUTER FAILURE OR MALFUNCTION; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */
.text

/*-----------------------------------------------------------------------------
 * Name: 	GDMA
 * Purpose:	Performs GDMA bandwidth.
 *  x0 = copy size
 *-----------------------------------------------------------------------------*/
#define GMDA_MAX_WIDTH	(1 << 13)
.globl GDMA
GDMA:
	ldr	x13, =0xe0015000
	mov	x6, xzr
	mov	x7, x0
	mov	w1, #0x01000000
	mov	w2, #0x02000000
.L5:
	str	w1, [x13, #0x00]		/* source1base */
	str	w2, [x13, #0x10]		/* destinationbase */
	ldr	w0, =(GMDA_MAX_WIDTH - 1)
	str	w0, [x13, #0x18]		/* width */
	mov	w0, #0x800
	str	w0, [x13, #0x2c]		/* pixelformat */
	str	wzr, [x13, #0x30]		/* alpha */
	str	wzr, [x13, #0x24]		/* opcode */

wait_gdma:
	ldr	w0, [x13, #0x28]		/* numpendingops */
	cbnz	w0, wait_gdma

	add	x6, x6, #GMDA_MAX_WIDTH
	cmp	x6, x7
	b.lo	.L5

	ret



/*-----------------------------------------------------------------------------
 * Name: 	DMA
 * Purpose:	Performs DMA bandwidth.
 * Params:
 *	x0 = address
 *-----------------------------------------------------------------------------*/
.globl DMA
DMA:
	ldr	x13, =0xe0005300
	mov	x5, #0x01000000
	add	x5, x5, x0
	mov	x6, xzr
	mov	x7, #0x8000000

.L4:
	/* clear dma status reg */
	str	wzr, [x13, #0xc]
	/* set dma src reg */
	str	w0, [x13, #0x4]
	/* set dma dst reg */
	str	w5, [x13, #0x8]
	/* set dma ctr reg */
	ldr	w0, =0xb2800000
	add	w0, w0, #0x200000
	str	w0, [x13]

wait_dma:
	ldr	w2, [x13]
	tbnz	w2, #31, wait_dma

	add	x6, x6, #0x200000
	cmp	x6, x7
	b.lo	.L4

	ret




/*-----------------------------------------------------------------------------
 * Name: 	Reader
 * Purpose:	Performs sequential reads from memory, as fast as possible.
 * Params:
 *	x0 = address
 *	x1 = length, multiple of 256
 *	x2 = loop
 *-----------------------------------------------------------------------------*/
.globl Reader
Reader:
	bic	x1, x1, #0x7f
	mov	x4, x0
	mov	x5, x1

/*
 * x4 = temp address
 * x5 = temp length
 */

.L2:
	mov	x0, x4
	mov	x1, x5

.L3:
	ldp x3, x6, [x0]
	ldp x7, x8, [x0, #16]
	ldp x9, x10, [x0, #32]
	ldp x11, x12, [x0, #48]
	ldp x13, x14, [x0, #64]
	ldp x15, x16, [x0, #80]
	ldp x17, x18, [x0, #96]
	ldp x19, x20, [x0, #112]
	ldp x21, x6, [x0, #128]
	ldp x7, x8, [x0, #144]
	ldp x9, x10, [x0, #160]
	ldp x11, x12, [x0, #176]
	ldp x13, x14, [x0, #192]
	ldp x15, x16, [x0, #208]
	ldp x17, x18, [x0, #224]
	ldp x19, x20, [x0, #240]
	add x0, x0, #256

	sub	x1, x1, #256
	cbnz	x1, .L3

	sub	x2, x2, #1
	cbnz	x2, .L2

	ret



/*-----------------------------------------------------------------------------
 * Name: 	Writer
 * Purpose:	Performs sequential write into memory, as fast as possible.
 * Params:
 *	x0 = address
 *	x1 = length, multiple of 256
 *	x2 = loop
 *-----------------------------------------------------------------------------*/
.globl Writer
Writer:
	bic	x1, x1, #0x7f
	mov	x4, x0
	mov	x5, x1

/*
 * x4 = temp address
 * x5 = temp length
 */

.L0:
	mov	x0, x4
	mov	x1, x5

.L1:
	stp x3, x6, [x0]
	stp x7, x8, [x0, #16]
	stp x9, x10, [x0, #32]
	stp x11, x12, [x0, #48]
	stp x13, x14, [x0, #64]
	stp x15, x16, [x0, #80]
	stp x17, x18, [x0, #96]
	stp x19, x20, [x0, #112]
	stp x21, x6, [x0, #128]
	stp x7, x8, [x0, #144]
	stp x9, x10, [x0, #160]
	stp x11, x12, [x0, #176]
	stp x13, x14, [x0, #192]
	stp x15, x16, [x0, #208]
	stp x17, x18, [x0, #224]
	stp x19, x20, [x0, #240]
	add x0, x0, #256

	sub	x1, x1, #256
	cbnz	x1, .L1

	sub	x2, x2, #1
	cbnz	x2, .L0

	ret



