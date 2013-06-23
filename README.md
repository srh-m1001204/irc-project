##NAME

  IRCBot - IRC Chatbot

##SYNOPSIS

  - IRCBot
  - IRCBot [nickname]
  - IRCBot [server] [port] [channel]
  - IRCBot [server] [port] [channel] [nickname]

##DESCRIPTION

  Dies ist ein IRC-Client, der sich mit einem IRC-Server verbindet und auf bestimmte Chat-Nachrichten reagiert. Hauptsächliche Aufgaben des Clients sind, den Chat mitzuloggen und auf Anfrage auszugeben und auf Anfrage anzuzeigen, welcher Benutzer, wann zuletzt etwas geschrieben hat. Dieser IRC-Client wurde auf dem Server irc.freenode.net getestet, auf dem er einwandfrei läuft. Jedoch spricht nichts dagegen, ihn auch auf anderen Servern zu testen. Es kann aber nicht versichert werden, dass dies ohne Probleme funktionieren wird. <br /> Servername, Port, Channel und der Nickname können beim Start angegeben werden. Falls nicht, werden diese aus einer Config-Datei herausgeladen. <br /> Auf folgende Befehle hört der IRC-Bot: <br />

##OPTIONS

  Damit der Bot auf die folgenden Befehle reagiert, muss vor den Befehlen der Botname und ein Doppelpunkt angegeben werden. (Bsp: TestBot: -nick TestBot2) Wenn Sie den Bot mit einer privaten Nachrichten ansprechen, brauchen Sie den Namen aber nicht anzugeben.
 
  - hi, hallo, hello		- der Bot antwortet mit einem Gruß
  - nick [name]			- zum Ändern des Nickname in [name]
  - join [channel]		- Bot betritt einen Channel
  - leave [channel]		- Bot verlässt einen Channel
  - topic [channel][topic]	- Bot ändert den das Thema des Channels
  - logging [on|off]		- Bot schaltet das Loggen des Chats ein/aus
  - show_log			- Bot gibt die letzten 10 Einträge des Logs aus
  - show_lastseen [name]	- Zeigt, wann ein Nutzer das letzte Mal etwas geschrieben hat
  - exit			- Bot verlässt den Server
 
##AUTHOR
  Buenyamin Eskiocak	buenyamin.eskiocak@fh-heidelberg.de