#ifndef NYU_CPP_BUILD_CLASS_HPP
#define NYU_CPP_BUILD_CLASS_HPP

#include <nyu/cpp/compilation_unit.hpp>

namespace nyu { namespace cpp {

class build_class : public compilation_unit {
    class_type&    class_;

    std::vector<
        std::tuple<std::string, std::string, std::string> > attrs_;

    typedef typename parser::stored<
        grammar::nyah::Attribute>::type          attr_type;
    typedef typename parser::stored<
        grammar::nyah::AssignedAttribute>::type  assigned_attr_type;

  public:
    build_class(builder& builder, module_type& module, class_type& clss)
      : compilation_unit(builder, module), class_(clss)
    { open(); }

    void operator()(enum_type& enm) { output_file::operator()(enm, 1); }
    void operator()(attr_type& attr);
    void operator()(assigned_attr_type& attr);

    void close();

  private:
    void open();
};

} }
#endif

