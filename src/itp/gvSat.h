/****************************************************************************
  FileName     [ sat.h ]
  PackageName  [ sat ]
  Synopsis     [ Define miniSat solver interface functions ]
  Author       [ Chung-Yang (Ric) Huang, Cheng-Yin Wu ]
  Copyright    [ Copyleft(c) 2010-2014 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef GV_SAT_H
#define GV_SAT_H

#include "SolverV.h"
#include "gvBitVec.h"
#include "gvNtk.h"
#include <cassert>
#include <iostream>
#include <vector>
using namespace std;

class SATMgr;

/********** MiniSAT_Solver **********/
class GVSatSolver
{
        friend class SATMgr;

    public:
        GVSatSolver(GVNtkMgr*);
        ~GVSatSolver();

        void       reset();
        void       assumeRelease();
        void       assumeProperty(const size_t&, const bool&);
        void       assertProperty(const size_t&, const bool&);
        void       assumeProperty(const GVNetId& id, const bool& invert, const uint32_t& depth);
        void       assertProperty(const GVNetId& id, const bool& invert, const uint32_t& depth);
        const bool simplify();
        const bool solve();
        const bool assump_solve();
        int        getNumClauses() const { return _solver->nRootCla(); }

        // Network to Solver Functions
        const size_t        getFormula(const GVNetId&, const uint32_t&);
        const GVBitVecX     getDataValue(const GVNetId&, const uint32_t&) const;
        const bool          getDataValue(const size_t&) const;
        // Variable Interface Functions
        inline const size_t reserveFormula() { return getPosVar(newVar()); }
        inline const bool   isNegFormula(const size_t& v) const { return (v & 1ul); }
        inline const size_t getNegFormula(const size_t& v) const { return (v ^ 1ul); }

        // Gate Formula to Solver Functions
        void add_FALSE_Formula(const GVNetId&, const uint32_t&);
        void add_PI_Formula(const GVNetId&, const uint32_t&);
        void add_FF_Formula(const GVNetId&, const uint32_t&);
        void add_AND_Formula(const GVNetId&, const uint32_t&);

        void       addBoundedVerifyData(const GVNetId&, const uint32_t&);
        const bool existVerifyData(const GVNetId&, const uint32_t&);
        void       resizeNtkData(const uint32_t& num);

    private:
        const Var newVar();
        const Var getVerifyData(const GVNetId&, const uint32_t&) const;
        void      addBoundedVerifyDataRecursively(const GVNetId&, const uint32_t&);

        inline const Var    getOriVar(const size_t& v) const { return (Var)(v >> 1ul); }
        inline const size_t getPosVar(const Var& v) const { return (((size_t)v) << 1ul); }
        inline const size_t getNegVar(const Var& v) const { return ((getPosVar(v)) | 1ul); }

        SolverV*     _solver;  // Pointer to a Minisat solver
        Var          _curVar;  // Variable currently
        vec<Lit>     _assump;  // Assumption List for assumption solve
        GVNtkMgr*    _ntk;     // Network Under Verification
        vector<Var>* _ntkData; // Mapping between GVNetId and Solver Data
};

#endif // SAT_H
