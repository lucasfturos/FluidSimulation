# Fluid Simulation in SDL2

Este projeto implementa uma simulação de fluidos utilizando C++ e SDL2. A simulação baseia-se em um método de diferenças finitas para resolver as equações de Navier-Stokes, com ruído Perlin adicionado para gerar uma textura inicial e efeitos.

Feito em C++, ImGui e SDL2.

## Instalação

Detalhes logo abaixo com os requisitos e instruções

<details><summary>Show</summary>

### Requisitos

-   CMake
-   GCC/G++
-   Make
-   SDL2
-   Emscripten

### Instruções para Gerar o Executável:

1. Clone o repositório:

```
git clone https://github.com/lucasfturos/FluidSimulation.git && cd FluidSimulation
```

2. Crie a pasta build:

```
cmake -S . -B build
```

3. Entre na pasta e Compile o programa:

```
cd build ; make -j4
```

4. Execute o programa:

```
./src/Fluid
```

### Instruções para Gerar o Site:

1. Clone o repositório como acima

2. Crie a pasta e entre nela

```
mkdir build_site && cd build_site
```

3. Inicialize as configurações de compilação:

```
emcmake cmake ..
```

4. Compile o programa:

```
emmake make
```

5. Execute o site no server de sua escolha na pasta src:

```
python3 -m http.server -d src/
```

E entre no site Fluid.html

</details>

## Reference

1. [Coding Challenge #132: Fluid Simulation](https://youtu.be/alhpH6ECFvQ)
2. [Fluid Simulation for Dummies - Mike Ash](https://mikeash.com/pyblog/fluid-simulation-for-dummies.html)
3. [NACA 4 digit airfoil generator - Airfoil Tools](http://airfoiltools.com/airfoil/naca4digit)
4. [Real-Time Fluid Dynamics for Games - Jos Stam](https://www.dgp.toronto.edu/public_user/stam/reality/Research/pdf/GDC03.pdf)
