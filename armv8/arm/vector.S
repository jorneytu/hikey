/**
 * bld/arm64/vector.S
 *
 * History:
 *    2015/11/20 - [Jorney Tu] AArch64
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

.macro	exception_save
	stp	x29, x30, [sp, #-16]!
	stp	x27, x28, [sp, #-16]!
	stp	x25, x26, [sp, #-16]!
	stp	x23, x24, [sp, #-16]!
	stp	x21, x22, [sp, #-16]!
	stp	x19, x20, [sp, #-16]!
	stp	x17, x18, [sp, #-16]!
	stp	x15, x16, [sp, #-16]!
	stp	x13, x14, [sp, #-16]!
	stp	x11, x12, [sp, #-16]!
	stp	x9, x10, [sp, #-16]!
	stp	x7, x8, [sp, #-16]!
	stp	x5, x6, [sp, #-16]!
	stp	x3, x4, [sp, #-16]!
	stp	x1, x2, [sp, #-16]!

	mrs	x2, elr_el3
	stp	x2, x0, [sp, #-16]!
.endm

.macro	exception_restore
	ldp x2, x0, [sp], #16
	msr elr_el3, x2
	ldp x1, x2, [sp], #16
	ldp x3, x4, [sp], #16
	ldp x5, x6, [sp], #16
	ldp x7, x8, [sp], #16
	ldp x9, x10, [sp], #16
	ldp x11, x12, [sp], #16
	ldp x13, x14, [sp], #16
	ldp x15, x16, [sp], #16
	ldp x17, x18, [sp], #16
	ldp x19, x20, [sp], #16
	ldp x21, x22, [sp], #16
	ldp x23, x24, [sp], #16
	ldp x25, x27, [sp], #16
	ldp x27, x28, [sp], #16
	ldp x29, x30, [sp], #16
.endm

arm_irq_handle:
	exception_save
	bl do_irq_handle
	exception_restore
	eret
__sync_handle:
	exception_save
	bl do_irq_handle
	exception_restore
	eret

.align 11

.global __vectors

__vectors:

/* Current EL with SP0 */

do_thread_sync:
	b	.
	.balign 0x80
do_thread_irq:
	b	.
	.balign 0x80
do_thread_fiq:
	b	.
	.balign 0x80
do_thread_error:
	b	.
	.balign 0x80

/* Current EL with ELX_SP */

do_sync:
	b	__sync_handle
	.balign 0x80
do_irq:
	b 	arm_irq_handle
	.balign 0x80
do_fiq:
	b	.
	.balign 0x80
do_error:
	b	.
	.balign 0x80

/* lower EL using AArch64 (lower exception switch to high) */

l64_do_sync:
	b	.
	.balign 0x80
l64_do_irq:
	b	.
	.balign 0x80
l64_do_fiq:
	b	.
	.balign 0x80
l64_do_error:
	b	.
	.balign 0x80

/* lower EL using AArch32 (lower exception switch to high) */

l32_do_sync:
	b	.
	.balign 0x80
l32_do_irq:
	b	.
	.balign 0x80
l32_do_fiq:
	b	.
	.balign 0x80
l32_do_error:
	b	.
	.balign 0x80
