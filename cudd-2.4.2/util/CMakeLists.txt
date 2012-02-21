set(UTIL_SRCS
 cpu_time.c cpu_stats.c getopt.c safe_mem.c
 strsav.c texpand.c ptime.c prtime.c
 pipefork.c pathsearch.c stub.c tmpfile.c datalimit.c
#Headers
 util.h
)

add_library(util ${UTIL_SRCS})