MODULE_NAME	= cmap
NAME		= cmap
_SRC		= cmap.c
_INC		= cmap.h
EXTRAFLAGS	+= -Wall -Wextra -Werror
DEBUGFLAGS	+= -g -fsanitize=address
EXLIBS +=			# -lm


#DEP1 =			# Dependent module's name
#DEP1LINK =		# Dependent module's github link

#DEP2 =
#DEP2LINK =


#DEPS=	DEP1	\	# Names of all dependencies.
#	DEP2	\
#	DEP3

include modMakefile
