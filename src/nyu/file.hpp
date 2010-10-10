#ifndef NYU_FILE_HPP
#define NYU_FILE_HPP

#include <nyu/grammar/meta.hpp>
#include <nyu/error/file.hpp>

#include <chilon/getset.hpp>

namespace nyu {

class file {
    typedef chilon::parser::source_code_stream<
        chilon::parser::file_stream, grammar::meta::Spacing>  stream_t;

    typedef grammar::meta::Grammar                            grammar_t;
    typedef typename chilon::parser::stored<grammar_t>::type  ast_type;

    stream_t   stream_;

    bool processed_;
  public:
    file() : processed_(false) {};

    bool parse_succeeded() const {
        return stream_.file_loaded() && stream_.empty();
    }

    bool parse(char const * const file_path, ast_type& ast) {
        if (! stream_.load(file_path))
            throw error::cannot_open_file(file_path);

        stream_.skip_whitespace();

        if (chilon::parser::parse<grammar_t>::skip(stream_, ast)) {
            stream_.skip_whitespace();
            return true;
        }
        else return false;
    }
};

}

#endif
