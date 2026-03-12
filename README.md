# Detecção de Colisão 2D

Sistema de detecção de colisão em 2D com aceleração através de particionamento espacial, desenvolvido com OpenGL/GLUT.

## 📋 Descrição

Este projeto implementa um sistema de detecção de colisão entre linhas em um ambiente 2D. O objetivo principal é demonstrar técnicas de aceleração de testes de colisão usando estruturas de particionamento espacial (subespaços), comparando o desempenho com e sem otimização.

## ✨ Funcionalidades

- **Detecção de Interseção**: Algoritmo de detecção de interseção entre segmentos de linha
- **Particionamento Espacial**: Divisão do espaço em subespaços para acelerar os testes de colisão
- **Visualização em Tempo Real**: Interface gráfica usando OpenGL/GLUT
- **Veículo Controlável**: Movimente e rotacione um veículo para testar colisões com as linhas
- **Medição de Performance**: Sistema de FPS para avaliar o desempenho
- **Visualização de Subespaços**: Opção para exibir as partições espaciais

## 🎮 Controles

| Tecla     | Ação                          |
| --------- | ----------------------------- |
| `e`       | Exibe/esconde linhas          |
| `s`       | Desenha/esconde subespaços    |
| `f`       | Exibe/esconde FPS             |
| `r/R`     | Rotaciona o veículo           |
| `↑ ↓ ← →` | Movimenta o veículo           |
| `ESPAÇO`  | Liga/desliga teste de colisão |

## 🛠️ Tecnologias Utilizadas

- **C++**: Linguagem de programação principal
- **OpenGL**: Biblioteca gráfica para renderização
- **GLUT**: Toolkit para criação de janelas e gerenciamento de entrada

## 📁 Estrutura do Projeto

```
collision-detection/
├── Basico2D/
│   ├── InterseccaoEntreLinhas.cpp  # Arquivo principal com lógica de colisão
│   ├── Linha.h                      # Definição da classe Linha
│   ├── Linha.cpp                    # Implementação da classe Linha
│   ├── Ponto.h                      # Definição da estrutura Ponto
│   ├── Arvore.h                     # Estruturas de dados auxiliares
│   ├── Temporizador.h/cpp           # Sistema de temporização
│   └── OpenGL.xcodeproj/            #
```

## 🚀 Como Compilar e Executar

### macOS

1. Abra o projeto no Xcode:

   ```bash
   open Basico2D/OpenGL.xcodeproj
   ```

2. Compile e execute o projeto através do Xcode (⌘ + R)

### Linux

1. Certifique-se de ter as dependências instaladas:

   ```bash
   sudo apt-get install freeglut3-dev
   ```

2. Compile o projeto:

   ```bash
   cd Basico2D
   g++ InterseccaoEntreLinhas.cpp Linha.cpp Temporizador.cpp -o colisao -lGL -lGLU -lglut
   ```

3. Execute:

   ```bash
   ./colisao
   ```

### Windows

1. Abra o projeto no Code::Blocks:
   - Arquivo: `Basico2D/OpenGL.cbp`

2. Compile e execute através do Code::Blocks (F9)

## 🎯 Conceitos Implementados

### Detecção de Interseção

O sistema utiliza um algoritmo de interseção entre segmentos de linha baseado em equações paramétricas. Duas linhas se interceptam quando os parâmetros `s` e `t` estão no intervalo [0, 1].

### Particionamento Espacial

Para acelerar os testes de colisão, o espaço é dividido em subespaços (partições). Cada partição mantém uma lista de índices das linhas que a intersectam ou estão contidas nela. Isso reduz drasticamente o número de testes de colisão necessários.

### Envelope (Bounding Box)

Cada linha possui um envelope (min/max x,y) que permite testes rápidos de exclusão antes de realizar o teste completo de interseção.

## 📊 Performance

O projeto permite comparar o desempenho com e sem o uso de particionamento espacial:

- **Sem otimização**: O(n) comparações por frame, onde n é o número de linhas
- **Com particionamento**: O(k) comparações por frame, onde k é o número de linhas em subespaços relevantes (k << n)

Use a tecla `f` para exibir o FPS e avaliar a diferença de performance.

## 👥 Autores

Larissa Magistrali e Nikolas Lacerda

## 📚 Contexto Acadêmico

Este projeto foi desenvolvido como exercício didático para a disciplina de Computação Gráfica, com foco em:

- Algoritmos de detecção de colisão
- Estruturas de dados espaciais
- Otimização de performance em sistemas gráficos
- Programação com OpenGL/GLUT"
