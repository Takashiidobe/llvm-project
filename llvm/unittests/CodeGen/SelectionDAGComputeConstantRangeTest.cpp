//===- llvm/unittest/CodeGen/SelectionDAGComputeConstantRangeTest.cpp -----===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "SelectionDAGTestBase.h"
#include "llvm/IR/ConstantRange.h"

using namespace llvm;

class SelectionDAGComputeConstantRangeTest : public SelectionDAGTestBase {};

TEST_F(SelectionDAGComputeConstantRangeTest, UnsignedMinMax) {
  SDLoc DL;
  SDValue Op = DAG->getCopyFromReg(DAG->getEntryNode(), DL,
                                   Register::index2VirtReg(1), MVT::i32);
  SDValue Ten = DAG->getConstant(10, DL, MVT::i32);
  SDValue OneHundred = DAG->getConstant(100, DL, MVT::i32);

  SDValue UMin = DAG->getNode(ISD::UMIN, DL, MVT::i32, Op, OneHundred);
  EXPECT_EQ(ConstantRange(APInt(32, 0), APInt(32, 101)),
            DAG->computeConstantRange(UMin, /*ForSigned=*/false));

  SDValue UMax = DAG->getNode(ISD::UMAX, DL, MVT::i32, UMin, Ten);
  EXPECT_EQ(ConstantRange(APInt(32, 10), APInt(32, 101)),
            DAG->computeConstantRange(UMax, /*ForSigned=*/false));
}

TEST_F(SelectionDAGComputeConstantRangeTest, SignedMinMax) {
  SDLoc DL;
  SDValue Op = DAG->getCopyFromReg(DAG->getEntryNode(), DL,
                                   Register::index2VirtReg(1), MVT::i32);
  SDValue NegativeTen = DAG->getConstant(APInt(32, -10, true), DL, MVT::i32);
  SDValue OneHundred = DAG->getConstant(100, DL, MVT::i32);

  SDValue SMin = DAG->getNode(ISD::SMIN, DL, MVT::i32, Op, OneHundred);
  EXPECT_EQ(ConstantRange(APInt::getSignedMinValue(32), APInt(32, 101)),
            DAG->computeConstantRange(SMin, /*ForSigned=*/true));

  SDValue SMax = DAG->getNode(ISD::SMAX, DL, MVT::i32, SMin, NegativeTen);
  EXPECT_EQ(ConstantRange(APInt(32, -10, true), APInt(32, 101)),
            DAG->computeConstantRange(SMax, /*ForSigned=*/true));
}
