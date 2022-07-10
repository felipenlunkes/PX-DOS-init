/*********************************************************************/
/*                                                                   */
/*                                                                   */
/*                                                                   */
/*                                                                   */
/*   #$$%%@!#$%                                                      */
/*   !!@#!$!$!$         Sistema Operacional PX-DOS �                 */
/*   !@@#   #$%                                                      */
/*   #$$%   &*$                                                      */
/*   $#%@   @#&                                                      */
/*   #%$&*(@*@&                                                      */
/*   @#$@$#@$%$     � 2013-2016 Felipe Miguel Nery Lunkes            */
/*   $%&*                Todos os direitos reservados                */
/*   @#&*                                                            */
/*   @&*%                                                            */
/*   #&*@                                                            */
/*                                                                   */
/*                                                                   */
/* O PX-DOS � � marca registrada de Felipe Miguel Nery Lunkes no     */
/* Brasil. � 2013-2016 Felipe Miguel Nery Lunkes. Todos os direitos  */
/* reservados. A reprodu��o total ou parcial, de quaisquer trechos   */
/* do c�digo aqui presente � expressamente probida, sendo pass�vel   */
/* de puni��o legal severa.                                          */
/*                                                                   */
/* Copyright � 2013-2016 Felipe Miguel Nery Lunkes                   */
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
// Assinaturas das fun��es presentes
//
//**********************************************************

void Abrir_Arquivo_Configuracao(void); // Fun��o de abertura do arquivo de configura��o
                                       // DRIVERS.SIS

void* processararquivo(void); // Fun��o que ir� processar o conte�do do arquivo

void __execdriver(char *cmd, void *ambiente); // Fun��o presente no m�dulo em Assembly,
                                              // que ser� chamada por este m�dulo,
											  // com seus par�metros, para o carregamento
											  // do Driver pelo Kernel


//**********************************************************
//
// Vari�veis e Constantes utilizadas
//
//**********************************************************

tamanho_t tamanho; // Tamanho

ARQUIVO *arquivo_de_drivers; // Aqui ser� aberto o arquivo de configura��es
 
int sistema;
 
int x;  
 
char driver[100] = ""; // Armazenar� o nome do Driver � ser carregado

char msg[64] = ""; // Armazenar� a mensagem a ser exibida na tela
 
char buf[200]; // Buffer tempor�rio para opera��es com Strings

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

 Abrir_Arquivo_Configuracao(); // Ir� abrir o arquivo de configura��o
 
 }
 

//**********************************************************
//
// Fun��o de Abertura do Arquivo de Configura��o
//
//**********************************************************
 
void Abrir_Arquivo_Configuracao(void) // Abre o arquivo de configura��o para que as
                                      // as informa��es possam ser acessadas
{
   
    arquivo_de_drivers = abrir("DRIVERS.SIS", "r"); // Abre o arquivo
	
	if (arquivo_de_drivers == NULL) // Se o arquivo n�o p�de ser aberto ou ele n�o foi
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

			
        while (fgets(buf, sizeof buf, arquivo_de_drivers) != NULL) // Enquanto o conte�do
		                                                           // total n�o for
                                                                   // analisado
        {
		
            processararquivo(); // Processe os comandos do arquivo
			
        }
		
        fechar(arquivo_de_drivers); // Fecha o arquivo e libera a mem�ria
		
    }
	
    return;
	
}

//**********************************************************
//
// Fun��o de compara��o entre duas Strings
//
//**********************************************************

static int compararString(char *um, char *dois) // Ir� comparar Strings
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
// Fun��o que processa e verifica os comandos presentes 
// no Arquivo de Configura��o
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
// Verifica��o e valida��o dos comandos
//
// Aviso! Ainda dentro da fun��o processararquivo()
//
//**********************************************************
							
	if (memcmp(buf, "DRIVER=", 7) == 0) // Comando para o carregamento de Drivers PX-DOS�
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
			
	else if (memcmp(buf, "MENSAGEM=", 9) == 0) // Comando para a exibi��o de mensagens
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
// Fim da Fun��o
//
//**********************************************************
	
	} // Fim da Fun��o
	

	