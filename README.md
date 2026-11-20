# 🖨️ Integração C e Impressora Elgin i9
### Sistema de comunicação e controle da impressora Elgin

---

## 📘 Sobre o Projeto

O projeto **Integração C e Impressora Elgin** foi desenvolvido por alunos da faculdade com o objetivo de criar uma aplicação em linguagem C capaz de se comunicar com a impressora térmica **Elgin i9**, utilizando a biblioteca dinâmica `E1_Impressora01.dll`.

A aplicação possui um menu interativo que permite ao usuário configurar e gerenciar a conexão com a impressora, realizar diferentes operações de impressão e executar funções adicionais, como abertura da gaveta de dinheiro e emissão de sinais sonoros.

O trabalho faz parte da disciplina de **Construção de Algoritmos e Programação**.

---

## 🧰 Tecnologias Utilizadas

- **Linguagem de programação:** C
- **Ambiente de desenvolvimento:** Dev-C++
- **Bibliotecas utilizadas:**
  - `stdio.h` – Entrada e saída de dados.
  - `stdlib.h` – Gerenciamento de memória e funções do sistema.
  - `string.h` – Manipulação de strings.
  - `windows.h` – Acesso às funções do Windows e carregamento da DLL.
- **Biblioteca externa:** `E1_Impressora01.dll` – Comunicação com a impressora Elgin i9.

---

## 📁 Estrutura do Projeto

Todos os arquivos necessários para o funcionamento do projeto estão organizados em uma única pasta.

```text
Integracao-C-Impressora-Elgin/
│
├── main.c
├── E1_Impressora01.dll
├── XMLSAT.xml
└── CANC_SAT.XML
```

**Descrição dos arquivos:**

- `main.c`: código-fonte principal da aplicação.
- `E1_Impressora01.dll`: biblioteca dinâmica responsável pela comunicação com a impressora.
- `XMLSAT.xml`: arquivo utilizado na impressão SAT.
- `CANC_SAT.XML`: arquivo utilizado na impressão do cancelamento SAT.

---

## ⚙️ Instalação e Execução

### 🔽 1. Baixar o Projeto do GitHub

1. Acesse o repositório do projeto no GitHub.
2. Clique no botão **Code**.
3. Selecione **Download ZIP**.
4. Aguarde o download dos arquivos.

### 📦 2. Extrair o Arquivo ZIP

1. Localize o arquivo ZIP baixado.
2. Clique com o botão direito do mouse.
3. Selecione **Extrair Tudo**.
4. Será criada uma pasta contendo os arquivos do projeto.

### 🧭 3. Abrir o Projeto no Dev-C++

1. Abra o **Dev-C++**.
2. Clique em **File → Open**.
3. Selecione o arquivo `main.c`.
4. Verifique se todos os arquivos necessários estão na pasta do projeto.
5. Compile o código utilizando a opção **Execute → Compile & Run**.

### 🖨️ 4. Executar o Programa

1. Certifique-se de que a impressora Elgin i9 esteja conectada ao computador.
2. Execute o programa.
3. Realize o login utilizando as credenciais de teste.
4. Após a autenticação, será exibido o menu principal.
5. Configure os parâmetros de conexão com a impressora.
6. Abra a conexão para utilizar as funcionalidades disponíveis.

### ⚠️ Requisitos e Observações

- O programa utiliza funções específicas do Windows, portanto precisa ser executado em um ambiente Windows.
- A DLL `E1_Impressora01.dll` deve estar acessível na pasta de execução do programa.
- A arquitetura da DLL deve ser compatível com a arquitetura do compilador:
  - Compilador de 32 bits: utilizar DLL de 32 bits.
  - Compilador de 64 bits: utilizar DLL de 64 bits.
- O nome da DLL deve permanecer exatamente como utilizado no código: `E1_Impressora01.dll`.
- Os arquivos `XMLSAT.xml` e `CANC_SAT.XML` devem permanecer acessíveis na pasta de execução, pois são utilizados nas operações de impressão SAT.

---

## 🔐 Login de Teste

Para acessar o sistema, utilize as seguintes credenciais:

| Campo | Credencial |
|---|---|
| Usuário | `admin` |
| Senha | `1234` |

O sistema permite até **3 tentativas de acesso** antes de negar a entrada.

---

## 🧪 Funcionalidades e Testes

O sistema disponibiliza as seguintes funcionalidades:

| Opção | Funcionalidade | Descrição |
|---|---|---|
| 1 | Configurar conexão | Define o tipo de conexão, modelo da impressora, porta e parâmetro adicional. |
| 2 | Abrir conexão | Estabelece a comunicação com a impressora. |
| 3 | Fechar conexão | Encerra a comunicação com o equipamento. |
| 4 | Imprimir texto | Permite informar um texto e a quantidade de cópias desejada. |
| 5 | Imprimir QR Code | Realiza a impressão de um QR Code com os dados informados. |
| 6 | Imprimir código de barras | Realiza a impressão de um código de barras. |
| 7 | Imprimir XML SAT | Realiza a impressão utilizando o arquivo `XMLSAT.xml`. |
| 8 | Imprimir XML de Cancelamento SAT | Realiza a impressão utilizando o arquivo `CANC_SAT.XML`. |
| 9 | Abrir gaveta Elgin | Aciona a abertura da gaveta por meio da função específica da Elgin. |
| 10 | Abrir gaveta | Disponibiliza outra função de acionamento da gaveta. |
| 11 | Emitir sinal sonoro | Permite emitir sinais sonoros pela impressora de acordo com a quantidade informada. |
| 0 | Sair | Encerra a execução do programa. |

As funcionalidades foram testadas utilizando uma impressora física **Elgin i9**.

---

## 📚 Documentação Adicional

A comunicação entre a aplicação e a impressora é realizada por meio da biblioteca dinâmica `E1_Impressora01.dll`, que disponibiliza as funções utilizadas pelo programa.

Os arquivos XML utilizados nas operações SAT acompanham o código-fonte do projeto.

---

## 👥 Autores

- **Evelyn Vital da Silva** — RA: 252166
- **Jonatas da Silva Santos Junior** — RA: 181867
- **Leonardo Araujo Morais da Silva** — RA: 247328
- **Ruan Lucas Silva de Assis** — RA: 078428

---

**Trabalho acadêmico desenvolvido para a disciplina de Construção de Algoritmos e Programação.**