# Fluid Simulation in SDL2

Este projeto implementa uma simulação de fluidos utilizando C++ e SDL2. A simulação baseia-se em um método de diferenças finitas para resolver as equações de Navier-Stokes, com ruído Perlin adicionado para gerar uma textura inicial e efeitos.

Feito em C++ e SDL2.

## Instalação

Detalhes logo abaixo com os requisitos e instruções

<details><summary>Show</summary>

### Requisitos

-   CMake
-   GCC/G++
-   Make
-   SDL2
-   Vulkan
-   Jsoncpp

### Instruções:

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

</details>

## Reference

1. [Coding Challenge #132: Fluid Simulation](https://youtu.be/alhpH6ECFvQ)
2. [Fluid Simulation for Dummies - Mike Ash](https://mikeash.com/pyblog/fluid-simulation-for-dummies.html)
3. [Real-Time Fluid Dynamics for Games - Jos Stam](https://www.dgp.toronto.edu/public_user/stam/reality/Research/pdf/GDC03.pdf)
