# 1. Objetivo
O que o enunciado pede e o que você entrega. Não explique a implementação aqui.

Este trabalho tem como objetivo implementar o mecanismo de sincronização do embarque de passageiros de um vagão de metrô para a METROREC. É necessário garantir que o vagão só parta quando todos os passageiros que ele liberou estiverem sentados e que o embarque respeite a capacidade, usando exclusivamente as funções de `pthread_mutex_*` e `pthread_cond_*`, com um único lock por estação, sem semáforos e sem busy wait, assumindo um vagão por carro e no máximo um carro na estação por vez. 

# 2. Checklist de Requisitos
Um requisito por linha. Sem parágrafo, se precisar explicar algo, use a coluna "como testar". Status: OK, Parcial ou Não feito.
| Requisito do enunciado | Status | Como testar / evidência |
|:-----------------------|:-------|:------------------------|
| 1. Usar apenas `pthread_mutex_*` e `pthread_cond_*` | Não feito | Sem implementação: `sync_car.c` contém só stubs vazios |
| 2. Sem semáforos nem outras primitivas | Não feito | Sem implementação: `sync_car.c` |
| 3. No máximo um lock por `struct station` | Parcial | `sync_car` com um único `pthread_mutex_t` no struct, mas ainda não há código que o adquira |
| 4. Premissa: um carro por vez, mesmo destino | Não feito | Sem implementação nem execução registrada no `evidencias.log` |
| 5. Embarque simultâneo | Não feito | Sem implementação nem execução registrada no `evidencias.log` |
| 6. Sem busy-wait | Não feito | Sem implementação: `sync_car.c` |
| 7. Makefile gera o executável `sync_car` | Ok | `evidencias.log`: Thu Oct 29 17:26:04 -03 2026 |


# 3. Como Reproduzir
Comandos reais, copiáveis. Se não rodar com o que está aqui, não é avaliado.
Compilar:
```
make clean
make
```
Executar:
```
./sync_car
```
# 4. Arquitetura (resumida)
Uma linha por arquivo, só o que é específico do seu projeto.

**Arquivos e responsabilidades:**

1. *[sync_car.h]* -> *[define struct station e os protótipos das funções]*
2. *[sync_car.c]* -> *[implementa station_init, station_wait_for_car, station_on_board, station_load_car]*
3. *[sync_car-runner.c]* -> *[teste fornecido pelo professor, ele não foi alterado]*
4. *[Makefile]* -> *[build com -std=gnu11 -Wall -Wextra -Werror -Wno-unused-parameter -O2 -g -pthread
LDFLAGS = -pthread, gera sync_car.]*

**Decisões técnicas:**
1. *[Struct no header]* → *[o runner faz struct station station na main]* → *[logo precisa do tipo completo]*
2. *[Decisão]* → *[Motivo]* → *[Consequência]*
3. *[Decisão]* → *[Motivo]* → *[Consequência]*

# 5. Estratégias e Diário de Desenvolvimento
Isto é o núcleo do relatório.
## 5.1 Estratégias da semana
Uma linha por abordagem diferente que você tentou. Se você não mudou de abordagem, preencha só S1.
| Estratégia | Nome curto | Contexto | Motivo da troca (se houve) |
|:-----------|:-----------|:---------|:---------------------------|
| S1 |  |  |  |
| S2 |  |  |  |
| S3 |  |  |  |
## 5.2 Diário de Tentativas
Uma linha por tentativa relevante, marcando a qual estratégia (S1/S2/S3) ela pertence, obviamente precisa ser em ordem cronológica. A coluna "Quando" precisa bater com a ordem cronológica do evidencias.log, é isso que comprova que o fluxo é real. "Hipótese/Causa" só é preenchida quando o resultado foi falha ou parcial.
| # | Estrat. | O que tentei | Resultado | Hipótese/Causa (se falhou) | Quando | Evidência |
|:--|:--------|:-------------|:----------|:---------------------------|:-------|:----------|
| 1 |  |  |  |  |  |  |
| 2 |  |  |  |  |  |  |
| 3 |  |  |  |  |  |  |
| 4 |  |  |  |  |  |  |
| 5 |  |  |  |  |  |  |
| 6 |  |  |  |  |  |  |
| 7 |  |  |  |  |  |  |
| 8 |  |  |  |  |  |  |

Regra prática: se algo deu muito errado (bug que te travou por horas), a linha da tabela resume em 1 frase e aponta para o trecho do evidencias.log com o detalhe.

# 6. Evidências
A regra continua: toda afirmação relevante precisa de prova. O que muda é como capturar essa prova, para não depender de print manual toda hora.

## 6.1 Log automático (faz o trabalho pesado por você)
No início da sessão de trabalho, rode:
```
script -a evidencias.log
```
Isso grava automaticamente todos os comandos e saídas do terminal num arquivo, sem você precisar tirar print de cada linha. Comece a sessão com date, whoami e pwd, assim o log já tem timestamp, usuário e diretório sem esforço extra. **O evidencias.log deve ser enviado dentro do .tar.**

## 6.2 Prints

- Erro principal: o momento do bug mais relevante da semana.
- Validação final: teste passando / saída correta.
- 
Prints extras só se algo excepcional aconteceu e o log sozinho não explica, você precisa ter esse feeling, mas posso dar uma ajuda (ex.: comportamento visual, crash com stack trace longo).

# 7. Uso de IA
Obrigatório preencher mesmo se a resposta for "não utilizei".
**Onde usei IA (2–3 linhas):** *[...]*
**Prompts principais (liste, sem história):**

**Não foi utilizada inteligência artifial nem para geração de código nem para o preenchimento deste relatório.**

**O que validei manualmente e como:** *[...]*
# 8. Reflexão Final (5–8 linhas)
O que você melhoraria com mais tempo, principais decisões técnicas e erros que evitaria na próxima. Tudo junto, sem subseções separadas.
**Reflexão:** *[...]*

# 9. Checklist Final de Entrega

| Item | Confirmado? |
|:-----|:------------|
| Compilei do zero seguindo só o que está no relatório |  |
| Rodei os testes e evidencias.log foi gerado |  |
| Tenho prints obrigatórios |  |
| Testei pelo menos um caso limite e um caso inválido |  |
| Preenchi a seção de uso de IA |  |
| Revisei o relatório e removi frases genéricas / vazias |  |

# 10. Se eu tivesse mais 2 horas

Aplique as instruções abaixo de forma objetiva e verificável, evitando texto genérico.
Escreva 5-10 linhas sobre o que você melhoraria (robustez, testes, warnings, performance, organização).

*[...]*

# Regras de Integridade (não mudam)
- Ocorrência relevante sem evidência (log ou print) = não conta para a nota.
- Contradição entre prints, log e relato = relatório perde credibilidade.
- Indício de evidência fabricada = entrega invalidada.