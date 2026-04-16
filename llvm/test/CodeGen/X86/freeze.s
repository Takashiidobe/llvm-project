	.att_syntax
	.file	"freeze.ll"
	.text
	.globl	freeze_int                      # -- Begin function freeze_int
	.prefalign	4, .Lfunc_end0, nop
	.type	freeze_int,@function
freeze_int:                             # @freeze_int
	.cfi_startproc
# %bb.0:
	xorl	%eax, %eax
	retq
.Lfunc_end0:
	.size	freeze_int, .Lfunc_end0-freeze_int
	.cfi_endproc
                                        # -- End function
	.globl	freeze_int2                     # -- Begin function freeze_int2
	.prefalign	4, .Lfunc_end1, nop
	.type	freeze_int2,@function
freeze_int2:                            # @freeze_int2
	.cfi_startproc
# %bb.0:
	xorl	%eax, %eax
	retq
.Lfunc_end1:
	.size	freeze_int2, .Lfunc_end1-freeze_int2
	.cfi_endproc
                                        # -- End function
	.globl	freeze_float                    # -- Begin function freeze_float
	.prefalign	4, .Lfunc_end2, nop
	.type	freeze_float,@function
freeze_float:                           # @freeze_float
	.cfi_startproc
# %bb.0:
	xorps	%xmm0, %xmm0
	retq
.Lfunc_end2:
	.size	freeze_float, .Lfunc_end2-freeze_float
	.cfi_endproc
                                        # -- End function
	.globl	freeze_half                     # -- Begin function freeze_half
	.prefalign	4, .Lfunc_end3, nop
	.type	freeze_half,@function
freeze_half:                            # @freeze_half
	.cfi_startproc
# %bb.0:
	xorps	%xmm0, %xmm0
	retq
.Lfunc_end3:
	.size	freeze_half, .Lfunc_end3-freeze_half
	.cfi_endproc
                                        # -- End function
	.globl	freeze_ivec                     # -- Begin function freeze_ivec
	.prefalign	4, .Lfunc_end4, nop
	.type	freeze_ivec,@function
freeze_ivec:                            # @freeze_ivec
	.cfi_startproc
# %bb.0:
	xorps	%xmm0, %xmm0
	retq
.Lfunc_end4:
	.size	freeze_ivec, .Lfunc_end4-freeze_ivec
	.cfi_endproc
                                        # -- End function
	.globl	freeze_ptr                      # -- Begin function freeze_ptr
	.prefalign	4, .Lfunc_end5, nop
	.type	freeze_ptr,@function
freeze_ptr:                             # @freeze_ptr
	.cfi_startproc
# %bb.0:
	movl	$4, %eax
	retq
.Lfunc_end5:
	.size	freeze_ptr, .Lfunc_end5-freeze_ptr
	.cfi_endproc
                                        # -- End function
	.globl	freeze_struct                   # -- Begin function freeze_struct
	.prefalign	4, .Lfunc_end6, nop
	.type	freeze_struct,@function
freeze_struct:                          # @freeze_struct
	.cfi_startproc
# %bb.0:
	xorl	%eax, %eax
	retq
.Lfunc_end6:
	.size	freeze_struct, .Lfunc_end6-freeze_struct
	.cfi_endproc
                                        # -- End function
	.globl	freeze_anonstruct               # -- Begin function freeze_anonstruct
	.prefalign	4, .Lfunc_end7, nop
	.type	freeze_anonstruct,@function
freeze_anonstruct:                      # @freeze_anonstruct
	.cfi_startproc
# %bb.0:
	xorl	%eax, %eax
	retq
.Lfunc_end7:
	.size	freeze_anonstruct, .Lfunc_end7-freeze_anonstruct
	.cfi_endproc
                                        # -- End function
	.globl	freeze_anonstruct2              # -- Begin function freeze_anonstruct2
	.prefalign	4, .Lfunc_end8, nop
	.type	freeze_anonstruct2,@function
freeze_anonstruct2:                     # @freeze_anonstruct2
	.cfi_startproc
# %bb.0:
	xorl	%eax, %eax
	retq
.Lfunc_end8:
	.size	freeze_anonstruct2, .Lfunc_end8-freeze_anonstruct2
	.cfi_endproc
                                        # -- End function
	.globl	freeze_array                    # -- Begin function freeze_array
	.prefalign	4, .Lfunc_end9, nop
	.type	freeze_array,@function
freeze_array:                           # @freeze_array
	.cfi_startproc
# %bb.0:
	xorl	%eax, %eax
	retq
.Lfunc_end9:
	.size	freeze_array, .Lfunc_end9-freeze_array
	.cfi_endproc
                                        # -- End function
	.globl	freeze_zext                     # -- Begin function freeze_zext
	.prefalign	4, .Lfunc_end10, nop
	.type	freeze_zext,@function
freeze_zext:                            # @freeze_zext
# %bb.0:                                # %entry
	movq	%rdi, %rax
	movl	%eax, %ecx
	movl	$3435973837, %edx               # imm = 0xCCCCCCCD
	imulq	%rcx, %rdx
	shrq	$35, %rdx
	addl	%edx, %edx
	leal	(%rdx,%rdx,4), %ecx
	subl	%ecx, %eax
                                        # kill: def $eax killed $eax killed $rax
	retq
.Lfunc_end10:
	.size	freeze_zext, .Lfunc_end10-freeze_zext
                                        # -- End function
	.globl	pr155345                        # -- Begin function pr155345
	.prefalign	4, .Lfunc_end11, nop
	.type	pr155345,@function
pr155345:                               # @pr155345
	.cfi_startproc
# %bb.0:                                # %entry
	movzbl	(%rdi), %eax
	
	xorl	%eax, %eax
	orb	$1, %dil
	movb	%dil, (%rdx)
	movzbl	%dil, %edx
	cmovel	%edx, %eax
	sete	%dil
	testb	$1, %sil
	cmovnel	%edx, %eax
	movb	%dl, (%rcx)
	movl	$1, %edx
	movl	%eax, %ecx
	shlq	%cl, %rdx
	orb	%sil, %dil
	movzbl	%dil, %eax
	andl	%edx, %eax
	andl	$1, %eax
	retq
.Lfunc_end11:
	.size	pr155345, .Lfunc_end11-pr155345
	.cfi_endproc
                                        # -- End function
	.section	".note.GNU-stack","",@progbits
