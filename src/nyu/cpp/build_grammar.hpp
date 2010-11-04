#ifndef NYU_CPP_BUILD_GRAMMAR_HPP
#define NYU_CPP_BUILD_GRAMMAR_HPP

#include <nyu/cpp/scope_ref_cache.hpp>

namespace nyu { namespace cpp {

class build_rule;

class build_grammar : public scope_ref_cache {
    friend class build_rule;
    typedef typename parser::stored<grammar::nyu::Rule>::type  rule_type;

    grammar_type&  grammar_;

  public:
    build_grammar(builder& builder, module_type& module, grammar_type& grmmr)
      : scope_ref_cache(builder, module), grammar_(grmmr)
    { open(); }

    void close();
    void operator()(rule_type& rule);

  private:
    void open();
};

} }
#endif

