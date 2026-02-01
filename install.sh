#!/bin/bash

# Настройки твоего репозитория
USER="DalvSync"
REPO="I2Cat"
BRANCH="master"

echo "🐱 Installing the I2Cat..."

# 1. Создаем временную папку
mkdir -p /tmp/i2cat_install
cd /tmp/i2cat_install

# 2. Скачиваем свежие исходники (твои файлы)
echo "📥 Downloading files from GitHub..."
BASE_URL="https://raw.githubusercontent.com/$USER/$REPO/$BRANCH"

wget -q "$BASE_URL/edriver.c" -O i2cat.c
wget -q "$BASE_URL/edriver.h" -O i2cat.h
wget -q "$BASE_URL/font.h" -O font.h

# Проверка: скачались ли файлы?
if [ ! -f i2cat.c ]; then
    echo "❌ Error: Failed to download files."
    exit 1
fi

# 3. Компиляция
echo "⚙️ Compilation..."
# Компилируем объектный файл
gcc -c i2cat.c -o i2cat.o
# Пакуем в библиотеку libi2cat.a
ar rcs libi2cat.a i2cat.o

# 4. Установка в систему
echo "📦 Copying to system folders (requires sudo password)..."
sudo cp libi2cat.a /usr/local/lib/
sudo cp i2cat.h /usr/local/include/
sudo cp font.h /usr/local/include/

# Обновляем кэш библиотек
sudo ldconfig

# 5. Уборка
cd ~
rm -rf /tmp/i2cat_install

echo "✅ SUCCESS! I2Cat is installed."
echo "---------------------------------------------------"
echo "How to use in code:"
echo "   #include <i2cat.h>"
echo ""
echo "How to compile:"
echo "   gcc main.c -li2cat -o app"
echo "---------------------------------------------------"
