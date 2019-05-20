SRC = converters/convert_bool_literal.cpp \
      converters/convert_int_literal.cpp \
      converters/convert_real_literal.cpp \
      converters/convert_string_value.cpp \
      converters/expr2structured_text.cpp \
      structured_text_language.cpp \
      structured_text_lex.yy.cpp \
      structured_text_parse_tree.cpp \
      structured_text_parser.cpp \
      structured_text_typecheck.cpp \
      structured_text_y.tab.cpp \
      # Empty last line

INCLUDES= -I ..

include ../config.inc
include ../common

CLEANFILES = stl$(LIBEXT) \
  structured_text_y.tab.h structured_text_y.tab.cpp \
  structured_text_lex.yy.cpp structured_text_y.tab.cpp.output \
  structured_text_y.output

all: structured-text$(LIBEXT)

###############################################################################

structured_text_y.tab.cpp: parser.y
	$(YACC) $(YFLAGS) $$flags -pyystructured_text -d parser.y -o $@

structured_text_y.tab.h: structured_text_y.tab.cpp
	if [ -e structured_text_y.tab.hpp ] ; then mv structured_text_y.tab.hpp \
	structured_text_y.tab.h ; else mv structured_text_y.tab.cpp.h \
	structured_text_y.tab.h ; fi

structured_text_lex.yy.cpp: scanner.l
	$(LEX) -Pyystructured_text -o$@ scanner.l

# extra dependencies
structured_text_y.tab$(OBJEXT): structured_text_y.tab.cpp \
  structured_text_y.tab.h
structured_text_lex.yy$(OBJEXT): structured_text_y.tab.cpp \
  structured_text_lex.yy.cpp structured_text_y.tab.h

###############################################################################

generated_files: structured_text_y.tab.cpp structured_text_lex.yy.cpp \
  structured_text_y.tab.h

###############################################################################

structured-text$(LIBEXT): $(OBJ)
	$(LINKLIB)

