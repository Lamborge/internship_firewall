#!/bin/bash

# Функция для генерации случайного IP-адреса
generate_ip() {
    echo "$((RANDOM % 256)).$((RANDOM % 256)).$((RANDOM % 256)).$((RANDOM % 256))"
}

# Функция для генерации случайного порта
generate_port() {
    echo "$((RANDOM % 65536))"
}

# Функция для генерации случайного протокола
generate_protocol() {
    protocols=(6 17 27 28)
    echo "${protocols[RANDOM % ${#protocols[@]}]}"
}

# Генерация строки формата
ip1=$(generate_ip)
ip2=$(generate_ip)
port1=$(generate_port)
port2=$(generate_port)
protocol=$(generate_protocol)
output="$ip1 $ip2 $port1 $port2 $protocol"

# Вывод строки для проверки (можно закомментировать)
echo "Generated string: $output"

# Подача строки на stdin другой программе
# Замените `other_program` на команду запуска другой программы
echo "$output" | ./main custom.txt

