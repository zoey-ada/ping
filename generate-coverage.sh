# run from the bin dir
llvm-profdata merge --sparse default.profraw > default.prof
llvm-cov export -format=lcov -instr-profile  default.prof platypus-tests.exe > lcov.info
