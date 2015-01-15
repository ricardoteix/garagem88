# garagem88
Garagem 88

O objetivo do projeto é controlar via smartphone um portão eletrônico e um sistema de alarme simples.

O portão e o sistema de alarme funciona com controles remoto de 433,92 MHz. São dois controles remoto, um para cada fim. O portão utiliza apenas 1 botão do controle, abrindo e fechando o portão ao pressionar o botão. Já o sistema de alarme utiliza 2 botões do controle, um para ligar e outro para desligar.

A ideia foi de criar um controlador utilizando Arduino com um shield Ethernet para criar um servidor web embarcado e através de um página web controlar tanto o portão quanto o alarme.

Para facilitar a parte do envio do sinal RF resolvi adaptar o sistema utilizando um controle remoto com 4 funções, do qual utilizei apenas 3, um para cada comando. Configurei no mesmo controle a abertura/fechamento do portão e o liga/desliga do alarme. A partir daí utilizei 3 relés, cada um conectado aos botões de ação, e os relés sendo ativamos pelo Arduino através da página web embarcada. 

O projeto funcionu satisfatóriamente, abrindo e fechando o portão e lindando e desligando o alarme.

O código foi baseado no Web_Buzzer, da biblioteca Webduino, com pouca variação.