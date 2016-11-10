#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFSIZE 512 // 읽혀질 block의 크기
#define PERM 0644 // 새 파일의 화일 허가
// name1 file 내용을 name2 파일에 복사한다.
int copyfile(const char *name1, const char *name2) {
  int infile, outfile;
  ssize_t nread;
  char buffer[BUFSIZE];

  if ((infile = open(name1,O_RDONLY)) == -1) return -1;

  if ((outfile = open(name2, O_WRONLY | O_CREAT | O_TRUNC, PERM)) == -1) {
  close(infile);
  return -2;
  }
//이제 name1으로부터 한 번에 BUFSIZE 문자를 읽는다.
  while((nread = read(infile,buffer,BUFSIZE)) > 0) {
// buffer를 출력 파일에 쓴다.
  if (write(outfile,buffer,nread) < nread) {
  close(infile);
  close(outfile);
  return -3; // 쓰기 오류
  }
  }

  close(infile);
  close(outfile);

  if (nread == -1)
  return -4; // 마지막 읽기에서 오류 발생
  else
  return 0; // success
}

// mycp.c
int main(int argc, char *argv[]) {
  int retcode;
  if (argc!=3) {
  printf("invalid arguments\n");
  return -1;
  }
  retcode = copyfile(argv[1],argv[2]);
  printf("copyfile return value : %d\n",retcode); // copyfile 결과값 출력
  return 0;
}
