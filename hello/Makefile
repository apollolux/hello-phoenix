include ../nall/Makefile
include ../phoenix/Makefile

application := hello
flags := -O3 -fomit-frame-pointer -I..
link := -s
objects := obj/phoenix.o obj/$(application).o

ifeq ($(platform),win)
  link += -mwindows
  objects += obj/resource.o
else ifeq ($(platform),windows)
  link += -mwindows
  objects += obj/resource.o
endif

all: $(objects)
	$(compiler) -o $(application) $(objects) $(link) $(phoenixlink)

#obj/resource.o: ../phoenix/windows/phoenix.rc
#	windres ../phoenix/windows/phoenix.rc obj/resource.o

obj/resource.o: app.rc
	windres app.rc obj/resource.o

obj/phoenix.o: ../phoenix/phoenix.cpp
	$(compiler) $(phoenixflags) -o obj/phoenix.o -c ../phoenix/phoenix.cpp

obj/$(application).o: $(application).cpp *.cpp *.hpp
	$(compiler) $(cppflags) $(flags) -o obj/$(application).o -c $(application).cpp

install:
	sudo cp $(application) /usr/local/bin/$(application)

clean:
	$(call delete,obj/*.o)
