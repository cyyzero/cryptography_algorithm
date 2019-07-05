g++ *.cpp -c -g;
cd bank;
g++ *.cpp ../*.o -g -I../ -pthread;
cd ../merchant;
g++ *.cpp ../*.o -g -I../ -pthread;
cd ../customer;
g++ *.cpp ../*.o -g -I../ -pthread;
