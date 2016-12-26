# This is the Makefile to use for Homework Assignment #2
# To use, at the prompt, type:
#
# 	make driver		# This will make executable driver
# or
#       make directories        # makes correct directories
#       make install            # copies files to correct place
#       make clean              # cleans up directory
#       make public            # installs assignment into public area

DIR=$(HOME)/../public/hw3

all:	driver

valgrind: driver.o mylib.o stack.o driver
	valgrind --leak-check=full --read-var-info=yes \
	    --show-reachable=yes ./driver $(DEBUG)

driver:	driver.o mylib.o stack.o
	gcc -o driver -g driver.o mylib.o stack.o

driver.o:	driver.c mylib.h stack.h
	gcc -Wall -pedantic -c -g driver.c

mylib.o:	mylib.c mylib.h
	gcc -Wall -pedantic -c -g mylib.c

stack.o:	stack.c stack.h
	gcc -Wall -pedantic -c -g stack.c

clean:
	rm -f core *.o driver

new:
	make clean
	make

backup:	*.[ch] Makefile
	cp -p *.[ch] Makefile $(HOME)/hw3/backup

directories:
	mkdir $(HOME)/hw3
	mkdir $(HOME)/hw3/backup

install:
	cp -p $(DIR)/Makefile $(DIR)/*.h $(DIR)/*.empty \
	$(DIR)/mylib.c $(DIR)/.cproject $(DIR)/.project \
	$(DIR)/.hw3.vcproj $(DIR)/.hw3.sln $(HOME)/hw3

public:	
	make new

	if [ ! -d $(DIR) ]; then mkdir $(DIR); fi

	# lock public directory
	chmod 700 $(DIR)
	#
	# copy all files to directory
	cp -p .cproject .project .hw3.vcproj .hw3.sln .hw3.suo  Makefile \
	    mylib.c *.h *.empty driver $(DIR)
	#
	# strip executable of debugging information
	strip $(DIR)/driver
	#
	# set correct permissions on project files
	chmod 644 $(DIR)/.cproject $(DIR)/.project $(DIR)/.hw3.vcproj $(DIR)/.hw3.sln
	#
	# set correct permissions on source files (access to students now)
	chmod 644 $(DIR)/Makefile $(DIR)/*.[ch] $(DIR)/*.empty
	#
	# set restrictive permissions on solution source files (before due date)
	chmod 640 $(DIR)/mylib.c
	#
	# set correct permissions on solution source files (after hw1 due date)
	chmod 644 $(DIR)/mylib.c
	#
	# set correct permissions on executable
	chmod 711 $(DIR)/driver
	
	# unlock public directory for tutors
	chmod 750 $(DIR)

	# move html files to public_html directory
	rm -r $(HOME)/public_html/hw3/
	mkdir $(HOME)/public_html/hw3
	cp -p *html $(HOME)/public_html/hw3
	chmod 744 $(HOME)/public_html/hw3/*html
	chmod 750 $(HOME)/public_html/hw3/

	# unlock public directory for all students

	chmod go+rx $(DIR)
	chmod 755 $(DIR)
	chmod go+rx $(HOME)/public_html/hw3/

	#
#	rm -fr $(HOME)/../cs12xzz/hw3/*
#	scp -p Makefile *.[ch] .cproject .project .hw3.vcproj .hw3.sln \
	   # $(DIR)/driver cs12xzz@ieng6-201.ucsd.edu:~/hw3

vimrc:
	cp -p $(DIR)/../vimrc $(HOME)/.vimrc

