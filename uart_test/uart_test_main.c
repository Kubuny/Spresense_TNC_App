#include <nuttx/config.h>
#include <stdio.h>
#include <fcntl.h>   // open() を使うために追加
#include <unistd.h>  // write(), close() を使うために追加
#include <string.h>  // strlen() を使うために追加

/* アプリのメイン関数（NuttXではコマンドとして呼ばれます） */
int main(int argc, FAR char *argv[])
{
  int fd;
  /* CM4へ送りつけるメッセージ（\r\n は改行コードです） */
  const char *msg = "Hello CM4! This is Spresense UART2.\r\n";

  printf("Starting UART transmission test...\n");

  /* SpresenseのD0/D1ピン (UART2) を書き込みモードで開く */
  fd = open("/dev/ttyS2", O_WRONLY);
  if (fd < 0)
    {
      printf("Error: Failed to open /dev/ttyS2\n");
      return -1;
    }

  /* データを送信する */
  write(fd, msg, strlen(msg));

  /* 使い終わったら閉じる */
  close(fd);

  printf("Transmission complete!\n");
  return 0;
}