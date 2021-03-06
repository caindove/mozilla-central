/*  GRAPHITE2 LICENSING

    Copyright 2010, SIL International
    All rights reserved.

    This library is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published
    by the Free Software Foundation; either version 2.1 of License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should also have received a copy of the GNU Lesser General Public
    License along with this library in the file named "LICENSE".
    If not, write to the Free Software Foundation, 51 Franklin Street, 
    Suite 500, Boston, MA 02110-1335, USA or visit their web page on the 
    internet at http://www.fsf.org/licenses/lgpl.html.

Alternatively, the contents of this file may be used under the terms of the
Mozilla Public License (http://mozilla.org/MPL) or the GNU General Public
License, as published by the Free Software Foundation, either version 2
of the License or (at your option) any later version.
*/

#pragma once

#include "inc/Code.h"
#include "inc/Slot.h"

namespace graphite2 {

struct Rule {
  const vm::Machine::Code * constraint, 
                 * action;
  unsigned short   sort;
  byte             preContext;
#ifndef NDEBUG
  uint16           rule_idx;
#endif

  Rule() : constraint(0), action(0) {}
  ~Rule();

  CLASS_NEW_DELETE;
};

inline Rule::~Rule()
{
  delete constraint;
  delete action;
}


struct RuleEntry
{
  const Rule   * rule;

  inline bool operator < (const RuleEntry &r) const
  { 
    const unsigned short lsort = rule->sort, rsort = r.rule->sort; 
    return lsort > rsort || (lsort == rsort && rule < r.rule);
  }
  
  inline bool operator == (const RuleEntry &r) const
  {
    return rule == r.rule;
  }
};


struct State
{
  const RuleEntry     * rules,
                      * rules_end;
  const State * const * transitions;
  
  size_t size() const;
  bool   is_success() const;
  bool   is_transition() const;
#ifndef NDEBUG
    uint32 index;
#endif
};

inline size_t State::size() const 
{
  return rules_end - rules;
}

inline bool State::is_success() const
{
  return (rules != NULL);
}

inline bool State::is_transition() const
{
  return (transitions != NULL);
}


class SlotMap
{
public:
  enum {MAX_SLOTS=64};
  SlotMap(Segment & seg);
  
  Slot       * * begin();
  Slot       * * end();
  size_t         size() const;
  unsigned short context() const;
  void           reset(Slot &, unsigned short);
  
  Slot * const & operator[](int n) const;
  Slot       * & operator [] (int);
  void           pushSlot(Slot * const slot);
  void			 collectGarbage();

  Slot         * highwater() { return m_highwater; }
  void           highwater(Slot *s) { m_highwater = s; m_highpassed = false; }
  bool			 highpassed() const { return m_highpassed; }
  void			 highpassed(bool v) { m_highpassed = v; }

  Segment &    segment;
private:
  Slot         * m_slot_map[MAX_SLOTS+1];
  unsigned short m_size;
  unsigned short m_precontext;
  Slot         * m_highwater;
  bool			 m_highpassed;
};


class FiniteStateMachine
{
public:
  enum {MAX_RULES=128};

private:
  class Rules
  {
  public:
      Rules();
      void              clear();
      const RuleEntry * begin() const;
      const RuleEntry * end() const;
      size_t            size() const;
      
      void accumulate_rules(const State &state);

  private:
      RuleEntry * m_begin, 
                * m_end,
                  m_rules[MAX_RULES*2];
  };

public:
  FiniteStateMachine(SlotMap & map);
  void      reset(Slot * & slot, const short unsigned int max_pre_ctxt);
  Rules     rules;
  SlotMap   & slots;
};

inline FiniteStateMachine::FiniteStateMachine(SlotMap& map)
: slots(map)
{
}

inline void FiniteStateMachine::reset(Slot * & slot, const short unsigned int max_pre_ctxt)
{
  rules.clear();
  int ctxt = 0;
  for (; ctxt != max_pre_ctxt && slot->prev(); ++ctxt, slot = slot->prev());
  slots.reset(*slot, ctxt);
}

inline FiniteStateMachine::Rules::Rules()
  : m_begin(m_rules)
{
  m_end = m_begin;
}

inline void FiniteStateMachine::Rules::clear() 
{
  m_end = m_begin;
}

inline const RuleEntry * FiniteStateMachine::Rules::begin() const
{
  return m_begin;
}

inline const RuleEntry * FiniteStateMachine::Rules::end() const
{
  return m_end;
}

inline size_t FiniteStateMachine::Rules::size() const
{
  return m_end - m_begin;
}

inline void FiniteStateMachine::Rules::accumulate_rules(const State &state)
{
  // Only bother if there are rules in the State object.
  if (state.size() == 0) return;
  
  // Merge the new sorted rules list into the current sorted result set.
  const RuleEntry * lre = begin(), * rre = state.rules;
  RuleEntry * out = m_rules + (m_begin == m_rules)*MAX_RULES;    
  const RuleEntry * lrend = out + MAX_RULES;
  m_begin = out; 
  while (lre != end() && out != lrend)
  {
    if (*lre < *rre)      *out++ = *lre++;
    else if (*rre < *lre) { *out++ = *rre++; }
    else                { *out++ = *lre++; ++rre; }

    if (rre == state.rules_end) 
    { 
      while (lre != end() && out != lrend) { *out++ = *lre++; }
      m_end = out;
      return;
    }
  }
  while (rre != state.rules_end && out != lrend) { *out++ = *rre++; }
  m_end = out;
}

inline SlotMap::SlotMap(Segment & seg)
: segment(seg), m_size(0), m_precontext(0)
{
    m_slot_map[0] = 0;
}

inline Slot * * SlotMap::begin()
{
  return &m_slot_map[1]; // allow map to go 1 before slot_map when inserting
                         // at start of segment.
}

inline Slot * * SlotMap::end()
{
  return m_slot_map + m_size + 1;
}

inline size_t SlotMap::size() const
{
  return m_size;
}

inline short unsigned int SlotMap::context() const
{
  return m_precontext;
}

inline void SlotMap::reset(Slot & slot, short unsigned int ctxt)
{
  m_size = 0;
  m_precontext = ctxt;
  *m_slot_map = slot.prev();
}

inline void SlotMap::pushSlot(Slot*const slot)
{
  m_slot_map[m_size++ + 1] = slot;
}

inline Slot * const & SlotMap::operator[](int n) const
{
  return m_slot_map[n + 1];
}

inline Slot * & SlotMap::operator[](int n)
{
  return m_slot_map[n + 1];
}

} // namespace graphite2
