#include <nuttx/config.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, FAR char *argv[])
{
  int fd;
  char rx_buf[128]; /* 1行分ためておくための大きな箱 */
  int rx_idx = 0;   /* 今、箱の何番目まで文字を入れたか（インデックス） */
  char tx_buf[256];

  printf("Starting Line-buffered Echo Test...\n");

  fd = open("/dev/ttyS2", O_RDWR);
  if (fd < 0) return -1;

  while (1)
    {
      char c;
      /* 1文字だけ読み込む！ */
      int nbytes = read(fd, &c, 1);
      
      if (nbytes > 0)
        {
          /* もし届いた文字が「改行（Enter）」だったら、1行完成！ */
          if (c == '\n' || c == '\r')
            {
              rx_buf[rx_idx] = '\0'; /* 文字列の終わりの看板を立てる */
              
              /* 1文字以上受け取っていたら、画面に出して返信する */
              if (rx_idx > 0)
                {
                  printf("CM4 -> Spresense: %s\n", rx_buf);
                  fflush(stdout);

                  snprintf(tx_buf, sizeof(tx_buf), "Spresense ACK: %s\n", rx_buf);
                  write(fd, tx_buf, strlen(tx_buf));
                  
                  rx_idx = 0; /* 箱を空っぽ（0番目）に戻して、次を待つ！ */
                }
            }
          else
            {
              /* 改行じゃなければ、箱に文字を1つずつ追加していく */
              if (rx_idx < sizeof(rx_buf) - 1)
                {
                  rx_buf[rx_idx] = c;
                  rx_idx++; /* 次の文字のために場所を1つ進める */
                }
            }
        }
    }

  close(fd);
  return 0;
}