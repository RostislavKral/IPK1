# Dokumentace projektu IPK1 2022/2023 #
## Jméno a příjmení: Rostislav Král
## Login: xkralr06 ##

---

## Základní informace:

#### Projekt používá:
- Překladač g++, standard C++20
- Python 3.8 pro testovací skript
- Make
- Lze spustit pouze na linuxovém prostředí např. (Debian based, CentOS aj.)
- Má pouze jeden soubor se zdrojovým kódem
- Licence GPL3
---
## Průběh programu

Program začíná registrací signal handlerů a parsováním argumentů z terminálu od uživatele použtím knihovny ``getopt``, 
kde se vyhodnocuje správný počet a jednotlivé omezení daného argumentu (např. přepínač ``-m`` může nabývat řetězců ``udp`` nebo
``tcp``, stejně tak je zamezeno přepínači ``-p`` přijmout well-known porty (0-1023) aby program neblokoval využívaný port).
Dále dle daného módu 
se volá buďto funkce ``void tcp(const std::string &hostname, int port)`` nebo ``void udp(const std::string &hostname, int port)``.
Číslo socketu a mód jsou globalními proměnnými, právě z důvodu signal handlerů.

#### TCP
Běží v nekonečné smyčce, která čeká na vstup od uživatele a končí pouze v případě SIGINTu nebo posláním příkazu ``BYE``.
V případě příkazu SIGINTu se handler postará o to, aby se na server poslal příkaz ``BYE`` a dal tak serveru vědět o konci
streamu. Samotný stream běží ve samostatné smyčce a přijímá sockety dokud nedostane ``\n``, tím smyčka končí a klient tak dostane
celou zprávu.

#### UDP
Také běží v nekonečné smyččce, která čeká na vstup uživatele, avšak narozdíl od TCP "parsuje" zadaný řetězec do bytestringu
a nastavuje správný ``OPCODE`` a délku do hlavičky. Odešle socket a na základě poslaného ``RETURN CODE`` vypíše odpověď serveru
ve správné formě (OK vs ERR). V případě ukončení přes SIGINT program zavírá socket a program končí.

---
## TESTY

Většinu testů jsem prováděl ručně ale podařil se mi naimplementovat (bohužel jen pro UDP) Python skript s pár testy na ukázku
jak byl program testován. Skript se dá spustit pomocí ``python3 tester.py``, kdy se vypíší všechny testy v adresáři ``tests\ ``.
Jsou potřeba dva soubory s příponou .txt, potom jeden soubor s názvemTestu a druhý názevTestu-output.