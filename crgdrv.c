/*********************************************************************/
/*                                                                   */
/*                                                                   */
/*                                                                   */
/*                                                                   */
/*   #$$%%@!#$%                                                      */
/*   !!@#!$!$!$         Sistema Operacional PX-DOS ®                 */
/*   !@@#   #$%                                                      */
/*   #$$%   &*$                                                      */
/*   $#%@   @#&                                                      */
/*   #%$&*(@*@&                                                      */
/*   @#$@$#@$%$     © 2013-2016 Felipe Miguel Nery Lunkes            */
/*   $%&*                Todos os direitos reservados                */
/*   @#&*                                                            */
/*   @&*%                                                            */
/*   #&*@                                                            */
/*                                                                   */
/*                                                                   */
/* O PX-DOS ® é marca registrada de Felipe Miguel Nery Lunkes no     */
/* Brasil. © 2013-2016 Felipe Miguel Nery Lunkes. Todos os direitos  */
/* reservados. A reprodução total ou parcial, de quaisquer trechos   */
/* do código aqui presente é expressamente probida, sendo passível   */
/* de punição legal severa.                                          */
/*                                                                   */
/* Copyright © 2013-2016 Felipe Miguel Nery Lunkes                   */
/* Todos os direitos reservados.                                     */
/*                                                                   */
/*********************************************************************/ 

#include <stdio.h>
#include <pxdos.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

//**********************************************************
//
// Assinaturas das funções presentes
//
//**********************************************************

void Abrir_Arquivo_Configuracao(void); // Função de abertura do arquivo de configuração
                                       // DRIVERS.SIS

void* processararquivo(void); // Função que irá processar o conteúdo do arquivo

void __execdriver(char *cmd, void *ambiente); // Função presente no módulo em Assembly,
                                              // que será chamada por este módulo,
											  // com seus parâmetros, para o carregamento
											  // do Driver pelo Kernel


//**********************************************************
//
// Variáveis e Constantes utilizadas
//
//**********************************************************

tamanho_t tamanho; // Tamanho

ARQUIVO *arquivo_de_drivers; // Aqui será aberto o arquivo de configurações
 
int sistema;
 
int x;  
 
char driver[100] = ""; // Armazenará o nome do Driver à ser carregado

char msg[64] = ""; // Armazenará a mensagem a ser exibida na tela
 
char buf[200]; // Buffer temporário para operações com Strings

char resp;
 
static unsigned char cmdt[140];

//**********************************************************
//
// Bloco de Controle de Arquivos DOS (FCB)
//
//**********************************************************

 static struct {

    int ambiente;
	
    unsigned char *fimcmd;
	
    char *parametro1;
	
    char *parametro2;
	
} blocodeParametros = { 0, cmdt, NULL, NULL }; // Bloco de Controle de Arquivos DOS

//**********************************************************
//
// Ponto de Entrada
//
//**********************************************************

 int main(void) // Ponto de Entrada
 {

 Abrir_Arquivo_Configuracao(); // Irá abrir o arquivo de configuração
 
 }
 

//**********************************************************
//
// Função de Abertura do Arquivo de Configuração
//
//**********************************************************
 
void Abrir_Arquivo_Configuracao(void) // Abre o arquivo de configuração para que as
                                      // as informações possam ser acessadas
{
   
    arquivo_de_drivers = abrir("DRIVERS.SIS", "r"); // Abre o arquivo
	
	if (arquivo_de_drivers == NULL) // Se o arquivo não pôde ser aberto ou ele não foi
	                                // encontrado
	{
	
printf("\nArquivo de DRIVERS nao encontrado!\n");

	asm{
	
	mov ah, 02h
	int 90h
	
	}
	
	}
	
    if (arquivo_de_drivers != NULL) // Caso o arquivo foi localizado e aberto
    {   

			
        while (fgets(buf, sizeof buf, arquivo_de_drivers) != NULL) // Enquanto o conteúdo
		                                                           // total não for
                                                                   // analisado
        {
		
            processararquivo(); // Processe os comandos do arquivo
			
        }
		
        fechar(arquivo_de_drivers); // Fecha o arquivo e libera a memória
		
    }
	
    return;
	
}

//**********************************************************
//
// Função de comparação entre duas Strings
//
//**********************************************************

static int compararString(char *um, char *dois) // Irá comparar Strings
{
    while (paramaiusculo(*um) == paramaiusculo(*dois))
    {
        if (*um == '\0')
        {
		
            return (0);
			
        }
		
        um++;
        dois++;
		
    }
	
    if ((*um) < paramaiusculo(*dois))
    {
	
        return (-1);
		
    }
	
    return (1);
	
}

//**********************************************************
//
// Função que processa e verifica os comandos presentes 
// no Arquivo de Configuração
//
//**********************************************************

 void* processararquivo(void) // Processa o arquivo
{
    char *p;

    tamanho = tamanhostring(buf);
    if ((tamanho > 0) && (buf[tamanho - 1] == '\n'))
    {
	
        tamanho--;
        buf[tamanho] = '\0';
		
    }
	
    p = strchr(buf, ' ');
	
    if (p != NULL)
    {
	
        *p++ = '\0';
		
    }
	
    else
    {
	
        p = buf + tamanho;
		
    }
	
    tamanho -= (tamanho_t)(p - buf);
		

//**********************************************************
//
// Verificação e validação dos comandos
//
// Aviso! Ainda dentro da função processararquivo()
//
//**********************************************************
							
	if (memcmp(buf, "DRIVER=", 7) == 0) // Comando para o carregamento de Drivers PX-DOS®
                {
                    char *p;
                    
                    memcpy(driver, buf + 7, sizeof driver);
                    driver[sizeof driver - 1] = '\0';
                    p = strchr(driver, '\r');
				
                   __execdriver(driver, &blocodeParametros);  // Execute o Driver!	
					
                    if (p != NULL)
                    {
                       
					   *p = '\0';
					   
                    }
					
                }
			
	else if (memcmp(buf, "MENSAGEM=", 9) == 0) // Comando para a exibição de mensagens
	{

	                char *p;
                    
                    memcpy(msg, buf + 9, sizeof msg);
                    msg[sizeof msg - 1] = '\0';
                    p = strchr(msg, '\r');
				
                   printf("\n%s\n", msg);  // Exibe a mensagem
					
                    if (p != NULL)
                    {
                       
					   *p = '\0';
					   
                    }
					
    }
				
//**********************************************************
//
// Fim da Função
//
//**********************************************************
	
	} // Fim da Função
	

	