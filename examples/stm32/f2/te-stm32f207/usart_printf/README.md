# README

This example program sends via USART6 "Hello World!" and
an integer, float and double all over again
while incrementing the numbers.

The sending is done in a blocking way.

## Board connections

| X13 connector | STM32 Port | Function      | Description                       |
| ------------- | ---------- | ------------- | --------------------------------- |
| 23            | `PC6`      | `(USART6_TX)` | TTL serial output `(9600,8,N,1)`  |
| 24            | `PC7`      | `(USART6_RX)` | TTL serial input `(9600,8,N,1)`   |
