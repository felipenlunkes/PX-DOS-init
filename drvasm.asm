;;********************************************************************/
;;                                                                   */
;;                                                                   */
;;                                                                   */
;;                                                                   */
;;   #$$%%@!#$%                                                      */
;;   !!@#!$!$!$         Sistema Operacional PX-DOS ®                 */
;;   !@@#   #$%                                                      */
;;   #$$%   &*$                                                      */
;;   $#%@   @#&                                                      */
;;   #%$&*(@*@&                                                      */
;;   @#$@$#@$%$     © 2013-2016 Felipe Miguel Nery Lunkes            */
;;   $%&*                Todos os direitos reservados                */
;;   @#&*                                                            */
;;   @&*%                                                            */
;;   #&*@                                                            */
;;                                                                   */
;;                                                                   */
;; O PX-DOS ® é marca registrada de Felipe Miguel Nery Lunkes no     */
;; Brasil. © 2013-2016 Felipe Miguel Nery Lunkes. Todos os direitos  */
;; reservados. A reprodução total ou parcial, de quaisquer trechos   */
;; do código aqui presente é expressamente probida, sendo passível   */
;; de punição legal severa.                                          */
;;                                                                   */
;; Copyright © 2013-2016 Felipe Miguel Nery Lunkes                   */
;; Todos os direitos reservados.                                     */
;;                                                                   */
;;********************************************************************/ 

;; Implementação para a chamada PX-DOS em Assembly

% .model memodel, c 

assume cs:_CODIGO, ds:DGROUP

_DADOS   segment word public 'DATA'
_DADOS   ends

_BSS    segment word public 'BSS'
_BSS    ends

_CODIGO segment word public 'CODE' ;; Início da área de Código (Texto)


public __execdriver ;; Declara público esta função, que será chamada pelo módulo em C

__execdriver proc ;; Função que irá pedir ao Kernel o carregamento do Driver

push bp

mov bp, sp

push ds
push dx
push es
push bx
push cx
push si
push di

mov dx, [bp + 6]
mov ds, [bp + 8]
mov bx, [bp + 10]
mov es, [bp + 12]

jmp short chamar_Kernel_PXDOS

salvar_sp dw ? ;; Aqui será salvo o valor de SP antes da chamada
salvar_ss dw ? ;; Aqui será salvo o valor de SS antes da chamada

chamar_Kernel_PXDOS: ;; Irá adequar os parâmetros recebidos e realizar a chamada

mov salvar_sp, sp ;; Salvar SP
mov salvar_ss, ss ;; Salvar SS

mov al, 0
mov ah, 07h
cld              ;; Para evitar BUGS com o Kernel
int 90h          ;; Chama o Kernel do PX-DOS

cli ;; Desabilita as interrupções

mov ss, salvar_ss ;; Restaurar o valor inicial de SS, antes da chamada ao Kernel
mov sp, salvar_sp ;; Restaurar o valor inicial de SP, antes da chamada ao Kernel

sti ;; Habilita as interrupções

;; Restaurar o estado da máquina

pop di 
pop si
pop cx
pop bx
pop es
pop dx
pop ds
pop bp

ret ;; Retorna ao Módulo em C

__execdriver endp ;; Fim da função

_CODIGO ends ;; Fim da área de Código (Texto)

end ; Fim do Módulo