# copy and pasted from colby cs
IDIR=include
CC=gcc
CFLAGS=-I $(IDIR) -Wall -g
SRCDIR=src
ODIR=obj
LIBS=sdl3 sdl3-ttf
rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))
DEPS=$(call rwildcard,$(IDIR),*.h)

CFILES=$(call rwildcard,$(SRCDIR),*.c)
OBJ=$(subst $(SRCDIR)/,$(ODIR)/,$(patsubst %.c,%.o,$(CFILES)))
OBJ_FILES=$(call rwildcard,$(ODIR),*.o)
SUBSTITUTE_WITH_SPACES=$(subst /, ,${1})
GET_OBJECT_FILE_NAME=$(lastword $(call SUBSTITUTE_WITH_SPACES,$(1)))
GET_PARENT_PATH=$(patsubst %/,%,$(subst $(call GET_OBJECT_FILE_NAME,$(1)),,$(1)))

NAME=main

$(NAME): $(OBJ)
	$(CC) `pkg-config --cflags --libs $(LIBS)` -o $(NAME) $(OBJ_FILES) $(CFLAGS)

$(ODIR)/%.o: $(DEPS) $(CFILES) 
	mkdir -p $(call GET_PARENT_PATH,$(word 1,$(OBJ))) 
	$(CC)  -c -o $(word 1,$(OBJ)) $(word 1,$(CFILES)) $(CFLAGS)
	$(eval CFILES=$(subst $(word 1,$(CFILES)),`,$(CFILES))) 
	$(eval OBJ=$(subst $(word 1,$(OBJ)),`,$(OBJ))) 
	$(eval CFILES=$(subst `,,$(CFILES))) 
	$(eval OBJ=$(subst `,,$(OBJ))) 


.PHONY: clean print initialize

initialize:
clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
	rm -r -f $(ODIR)
	rm main

print: 
	$(info CFILES is $(CFILES))
	$(info DEPS is $(DEPS))
	$(info OBJ is $(OBJ))
	$(info FIRST FILE OF CFILES IS $(word 1,$(CFILES)))
	$(info FIRST FILE OF OBJ IS $(word 1,$(OBJ)))
	$(info SECOND FILE OF CFILES IS $(word 2,$(CFILES)))
	$(info SECOND FILE OF OBJ IS $(word 2,$(OBJ)))


	$(info OBJ is $(OBJ))
	$(info CFILES is $(CFILES))
	$(info NEXT FILE OF CFILES IS $(word 1,$(CFILES)))
	$(info NEXT FILE OF OBJ IS $(word 1,$(OBJ)))

	$(info WITH_SPACES is $(call SUBSTITUTE_WITH_SPACES,$(OBJ)))
	$(info OBJECT_FILE_NAMES is $(foreach wrd,$(OBJ),$(lastword $(call SUBSTITUTE_WITH_SPACES,$(wrd)))))
	$(info PARENT_PATHS is $(foreach wrd,$(OBJ),$(call GET_PARENT_PATH,$(wrd))))

