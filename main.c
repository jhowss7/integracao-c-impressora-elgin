#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

/*
- Controle da impressora Elgin (i8 / i9) em C, usando a DLL E1_Impressora01.dll.
 *
- 		================ GRUPO: JONATAS DA SILVA SANTOS JUNIOR - RA: 181867
								LEONARDO ARAUJO MORAIS DA SILVA - RA: 247328
								EVELYN VITAL DA SILVA - RA: 252166
								RUAN LUCAS SILVA DE ASSIS - RA: 078428
								
 */

/* ===================== Ligacao com a DLL (equivalente a interface ImpressoraDLL do Java) ===================== */

/* Cada typedef descreve a "forma" de um comando da DLL: o que devolve e quais parametros recebe */
typedef int (*FnAbreConexaoImpressora)(int tipo, const char *modelo, const char *conexao, int param);
typedef int (*FnFechaConexaoImpressora)(void);
typedef int (*FnImpressaoTexto)(const char *dados, int posicao, int estilo, int tamanho);
typedef int (*FnCorte)(int avanco);
typedef int (*FnImpressaoQRCode)(const char *dados, int tamanho, int nivelCorrecao);
typedef int (*FnImpressaoCodigoBarras)(int tipo, const char *dados, int altura, int largura, int HRI);
typedef int (*FnAvancaPapel)(int linhas);
typedef int (*FnStatusImpressora)(int param);
typedef int (*FnAbreGavetaElgin)(int pino, int ti, int tf);
typedef int (*FnAbreGaveta)(int pino, int ti, int tf);
typedef int (*FnSinalSonoro)(int qtd, int tempoInicio, int tempoFim);
typedef int (*FnModoPagina)(void);
typedef int (*FnLimpaBufferModoPagina)(void);
typedef int (*FnImprimeModoPagina)(void);
typedef int (*FnModoPadrao)(void);
typedef int (*FnPosicaoImpressaoHorizontal)(int posicao);
typedef int (*FnPosicaoImpressaoVertical)(int posicao);
typedef int (*FnImprimeXMLSAT)(const char *dados, int param);
typedef int (*FnImprimeXMLCancelamentoSAT)(const char *dados, const char *assQRCode, int param);

/* Um ponteiro para cada comando. Depois de carregarDLL(), e so chamar: AbreConexaoImpressora(...) */
static FnAbreConexaoImpressora AbreConexaoImpressora;
static FnFechaConexaoImpressora FechaConexaoImpressora;
static FnImpressaoTexto ImpressaoTexto;
static FnCorte Corte;
static FnImpressaoQRCode ImpressaoQRCode;
static FnImpressaoCodigoBarras ImpressaoCodigoBarras;
static FnAvancaPapel AvancaPapel;
static FnStatusImpressora StatusImpressora;
static FnAbreGavetaElgin AbreGavetaElgin;
static FnAbreGaveta AbreGaveta;
static FnSinalSonoro SinalSonoro;
static FnModoPagina ModoPagina;
static FnLimpaBufferModoPagina LimpaBufferModoPagina;
static FnImprimeModoPagina ImprimeModoPagina;
static FnModoPadrao ModoPadrao;
static FnPosicaoImpressaoHorizontal PosicaoImpressaoHorizontal;
static FnPosicaoImpressaoVertical PosicaoImpressaoVertical;
static FnImprimeXMLSAT ImprimeXMLSAT;
static FnImprimeXMLCancelamentoSAT ImprimeXMLCancelamentoSAT;

static HMODULE dll = NULL;

/* Procura um comando dentro da DLL. Devolve 1 se achou, 0 se nao achou. */
static int carregaFuncao(void *destino, const char *nome) {
    FARPROC f = GetProcAddress(dll, nome);
    if (f == NULL) {
        printf("Comando '%s' nao encontrado na DLL.\n", nome);
        return 0;
    }
    *(FARPROC *)destino = f;
    return 1;
}

/* Carrega a DLL da raiz do projeto e todos os comandos. Devolve 1 se deu certo. */
static int carregarDLL(void) {
    dll = LoadLibraryA("E1_Impressora01.dll");
    if (dll == NULL) {
        printf("Nao foi possivel carregar E1_Impressora01.dll (erro %lu).\n", GetLastError());
        printf("Confira: a DLL esta na raiz do projeto? Ela tem a mesma arquitetura (32/64 bits) do compilador?\n");
        return 0;
    }

    int ok = 1;
    ok &= carregaFuncao(&AbreConexaoImpressora, "AbreConexaoImpressora");
    ok &= carregaFuncao(&FechaConexaoImpressora, "FechaConexaoImpressora");
    ok &= carregaFuncao(&ImpressaoTexto, "ImpressaoTexto");
    ok &= carregaFuncao(&Corte, "Corte");
    ok &= carregaFuncao(&ImpressaoQRCode, "ImpressaoQRCode");
    ok &= carregaFuncao(&ImpressaoCodigoBarras, "ImpressaoCodigoBarras");
    ok &= carregaFuncao(&AvancaPapel, "AvancaPapel");
    ok &= carregaFuncao(&StatusImpressora, "StatusImpressora");
    ok &= carregaFuncao(&AbreGavetaElgin, "AbreGavetaElgin");
    ok &= carregaFuncao(&AbreGaveta, "AbreGaveta");
    ok &= carregaFuncao(&SinalSonoro, "SinalSonoro");
    ok &= carregaFuncao(&ModoPagina, "ModoPagina");
    ok &= carregaFuncao(&LimpaBufferModoPagina, "LimpaBufferModoPagina");
    ok &= carregaFuncao(&ImprimeModoPagina, "ImprimeModoPagina");
    ok &= carregaFuncao(&ModoPadrao, "ModoPadrao");
    ok &= carregaFuncao(&PosicaoImpressaoHorizontal, "PosicaoImpressaoHorizontal");
    ok &= carregaFuncao(&PosicaoImpressaoVertical, "PosicaoImpressaoVertical");
    ok &= carregaFuncao(&ImprimeXMLSAT, "ImprimeXMLSAT");
    ok &= carregaFuncao(&ImprimeXMLCancelamentoSAT, "ImprimeXMLCancelamentoSAT");
    return ok;
}

/* ===================== Variaveis do programa ===================== */

static int conexaoAberta = 0;       /* 0 = fechada, 1 = aberta */
static int tipo = 1;                /* 1 = USB, 2 = Serial, 3 = Rede... */
static char modelo[50] = "i9";      /* "i8" ou "i9" */
static char conexao[50] = "USB";
static int parametro = 0;

/* Chave do QR Code usada no cancelamento SAT */
static const char *ASS_QRCODE =
    "Q5DLkpdRijIRGY6YSSNsTWK1TztHL1vD0V1Jc4spo/CEUqICEb9SFy82ym8EhBRZjbh3btsZhF+sjHqEMR159i4agru9x6KsepK/"
    "q0E2e5xlU5cv3m1woYfgHyOkWDNcSdMsS6bBh2Bpq6s89yJ9Q6qh/J8YHi306ce9Tqb/drKvN2XdE5noRSS32TAWuaQEVd7u+"
    "TrvXlOQsE3fHR1D5f1saUwQLPSdIv01NF6Ny7jZwjCwv1uNDgGZONJdlTJ6p0ccqnZvuE70aHOI09elpjEO6Cd+orI7XHHrFCwhFhAcbalc+"
    "ZfO5b/+vkyAHS6CYVFCDtYR9Hi5qgdk31v23w==";

/* ===================== Funcoes auxiliares ===================== */

/* Mostra a mensagem e le uma linha de texto digitada (sem o ENTER no final) */
static void capturarEntrada(const char *mensagem, char *destino, int tamanho) {
    printf("%s", mensagem);
    if (fgets(destino, tamanho, stdin) == NULL) {
        destino[0] = '\0';
        return;
    }
    destino[strcspn(destino, "\r\n")] = '\0';
}

/* Mostra a mensagem e le um numero inteiro. Se digitar letras, pergunta de novo. */
static int lerInteiro(const char *mensagem) {
    char linha[50];
    char *fim;
    while (1) {
        capturarEntrada(mensagem, linha, sizeof(linha));
        long valor = strtol(linha, &fim, 10);
        if (linha[0] != '\0' && *fim == '\0') {
            return (int)valor;
        }
        printf("Digite apenas numeros!\n");
    }
}

/* Le um arquivo inteiro para a memoria. Devolve NULL se nao achar. Quem chama deve dar free(). */
static char *lerArquivo(const char *caminho) {
    FILE *arq = fopen(caminho, "rb");
    if (arq == NULL) {
        return NULL;
    }
    fseek(arq, 0, SEEK_END);
    long tamanho = ftell(arq);
    rewind(arq);

    char *conteudo = (char *)malloc(tamanho + 1);
    if (conteudo == NULL) {
        fclose(arq);
        return NULL;
    }
    size_t lidos = fread(conteudo, 1, tamanho, arq);
    conteudo[lidos] = '\0';
    fclose(arq);
    return conteudo;
}

/* ===================== Funcoes da impressora ===================== */

/* Configura os parâmetros de conexão caso ela esteja fechada */
void configurarConexao(void) {
    if (!conexaoAberta) {
        tipo = lerInteiro("Digite o tipo de conexao (1 para USB, 2 para Serial, etc.): ");
        capturarEntrada("Digite o modelo da impressora: ", modelo, sizeof(modelo));
        capturarEntrada("Digite a porta de conexao (ex: USB): ", conexao, sizeof(conexao));
        parametro = lerInteiro("Digite o parametro adicional (ex: 0 para padrao): ");
        system("cls");
        printf("Parametros de conexao configurados com sucesso.\n");
    } else {
        printf("Conexao ja configurada. Pronta para uso.\n");
    }
}

/* Abre a conexão com a impressora  */
void abrirConexao(void) {
    if (!conexaoAberta){
        int retorno = AbreConexaoImpressora(tipo, modelo, conexao, parametro);
        if (retorno == 0) {
            conexaoAberta = 1;
            printf("Conexao aberta com sucesso.\n");
        } else {
            printf("Erro ao abrir conexao. Codigo de erro: %d\n", retorno);
        }
    } else {
        printf("Conexao ja esta aberta.\n");
    }
}

/* Fecha a conexão ativa com a impressora */
void fecharConexao(void) {
	 if (conexaoAberta){
        int retorno = FechaConexaoImpressora();
        if (retorno == 0) {
            conexaoAberta = 0;
            printf("Conexao Fechada com sucesso.\n");
        } else {
            printf("Erro ao abrir conexao. Codigo de erro: %d\n", retorno);
        }
    } else {
        printf("A Conexao ja esta Fechada.\n");
    }
}

/* Lê um texto digitado pelo usuário e realiza a impressão com base na quantidade de cópias informada */
void imprimirTexto(void) {
   if (!conexaoAberta) { 
		printf("Conexao fechada. Abra a conexao antes (opcao 2).\n");
		return; 
}
	char texto[200];
	capturarEntrada("Texto a imprimir: ", texto, sizeof(texto));
	int copias = lerInteiro("Quantas copias? ");
	int i;
	
	for (i = 1; i <= copias; i++) { 
		int retorno = ImpressaoTexto(texto, 1, 4, 0); 
		if (retorno >= 0){
			printf("Copia %d impressa!\n", i);
		}else{
			printf("Erro ao imprimir. Codigo: %d\n", retorno);
		}
	}
		Corte(5);
}

/* imprime o QR Code */
void imprimirQRCode(void) {
    if (!conexaoAberta) { 
		printf("Conexao fechada. Abra a conexao antes (opcao 2).\n");
		return; 
}
	char dados[200];
	capturarEntrada("Texto a Imprimir: ", dados, sizeof(dados));
	int copias = lerInteiro("Quantas copias? ");
	int i;
	
	for(i=1; i<=copias; i++){
		int retorno = ImpressaoQRCode(dados, 6, 4);
		if(retorno>=0){
			printf("Copia %d impressa!\n", i);
		}else{
			printf("Erro ao imprimir. Codigo: %d\n", retorno);
		}
			Corte(5);
	}
}

/* Imprime um código de barras  */
void imprimirCodigoBarras(void) {
    if (!conexaoAberta) { 
		printf("Conexao fechada. Abra a conexao antes (opcao 2).\n");
		return; 
}
	int retorno = ImpressaoCodigoBarras(8,"{A012345678912", 100, 2, 3);
	if(retorno == 0){
		printf("Impressao Bem sucedida");
	}else{
		printf("Erro na Impressao. Codigo: %d", retorno);
	}
		Corte(5);
}

/* realiza a impressão do cancelamento do XML SAT */
void imprimirXMLSAT(void) {
    if (!conexaoAberta) { 
		printf("Conexao fechada. Abra a conexao antes (opcao 2).\n");
		return; 
	}	
		char *dadosXML = lerArquivo("XMLSAT.xml");
		if (dadosXML == NULL) {
        	printf("Nao foi possivel ler o arquivo.xml\n");
        	return;
    	}
		int retorno = ImprimeXMLSAT(dadosXML, 0);
		free(dadosXML); 
		if(retorno == 0){
			printf("XML SAT impresso!\n");
		}else{
			printf("Erro ao imprimir XML SAT. Codigo: %d\n", retorno);
	}
		Corte(5);
}

/* realiza a impressão do cancelamento do XML Cancelamento SAT */
void imprimirXMLCancelamentoSAT(void) {
    if(!conexaoAberta){ 
			printf("Conexao fechada. Abra a conexao antes (opcao 2).\n");
	return; 
}
	char *dadosXML = lerArquivo("CANC_SAT.XML");
		if (dadosXML == NULL) {
        	printf("Nao foi possivel ler o arquivo.xml\n");
        	return;
    	}
	int retorno = ImprimeXMLCancelamentoSAT(dadosXML, ASS_QRCODE, 0);
	free(dadosXML);
		if(retorno == 0){
			printf("XML Cancelamento SAT impresso!\n");
		}else{
			printf("Erro ao imprimir XML Cancelamento SAT. Codigo: %d\n", retorno);
	}
		Corte(5);    
}

/* Aciona a abertura da gaveta de dinheiro usando a função específica da Elgin */
void abrirGavetaElgin(void) {
    if (!conexaoAberta) { 
		printf("Conexao fechada. Abra a conexao antes (opcao 2).\n");
		return; 
}
	int retorno = AbreGavetaElgin(1, 50, 5);
	if(retorno == 0){
		printf("Gaveta Aberta com sucesso\n");
	}else{
		printf("Erro ao abrir a Gaveta\n");
	}
	
}

/* Aciona a abertura da gaveta de dinheiro */
void abrirGaveta(void) {
    if (!conexaoAberta) { 
		printf("Conexao fechada. Abra a conexao antes (opcao 2).\n");
		return; 
}
	int retorno = AbreGaveta(1, 5, 10);
	if(retorno == 0){
		printf("Gaveta Aberta com sucesso\n");
	}else{
		printf("Erro ao abrir a Gaveta\n");
	}
}

/* Emite sinais sonoros na impressora de acordo com a quantidade informada pelo usuário */
void emitirSinalSonoro(void) {

    if (!conexaoAberta) {
        printf("Conexao fechada. Abra a conexao antes (opcao 2).\n");
        return;
    }

    int i, retorno = -1;
    int qtd = lerInteiro("Quantas vezes? ");

    if (qtd <= 0) {
        printf("Informe uma quantidade valida!\n");
        return;
    }

    for(i = 1; i <= qtd; i++) {
        retorno = SinalSonoro(4, 50, 5);
    }

    if(retorno == 0) {
        printf("Sinal Emitido com Sucesso");
    } else {
        printf("Falha na emissao de Sinal Sonoro");
    }
}

/* Exibe o menu principal de opções do sistema */
void exibirMenu(void) {
    printf("\n*************************************************\n");
    printf("**************** MENU IMPRESSORA ****************\n");
    printf("*************************************************\n\n");

    printf("1  - Configurar Conexao\n");  
	printf("2  - Abrir Conexao\n");
	printf("3  - Fechar Conexao\n");
	printf("4  - Imprimir Texto\n");
	printf("5  - Imprimir QRCode\n");
	printf("6  - Imprimir Código de Barras\n");
	printf("7  - Imprimir XMLSAT\n");
	printf("8  - Imprimir XML Cancelamento SAT\n");
	printf("9  - Abrir Gaveta Elgin\n");
	printf("10 - Abrir Gaveta\n");
	printf("11 - Emitir Sinal Sorono\n");
	printf("0  - Sair\n");
    printf("--------------------------------------\n");
}

/* Realiza a validação de usuário e senha com limite de até 3 tentativas de acesso */
int Login(void){
	char usuario[50], senha[50];
	int j=2, tentativa;
	
	for(tentativa = 1; tentativa<= 3; tentativa++){
		printf("/======================= LOGIN =======================/" "\n\n");
		capturarEntrada("Digite seu usuario: ",usuario, 50);
		capturarEntrada("Digite a senha: ",senha,50);
		if(strcmp(usuario, "admin") == 0 && strcmp(senha, "1234")==0){
			system("cls");
			return 1;
		}else{
			if(j==0){
				return 0;
			}
			system("cls");
			printf("\nRestam %d tentativas\n\n", j--);
			} 
		}
	return 0;
}

/* Função principal  autenticação e execução do menu */
int main(void) {
    
	if (!carregarDLL()) {
        printf("PROGRAMA ENCERRADO.\n");
        return 1;
    }
    

	if(Login()==1){
    	int opcao = -1;
    		while (opcao != 0){
        		exibirMenu();
        		opcao = lerInteiro("\nDigite a opcao desejada: \n");
        	switch(opcao){
        		case 1:
        			system("cls");
        			configurarConexao();
        			break;
        		case 2:
        			system("cls");
        			abrirConexao();
        			break;
        		case 3:
        			system("cls");
        			fecharConexao();
        			break;
        		case 4:
        			system("cls");
        			imprimirTexto();
        			break;
        		case 5:
        			system("cls");
        			imprimirQRCode();
        			break;
        		case 6:
        			system("cls");
        			imprimirCodigoBarras();
        			break;
        		case 7:
        			system("cls");
        			imprimirXMLSAT();
        			break;
        		case 8:
        			system("cls");
        			imprimirXMLCancelamentoSAT();
        			break;
        		case 9:
        			system("cls");
        			abrirGavetaElgin();
        			break;
        		case 10:
        			system("cls");
        			abrirGaveta();
        			break;
        		case 11:
        			system("cls");
        			emitirSinalSonoro();
        			break;
        		default:
        			system("cls");
        			printf("\nDigite uma opcao valida!\n\n");
        			break;
			}
		}
			}else{
				system("cls");
				printf("\nACESSO NEGADO!!!\n\n");
    			FreeLibrary(dll);
    			return 1;
			}

    FreeLibrary(dll);
    return 0;
}
