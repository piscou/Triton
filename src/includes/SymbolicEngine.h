
#ifndef   SYMBOLICENGINE_H
#define   SYMBOLICENGINE_H

#include <cstdint>

#include <map>
#include <list>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
#include <sstream>

#include "SymbolicElement.h"
#include "SMT2Lib.h"
#include "Registers.h"

/* The name of the symbolic variables */
#define SYMVAR_NAME       "SymVar_"
#define SYMVAR_NAME_SIZE  (sizeof(SYMVAR_NAME) - 1)

const uint64_t UNSET = -1;


/* Symbolic Engine */
class SymbolicEngine {

  private:

    /* symbolic expression ID */
    uint64_t uniqueID;

    /* Number of symbolic variables used */
    uint64_t numberOfSymVar;

    /*
     * Addresses -> symbolic expression
     * item1: memory address
     * item2: symbolic reference ID
     */
    std::map<uint64_t, uint64_t> memoryReference;

    /*
     * Z3 Symbolic Addresse <-> variable
     */
    std::map<uint64_t, uint64_t> symVarMemoryReference;         // memory -> symbolic
    std::map<uint64_t, uint64_t> symVarMemoryReferenceInverse;  // symbolic -> memory
    std::map<uint64_t, uint64_t> symVarSizeReference;           // symbolic -> bitvec size

    /*
     * List of path constaints (PC).
     * Item = symbolic reference ID.
     *
     * When a branch instruction is executed,
     * it must add the PC in this list.
     */
    std::list<uint64_t> pathConstaints;

    /* List of variables decl in smt2lib */
    std::list<std::string> symVarDeclaration;

    /* List of symbolic elements ID */
    std::vector<SymbolicElement *> symbolicVector;

    /* Private classes used by getBacktrackedExpressionFromId() */
    std::string deepReplace(std::stringstream &formula);
    std::string replaceEq(std::string str, const std::string from, const std::string to);


  public:

    /* Symbolic trace */
    /* sizeof(symbolicReg) = enum REG */
    uint64_t              symbolicReg[ID_LAST_ITEM];

    /* public methods */
    SymbolicElement       *getElementFromId(uint64_t id);
    SymbolicElement       *newSymbolicElement(std::stringstream &src);
    SymbolicElement       *newSymbolicElement(std::stringstream &src, std::string comment);
    bool                  assignExprToSymVar(uint64_t exprId, uint64_t symVarId);
    bool                  convertExprToSymVar(uint64_t exprId, uint64_t symVarSize);
    std::list<uint64_t>   getPathConstraints(void);
    std::string           getBacktrackedExpressionFromId(uint64_t id);
    std::string           getSmt2LibVarsDecl();
    uint64_t              getMemSymbolicID(uint64_t addr);
    uint64_t              getRegSymbolicID(uint64_t regID);
    uint64_t              getUniqueID();
    uint64_t              getUniqueSymVarID();
    uint64_t              getMemoryFromSymVar(uint64_t symVar);
    uint64_t              getSymVarFromMemory(uint64_t mem);
    uint64_t              getSymVarSize(uint64_t symVarId);
    void                  concretizeMem(uint64_t mem);
    void                  concretizeReg(uint64_t regID);
    void                  addMemoryReference(uint64_t mem, uint64_t id);
    void                  addPathConstraint(uint64_t exprId);
    void                  addSmt2LibVarDecl(uint64_t symVarID, uint64_t size);
    void                  addSymVarMemoryReference(uint64_t mem, uint64_t symVarID);
    void                  init(const SymbolicEngine &other);
    void                  operator=(const SymbolicEngine &other);

    SymbolicEngine();
    SymbolicEngine(const SymbolicEngine &copy);
    ~SymbolicEngine();

};

#endif     /* !__SYMBOLICENGINE_H__ */

