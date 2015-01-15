# garagem88
Garagem 88

O objetivo do projeto � controlar via smartphone um port�o eletr�nico e um sistema de alarme simples.

O port�o e o sistema de alarme funciona com controles remoto de 433,92 MHz. S�o dois controles remoto, um para cada fim. O port�o utiliza apenas 1 bot�o do controle, abrindo e fechando o port�o ao pressionar o bot�o. J� o sistema de alarme utiliza 2 bot�es do controle, um para ligar e outro para desligar.

A ideia foi de criar um controlador utilizando Arduino com um shield Ethernet para criar um servidor web embarcado e atrav�s de um p�gina web controlar tanto o port�o quanto o alarme.

Para facilitar a parte do envio do sinal RF resolvi adaptar o sistema utilizando um controle remoto com 4 fun��es, do qual utilizei apenas 3, um para cada comando. Configurei no mesmo controle a abertura/fechamento do port�o e o liga/desliga do alarme. A partir da� utilizei 3 rel�s, cada um conectado aos bot�es de a��o, e os rel�s sendo ativamos pelo Arduino atrav�s da p�gina web embarcada. 

O projeto funcionu satisfat�riamente, abrindo e fechando o port�o e lindando e desligando o alarme.

O c�digo foi baseado no Web_Buzzer, da biblioteca Webduino, com pouca varia��o.