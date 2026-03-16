//===- X86FlagsClusterMutation.h - X86 EFLAGS reorder pass ----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
/// \file Declares the X86 EFLAGS producer reorder pass, which moves
/// EFLAGS-producing instructions (LZCNT, TZCNT, POPCNT, NEG, BLS*)
/// immediately before the CMP/TEST-against-zero that consumes the same
/// register, so that the existing PeepholeOptimizer can eliminate the
/// redundant CMP/TEST via optimizeCompareInstr.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_X86_X86FLAGSCLUSTERMUTATION_H
#define LLVM_LIB_TARGET_X86_X86FLAGSCLUSTERMUTATION_H

namespace llvm {

class FunctionPass;

/// Create a pass that moves EFLAGS producers immediately before their
/// CMP/TEST-against-zero consumers within each basic block.
FunctionPass *createX86EFlagsReorderPass();

} // namespace llvm

#endif // LLVM_LIB_TARGET_X86_X86FLAGSCLUSTERMUTATION_H
