## Graph manipulation with graphviz

![Graph 1](./screenshots/1.png)

## Running project

Install the [Graphviz](https://graphviz.org/) software 

This software is necessary for graphical visualization of the graph or digraph

##### For Debian variants systems

```
sudo apt install graphviz
```

### Usage

```
g++ main.cpp -o program.o
```

```
./program.o <text-file>
```

```
./program.o 
```

The last command above will use the default "input.txt" file as input

## Text file structure with examples

```
V = {1,2,3,4,5,6}; A = {(4,2),(3,4),(4,1),(4,6),(5,4)}; P = {2,3,1,5,7};
```

![Graph 1](./screenshots/1.png)

```
V = {1,2,3,4,5,6,7,8}; A = {(4,2),(2,3),(3,4),(4,1),(3,1),(4,6),(5,7),(6,8),(7,8),(1,5),(2,6),(3,7),(4,8)};
```

![Graph 2](./screenshots/2.png)


```
V = {1,2,3,4,5,6}; A = {(4,2),(3,4),(4,1),(4,6),(5,4)}; P = {2,3,1,5,7};
```

![Graph 3](./screenshots/3.png)


```
V = {0,1,2,3,4,5,6,7}; A = {(0,1),(1,2),(2,3),(3,0),(2,4),(4,5),(5,6),(6,4),(6,7)};
```

![Graph 4](./screenshots/4.png)

