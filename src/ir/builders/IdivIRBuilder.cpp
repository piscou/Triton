#include <iostream>
#include <sstream>
#include <stdexcept>

#include <IdivIRBuilder.h>
#include <Registers.h>
#include <SMT2Lib.h>
#include <SymbolicElement.h>


IdivIRBuilder::IdivIRBuilder(uint64_t address, const std::string &disassembly):
  BaseIRBuilder(address, disassembly) {
}


void IdivIRBuilder::reg(AnalysisProcessor &ap, Inst &inst) const {
  SymbolicElement   *se;
  std::stringstream expr, result, dividend, divisor, mod;
  uint64_t          reg       = this->operands[0].getValue();
  uint32_t          regSize   = this->operands[0].getSize();

  /* Create the SMT semantic */
  divisor << ap.buildSymbolicRegOperand(reg, regSize);

  switch (regSize) {

    case 1:
      /* AX */
      dividend << ap.buildSymbolicRegOperand(ID_RAX, WORD_SIZE);
      /* res = AX / Source */
      result << smt2lib::bvsdiv(dividend.str(), smt2lib::zx(divisor.str(), BYTE_SIZE_BIT));
      /* mod = AX % Source */
      mod << smt2lib::bvsrem(dividend.str(), smt2lib::zx(divisor.str(), BYTE_SIZE_BIT));
      /* AL = res */
      /* AH = mod */
      expr << smt2lib::concat(
                smt2lib::extract(7, 0, result.str()), /* AL = res */
                smt2lib::extract(7, 0, mod.str())     /* AH = mod */
              );
      /* Create the symbolic element */
      se = ap.createRegSE(inst, expr, ID_RAX, WORD_SIZE);
      /* Apply the taint */
      ap.aluSpreadTaintRegReg(se, ID_RAX, reg);
      break;

    case 2:
      /* DX:AX */
      dividend << smt2lib::concat(ap.buildSymbolicRegOperand(ID_RDX, WORD_SIZE), ap.buildSymbolicRegOperand(ID_RAX, WORD_SIZE));
      /* res = DX:AX / Source */
      result << smt2lib::extract(15, 0, smt2lib::bvsdiv(dividend.str(), smt2lib::zx(divisor.str(), WORD_SIZE_BIT)));
      /* mod = DX:AX % Source */
      mod << smt2lib::extract(15, 0, smt2lib::bvsrem(dividend.str(), smt2lib::zx(divisor.str(), WORD_SIZE_BIT)));
      /* Create the symbolic element for AX */
      se = ap.createRegSE(inst, result, ID_RAX, WORD_SIZE);
      /* Apply the taint for AX */
      ap.aluSpreadTaintRegReg(se, ID_RAX, reg);
      /* Create the symbolic element for DX */
      se = ap.createRegSE(inst, mod, ID_RDX, WORD_SIZE);
      /* Apply the taint for DX */
      ap.aluSpreadTaintRegReg(se, ID_RDX, reg);
      break;

    case 4:
      /* EDX:EAX */
      dividend << smt2lib::concat(ap.buildSymbolicRegOperand(ID_RDX, DWORD_SIZE), ap.buildSymbolicRegOperand(ID_RAX, DWORD_SIZE));
      /* res = EDX:EAX / Source */
      result << smt2lib::extract(31, 0, smt2lib::bvsdiv(dividend.str(), smt2lib::zx(divisor.str(), DWORD_SIZE_BIT)));
      /* mod = EDX:EAX % Source */
      mod << smt2lib::extract(31, 0, smt2lib::bvsrem(dividend.str(), smt2lib::zx(divisor.str(), DWORD_SIZE_BIT)));
      /* Create the symbolic element for EAX */
      se = ap.createRegSE(inst, result, ID_RAX, DWORD_SIZE);
      /* Apply the taint for EAX */
      ap.aluSpreadTaintRegReg(se, ID_RAX, reg);
      /* Create the symbolic element for EDX */
      se = ap.createRegSE(inst, mod, ID_RDX, DWORD_SIZE);
      /* Apply the taint for EDX */
      ap.aluSpreadTaintRegReg(se, ID_RDX, reg);
      break;

    case 8:
      /* RDX:RAX */
      dividend << smt2lib::concat(ap.buildSymbolicRegOperand(ID_RDX, QWORD_SIZE), ap.buildSymbolicRegOperand(ID_RDX, QWORD_SIZE));
      /* res = RDX:RAX / Source */
      result << smt2lib::extract(63, 0, smt2lib::bvsdiv(dividend.str(), smt2lib::zx(divisor.str(), QWORD_SIZE_BIT)));
      /* mod = RDX:RAX % Source */
      mod << smt2lib::extract(63, 0, smt2lib::bvsrem(dividend.str(), smt2lib::zx(divisor.str(), QWORD_SIZE_BIT)));
      /* Create the symbolic element for RAX */
      se = ap.createRegSE(inst, result, ID_RAX, QWORD_SIZE);
      /* Apply the taint for RAX */
      ap.aluSpreadTaintRegReg(se, ID_RAX, reg);
      /* Create the symbolic element for RDX */
      se = ap.createRegSE(inst, mod, ID_RDX, QWORD_SIZE);
      /* Apply the taint for RDX */
      ap.aluSpreadTaintRegReg(se, ID_RDX, reg);
      break;
  }
}


void IdivIRBuilder::mem(AnalysisProcessor &ap, Inst &inst) const {
  SymbolicElement   *se;
  std::stringstream expr, result, dividend, divisor, mod;
  uint64_t          mem       = this->operands[0].getValue();
  uint32_t          memSize   = this->operands[0].getSize();

  /* Create the SMT semantic */
  divisor << ap.buildSymbolicMemOperand(mem, memSize);

  switch (memSize) {

    case 1:
      /* AX */
      dividend << ap.buildSymbolicRegOperand(ID_RAX, WORD_SIZE);
      /* res = AX / Source */
      result << smt2lib::bvsdiv(dividend.str(), smt2lib::zx(divisor.str(), BYTE_SIZE_BIT));
      /* mod = AX % Source */
      mod << smt2lib::bvsrem(dividend.str(), smt2lib::zx(divisor.str(), BYTE_SIZE_BIT));
      /* AL = res */
      /* AH = mod */
      expr << smt2lib::concat(
                smt2lib::extract(7, 0, result.str()), /* AL = res */
                smt2lib::extract(7, 0, mod.str())     /* AH = mod */
              );
      /* Create the symbolic element */
      se = ap.createRegSE(inst, expr, ID_RAX, WORD_SIZE);
      /* Apply the taint */
      ap.aluSpreadTaintRegMem(se, ID_RAX, mem, memSize);
      break;

    case 2:
      /* DX:AX */
      dividend << smt2lib::concat(ap.buildSymbolicRegOperand(ID_RDX, WORD_SIZE), ap.buildSymbolicRegOperand(ID_RAX, WORD_SIZE));
      /* res = DX:AX / Source */
      result << smt2lib::extract(15, 0, smt2lib::bvsdiv(dividend.str(), smt2lib::zx(divisor.str(), WORD_SIZE_BIT)));
      /* mod = DX:AX % Source */
      mod << smt2lib::extract(15, 0, smt2lib::bvsrem(dividend.str(), smt2lib::zx(divisor.str(), WORD_SIZE_BIT)));
      /* Create the symbolic element for AX */
      se = ap.createRegSE(inst, result, ID_RAX, WORD_SIZE);
      /* Apply the taint for AX */
      ap.aluSpreadTaintRegMem(se, ID_RAX, mem, memSize);
      /* Create the symbolic element for DX */
      se = ap.createRegSE(inst, mod, ID_RDX, WORD_SIZE);
      /* Apply the taint for DX */
      ap.aluSpreadTaintRegMem(se, ID_RDX, mem, memSize);
      break;

    case 4:
      /* EDX:EAX */
      dividend << smt2lib::concat(ap.buildSymbolicRegOperand(ID_RDX, DWORD_SIZE), ap.buildSymbolicRegOperand(ID_RAX, DWORD_SIZE));
      /* res = EDX:EAX / Source */
      result << smt2lib::extract(31, 0, smt2lib::bvsdiv(dividend.str(), smt2lib::zx(divisor.str(), DWORD_SIZE_BIT)));
      /* mod = EDX:EAX % Source */
      mod << smt2lib::extract(31, 0, smt2lib::bvsrem(dividend.str(), smt2lib::zx(divisor.str(), DWORD_SIZE_BIT)));
      /* Create the symbolic element for EAX */
      se = ap.createRegSE(inst, result, ID_RAX, DWORD_SIZE);
      /* Apply the taint for EAX */
      ap.aluSpreadTaintRegMem(se, ID_RAX, mem, memSize);
      /* Create the symbolic element for EDX */
      se = ap.createRegSE(inst, mod, ID_RDX, DWORD_SIZE);
      /* Apply the taint for EDX */
      ap.aluSpreadTaintRegMem(se, ID_RDX, mem, memSize);
      break;

    case 8:
      /* RDX:RAX */
      dividend << smt2lib::concat(ap.buildSymbolicRegOperand(ID_RDX, QWORD_SIZE), ap.buildSymbolicRegOperand(ID_RDX, QWORD_SIZE));
      /* res = RDX:RAX / Source */
      result << smt2lib::extract(63, 0, smt2lib::bvsdiv(dividend.str(), smt2lib::zx(divisor.str(), QWORD_SIZE_BIT)));
      /* mod = RDX:RAX % Source */
      mod << smt2lib::extract(63, 0, smt2lib::bvsrem(dividend.str(), smt2lib::zx(divisor.str(), QWORD_SIZE_BIT)));
      /* Create the symbolic element for RAX */
      se = ap.createRegSE(inst, result, ID_RAX, QWORD_SIZE);
      /* Apply the taint for RAX */
      ap.aluSpreadTaintRegMem(se, ID_RAX, mem, memSize);
      /* Create the symbolic element for RDX */
      se = ap.createRegSE(inst, mod, ID_RDX, QWORD_SIZE);
      /* Apply the taint for RDX */
      ap.aluSpreadTaintRegMem(se, ID_RDX, mem, memSize);
      break;
  }
}


void IdivIRBuilder::imm(AnalysisProcessor &ap, Inst &inst) const {
  /* There is no <inc imm> available in x86 */
  OneOperandTemplate::stop(this->disas);
}


void IdivIRBuilder::none(AnalysisProcessor &ap, Inst &inst) const {
  /* There is no <inc none> available in x86 */
  OneOperandTemplate::stop(this->disas);
}


Inst *IdivIRBuilder::process(AnalysisProcessor &ap) const {
  this->checkSetup();

  Inst *inst = new Inst(ap.getThreadID(), this->address, this->disas);

  try {
    this->templateMethod(ap, *inst, this->operands, "IDIV");
    ap.incNumberOfExpressions(inst->numberOfElements()); /* Used for statistics */
    ControlFlow::rip(*inst, ap, this->nextAddress);
  }
  catch (std::exception &e) {
    delete inst;
    throw;
  }

  return inst;
}

