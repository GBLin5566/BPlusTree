g++ -c BPTree/BPTree.cpp -std=gnu++0x
g++ -c BPTree/IndexPageManager.cpp -std=gnu++0x -fpermissive
g++ -c DataPageManager.cpp -std=gnu++0x
g++ -c KeyString.cpp -std=gnu++0x
g++ -c Record.cpp -std=gnu++0x
g++ -c Table.cpp Record.o -std=gnu++0x
g++ Parser.cpp BPTree.o IndexPageManager.o DataPageManager.o KeyString.o Table.o Record.o -std=gnu++0x
