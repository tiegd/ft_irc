#!/usr/bin/env python3
import asyncio

# === Configuration pour ton serveur IRC C++98 ===
HOST = "127.0.0.1"      # Adresse de ton serveur
PORT = 1800            # Port utilisé par ton serveur
CHANNEL = "#test"       # Canal où les clients vont rejoindre
PASSWORD = "mdp"           # mot de passe si ton serveur exige PASS (mettre "" si pas utilisé)
NUM_CLIENTS = 10        # Nombre de clients à simuler
MESSAGE_DELAY = 2.0     # Délai entre chaque message (en secondes)


class IRCClient:
    def __init__(self, nick):
        self.nick = nick
        self.reader = None
        self.writer = None
        self.running = True

    async def connect(self):
        """Connecte le client et fait la registration IRC."""
        self.reader, self.writer = await asyncio.open_connection(HOST, PORT)

        # Si ton serveur demande un mot de passe, envoie PASS
        if PASSWORD:
            await self.send(f"PASS {PASSWORD}")

        # Handshake IRC : NICK puis USER
        await self.send(f"NICK {self.nick}")
        await self.send(f"USER {self.nick} 0 * :{self.nick}")

        # Join au canal
        await self.send(f"JOIN {CHANNEL}")

    async def send(self, msg):
        """Envoie un message IRC (terminé par \\r\\n)."""
        self.writer.write((msg + "\r\n").encode())
        await self.writer.drain()

    async def handle_server_messages(self):
        """Lit les réponses du serveur et répond aux PING."""
        while self.running:
            try:
                data = await self.reader.readline()
                if not data:
                    break
                line = data.decode(errors="ignore").strip()
                if not line:
                    continue

                # Répondre aux PING
                if line.startswith("PING "):
                    token = line.split(" ", 1)[1]
                    await self.send(f"PONG {token}")

                # Optionnel : afficher les messages du serveur pour debug
                # print(f"[{self.nick}] {line}")
            except Exception:
                break

    async def send_messages(self):
        """Envoie des PRIVMSG en continu dans le canal."""
        count = 0
        while self.running:
            count += 1
            msg = f"Message #{count} de {self.nick}"
            await self.send(f"PRIVMSG {CHANNEL} :{msg}")
            await asyncio.sleep(MESSAGE_DELAY)

    async def run(self):
        """Lance connexion, lecture PING/PONG et envoi de messages."""
        await self.connect()
        # Lecture des messages du serveur (PING/PONG)
        reader_task = asyncio.create_task(self.handle_server_messages())
        # Envoi de messages
        sender_task = asyncio.create_task(self.send_messages())

        # Attendre que l'un des deux se termine (ici, on ne les arrête pas)
        await asyncio.gather(reader_task, sender_task, return_exceptions=True)


async def run_client(i):
    nick = f"bot{i}"
    c = IRCClient(nick)
    await c.run()


async def main():
    print(f"Lancement de {NUM_CLIENTS} clients IRC vers {HOST}:{PORT}")
    print(f"Canal : {CHANNEL}")
    if PASSWORD:
        print(f"Mot de passe : {PASSWORD}")
    else:
        print("Pas de mot de passe")

    await asyncio.gather(*(run_client(i) for i in range(1, NUM_CLIENTS + 1)))


if __name__ == "__main__":
    try:
        asyncio.run(main())
    except KeyboardInterrupt:
        print("\nArrêt des clients.")