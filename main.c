
#include "mq.h"
#include "timer.h"
#include "uart.h"

#define ID_MSG_TIMER_1S        0x01
#define ID_MSG_UART_DATA_READY 0x02

@far @interrupt void EXTI_PORTA_IRQHandler(void) {
}

@far @interrupt void UART1_RCV_IRQHandler(void) {
  unsigned char u = UART1_DR;
  UART_PutBuffer(u);
  if (u == '\r' || u == '\n') {
    MQ_PutMessage(ID_MSG_UART_DATA_READY);
  }
}

@far @interrupt void TIM4_UPD_OVF_IRQHandler(void) {
	static int cnt = 0;
	cnt++;

	TIM4_SR = 0x00;
	if (cnt >= 62) { // 16.128ms * 62 = 0.999936s
    cnt = 0;
    MQ_PutMessage(ID_MSG_TIMER_1S);
  }
}

void main() {
  struct MessageQueue mq;
  unsigned char msg;

  if (MQ_Init() != 0) {
    return;
  }

  if (UART_Init() != 0) {
    return;
  }

  while (1) {
    if (MQ_GetMessage(&msg) == 0) {
      switch (msg) {
      case ID_MSG_TIMER_TIMEOUT:
        break;
      case ID_MSG_UART_DATA_READY:
        break;
      default:
      }
    }
  }
}

