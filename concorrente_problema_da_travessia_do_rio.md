# Problema da Travessia do Rio
## Programação Concorrente - UnB - Nicholas Marques - 150019343

O problema da travessia do rio consiste em um jogo antigo de lógica para saber como fazer a travessia de uma família, um ladrão e um policial para o outro lado do rio, levando em conta a existência de um rio, 2 margens, um único barco com capacidade para 2 pessoas, e uma série de condições de como o barco deve ser operado e como os integrantes da família podem ser alocados no barco e nas margens do rio:
- "Seu objetivo é fazer todo o mundo atravessar esse rio. Mas a jangada só permite levar duas pessoas. No entanto, o Pai não pode ficar com as filhas, a Mamãe não pode ficar com os filhos e o Criminoso não pode ficar com ninguém, excepto o Policial. Apenas a Mãe, o Pai e o Policial conseguem dirigir a jangada. Resolva o enigma."
![Imagem do Jogo](https://i.imgur.com/fcue7Du.png).

A partir dele é possível fazer um algoritmo que resolva o problema de forma concorrente. O barco pode ser visto como um lock para que pessoas de diferentes margens do rio não utilizem ele ao mesmo tempo, as condições de quem pode ir com quem podem ser uma forma de prioridade, já que apenas adultos podem ocupar a primeira posição do barco (posição que dirige o barco), então enquanto o barco estiver vazio as crianças tem que esperar pra entrar no barco.\\
O ladrão não poder estar com ninguém a não ser o policial cria outra prioridade, além disso enquanto a travessia do barco é feita para o outro lado os indivíduos podem ser organizar na outra margem do rio.
