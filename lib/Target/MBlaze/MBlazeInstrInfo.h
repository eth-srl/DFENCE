//===- MBlazeInstrInfo.h - MBlaze Instruction Information -------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the MBlaze implementation of the TargetInstrInfo class.
//
//===----------------------------------------------------------------------===//

#ifndef MBLAZEINSTRUCTIONINFO_H
#define MBLAZEINSTRUCTIONINFO_H

#include "MBlaze.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Target/TargetInstrInfo.h"
#include "MBlazeRegisterInfo.h"

namespace llvm {

namespace MBlaze {

  // MBlaze Branch Codes
  enum FPBranchCode {
    BRANCH_F,
    BRANCH_T,
    BRANCH_FL,
    BRANCH_TL,
    BRANCH_INVALID
  };

  // MBlaze Condition Codes
  enum CondCode {
    // To be used with float branch True
    FCOND_F,
    FCOND_UN,
    FCOND_EQ,
    FCOND_UEQ,
    FCOND_OLT,
    FCOND_ULT,
    FCOND_OLE,
    FCOND_ULE,
    FCOND_SF,
    FCOND_NGLE,
    FCOND_SEQ,
    FCOND_NGL,
    FCOND_LT,
    FCOND_NGE,
    FCOND_LE,
    FCOND_NGT,

    // To be used with float branch False
    // This conditions have the same mnemonic as the
    // above ones, but are used with a branch False;
    FCOND_T,
    FCOND_OR,
    FCOND_NEQ,
    FCOND_OGL,
    FCOND_UGE,
    FCOND_OGE,
    FCOND_UGT,
    FCOND_OGT,
    FCOND_ST,
    FCOND_GLE,
    FCOND_SNE,
    FCOND_GL,
    FCOND_NLT,
    FCOND_GE,
    FCOND_NLE,
    FCOND_GT,

    // Only integer conditions
    COND_E,
    COND_GZ,
    COND_GEZ,
    COND_LZ,
    COND_LEZ,
    COND_NE,
    COND_INVALID
  };

  // Turn condition code into conditional branch opcode.
  unsigned GetCondBranchFromCond(CondCode CC);

  /// GetOppositeBranchCondition - Return the inverse of the specified cond,
  /// e.g. turning COND_E to COND_NE.
  CondCode GetOppositeBranchCondition(MBlaze::CondCode CC);

  /// MBlazeCCToString - Map each FP condition code to its string
  inline static const char *MBlazeFCCToString(MBlaze::CondCode CC)
  {
    switch (CC) {
      default: llvm_unreachable("Unknown condition code");
      case FCOND_F:
      case FCOND_T:   return "f";
      case FCOND_UN:
      case FCOND_OR:  return "un";
      case FCOND_EQ:
      case FCOND_NEQ: return "eq";
      case FCOND_UEQ:
      case FCOND_OGL: return "ueq";
      case FCOND_OLT:
      case FCOND_UGE: return "olt";
      case FCOND_ULT:
      case FCOND_OGE: return "ult";
      case FCOND_OLE:
      case FCOND_UGT: return "ole";
      case FCOND_ULE:
      case FCOND_OGT: return "ule";
      case FCOND_SF:
      case FCOND_ST:  return "sf";
      case FCOND_NGLE:
      case FCOND_GLE: return "ngle";
      case FCOND_SEQ:
      case FCOND_SNE: return "seq";
      case FCOND_NGL:
      case FCOND_GL:  return "ngl";
      case FCOND_LT:
      case FCOND_NLT: return "lt";
      case FCOND_NGE:
      case FCOND_GE:  return "ge";
      case FCOND_LE:
      case FCOND_NLE: return "nle";
      case FCOND_NGT:
      case FCOND_GT:  return "gt";
    }
  }
}

/// MBlazeII - This namespace holds all of the target specific flags that
/// instruction info tracks.
///
namespace MBlazeII {
  /// Target Operand Flag enum.
  enum TOF {
    //===------------------------------------------------------------------===//
    // MBlaze Specific MachineOperand flags.
    MO_NO_FLAG,

    /// MO_GOT - Represents the offset into the global offset table at which
    /// the address the relocation entry symbol resides during execution.
    MO_GOT,

    /// MO_GOT_CALL - Represents the offset into the global offset table at
    /// which the address of a call site relocation entry symbol resides
    /// during execution. This is different from the above since this flag
    /// can only be present in call instructions.
    MO_GOT_CALL,

    /// MO_GPREL - Represents the offset from the current gp value to be used
    /// for the relocatable object file being produced.
    MO_GPREL,

    /// MO_ABS_HILO - Represents the hi or low part of an absolute symbol
    /// address.
    MO_ABS_HILO

  };
}

class MBlazeInstrInfo : public TargetInstrInfoImpl {
  MBlazeTargetMachine &TM;
  const MBlazeRegisterInfo RI;
public:
  explicit MBlazeInstrInfo(MBlazeTargetMachine &TM);

  /// getRegisterInfo - TargetInstrInfo is a superset of MRegister info.  As
  /// such, whenever a client has an instance of instruction info, it should
  /// always be able to get register info as well (through this method).
  ///
  virtual const MBlazeRegisterInfo &getRegisterInfo() const { return RI; }

  /// Return true if the instruction is a register to register move and return
  /// the source and dest operands and their sub-register indices by reference.
  virtual bool isMoveInstr(const MachineInstr &MI,
                           unsigned &SrcReg, unsigned &DstReg,
                           unsigned &SrcSubIdx, unsigned &DstSubIdx) const;

  /// isLoadFromStackSlot - If the specified machine instruction is a direct
  /// load from a stack slot, return the virtual or physical register number of
  /// the destination along with the FrameIndex of the loaded stack slot.  If
  /// not, return 0.  This predicate must return 0 if the instruction has
  /// any side effects other than loading from the stack slot.
  virtual unsigned isLoadFromStackSlot(const MachineInstr *MI,
                                       int &FrameIndex) const;

  /// isStoreToStackSlot - If the specified machine instruction is a direct
  /// store to a stack slot, return the virtual or physical register number of
  /// the source reg along with the FrameIndex of the loaded stack slot.  If
  /// not, return 0.  This predicate must return 0 if the instruction has
  /// any side effects other than storing to the stack slot.
  virtual unsigned isStoreToStackSlot(const MachineInstr *MI,
                                      int &FrameIndex) const;

  /// Branch Analysis
  virtual unsigned InsertBranch(MachineBasicBlock &MBB, MachineBasicBlock *TBB,
                                MachineBasicBlock *FBB,
                            const SmallVectorImpl<MachineOperand> &Cond) const;
  virtual bool copyRegToReg(MachineBasicBlock &MBB,
                            MachineBasicBlock::iterator I,
                            unsigned DestReg, unsigned SrcReg,
                            const TargetRegisterClass *DestRC,
                            const TargetRegisterClass *SrcRC) const;
  virtual void storeRegToStackSlot(MachineBasicBlock &MBB,
                                   MachineBasicBlock::iterator MBBI,
                                   unsigned SrcReg, bool isKill, int FrameIndex,
                                   const TargetRegisterClass *RC) const;

  virtual void loadRegFromStackSlot(MachineBasicBlock &MBB,
                                    MachineBasicBlock::iterator MBBI,
                                    unsigned DestReg, int FrameIndex,
                                    const TargetRegisterClass *RC) const;

  virtual MachineInstr* foldMemoryOperandImpl(MachineFunction &MF,
                                              MachineInstr* MI,
                                           const SmallVectorImpl<unsigned> &Ops,
                                              int FrameIndex) const;

  virtual MachineInstr* foldMemoryOperandImpl(MachineFunction &MF,
                                              MachineInstr* MI,
                                           const SmallVectorImpl<unsigned> &Ops,
                                              MachineInstr* LoadMI) const {
    return 0;
  }

  /// Insert nop instruction when hazard condition is found
  virtual void insertNoop(MachineBasicBlock &MBB,
                          MachineBasicBlock::iterator MI) const;

  /// getGlobalBaseReg - Return a virtual register initialized with the
  /// the global base register value. Output instructions required to
  /// initialize the register in the function entry block, if necessary.
  ///
  unsigned getGlobalBaseReg(MachineFunction *MF) const;
};

}

#endif
