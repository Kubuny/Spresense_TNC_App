#include <nuttx/config.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, FAR char *argv[])
{
  int fd;
  char rx_buf[128];
  int rx_idx = 0;
  char tx_buf[256];

  printf("Starting Mission Command Dispatcher...\n");
  printf("Waiting for commands: [TAKE_PHOTO], [SEND_APRS], [STATUS]\n");

  fd = open("/dev/ttyS2", O_RDWR);
  if (fd < 0) return -1;

  while (1)
    {
      char c;
      int nbytes = read(fd, &c, 1);
      
      if (nbytes > 0)
        {
          if (c == '\n' || c == '\r')
            {
              rx_buf[rx_idx] = '\0';
              
              if (rx_idx > 0)
                {
                  printf("\n[Command Received] %s\n", rx_buf);

                  /*　ここからミッションの分岐（コマンド判定） */
                  
                  if (strcmp(rx_buf, "TAKE_PHOTO") == 0)
                    {
                      printf(">>> MISSION: Camera sequence started...\n");
                      /* ここに後でカメラ撮影のコードを書く */
                      snprintf(tx_buf, sizeof(tx_buf), "ACK: Photo taken successfully.\n");
                    }
                  else if (strcmp(rx_buf, "SEND_APRS") == 0)
                    {
                      printf(">>> MISSION: APRS transmission started...\n");
                      /* ここに後でAPRS送信のコードを書く */
                      snprintf(tx_buf, sizeof(tx_buf), "ACK: APRS packet sent.\n");
                    }
                  else if (strcmp(rx_buf, "STATUS") == 0)
                    {
                      printf(">>> MISSION: Sending system status...\n");
                      snprintf(tx_buf, sizeof(tx_buf), "ACK: All systems GREEN. Battery 98%%.\n");
                    }
                  else
                    {
                      /* 知らないコマンドが来た時 */
                      printf(">>> ERROR: Unknown command.\n");
                      snprintf(tx_buf, sizeof(tx_buf), "NACK: Unknown command '%s'\n", rx_buf);
                    }

                  /* 判定結果（返事）をCM4へ送信 */
                  write(fd, tx_buf, strlen(tx_buf));
                  
                  rx_idx = 0; /* 箱を空っぽにして次のコマンドを待つ */
                  printf("Waiting for next command...\n");
                }
            }
          else
            {
              if (rx_idx < sizeof(rx_buf) - 1)
                {
                  rx_buf[rx_idx] = c;
                  rx_idx++;
                }
            }
        }
    }

  close(fd);
  return 0;
}