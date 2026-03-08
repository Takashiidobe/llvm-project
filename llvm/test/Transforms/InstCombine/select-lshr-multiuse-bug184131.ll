; RUN: opt < %s -passes='instcombine,early-cse,instcombine' -S | FileCheck %s

define i16 @or(i16 %x, i16 %y) {
; CHECK-LABEL: define i16 @or(
; CHECK-SAME: i16 [[X:%.*]], i16 [[Y:%.*]]) {
; CHECK-NEXT:    [[X_B8:%.*]] = and i16 [[X]], 256
; CHECK-NEXT:    [[CND_NOT:%.*]] = icmp eq i16 [[X_B8]], 0
; CHECK-NEXT:    [[SEL_XY:%.*]] = select i1 [[CND_NOT]], i16 [[Y]], i16 [[X]]
; CHECK-NEXT:    ret i16 [[SEL_XY]]
;
  %x_hi = lshr i16 %x, 8
  %cnd = trunc i16 %x_hi to i1
  %y_hi = lshr i16 %y, 8
  %sel_hi = select i1 %cnd, i16 %x_hi, i16 %y_hi
  %sel_xy = select i1 %cnd, i16 %x, i16 %y
  %lo = and i16 %sel_xy, 255
  %out_hi = shl nuw i16 %sel_hi, 8
  %out = or disjoint i16 %out_hi, %lo
  ret i16 %out
}
