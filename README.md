# ft_irc

*This project has been created as part of the 42 curriculum by jpiquet, gaducurt.*

## Description

```mermaid
flowchart LR

subgraph Server

S1@{label: list cmd}

subgraph channels

subgraph channel_1

C01@{label: Client_1}

style C01 fill:#ff0000,stroke:#333,stroke-width:4px

C02@{label: Client_2}

style C02 fill:#66cc99,stroke:#333,stroke-width:4px

end

subgraph channel_2

C11@{label: Client_1}

style C11 fill:#ff0000,stroke:#333,stroke-width:4px

C12@{label: Client_2}

style C12 fill:#66cc99,stroke:#333,stroke-width:4px

C13@{label: Client_3}

style C13 fill:#99ccff,stroke:#333,stroke-width:4px

end

subgraph channel_3

C22@{label: Client_2}

style C22 fill:#66cc99,stroke:#333,stroke-width:4px

C23@{label: Client_3}

style C23 fill:#99ccff,stroke:#333,stroke-width:4px

end

end

subgraph lst[list cmd]

H@{label: JOIN}

I@{label: KICK}

J@{label: INVITE}

K@{label: TOPIC}

L@{label: MODE}

M@{label: channel.addUser}

MM@{label: Channel constructor}

N@{label: channel.kick}

O@{label: }

P@{label: channel.displayTopic}

Q@{label: call some methode depending of parameters}

end

C@{label: Parser prompt}

D@{label: Parser cmd}

F@{label: Cmd}

subgraph lstclt[list clients]

Cl11@{label: Client_1}

style Cl11 fill:#ff0000,stroke:#333,stroke-width:4px

Cl12@{label: Client_2}

style Cl12 fill:#66cc99,stroke:#333,stroke-width:4px

Cl13@{label: Client_3}

style Cl13 fill:#99ccff,stroke:#333,stroke-width:4px

end

  

end

Cl1@{label: Client_1}

style Cl1 fill:#ff0000,stroke:#333,stroke-width:4px

Cl2@{label: Client_2}

style Cl2 fill:#66cc99,stroke:#333,stroke-width:4px

Cl3@{label: Client_3}

style Cl3 fill:#99ccff,stroke:#333,stroke-width:4px

  

style Server fill:#003366,stroke:#333,stroke-width:4px

style lst fill:#336633,stroke:#333,stroke-width:4px

style channels fill:#330033,stroke:#333,stroke-width:4px

style lstclt fill:#330000,stroke:#333,stroke-width:4px

  

S11[(Another Server)]

S12[(Another Server)]

  

Cl1 --> |prompt| C

Cl2 --> |prompt| C

Cl3 --> |prompt| C

C --> F

F --> D

D -.- |match checker| S1

  

S1 --- H

S1 --- I

S1 --- J

S1 --- K

S1 --- L

  

H -.- M

H -.- MM

I -.- N

J -.- O

K -.- P

L -.- Q

  

lst --> |execute command| channel_3

  

Server --- S11

Server --- S12
```

## Instruction

## Ressources

- https://ubuntu.com/tutorials/irc-server#1-overview
- https://datatracker.ietf.org/doc/html/rfc1459
- https://mathieu-lemoine.developpez.com/tutoriels/irc/protocole/?page=page-2
- https://broux.developpez.com/articles/c/sockets/
- https://en.wikipedia.org/wiki/IRC
- https://celeo.github.io/2021/06/18/Implementing-an-IRC-server-from-scratch-part-1/
- https://mathieu-lemoine.developpez.com/tutoriels/irc/protocole/?page=page-3
