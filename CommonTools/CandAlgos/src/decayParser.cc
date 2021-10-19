// -*- C++ -*-
//
// Package:     CandCombiner
// Class  :     decayParser
//
// Implementation:
//     <Notes on implementation>
//
// Original Author:
//         Created:  Sun Aug  7 20:26:31 EDT 2005
// $Id: decayParser.cc,v 1.1 2009/03/03 13:50:55 llista Exp $
//
#include "CommonTools/CandAlgos/interface/decayParser.h"

#include <boost/spirit/home/x3.hpp>

#include <vector>

using namespace boost::spirit;
using namespace std;
using namespace boost::spirit::x3;
namespace cand {
  namespace parser {

    class ModeSetter {
    public:
      ModeSetter(vector<ConjInfo>& iVect, ConjInfo::Mode iMode) : conjInfo_(iVect), mode_(iMode) {}
      void operator()(const char) const { conjInfo_.back().mode_ = mode_; }

    private:
      vector<ConjInfo>& conjInfo_;
      ConjInfo::Mode mode_;
    };

    //typedef scanner_policies<skip_parser_iteration_policy<nothing_parser, iteration_policy>, match_policy, action_policy>
      //  ScannerPolicy;
    //typedef scanner<const char*, ScannerPolicy> ScannerUsed_1;
    //typedef rule<ScannerUsed_1> Rule_1;

    bool decayParser(const std::string& iValue, vector<ConjInfo>& oStrings) {
      using namespace boost::spirit;
      auto push_back = [&](auto& ctx){ oStrings.push_back(_attr(ctx)); };

      auto label = ((+alnum) >> *char_(':') >> *char_('_') >> *alnum)[push_back];
      auto conj = (char_('@') >> !((char_('b') >> char_('a') >> char_('r')[ModeSetter(oStrings, ConjInfo::kBar)]) |
                                    char_('+')[ModeSetter(oStrings, ConjInfo::kPlus)] |
                                    char_('-')[ModeSetter(oStrings, ConjInfo::kMinus)]));

      return parse(iValue.c_str(), ((label >> !conj) % blank), eps(false)).full;
    }
  }  // namespace parser
}  // namespace cand
