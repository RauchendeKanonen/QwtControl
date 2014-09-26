

INC     = -I. 
LDLIBS  = -L/lib -L/usr/lib -L/usr/local/lib -lm 
CFLAGS = -D_FILE_OFFSET_BITS=64 -g 
  
 
  
  
TARGET1 = weeksfft
FILES1  = main.c \
			xfft.c  
		
all:    $(TARGET1)  

$(TARGET1): $(FILES1)
	$(CC) $(FILES1) $(CFLAGS) -o $(TARGET1) $(USERAPP_CFLAGS) $(USERAPP_LDFLAGS) $(USERAPP_LIBS) $(CFLAGS) $(LDLIBS)$(INC)

clean:
	rm -f $(SRC)/*~ $(SRC)/*.o *~ $(TARGET1)
	
install:
	cp $(TARGET1) /usr/local/bin


