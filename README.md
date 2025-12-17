# Desafio de Firmware – Controle de Motor

Este repositório contém o firmware desenvolvido para um desafio técnico de hardware e firmware.

O objetivo do projeto é detectar um sinal externo de controle e acionar um motor DC por um período fixo, além de enviar mensagens de status por meio de uma interface serial dedicada.

---

## Funcionalidades

- Leitura de entrada digital externa (ativo em nível baixo)
- Acionamento de motor DC por 10 segundos
- Envio de mensagens de status via UART
- Código escrito em C seguindo boas práticas de organização

---

## Funcionamento

Quando o sinal externo em nível baixo é detectado, o motor é acionado.  
O motor permanece ligado por 10 segundos e é desligado automaticamente após esse período.

Mensagens de status são transmitidas pela interface UART durante a operação.

---

## Observações

- A interface UART implementada no firmware representa a comunicação lógica; a conversão para RS232 é realizada no hardware.
- O código foi desenvolvido com foco em simplicidade e clareza.

---
