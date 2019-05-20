generic_bison(structured_text)
generic_flex(structured_text)

file(GLOB_RECURSE sources "*.cpp" "*.h")

add_library(structured-text
    ${sources}
    ${BISON_parser_OUTPUTS}
    ${FLEX_scanner_OUTPUTS}
)

generic_includes(structured-text)

target_link_libraries(structured-text util)