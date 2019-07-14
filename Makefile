target=main.cpp char.cpp string.cpp source.cpp load.cpp score.cpp pos.cpp errmsg.cpp
header=char.h string.h source.h load.h score.h pos.h errmsg.h
jackdaw: $(target) $(header)
	g++ $(target) -ojackdaw -Wall -Wextra
