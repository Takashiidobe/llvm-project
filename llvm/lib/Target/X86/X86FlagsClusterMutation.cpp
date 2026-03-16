//===- X86FlagsClusterMutation.cpp - X86 EFLAGS reorder pass --------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements a MachineFunctionPass that moves EFLAGS-producing
// instructions (LZCNT, TZCNT, POPCNT, NEG, BLSI, BLSR, BLSMSK) immediately
// before the CMP/TEST instruction that compares the same source register
// against zero, within the same basic block.
//
// Motivation
// ----------
// optimizeCompareInstr (X86InstrInfo.cpp) can eliminate a CMP/TEST that
// immediately follows an instruction like LZCNT which sets EFLAGS in a
// compatible way.  For patterns like i128 CTLZ expansion, a second LZCNT (for
// the low word) may appear between the high-word LZCNT and the CMP, blocking
// the elimination.  This pass moves the producer adjacent to the CMP so that
// the existing PeepholeOptimizer can fire without a second pass.
//
//===----------------------------------------------------------------------===//

#include "X86FlagsClusterMutation.h"
#include "X86InstrInfo.h"
#include "llvm/ADT/DenseSet.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/Support/Debug.h"

#define DEBUG_TYPE "x86-eflags-reorder"

using namespace llvm;

namespace {

class X86EFlagsReorder : public MachineFunctionPass {
public:
  static char ID;
  X86EFlagsReorder() : MachineFunctionPass(ID) {}

  StringRef getPassName() const override {
    return "X86 EFLAGS producer reorder";
  }

  bool runOnMachineFunction(MachineFunction &MF) override;
};

} // anonymous namespace

char X86EFlagsReorder::ID = 0;

bool X86EFlagsReorder::runOnMachineFunction(MachineFunction &MF) {
  const X86InstrInfo *TII =
      static_cast<const X86InstrInfo *>(MF.getSubtarget().getInstrInfo());
  MachineRegisterInfo &MRI = MF.getRegInfo();
  bool Changed = false;

  for (MachineBasicBlock &MBB : MF) {
    for (MachineInstr &CmpMI : MBB) {
      if (!CmpMI.isCompare())
        continue;

      Register SrcReg, SrcReg2;
      int64_t CmpMask, CmpValue;
      if (!TII->analyzeCompare(CmpMI, SrcReg, SrcReg2, CmpMask, CmpValue))
        continue;
      // Only handle comparisons against zero with a single source register.
      if (!SrcReg || SrcReg2 || CmpMask == 0 || CmpValue != 0)
        continue;
      if (!SrcReg.isVirtual())
        continue;

      // Find an EFLAGS-producing instruction that reads SrcReg. Iterating
      // use_nodbg_instructions is O(uses) which is typically 1-2 in SSA.
      for (MachineInstr &ProdMI : MRI.use_nodbg_instructions(SrcReg)) {
        auto [CC, SrcOpNo] = X86::isUseDefConvertible(ProdMI);
        (void)SrcOpNo;
        if (CC == X86::COND_INVALID)
          continue;
        if (&ProdMI == &CmpMI)
          continue;
        // Must be in the same basic block.
        if (ProdMI.getParent() != &MBB)
          continue;
        // Collect ProdMI and any instructions between it and CmpMI that
        // transitively depend on ProdMI's output registers. These must move
        // together to preserve def-use order (e.g. a COPY of the LZCNT result
        // sub-register that sits between the LZCNT and the TEST).
        DenseSet<Register> MovedDefs;
        for (const MachineOperand &MO : ProdMI.operands())
          if (MO.isReg() && MO.isDef() && MO.getReg().isVirtual())
            MovedDefs.insert(MO.getReg());

        // Chain: instructions between ProdMI and CmpMI that use a moved def.
        SmallVector<MachineInstr *, 4> Chain;
        Chain.push_back(&ProdMI);
        for (auto It = std::next(ProdMI.getIterator());
             It != CmpMI.getIterator(); ++It) {
          bool UsesMoved = false;
          for (const MachineOperand &MO : It->operands())
            if (MO.isReg() && MO.isUse() && MovedDefs.count(MO.getReg()))
              UsesMoved = true;
          if (!UsesMoved)
            continue;
          Chain.push_back(&*It);
          for (const MachineOperand &MO : It->operands())
            if (MO.isReg() && MO.isDef() && MO.getReg().isVirtual())
              MovedDefs.insert(MO.getReg());
        }

        // Already in place — nothing to do.
        if (Chain.size() == 1 &&
            std::next(ProdMI.getIterator()) == CmpMI.getIterator())
          break;

        LLVM_DEBUG({
          dbgs() << "X86EFlagsReorder: moving chain of " << Chain.size()
                 << " instr(s) before " << CmpMI;
        });
        for (MachineInstr *MI : Chain)
          MBB.splice(CmpMI.getIterator(), &MBB, MI->getIterator());
        Changed = true;
        break;
      }
    }
  }
  return Changed;
}

namespace llvm {

FunctionPass *createX86EFlagsReorderPass() { return new X86EFlagsReorder(); }

} // namespace llvm
