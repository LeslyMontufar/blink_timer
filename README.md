# Blink usando temporizadores

Blink por interrupção usando temporizador e entrada para troca de delay do blink com debouncing implementado em outro temporizador.

## Índice 

* [Descrição](#descrição)
* [Desenvolvimento](#desenvolvimento)

## Descrição

Parta do código criado anteriormente onde, através de um jumper em PA0, era simulada a ação de
aperto de um botão, alterando a o tempo de piscada de um led. Vamos repetir esse trabalho mas
agora utilizando um temporizador e não o systick.

A taxa de acendimento do led deve ser alterada entre os padrões de 250ms on/off e 500ms on/off, a
cada novo pressionamento, com o botão tratado na interrupção e com uma rotina de debouncing
descrita mais adiante.

Nesse exercício a taxa de piscada do led será dada por um timer cujo tempo irá variar a cada
pressionamento em PA0 e quando o timer estourar, deverá ser feita a operação de toggle no led.

O debouncing também será obrigatório mas sendo realizado por outro timer. No caso, ao chegar a
interrupção PA0, essa interrupção EXTI deve ser desabilitada e o timer de deboucing ligado. Ao
estourar o tempo do timer, a interrupção deve ser novamente ativada, dentro da função de callback
do timer e esse timer desativado.

Perceba que todo o seu código será executado em modo interrompido. Coloque o controlador em
estado de economia de energia dentro da função main.

## Desenvolvimento
