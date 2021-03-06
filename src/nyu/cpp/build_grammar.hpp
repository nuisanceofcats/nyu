#ifndef NYU_CPP_BUILD_GRAMMAR_HPP
#define NYU_CPP_BUILD_GRAMMAR_HPP

#include <nyu/cpp/compilation_unit.hpp>

namespace nyu { namespace cpp {

class build_rule;

class build_grammar : public compilation_unit {
    friend class build_rule;
    typedef typename parser::stored<grammar::nyu::Rule>::type  rule_type;

    grammar_type&  grammar_;
    grammar_type  *parent_grammar_;

    std::string const& namespace_alias() const {
        return get_builder().options_.namespace_alias_;
    }

  public:
    build_grammar(builder& builder, module_type& module, grammar_type& grmmr)
      : compilation_unit(builder, module), grammar_(grmmr), parent_grammar_(0)
    { open(); }

    void close();
    void operator()(rule_type& rule);

  private:
    void open();
};

} }
#endif

