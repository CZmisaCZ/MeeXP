# MeeXP / Mee6like-XP-Bot
Simple C++ discord bot, that is using the same system for use XP and ranks as Mee6.

# How to setup (linux - debian)

note: this installation might slightly differ if you are using different linux distribution, if you are using different distro please read how to install libraries from official sites

### Dependencies:

DPP library [official site / documentation](https://dpp.dev/install-linux-deb.html)

libsodium library [official site / documentation](https://doc.libsodium.org/)

g++ compiler, wget, unzip

### Installing dependencies:
```
sudo apt install g++ unzip wget

# DPP instalation:

wget -O dpp.deb https://dl.dpp.dev/

dpkg -i dpp.deb

rm dpp.deb -fr

# libsodium instalation:

wget https://download.libsodium.org/libsodium/releases/LATEST.tar.gz

tar -xvzf LATEST.tar.gz -C .

cd libsodium-stable

./configure

make && make check

sudo make install

cd ..

rm libsodium-stable -fr

rm LATEST.tar.gz -fr
```

### Instalation/Compiling:
```
wget https://github.com/CZmisaCZ/MeeXP/archive/refs/heads/master.zip

unzip master.zip

cd MeeXP

cd LinuxBot

# compilig (without cmake):

g++ -o3 -std=c++17 main.cpp fileIO.cpp event.cpp -o xpbot -ldpp
```
### Configuring:

`nano settings.h`

**In this file change: BOT TOKEN to your bot token** [how to get bot token](https://youtu.be/aI4OmIbkJH8).

Change admins `{ DISCORD-USER-ID, DISCORD-USER-ID-2, DISCORD-USER-ID-3... }` this is a list of admins that can use /givexp and /setxp, you can put there as many users as you want or leave it empty, its up to you.

You can change other settings like save intervals, nuber of backups... therese are not as important.

save and exit using: 

Ctrl+X

Y

enter

### Compilation and Start:

compile using: `g++ -o3 -std=c++17 main.cpp fileIO.cpp event.cpp -o xpbot -ldpp`

start using: `./xpbot`
