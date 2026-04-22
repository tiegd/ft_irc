# ft_irc

*This project has been created as part of the 42 curriculum by jpiquet, gaducurt.*

## Description

```mermaid
flowchart LR
    A@{ shape: cyl, label: "Server" }
    B@{ shape: stadium, label: "Client_1" }
    C@{ shape: stadium, label: "Client_2" }
    D@{ shape: stadium, label: "Client_3" }

    E@{ shape: circle, label: "Channel_1" }
    F@{ shape: circle, label: "Channel_2" }
    G@{ shape: circle, label: "Channel_3" }

    B e1@-->|Connect to server| A
    e1@{ animate: true }
    linkStyle 0 stroke:#ff0000

    C e2@-->|Connect to server| A
    e2@{ animate: true }
    linkStyle 1 stroke:#00ff00

    D e3@-->|Connect to server| A
    e3@{ animate: true }
    linkStyle 2 stroke:#00ffff

    A e4@-->E
    linkStyle 3 stroke:#ff0000
    A e5@-->E
    linkStyle 4 stroke:#00ff00

    A e4@-->F
    linkStyle 5 stroke:#ff0000
    A e5@-->F
    linkStyle 6 stroke:#00ff00
    A e5@-->F
    linkStyle 7 stroke:#00ffff

    A e5@-->G
    linkStyle 8 stroke:#00ff00
    A e5@-->G
    linkStyle 9 stroke:#00ffff
```

```mermaid
flowchart TB
    subgraph Server
        S1@{label: list cmd}
        S2@{label: list channels}
        S3@{label: list clients}
    end
    A@{ shape: stadium, label: Client}
    C@{label: Parser prompt}
    D@{label: Parser cmd}
    E@{label: Msg}
    F@{label: Cmd}

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


    A --> |prompt| Server
    Server --> C
    C --> E
    C --> F
    F --> D
    D -.- |match checker| Server

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
