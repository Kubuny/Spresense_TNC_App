#include <nuttx/config.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, FAR char *argv[])
{
  int fd;
  char rx_buf[64];

  printf("Waiting for message from CM4... (Press RESET button to exit)\n");

  fd = open("/dev/ttyS2", O_RDONLY);
  if (fd < 0)
    {
      printf("Error: Failed to open /dev/ttyS2\n");
      return -1;
    }

  /* 無限ループで常に受信を監視する！ */
  while (1)
    {
      int nbytes = read(fd, rx_buf, sizeof(rx_buf) - 1);
      if (nbytes > 0)
        {
          rx_buf[nbytes] = '\0'; /* 文字列の終端をセット */
          printf("%s", rx_buf);  /* 受信した文字をそのまま画面に出す */
          fflush(stdout);        /* 画面表示を即座に反映させる魔法 */
        }
    }

  close(fd);
  return 0;
}