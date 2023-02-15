# Mee6like-XP-Bot
Simple C++ discord bot, that is using the same system for use XP and ranks as Mee6.

# how to setup (linux - debian)

note: this installation might slightly differ if you are using different linux distribution, if you are using different distro please read how to install libraries from official sites

### requirements:

DPP library [official site / documentation](https://dpp.dev/install-linux-deb.html)

libsodium library [official site / documentation](https://doc.libsodium.org/)

g++ compiler

### requirements instalation:

sudo apt install g++

wget https://download.libsodium.org/libsodium/releases/LATEST.tar.gz

tar -xvzf LATEST.tar.gz -C .

cd libsodium-stable

./configure

make && make check

sudo make install

### instalation:

