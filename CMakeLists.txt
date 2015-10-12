cmake_minimum_required(VERSION 2.8.0)
project(small-prolog)
add_executable(small-prolog
  pralloc.c
  prassert.c
  prbltin.c
  prcnsult.c
  prdebug.c
  prerror.c
  prhash.c
  prlush.c
  prmachine.c
  prmain.c
  prparse.c
  prprint.c
  prscan.c
  prunify.c
)
