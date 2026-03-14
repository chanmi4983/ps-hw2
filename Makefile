CC = gcc
CFLAGS = -g -Wall
TARGET = manager
OBJECTS = main.o class.o

# 최종 실행 파일 생성
$(TARGET) : $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

# 개별 소스 파일 컴파일 (목적 파일 생성)
main.o : main.c class.h
	$(CC) $(CFLAGS) -c main.c

class.o : class.c class.h
	$(CC) $(CFLAGS) -c class.c

# 정리 (맥북/리눅스용 rm 사용)
clean:
	rm -f *.o $(TARGET)