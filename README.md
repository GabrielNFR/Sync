# Sync

Implementação do mecanismo de sincronização do embarque de passageiros em um vagão de metrô, escrita em C com pthreads. Cada passageiro é uma thread, cada vagão é outra, e o trabalho é garantir que ninguém embarque sem assento e que o vagão não parta com passageiro liberado ainda em pé.

## O problema

A estação tem um único vagão por vez. Quando o vagão chega, ele chama `station_load_car` com o número de assentos disponíveis e só retorna quando todos os passageiros que já estavam esperando embarcaram ou quando os assentos acabaram. Cada passageiro chama `station_wait_for_car`, que o deixa dormir até existir vagão com assento livre, e depois sinaliza o embarque com `station_on_board`.

O ponto delicado é que a reserva do assento e a confirmação do embarque são eventos diferentes. Vários passageiros podem reservar antes de qualquer um deles chamar `station_on_board`, e o vagão precisa esperar pelas confirmações, não pelas reservas.

## O modelo de estado

`struct station` guarda tudo o que o vagão e os passageiros precisam saber um sobre o outro.

| Campo | Papel |
|:--|:--|
| `mutex` | único lock da estação, protege todos os campos abaixo |
| `car_arrived` | variável de condição onde os passageiros dormem esperando assento |
| `passenger_boarded` | variável de condição onde o vagão dorme esperando confirmações |
| `car_present` | vale 1 enquanto existe vagão na estação |
| `free_seats` | assentos que este vagão ainda pode reservar |
| `waiting` | passageiros que entraram em `station_wait_for_car` e ainda não reservaram |
| `boarding` | confirmações pendentes deste vagão |

A ideia central é que `boarding` funciona como contador decrescente. Ele sobe quando um passageiro reserva o assento e desce quando esse passageiro chama `station_on_board`. Quando chega a zero, o vagão já recebeu todas as confirmações que precisava.

## As quatro funções

### `station_init`

Zera os quatro contadores e inicializa o mutex e as duas variáveis de condição com `pthread_mutex_init` e `pthread_cond_init`. Sem isso, a primeira chamada a qualquer uma das outras funções encontraria lixo de memória.

### `station_wait_for_car`

O passageiro se registra em `waiting` antes de dormir, porque o vagão precisa saber quantas pessoas já estão esperando no instante em que chega. Depois ele fica no laço:

- condição para continuar esperando: não existe vagão ou não existe assento livre
- ao sair do laço: decrementa `waiting`, decrementa `free_seats` e incrementa `boarding`

A saída do laço é o momento da reserva. Decrementar `free_seats` e decrementar `waiting` acontecem sob o mesmo lock, então dois passageiros nunca conseguem reservar o mesmo assento.

### `station_on_board`

Apenas decrementa `boarding` e acorda o vagão com `pthread_cond_signal` em `passenger_boarded`. O decremento vem antes do sinal para que o vagão, ao reobter o lock, já encontre o estado atualizado.

### `station_load_car`

O vagão publica o estado, acorda os passageiros e espera as confirmações:

- marca `car_present = 1` e define `free_seats` a partir de `count`, tratando valor não positivo como zero
- zera `boarding`, que sempre está em zero na chegada mas fica como garantia
- faz `pthread_cond_broadcast` em `car_arrived`, porque vários passageiros podem embarcar
- espera enquanto houver confirmação pendente, ou enquanto houver assento para oferecer e alguém esperando
- antes de soltar o lock, zera `car_present` e `free_seats`

O último passo é o que impede um passageiro atrasado de reservar assento em um vagão que já partiu. O encerramento acontece com o lock na mão, então nenhum passageiro consegue observar o estado no meio do caminho.

## Por que duas variáveis de condição

São dois predicados diferentes, um para passageiros e outro para o vagão. Com uma condvar só, um `signal` poderia acordar a thread que não estava esperando aquela condição, e a notificação se perderia. Com duas, cada grupo é acordado pelo seu próprio canal. Como só existe um vagão por vez, o vagão pode usar `signal` em vez de `broadcast`, porque ele é o único que dorme em `passenger_boarded`.

## Como compilar e executar

```
make clean
make
./sync_car
```

A saída esperada mostra cada vagão entrando com um número de assentos, saindo com o número de passageiros que embarcaram, e termina com `Looks good!`. Qualquer linha com asterisco indica divergência entre o esperado e o realizado.

## Arquivos

- `sync_car.h`: define `struct station` e declara as quatro funções. O tipo fica completo no header porque o runner aloca a estação na pilha da `main`.
- `sync_car.c`: implementação das quatro funções.
- `sync_car-runner.c`: teste fornecido no exercício, com um bloco acrescentado no fim para o caso inválido de capacidade negativa.
- `Makefile`: compila com `-std=gnu11 -Wall -Wextra -Werror -pthread` e gera o executável `sync_car`.
- `evidencias.log`: registro de terminal das sessões de trabalho.
- `gnfr.md`: relatório do trabalho.

## Testes

O runner cria 100 threads de passageiro e faz vários vagões passarem, com número de assentos sorteado entre 0 e 49. Ele verifica o caso de vagão vazio sem ninguém esperando, vagão sem assento com muita gente esperando, e vagão com mais assentos do que passageiros.

Além dele, a implementação foi executada 30 vezes seguidas para procurar falha intermitente e uma vez sob o helgrind. O helgrind reportou condições de corrida nos indicadores de controle do próprio runner, entre eles `load_car_returned`, que é declarado `volatile` mas não é atômico. O recorte de saída registrado no log mostra apenas um desses contextos, os outros três aparecem no resumo do valgrind mas não foram capturados.

## Decisões e limites

- Um único mutex para a estação inteira, como pede o enunciado. Isso serializa o acesso ao estado e elimina a corrida pela reserva do assento.
- Nenhuma espera ativa. Todo bloqueio é `pthread_cond_wait`, então as threads não consomem CPU enquanto esperam.
- `count` negativo é tratado como zero. O contrato não define esse caso, e a escolha foi não oferecer assento nenhum.
- O vagão espera enquanto houver assento e alguém esperando. Isso faz com que ele absorva passageiros que chegam um pouco depois da abertura das portas, em vez de partir com assento vazio.
