# Filetree
```
├── bin              # folder will be created after build project
│   -----
├── build.sh         # build script
├── custom_db.txt    # db file created by myself
├── examples.txt     # example output shows how program should work
├── intership_db.txt # db file given by company
├── in.txt           # contains exaples of input strings (can be used by program)
├── main.c           # main source file of program
├── random.sh        # bash script for generating random input string and put it to program stdin
└── README.md        # README
```

# Build
For Linux
```
$ git clone https://github.com/Lamborge/internship_firewall.git
$ cd internship_firewall
$ chmod +x ./build.sh
$ chmod +x ./random.sh
$ sh build.sh
```
# Run
```
$ cd bin
# examples:
$ ./firewall custom.db
$ ./firewall custom.db in.txt
```
# Program run cases
```
./firewall [db filename]
```
Program will run and use rules database from file given by first argument  
Packages will must be inputted by user  

```
./firewall [db filename] [inputs file filename]
```
Same as previous variant, but input packages will be taken from file from second argument
# Random packages generator
```random.sh``` - random packages generator script  
It generate random packages from template inside script and gives generated package line into program stdin
# Rule db file syntax
```src: [IPv4]  	dst: [IPv4]	src_port: [0-65536]	dst_port: [0-65536] 	proto: [tcp/udp/rdp/irtp]	=> [ACCEPT/DROP]```  
Pair of fields must be separated by space or tab symbol  
  
```=> [ACCEPT/DROP]``` is the only one required pair, others can be omitted  
The last rule string must NOT be ending at new line symbol
